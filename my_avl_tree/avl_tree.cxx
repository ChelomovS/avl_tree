#include "avl_tree.hxx"

#include <iostream>

int main() {
    AVLTree::avl_tree<int> search_tree{};

    char c;
    int value;
    int first, second;
    int number_of_range_query;

    while (std::cin >> c) {
        if (c == AVLTree::KEY) {
            std::cin >> value;
            if (!std::cin.good()) {
                std::cerr << "Error input" << std::endl;
                return EXIT_FAILURE;
            }
            search_tree.insert(value);
        }

        else if (c == AVLTree::QUERY) {
            std::cin >> first >> second;
            if (!std::cin.good()) {
                std::cerr << "Error input" << std::endl;
                return EXIT_FAILURE;
            }
            number_of_range_query = AVLTree::range_query(search_tree, first, second);
        }

        else {
            std::cerr << "Error input" << std::endl;
            return EXIT_FAILURE;
        }
    }
    
    return EXIT_SUCCESS;
}
