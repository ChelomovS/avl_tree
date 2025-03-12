#include <iostream>
#include <set>

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
    long number_of_range_query;

    while (std::cin >> c) {
        if (c == KEY) {
            if (!(std::cin >> value)) {
                std::cerr << "Error input" << std::endl;
                return EXIT_FAILURE;
            }
            set.insert(value);
        }

        else if (c == QUERY) {
            if (!(std::cin >> first >> second)) {
                std::cerr << "Error input" << std::endl;
                return EXIT_FAILURE;
            }
            number_of_range_query = range_query(set, first, second);
        }

        else {
            std::cerr << "Error input" << std::endl;
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}