#include "avl_tree.hxx"

#include <iostream>
#include <chrono>

int main() {
    AVLTree::avl_tree<int> search_tree{};

    char c;
    int value;
    int first, second;
    int number_of_range_query;

    std::chrono::duration<double> total_time{0};

    while (std::cin >> c) {
        if (c == AVLTree::KEY) {
            std::cin >> value;
            if (!std::cin.good()) {
                std::cerr << "Error input" << std::endl;
                return EXIT_FAILURE;
            }

            auto begin_time_of_insertion = std::chrono::high_resolution_clock::now();
            search_tree.insert(value);
            auto end_time_of_insertion = std::chrono::high_resolution_clock::now();
            total_time += end_time_of_insertion - begin_time_of_insertion;
        }

        else if (c == AVLTree::QUERY) {
            std::cin >> first >> second;
            if (!std::cin.good()) {
                std::cerr << "Error input" << std::endl;
                return EXIT_FAILURE;
            }

            auto begin_time_of_range_query = std::chrono::high_resolution_clock::now();
            number_of_range_query = AVLTree::range_query(search_tree, first, second);
            auto end_time_of_range_query = std::chrono::high_resolution_clock::now();
            total_time += end_time_of_range_query - begin_time_of_range_query;
            std::cout << number_of_range_query << " ";
        }

        else {
            std::cerr << "Error input" << std::endl;
            return EXIT_FAILURE;
        }
    }

    std::cerr << "Total time of avl: " << std::chrono::duration_cast<std::chrono::microseconds>(total_time).count(); 
    std::cerr << " in microseconds" << std::endl;

    return EXIT_SUCCESS;
}
