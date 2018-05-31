#include <iomanip>
#include <iostream>
#include <mutex>
#include <random>
#include <thread>
#include <vector>


class Item {
  private:
    size_t ratings = 0;
    double avg_rating = 0.;
    std::recursive_mutex m;

  public:
    double get_rating() {
        std::lock_guard<std::recursive_mutex> guard(m);
        return avg_rating;
    }

    void add_rating(int stars) {
        std::lock_guard<std::recursive_mutex> guard(m);
        avg_rating = (avg_rating * ratings + stars) / (ratings + 1.);
        ++ratings;
    }

    int get_rating_count() {
        std::lock_guard<std::recursive_mutex> guard(m);
        return ratings;
    }

    void out_rating() {
        std::lock_guard<std::recursive_mutex> guard(m);

        const size_t sep = 10;
        std::cout << std::setw(sep) << "ratings:"
                  << std::setw(sep) << get_rating_count()
                  << std::setw(sep) << "score:"
                  << std::setw(sep) << get_rating()
                  << std::endl;
    }
};

void out_ratings(std::vector<Item>& items) {
    for (auto & item : items)
        item.out_rating();
    std::cout << std::endl;
}

void rate_items(std::vector<Item>& items, int num) {
    std::default_random_engine e1(42);
    std::uniform_int_distribution<int> item_dist(0, items.size()-1);
    std::uniform_int_distribution<int> star_dist(1,5);

    for (; num > 0; --num)
        items[item_dist(e1)].add_rating(star_dist(e1));
}

void run_scenario(int n_items, int n_threads, bool sequential) {
    std::vector<Item> items(n_items);
    std::vector<std::thread> threads(n_threads);

    for (auto &thread : threads) {
        thread = std::thread(rate_items, std::ref(items), 10000);
        if (sequential) thread.join();
    }

    if (!sequential)
        for (auto & thread : threads)
            thread.join();

    out_ratings(items);
}


int main() {
    int n_items, n_threads;
    std::cin >> n_items >> n_threads;
    std::cout << std::endl;

    std::cout << "Sequential" << std::endl;
    run_scenario(n_items, n_threads, true);
    std::cout << "Parallel" << std::endl;
    run_scenario(n_items, n_threads, false);

    return 0;
}

