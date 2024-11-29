#include <stack>
#include <functional>

namespace AVLTree {

const char KEY   = 'k';
const char QUERY = 'q';

template <typename ElemT>
class avl_node {
public:
    ElemT     value_;
    avl_node* parent_ = nullptr;
    avl_node* left_   = nullptr;
    avl_node* right_  = nullptr;
    int       height_;
    int       size_of_subtree_;

public:
    avl_node(const ElemT& value, avl_node* parent = nullptr, avl_node* left = nullptr, avl_node* right = nullptr,
                                                             int height = 1, int size_of_subtree = 1):
    value_{value}, parent_{parent}, left_{left}, right_{right}, height_{height}, size_of_subtree_{size_of_subtree} {}
};

template <typename ElemT>
int get_height(const avl_node<ElemT>* node) {
    return node ? node->height_ : 0;
}

template <typename ElemT>
int get_size_of_subtree(const avl_node<ElemT>* node) {
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

    int size_left          = get_size_of_subtree(node->left_);
    int size_right         = get_size_of_subtree(node->right_);
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
        if (!root_) { 
            root_ = new avl_node<ElemT>{elem_to_insert};
            return;
        }

        avl_node<ElemT>* current = root_;
        avl_node<ElemT>* parent = nullptr;

        while (current) {
            parent = current;
            if (cmp_(elem_to_insert, current->value_))
                current = current->left_;
            else if (cmp_(current->value_, elem_to_insert))
                current = current->right_;
            else 
                return;
        }

        current = new avl_node<ElemT>{elem_to_insert, parent};

        if (cmp_(elem_to_insert, parent->value_))
            parent->left_ = current;
        else
            parent->right_ = current;

        avl_node<ElemT>* node = current;
        
        while (node) {
            node = balance(node);
            node = node->parent_;
        }
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

    int get_num_nodes_less_then_cur(const avl_node<ElemT>* node) const {
        if (!node) 
            return 0;

        int number_of_nodes = get_size_of_subtree(node->left_);
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

    int my_distance(const avl_node<ElemT>* lower, const avl_node<ElemT>* upper) const {
        if (!root_)
            return 0;

        if (!lower)
            return 0;

        if (!upper)
            return get_size_of_subtree(root_) - get_num_nodes_less_then_cur(lower);

        return get_num_nodes_less_then_cur(upper) - get_num_nodes_less_then_cur(lower);
    }

public:
    ~avl_tree() {
        if (!root_) 
            return;

        std::stack<avl_node<ElemT>*> node_stack;
        node_stack.push(root_);

        while (!node_stack.empty()) {
            avl_node<ElemT>* node = node_stack.top();
            node_stack.pop(); 

            if (node->left_) {
                node_stack.push(node->left_);
            }

            if (node->right_) {
                node_stack.push(node->right_);
            }

            delete node;
        }
    }
    
    avl_tree(const avl_tree<ElemT>& other_tree): cmp_{other_tree.cmp_} {
        if (!other_tree.root_) {
            return;
        }

        std::stack<std::pair<avl_node<ElemT>*, avl_node<ElemT>*>> node_stack; 

        root_ = new avl_node<ElemT>{other_tree.root_->value_, nullptr, nullptr, nullptr, other_tree.root_->height_,
                                    other_tree.root_->size_of_subtree_};
        node_stack.push({root_, other_tree.root_});
        
        while (!node_stack.empty()) {
            auto [current_copy, current_source] = node_stack.top();
            node_stack.pop();

            if (current_source->left_) {
                current_copy->left_ = 
                        new avl_node<ElemT>{current_source->left_->value_, current_copy, nullptr, nullptr, 
                                            current_source->left_->height_, current_source->left_->size_of_subtree_};

                node_stack.push({current_copy->left_, current_source->left_});
            }
            if (current_source->right_) {
                current_copy->right_ = 
                        new avl_node<ElemT>{current_source->right_->value_, current_copy, nullptr, nullptr, 
                                            current_source->right_->height_, current_source->right_->size_of_subtree_};
               
                node_stack.push({current_copy->right_, current_source->right_});
            }
        }
    }

    avl_tree(avl_tree<ElemT>&& other_tree) noexcept: root_{other_tree.root_} {
        other_tree.root_ = nullptr;
    }

    avl_tree<ElemT>& operator=(const avl_tree<ElemT>& other_tree) { 
        if (this == &other_tree)
            return *this;
        
        avl_tree<ElemT> tmp_tree{other_tree};
        std::swap(*this, tmp_tree);

        return *this;
    }

    avl_tree<ElemT>& operator=(avl_tree<ElemT>&& other_tree) noexcept {
        if (this == &other_tree) 
            return *this;

        // if lhs has root we have to delete lhs tree
        if (root_) {
            std::stack<avl_node<ElemT>*> node_stack;
            node_stack.push(root_);

            while (!node_stack.empty()) {
                avl_node<ElemT>* node = node_stack.top();
                node_stack.pop();

                if (node->left_) 
                    node_stack.push(node->left_);
                if (node->right_) 
                    node_stack.push(node->right_);
                delete node;
            }
        }

        root_ = other_tree.root_;
        other_tree.root_ = nullptr;

        std::swap(cmp_, other_tree.cmp_);

        return *this;
    }
};

template <typename ElemT, typename Comparator = std::less<ElemT>>
long range_query(const avl_tree<ElemT, Comparator>& tree, const ElemT& first, const ElemT& second) {
    if (first > second) 
        return 0;
        
    avl_node<ElemT>* lower = tree.lower_bound(first);
    avl_node<ElemT>* upper = tree.upper_bound(second);
    
    return tree.my_distance(lower, upper);
}

} // namespace AVLTree
