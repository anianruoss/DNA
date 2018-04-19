#include <cassert>
#include <iomanip>
#include <iostream>
#include <vector>


template <typename datatype>
void out(std::vector<datatype> v) {
    std::cout << "approximations: " << std::endl;

    for (auto x = v.begin(); x != v.end(); ++x) {
        std::cout << *x;
        if (x+1 != v.end())
            std::cout << " ";
    }

    std::cout << std::endl;
}

template <typename function>
std::vector<double> sum_vector(unsigned int sz, function f) {
    std::vector<double> vec(sz+1);
    double sum = 0;

    for (unsigned int i = 0; i < sz; ++i) {
        vec[i] = sum;
        sum += f(i);
    }

    vec[sz] = sum;
    return vec;
}

double getsum(const std::vector<double>& sum, unsigned int from, unsigned int to) {
    return sum[to]-sum[from];
}

void fill(std::vector<double>& vec, double value, unsigned int from, unsigned int to) {
    std::fill(vec.begin() + from, vec.begin() + to, value);
}


class Statistics {
  private:
    std::vector<double> vsum;
    std::vector<double> vssq;
    unsigned int n;

  public:
    Statistics(const std::vector<double>& data): n{(unsigned int)data.size()} {
        vsum = sum_vector(n, [&data] (unsigned int i) {
            return data[i];
        });

        vssq = sum_vector(n, [&data] (unsigned int i) {
            return data[i]*data[i];
        });
    }

    double mean(unsigned int from, unsigned int to) const {
        return getsum(vsum, from, to) / (to - from);
    }

    double err(unsigned int from, unsigned int to) const {
        double s = getsum(vsum, from, to);
        return getsum(vssq, from, to) - (s*s / (to - from));
    }

    int size() const {
        return n;
    }
};


void MinimizeH(double gamma, const Statistics& s) {
    int n = s.size();
    std::vector<int> B(n+1);
    std::vector<double> V(n+1);

    B[0] = 0;
    V[0] = 0;

    for (int right = 1; right <= n; ++right) {
        int best = 0;
        double min = gamma + s.err(0, right);

        for (int left = 1; left < right; ++left) {
            double h = V[left] + gamma + s.err(left, right);

            if (h < min) {
                min = h;
                best = left;
            }
        }

        B[right] = best;
        V[right] = min;
    }

    std::vector<double> result(n);
    unsigned int right = n;

    while (right != 0) {
        unsigned int left = B[right];
        fill(result, s.mean(left, right), left, right);
        right = left;
    }

    std::cout << "error = " << V[n] << std::endl;
    out(result);
}


int main() {
    std::cout << std::setprecision(3);

    std::cout << "gamma = ";
    double gamma;
    std::cin >> gamma;

    std::vector<double> data;
    double value;

    std::cout << "signal values: (terminate with \\n)" << std::endl;
    while (std::cin >> value) {
        data.push_back(value);
    }
    std::cout << std::endl;

    Statistics stat(data);
    MinimizeH(gamma, data);

    return 0;
}

