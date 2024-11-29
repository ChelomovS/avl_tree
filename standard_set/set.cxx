#include <iostream>
#include <set>
#include <chrono>

namespace {
const char KEY = 'k';
const char QUERY = 'q';
} // namespace

template <typename C, typename T>
long range_query(const C &set, const T& first, const T& second) {
    if (first > second) 
        return 0;

    auto start = set.lower_bound(first);
    auto fin   = set.upper_bound(second);
    return std::distance(start, fin);
}

int main() {
    std::set<int> set;

    char c;
    int value;
    int first, second;
    size_t number_of_range_query;

    std::chrono::duration<double> total_time{0};

    while (std::cin >> c) {
        if (c == KEY) {
            if (!(std::cin >> value)) {
                std::cerr << "Error input" << std::endl;
                return EXIT_FAILURE;
            }
            auto begin_time_of_insertion = std::chrono::high_resolution_clock::now();
            set.insert(value);
            auto end_time_of_insertion = std::chrono::high_resolution_clock::now();
            total_time += end_time_of_insertion - begin_time_of_insertion;
        }

        else if (c == QUERY) {
            if (!(std::cin >> first >> second)) {
                std::cerr << "Error input" << std::endl;
                return EXIT_FAILURE;
            }
            auto begin_time_of_range_query = std::chrono::high_resolution_clock::now();
            number_of_range_query = range_query(set, first, second);
            auto end_time_of_range_query = std::chrono::high_resolution_clock::now();
            std::cout << number_of_range_query << " ";
            total_time += end_time_of_range_query - begin_time_of_range_query;
        }

        else {
            std::cerr << "Error input" << std::endl;
            return EXIT_FAILURE;
        }
    }

    std::cerr << "Total time of set: " << std::chrono::duration_cast<std::chrono::microseconds>(total_time).count(); 
    std::cerr << " in microseconds" << std::endl;

    return EXIT_SUCCESS;
}