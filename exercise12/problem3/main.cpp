#include <atomic>
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <random>
#include <thread>
#include <vector>


class Bridge {
  private:
    std::atomic_int cars{0};
    std::atomic_int trucks{0};
    std::mutex m;
    std::condition_variable cond;
    using guard = std::unique_lock<std::mutex>;

  public:
    void enter_car() {
        guard g(m);
        cond.wait(g, [&] {return cars < 3 && trucks == 0;});
        ++cars;
        cond.notify_all();
    }

    void leave_car() {
        guard g(m);
        cond.wait(g, [&] {return cars > 0;});
        --cars;
        cond.notify_all();
    }

    void enter_truck() {
        guard g(m);
        cond.wait(g, [&] {return trucks == 0 && cars == 0;});
        ++trucks;
        cond.notify_all();
    }

    void leave_truck() {
        guard g(m);
        cond.wait(g, [&] {return trucks == 1;});
        --trucks;
        cond.notify_all();
    }
};


struct Test {
    std::mutex m;
    int cars = 0;
    int trucks = 0;
    bool passed = true;

    void Check() {
        if (!passed) return;
        if (cars > 3 || trucks > 1 || (trucks >0 && cars > 0)) {
            std::cout << "BRIDGE COLLAPSED!!";
            passed = false;
        }
    }

    void EnterCar() {
        std::lock_guard<std::mutex> g(m);
        cars++;
        Check();
    }

    void LeaveCar() {
        std::lock_guard<std::mutex> g(m);
        Check();
        cars--;
    }

    void EnterTruck() {
        std::lock_guard<std::mutex> g(m);
        Check();
        trucks++;
    }

    void LeaveTruck() {
        std::lock_guard<std::mutex> g(m);
        Check();
        trucks--;
    }
};


void drive_car(Bridge &b, Test&t, int id) {
    std::default_random_engine e1(id);
    std::uniform_int_distribution<int> uniform_dist(0, 300);

    std::this_thread::sleep_for(std::chrono::milliseconds(uniform_dist(e1)));
    b.enter_car();
    t.EnterCar();

    std::this_thread::sleep_for(std::chrono::milliseconds(uniform_dist(e1)));
    t.LeaveCar();
    b.leave_car();
}

void drive_truck(Bridge &b, Test&t, int id) {
    std::default_random_engine e1(id);
    std::uniform_int_distribution<int> uniform_dist(0, 300);

    std::this_thread::sleep_for(std::chrono::milliseconds(uniform_dist(e1)));
    b.enter_truck();
    t.EnterTruck();

    std::this_thread::sleep_for(std::chrono::milliseconds(uniform_dist(e1)));
    t.LeaveTruck();
    b.leave_truck();
}

void simulate(int nThreads) {
    std::default_random_engine e1(42);
    std::uniform_int_distribution<int> uniform_dist(0,1);

    Bridge b;
    Test t;

    std::vector<std::thread> threads;

    for (int i = 0; i < nThreads; i++) {
        if (uniform_dist(e1) == 0) {
            threads.emplace_back(std::thread(drive_car, std::ref(b), std::ref(t), i));
        } else {
            threads.emplace_back(std::thread(drive_truck, std::ref(b), std::ref(t), i));
        }
    }

    for (auto &t: threads) t.join();
    if (t.passed) std::cout << "All cars passed!" << std::endl;
}


int main() {
    int nThreads;
    std::cin >> nThreads;

    simulate(nThreads);

    return 0;
}

