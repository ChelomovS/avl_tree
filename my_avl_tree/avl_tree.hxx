#include <stack>
#include <functional>
#include <cassert>
#include <iostream>
#include <memory>
#include <queue>
#include <vector>

namespace AVLTree {

const char KEY   = 'k';
const char QUERY = 'q';

enum class Part {
    Left, Right
};

template <typename ElemT = int>
class avl_node {
public:
    ElemT value_;
    avl_node* parent_ = nullptr;
    avl_node* left_ = nullptr;
    avl_node* right_ = nullptr;
    int height_;
    size_t size_of_subtree_;

public:
    avl_node() {}
    avl_node(const ElemT& value, avl_node* parent = nullptr, avl_node* left = nullptr, avl_node* right = nullptr,
             int height = 1, size_t size_of_subtree = 1): 
             value_{value}, parent_{parent}, left_{left}, right_{right}, height_{height}, size_of_subtree_{size_of_subtree} {} 
};

template <typename ElemT>
int get_height(const avl_node<ElemT>* node) {
    return node ? node->height_ : 0;
}

template <typename ElemT>
size_t get_size_of_subtree(const avl_node<ElemT>* node) { 
    return node ? node->size_of_subtree_ : 0;
}

template <typename ElemT>
int get_balance_factor(const avl_node<ElemT>* node) {
    return node ? (get_height(node->right_) - get_height(node->left_)) : 0;
}

template <typename ElemT>
void fix_height_and_size_of_subtree(avl_node<ElemT>* node) {
    if (!node)
        return;

    int height_of_left  = get_height(node->left_);
    int height_of_right = get_height(node->right_);
    node->height_       = 1 + std::max(height_of_left, height_of_right);

    size_t size_left    = get_size_of_subtree(node->left_); 
    size_t size_right   = get_size_of_subtree(node->right_); 
    node->size_of_subtree_ = 1 + size_left + size_right;
}

template <typename ElemT, typename Comparator = std::less<ElemT>>
class avl_tree {
private:
    avl_node<ElemT>* root_ = nullptr;
    Comparator cmp_;

public:
    avl_tree() {}

private:
    avl_node<ElemT>* safe_copy(const avl_node<ElemT>* src_root) {
        if (!src_root) return nullptr;

        std::vector<avl_node<ElemT>*> allocated_nodes;
        avl_node<ElemT>* new_root = nullptr;

        try {
            new_root = new avl_node<ElemT>();
            allocated_nodes.push_back(new_root);

            new_root->value_ = src_root->value_;
            new_root->height_ = 1; 
            new_root->size_of_subtree_ = 1;

            std::stack<std::pair<const avl_node<ElemT>*, avl_node<ElemT>*>> stack;
            stack.emplace(src_root, new_root);

            while (!stack.empty()) {
                auto [src_node, current] = stack.top();
                stack.pop();

                if (src_node->right_) {
                    avl_node<ElemT>* right = new avl_node<ElemT>();
                    allocated_nodes.push_back(right);
                    right->parent_ = current;
                    current->right_ = right;
                    right->value_ = src_node->right_->value_;
                    right->height_ = 1;
                    right->size_of_subtree_ = 1;

                    stack.emplace(src_node->right_, right);
                }

                if (src_node->left_) {
                    avl_node<ElemT>* left = new avl_node<ElemT>();
                    allocated_nodes.push_back(left);
                    left->parent_ = current;
                    current->left_ = left;
                    left->value_ = src_node->left_->value_;
                    left->height_ = 1;
                    left->size_of_subtree_ = 1;

                    stack.emplace(src_node->left_, left);
                }
            }

            std::stack<std::pair<avl_node<ElemT>*, bool>> post_stack;
            post_stack.push({new_root, false});

            while (!post_stack.empty()) {
                auto [current, visited] = post_stack.top();
                post_stack.pop();

                if (current) {
                    if (!visited) {
                        post_stack.push({current, true});
                        if (current->right_) post_stack.push({current->right_, false});
                        if (current->left_) post_stack.push({current->left_, false});
                    } else {
                        fix_height_and_size_of_subtree(current);
                    }
                }
            }

            return new_root;

        } catch (...) {
            for (auto node : allocated_nodes) {
                delete node;
            }
            throw;
        }
    }

    avl_node<ElemT>* rotate_right(avl_node<ElemT>* p) {
        auto q = p->left_;
        p->left_ = q->right_;
        if (q->right_) 
            q->right_->parent_ = p;
        q->right_ = p;

        q->parent_ = p->parent_;
        if (p->parent_) {
            if (p->parent_->left_ == p)
                p->parent_->left_ = q;
            else
                p->parent_->right_ = q;
        } 
        else {
            root_ = q;
        }
        p->parent_ = q;

        fix_height_and_size_of_subtree(p);
        fix_height_and_size_of_subtree(q);

        return q;
    }

    avl_node<ElemT>* rotate_left(avl_node<ElemT>* q) {
        auto p = q->right_;
        q->right_ = p->left_;
        if (p->left_) 
            p->left_->parent_ = q;
        p->left_ = q;

        p->parent_ = q->parent_;
        if (q->parent_) {
            if (q->parent_->left_ == q)
                q->parent_->left_ = p;
            else
                q->parent_->right_ = p;
        } 
        else {
            root_ = p;
        }

        q->parent_ = p;

        fix_height_and_size_of_subtree(q);
        fix_height_and_size_of_subtree(p);

        return p;
    }

    avl_node<ElemT>* balance(avl_node<ElemT>* node) {
        fix_height_and_size_of_subtree(node);
        
        int balance_factor = get_balance_factor(node);
        
        if (balance_factor == 2) {
            if (get_balance_factor(node->right_) < 0)
                node->right_ = rotate_right(node->right_);

            return rotate_left(node);
        }

        if (balance_factor == -2) {
            if (get_balance_factor(node->left_) > 0)
                node->left_ = rotate_left(node->left_);

            return rotate_right(node);
        }
        
        return node;
    }

public:
    void insert(const ElemT& elem_to_insert) {
        std::unique_ptr<avl_node<ElemT>> new_node;
        avl_node<ElemT>* parent = nullptr;

        try {
            if (!root_) {
                new_node.reset(new avl_node<ElemT>{elem_to_insert});
                root_ = new_node.release();
                return;
            }

            avl_node<ElemT>* current = root_;
            while (current) {
                parent = current;
                if (cmp_(elem_to_insert, current->value_))
                    current = current->left_;
                else if (cmp_(current->value_, elem_to_insert))
                    current = current->right_;
                else 
                    return;
            }

            new_node.reset(new avl_node<ElemT>{elem_to_insert, parent});

            if (cmp_(elem_to_insert, parent->value_))
                parent->left_ = new_node.get();
            else
                parent->right_ = new_node.get();

            avl_node<ElemT>* node = new_node.get();
            
            while (node) {
                node = balance(node);
                node = node->parent_;
            }

            new_node.release();
        } catch (...) {
            if (parent) {
                if (parent->left_ == new_node.get())
                    parent->left_ = nullptr;
                else if (parent->right_ == new_node.get())
                    parent->right_ = nullptr;
            }
            throw;
        }
    }

    void delete_tree() {
        if (!root_)
            return;

        std::stack<avl_node<ElemT>*> stack;
        avl_node<ElemT>* current = root_;
        avl_node<ElemT>* last = nullptr;

        while (current || !stack.empty()) {
            if (current) {
                stack.push(current);
                current = current->left_;
            } else {
                avl_node<ElemT>* top = stack.top();
                if (top->right_ && (last != top->right_)) {
                    current = top->right_;
                } else {
                    stack.pop();
                    delete top;
                    last = top;
                    current = nullptr;
                }
            }
        }
        root_ = nullptr;
    }

    avl_node<ElemT>* lower_bound(const ElemT& value) const {
        avl_node<ElemT>* current = root_;
        avl_node<ElemT>* result  = nullptr;
        
        while (current) {
            if (!cmp_(current->value_, value)) {
                result = current;
                current = current->left_;
            } 
            else {
                current = current->right_;
            }
        }
        
        return result;
    }

    avl_node<ElemT>* upper_bound(const ElemT& value) const {
        avl_node<ElemT>* current = root_;
        avl_node<ElemT>* result =  nullptr;

        while (current) {
            if (cmp_(value, current->value_)) {
                result = current;
                current = current->left_;
            }
            else {
                current = current->right_;
            }
        }
        return result;
    }

    size_t get_num_nodes_less_then_cur(const avl_node<ElemT>* node) const { 
        if (!node) 
            return 0;

        size_t number_of_nodes = get_size_of_subtree(node->left_);
        const avl_node<ElemT>* current = node;
        const avl_node<ElemT>* parent  = node->parent_;

        while (parent) {
            if (parent->right_ == current) {
                number_of_nodes += 1 + get_size_of_subtree(parent->left_);
            }

            current = parent;
            parent  = parent->parent_;
        }

        return number_of_nodes;
    }

    size_t my_distance(const avl_node<ElemT>* lower, const avl_node<ElemT>* upper) const { 
        if (!root_)
            return 0;

        if (!lower)
            return 0;

        if (!upper)
            return get_size_of_subtree(root_) - get_num_nodes_less_then_cur(lower);

        size_t upper_count = get_num_nodes_less_then_cur(upper);
        size_t lower_count = get_num_nodes_less_then_cur(lower);
        
        return (upper_count >= lower_count) ? (upper_count - lower_count) : 0;
    }

public:
    avl_tree(const avl_tree<ElemT>& other_tree) : root_{nullptr}, cmp_{other_tree.cmp_} {
        try {
            root_ = safe_copy(other_tree.root_);
        } catch (...) {
            throw;
        }
    }

    avl_tree(avl_tree<ElemT>&& other_tree) noexcept: root_{other_tree.root_} {
        other_tree.root_ = nullptr;
    }

    avl_tree<ElemT>& operator=(const avl_tree<ElemT>& other_tree) { 
        if (this == &other_tree)
            return *this;
        try {
            avl_tree<ElemT> tmp_tree{other_tree};
            std::swap(*this, tmp_tree);
        } catch (...) {
            throw;
        }

        return *this;
    }

    avl_tree<ElemT>& operator=(avl_tree<ElemT>&& other_tree) noexcept {
        if (this == &other_tree) 
            return *this;

        delete_tree();

        root_ = other_tree.root_;
        other_tree.root_ = nullptr;

        std::swap(cmp_, other_tree.cmp_);

        return *this;
    }

    ~avl_tree() {
        delete_tree();
    }
};

template <typename ElemT, typename Comparator = std::less<ElemT>>
size_t range_query(const avl_tree<ElemT, Comparator>& tree, const ElemT& first, const ElemT& second) { 
    if (first > second) 
        return 0;
        
    avl_node<ElemT>* lower = tree.lower_bound(first);
    avl_node<ElemT>* upper = tree.upper_bound(second);
    
    return tree.my_distance(lower, upper);
}

} // namespace AVLTree
