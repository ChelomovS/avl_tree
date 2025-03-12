#include <gtest/gtest.h>

#include "avl_tree.hxx"
#include "controllable.hxx"

int Controllable::control = 3;

TEST(AVL_TREE_FUNCTIONS, range_query_1) {
    AVLTree::avl_tree<int> tree;

    tree.insert(8);
    tree.insert(10);
    tree.insert(1);
    
    int count = tree.my_distance(tree.lower_bound(0), tree.upper_bound(9));
    ASSERT_EQ(count, 2);
}

TEST(AVL_TREE_FUNCTIONS, range_query_2) {
    AVLTree::avl_tree<int> tree;

    tree.insert(-1);
    tree.insert(10);
    tree.insert(2);
    
    int count = tree.my_distance(tree.lower_bound(-2), tree.upper_bound(10));
    ASSERT_EQ(count, 3);
}

TEST(AVL_TREE_FUNCTIONS, range_query_3) {
    AVLTree::avl_tree<int> tree;

    tree.insert(-1);
    tree.insert(10);
    tree.insert(2);
    
    int count = tree.my_distance(tree.lower_bound(-2), tree.upper_bound(10));
    ASSERT_EQ(count, 3);
}

TEST(AVL_TREE_FUNCTIONS, range_query_4) {
    AVLTree::avl_tree<int> tree;

    tree.insert(-5);
    tree.insert(0);
    tree.insert(-1);
    tree.insert(10);
    tree.insert(2);
    tree.insert(8);
    
    int count = tree.my_distance(tree.lower_bound(-2), tree.upper_bound(10));
    ASSERT_EQ(count, 5);
}

TEST(AVL_TREE_FUNCTIONS, range_query_5) {
    AVLTree::avl_tree<int> tree;

    tree.insert(9);
    tree.insert(-3);
    tree.insert(79);
    tree.insert(-5);
    tree.insert(0);
    tree.insert(-1);
    tree.insert(10);
    tree.insert(2);
    tree.insert(8);
    
    int count = tree.my_distance(tree.lower_bound(2), tree.upper_bound(10));
    ASSERT_EQ(count, 4);
}

TEST(AVL_TREE_FUNCTIONS, copy_ctor) {
    AVLTree::avl_tree<int> tree;

    tree.insert(9);
    tree.insert(-3);
    tree.insert(79);
    tree.insert(-5);
    tree.insert(0);
    tree.insert(-1);
    tree.insert(10);
    tree.insert(2);
    tree.insert(8);

    AVLTree::avl_tree<int> copy_tree{tree};

    int count = copy_tree.my_distance(copy_tree.lower_bound(2), copy_tree.upper_bound(10));
    ASSERT_EQ(count, 4);
}

TEST(AVL_TREE_FUNCTIONS, move_ctor) {
    AVLTree::avl_tree<int> tree;

    tree.insert(9);
    tree.insert(-3);
    tree.insert(79);
    tree.insert(-5);
    tree.insert(0);
    tree.insert(-1);
    tree.insert(10);
    tree.insert(2);
    tree.insert(8);

    AVLTree::avl_tree<int> copy_tree{std::move(tree)};

    int count = copy_tree.my_distance(copy_tree.lower_bound(2), copy_tree.upper_bound(10));
    ASSERT_EQ(count, 4);
}

TEST(AVL_TREE_FUNCTIONS, copy_assignment) {
    AVLTree::avl_tree<int> tree;

    tree.insert(9);
    tree.insert(-3);
    tree.insert(79);
    tree.insert(-5);
    tree.insert(0);
    tree.insert(-1);
    tree.insert(10);
    tree.insert(2);
    tree.insert(8);

    AVLTree::avl_tree<int> copy_tree;

    copy_tree = tree;
    int count = copy_tree.my_distance(copy_tree.lower_bound(2), copy_tree.upper_bound(10));
    ASSERT_EQ(count, 4);
}

TEST(AVL_TREE_FUNCTIONS, move_assignment) {
    AVLTree::avl_tree<int> tree;

    tree.insert(9);
    tree.insert(-3);
    tree.insert(79);
    tree.insert(-5);
    tree.insert(0);
    tree.insert(-1);
    tree.insert(10);
    tree.insert(2);
    tree.insert(8);

    AVLTree::avl_tree<int> copy_tree;

    copy_tree = std::move(tree);
    int count = copy_tree.my_distance(copy_tree.lower_bound(2), copy_tree.upper_bound(10));
    ASSERT_EQ(count, 4);
}

TEST(AVL_TREE_EXCEPTION_SAFETY, insert_strong_garantee) {
    AVLTree::avl_tree<Controllable, std::less<Controllable>> tree;
    const int unsafe_point = 3;
    Controllable::control = unsafe_point;

    try {
        for (size_t i = 0; i < 5; ++i) {
            tree.insert(Controllable());
        }
        FAIL() << "Expected std::bad_alloc!\n";
    } catch (const std::bad_alloc&) {
        SUCCEED();
    }
}

TEST(AVL_TREE_EXCEPTION_SAFETY, сopy_strong_garantee) {
    AVLTree::avl_tree<Controllable> src;
    Controllable::control = 3; 
    src.insert(Controllable());
    src.insert(Controllable());

    Controllable::control = 1; 
    
    try {
        AVLTree::avl_tree<Controllable> copy(src);
        FAIL() << "Expected std::bad_alloc!\n";
    } catch (const std::bad_alloc&) {
        SUCCEED();
    }
}


#if 0
TEST(AVL_TREE_FUNCTIONS, exception_safety_insert) {
    AVLTree::avl_tree<Controllable, comparator_for_controllable> source_tree;

    bool exeption_thrown = false;

    for (int i = 0; i < 10; ++i) {
        try {
            source_tree.insert(Controllable());
        } catch (...) {
            std::cerr << "Catch" << std::endl; 
            exeption_thrown = true;
        }
    }

    ASSERT_TRUE(exeption_thrown);
}

TEST(AVL_TREE_FUNCTIONS, move_assigmet) {
    AVLTree::avl_tree<Controllable, comparator_for_controllable> source_tree;

    Controllable::control = 4;

    bool exeption_thrown = false;

    for (int i = 0; i < 10; ++i) {
        try {
            source_tree.insert(Controllable());
            std::cerr << "INSERT "<< std::endl;
        } catch (...) {
            std::cerr << "INSERT ISKL "<< std::endl; 
        }
    }

    try {
        AVLTree::avl_tree<Controllable, comparator_for_controllable> copy_tree = source_tree;
        std::cerr << "move" << std::endl; 
    } catch (...) {
        std::cerr << "Catch" << std::endl; 
        exeption_thrown = true;
    }

    ASSERT_TRUE(exeption_thrown);
}
#endif

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}