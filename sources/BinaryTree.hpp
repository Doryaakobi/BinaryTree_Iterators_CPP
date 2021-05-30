#include <iostream>
#include <typeinfo>
#include <stack>
#include <map>

namespace ariel {  

    enum traversal{InOrder,PreOrder,PostOrder};  

    template<typename T>
    class BinaryTree {
        private:

            struct Node {
                T val;
                Node *l_child, *r_child, *parent;
                Node(T value):val(value), l_child(nullptr), r_child(nullptr), parent(nullptr){} 
                ~Node(){delete l_child; delete r_child;}
            };

            std::map<T, Node*> tree;
            traversal seq;
            Node *root = nullptr;
            
            class iterator {
                private:
                    Node* curr_node_ptr;
                    traversal seq;
                    std::stack<Node*> stk;
                public:
                    iterator(struct Node* n = nullptr):curr_node_ptr(n) {}
                    
                    iterator& operator++ (){
                        return *this;
                    }
                    iterator operator++ (int a){
                        iterator t{curr_node_ptr};
                        return t;
                    }

                    bool operator!= (const iterator& other){return false;}
                    const T& operator* () const {return curr_node_ptr->val;}
                    const T* operator->() const { return &(curr_node_ptr->val);}


                    void inOrder_seq(struct Node* root, std::stack<Node*>& stk) {
                        Node *curr = root;
                        
                            while (curr != NULL || stk.empty() == false)
                            {
                                /* Reach the left most Node of the
                                curr Node */
                                while (curr !=  NULL)
                                {
                                    /* place pointer to a tree node on
                                    the stack before traversing
                                    the node's left subtree */
                                    stk.push(curr);
                                    curr = curr->l_child;
                                }
                        
                                /* Current must be NULL at this point */
                                curr = stk.top();
                                stk.pop();
                        
                                std::cout << curr->val << " ";
                        
                                /* we have visited the node and its
                                left subtree.  Now, it's right
                                subtree's turn */
                                curr = curr->r_child;
                        
                            } /* end of while */
                        }
                    
                    
                    
                    void preOrder_seq(struct Node* root, std::stack<Node*>& stk) {
                    // Base Case
                        if(root == NULL)
                            return;
                    
                        // Create an empty stack and push root to it
                        stk.push(root);
                    
                        /* Pop all items one by one. Do following for every popped item
                        a) print it
                        b) push its right child
                        c) push its left child
                        Note that right child is pushed first so that left is processed first */
                        while (stk.empty() == false) {
                            // Pop the top item from stack and print it
                            Node* node = stk.top();
                            // printf("%f ", node->val);
                            stk.pop();
                    
                            // Push right and left children of the popped node to stack
                            if (node->r_child)
                                stk.push(node->r_child);
                            if (node->l_child)
                                stk.push(node->l_child);
                        }
                    }


                    void postOrder_seq(struct Node* root, std::stack<Node*>& stk) {
                        if (root == NULL)
                            return;
                    
                        // Create two stacks
                        std::stack<Node *> stk2;
                    
                        // push root to first stack
                        stk.push(root);
                        Node* node;
                    
                        // Run while first stack is not empty
                        while(!stk.empty()) {
                            // Pop an item from s1 and push it to s2
                            node = stk.top();
                            stk.pop();
                            stk2.push(node);
                    
                            // Push left and right children
                            // of removed item to s1
                            if (node->l_child)
                                stk.push(node->l_child);
                            if (node->r_child)
                                stk.push(node->r_child);
                        }
                    
                        // Print all elements of second stack
                        while(!stk2.empty()) {
                            node = stk2.top();
                            stk2.pop();
                            std::cout << node->val << " ";
                        }
                    }

                    iterator(traversal seq, struct Node* n = nullptr):curr_node_ptr(n), seq(seq) {
                        if (n == nullptr) return;
                        switch(seq) {
                            case traversal::PreOrder:
                                preOrder_seq(curr_node_ptr, stk);
                                break;
                            case traversal::InOrder:
                                inOrder_seq(curr_node_ptr, stk);
                                break;
                            case traversal::PostOrder:
                                postOrder_seq(curr_node_ptr, stk);
                                break;
                        }
                        stk.push(nullptr);            // Push last element to avoid an empty stack when pointing to last node.
                        curr_node_ptr = stk.top();  // Points to first node in reqeusted traverse order
                        stk.pop();                   // pop node out of the queue.
                    }



            };


        public:
            BinaryTree():root(nullptr){}


            BinaryTree<T>& add_root(const T& val){

                if(root == nullptr){
                    root = new Node(val);
                    tree[val] = root;
                }
                else{
                    tree[val] = root;
                }

                return *this;
            }



            BinaryTree<T>& add_left(const T& val1, const T& val2){
                
                if (root == nullptr){
                    throw std::invalid_argument("Tree is Empty! \n");
                }

                if (tree.count(val1) == 0){
                    throw std::invalid_argument("Unable to add left decendent, first value doens't exsit in tree \n");
                }

                if (tree[val1]->l_child == nullptr){

                    tree[val1]->l_child = new Node(val2);
                    tree[val2] = tree[val1]->l_child;
                    tree[val2]->parent = tree[val1];
                }
                else{
                    tree[val1]->l_child->val = val2;
                }

                return *this;
            }



            BinaryTree<T>& add_right(const T& val1, const T& val2){
                
                if (root == nullptr){
                    throw std::invalid_argument("Tree is Empty! \n");
                }

                if (tree.count(val1) == 0){
                    throw std::invalid_argument("Unable to add right decendent, first value doens't exsit in tree \n");
                }

                if (tree[val1]->r_child == nullptr){

                    tree[val1]->r_child = new Node(val2);
                    tree[val2] = tree[val1]->r_child;
                    tree[val2]->parent = tree[val1];
                }
                else{
                    tree[val1]->r_child->val = val2;
                }

                return *this;
            }
            
            
            iterator begin_preorder(){return iterator{traversal::PreOrder,root};}
            iterator end_preorder(){return iterator{traversal::PreOrder,nullptr};} 

            iterator begin_inorder(){return iterator{traversal::InOrder,root};}
            iterator end_inorder(){return iterator{traversal::InOrder,nullptr};}

            iterator begin_postorder(){return iterator{traversal::PostOrder,root};}
            iterator end_postorder(){return iterator{traversal::PostOrder,nullptr};}
            
            iterator begin() {return iterator{root};}
            iterator end() {return iterator{nullptr};}
            
            friend std::ostream& operator <<(std::ostream& os, const BinaryTree& tree) {
                return os << "tree" << std::endl;
            }

    };
};