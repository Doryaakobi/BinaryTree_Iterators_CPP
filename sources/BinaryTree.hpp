#pragma once
#include <iostream>
#include <list>
#include <map>
#include <stack>
#include <vector>
#include <unordered_set>
#include <fstream>
#include <sstream>


namespace ariel{

    enum iterator_type {postOrder,inOrder,preOrder};   

    template <class T>

    class BinaryTree{
    

    private:
        typedef struct node{
            T mValue;
            struct node *mRight, *mLeft;

            node(T data) : mValue(data), mRight(nullptr), mLeft(nullptr) {}
        } Node;

        Node *root;
        std::string result = "";
        std::ostringstream stream;

    public:
        
        BinaryTree() { root = nullptr; }

        ~BinaryTree() { clearTree(); }

        BinaryTree(const BinaryTree &someTree){
            clearTree();
            createTree(someTree);
        }

        BinaryTree(BinaryTree &&someTree) noexcept{
            root = someTree.root;
            someTree.root = nullptr;
        }

        BinaryTree &operator=(const BinaryTree &someTree){
            if (this != &someTree){
                clearTree();
                createTree(someTree);
            }
            return *this;
        }

        BinaryTree &operator=(BinaryTree &&other) noexcept{
            root = other.root;
            other.root = nullptr;
            return *this;
        }

        BinaryTree &add_root(const T &node);
        BinaryTree &add_left(const T &pNode, const T &val);
        BinaryTree &add_right(const T &pNode, const T &val);

        void clearTree(){
            if (root){
                return;
            }
            for (auto it = begin(); it != end(); ++it){
                Node *temp = it.getCurr();
                delete temp;
            }
        }

        void copyTree(Node *nRoot, Node *subTree){
            if (subTree->mLeft){
                nRoot->mLeft = new Node(subTree->mLeft->mValue);
                copyTree(nRoot->mLeft, subTree->mLeft);
            }
            if (subTree->mRight){
                nRoot->mRight = new Node(subTree->mRight->mValue);
                copyTree(nRoot->mRight, subTree->mRight);
            }
        }

        void createTree(const BinaryTree &tree){
            if (!tree.root){
                return;
            }
            root = new Node(tree.root->mValue);
            copyTree(root, tree.root);
        }

        Node *findNode(T value){
            if (root){
                for (auto it = begin_inorder(); it != end_inorder(); ++it){
                    if (*it == value){
                        return it.getCurr();
                    }
                }
            }
            return nullptr;
        }


        std::string printTree(){
            return printBinaryTreeVisualy(this->root);
        }


        std::string printBinaryTreeVisualy(Node *_root){
            stream << "\n*********************************\n\n";
            printVisualyTree2(_root, 0 );
            stream << "\n*********************************\n";
            return stream.str();  
        }

        void printVisualyTree(Node *root, int level){
            if (root == nullptr){
                return;
            }
            printVisualyTree(root->mRight, level + 1);
            if (level != 0){
                for (int i = 0; i < level - 1; i++){
                    stream << "|\t";
                }
                stream <<"|-------";
                stream << root->mValue;
                stream <<"\n";
            }
            else{
                stream << root->mValue;
                stream << "\n";
            }
            printVisualyTree(root->mLeft, level + 1);
        }


        void printVisualyTree2(Node *root, int level){
            if (root == nullptr){
                return;
            }
            printVisualyTree2(root->mRight, level + 1);
            if (level != 0){
                for (int i = 0; i < level - 1; i++){
                    stream << "|\t";
                }
                stream << "|-------" << root->mValue << std::endl;
            }else{
                stream << root->mValue << std::endl;
            }
            printVisualyTree2(root->mLeft, level + 1);
        }

        const friend std::ostream &operator<<(std::ostream &os, BinaryTree &tree) {
            return os << tree.printTree() << std::endl;
        }


        class iterator{
        public:
            Node *curr_node;
            Node *temp;
            std::stack<Node *> stk;
            iterator_type type;
            std::unordered_set<Node *> ordered;
            iterator(Node *root, iterator_type type) : type(type){
                if(type == iterator_type::inOrder){
                    inorder_cons(root);
                }else if(type == iterator_type::postOrder){
                    postorder_cons(root);
                }else{
                    preorder_cons(root);
                }
            }

            void preorder_cons(Node *root = nullptr){
                if (root == nullptr){
                    curr_node = nullptr;
                    return;
                }
                stk.push(root);
                curr_node = stk.top();
            }


            void inorder_cons(Node *root = nullptr){
                while (root != nullptr){
                    stk.push(root);
                    root = root->mLeft;
                }
                if (stk.empty()){
                    curr_node = nullptr;
                }
                else{
                    curr_node = stk.top();
                    stk.pop();
                }
            }


            void postorder_cons(Node *root = nullptr){

                if (root == nullptr) {
                    curr_node = nullptr;
                }
                else{
                    temp = root;
                    while (temp != nullptr && ordered.find(temp) == ordered.end()){
                    if (temp->mLeft != nullptr && ordered.find(temp->mLeft) == ordered.end()){
                        temp = temp->mLeft;
                    }
                    else if (temp->mRight && ordered.find(temp->mRight) == ordered.end()){
                        temp = temp->mRight;
                    }
                    else{
                        iterator::curr_node = temp;
                        ordered.insert(temp);
                        temp = root;
                        break;
                    }
                }
                }
            }
            
            T &operator*() const{
                return curr_node->mValue;
            }

            T *operator->() const{
                return &(curr_node->mValue);
            }

            bool operator==(const iterator &itr) const{
                return (curr_node == itr.curr_node);
            }

            bool operator!=(const iterator &itr) const{
                return !(curr_node == itr.curr_node);
            }

            Node *getCurr(){
                return curr_node;
            }


            iterator &operator++() {
                switch (type){
                case iterator_type::inOrder:
                   return inorder_operatorPlus();
                case iterator_type::postOrder:
                   return postorder_operatorPlus();
                case iterator_type::preOrder:
                   return preorder_operatorPlus();
                }
            }

            iterator &postorder_operatorPlus(){
                Node *node = temp;
                if (temp && ordered.find(temp) == ordered.end()){
                    while (temp != nullptr && ordered.find(temp) == ordered.end()){
                    if (temp->mLeft != nullptr && ordered.find(temp->mLeft) == ordered.end()){
                        temp = temp->mLeft;
                    }
                    else if (temp->mRight && ordered.find(temp->mRight) == ordered.end()){
                        temp = temp->mRight;
                    }
                    else{
                        iterator::curr_node = temp;
                        ordered.insert(temp);
                        temp = node;
                        break;
                    }
                }
                }
                else{
                    iterator::curr_node = nullptr;
                }
                return *this;
            }

            iterator &inorder_operatorPlus(){
                if (stk.empty() && iterator::curr_node == nullptr){
                    iterator::curr_node = nullptr;
                }
                else{
                    if (iterator::curr_node->mRight == nullptr){
                        if (!stk.empty()){
                            iterator::curr_node = stk.top();
                            stk.pop();
                        }
                        else{
                            iterator::curr_node = nullptr;
                        }
                    }
                    else{
                        iterator::curr_node = iterator::curr_node->mRight;
                        while (iterator::curr_node != nullptr)
                        {
                            stk.push(iterator::curr_node);
                            iterator::curr_node = iterator::curr_node->mLeft;
                        }
                        iterator::curr_node = stk.top();
                        stk.pop();
                    }
                }
                return *this;
            }

            iterator &preorder_operatorPlus(){
                if (stk.empty()){
                    curr_node = nullptr;
                }
                else{
                    Node *node = curr_node;
                    stk.pop();
                    if (node->mRight != nullptr){
                        stk.push(node->mRight);
                    }
                    if (node->mLeft != nullptr){
                        stk.push(node->mLeft);
                    }
                    if (!(stk.empty())){
                        curr_node = stk.top();
                    }
                    else{
                        curr_node = nullptr;
                    }
                }
                return *this;
            }

            iterator operator++(int){

                switch (type){
                case iterator_type::inOrder:
                   return inorder_operatorPlus2();
                case iterator_type::postOrder:
                   return postorder_operatorPlus2();
                case iterator_type::preOrder:
                   return preorder_operatorPlus2();
                }
            }

            iterator preorder_operatorPlus2(){
                iterator it = *this;
                if (stk.empty()){
                    iterator::curr_node = nullptr;
                }
                else{
                    Node *node = iterator::curr_node;
                    stk.pop();
                    if (node->mRight != nullptr){
                        stk.push(node->mRight);
                    }
                    if (node->mLeft != nullptr){
                        stk.push(node->mLeft);
                    }
                    if (!(stk.empty())){
                        iterator::curr_node = stk.top();
                    }
                    else{
                        iterator::curr_node = nullptr;
                    }
                }
                return it;
            }

            iterator postorder_operatorPlus2(){
                iterator it = *this;
                Node *node = temp;
                if (temp && ordered.find(temp) == ordered.end()){
                   while (temp != nullptr && ordered.find(temp) == ordered.end()){
                    if (temp->mLeft != nullptr && ordered.find(temp->mLeft) == ordered.end()){
                        temp = temp->mLeft;
                    }
                    else if (temp->mRight && ordered.find(temp->mRight) == ordered.end()){
                        temp = temp->mRight;
                    }
                    else{
                        iterator::curr_node = temp;
                        ordered.insert(temp);
                        temp = node;
                        break;
                    }
                    }
                }
                else{
                    iterator::curr_node = nullptr;
                }
                return it;
            }
            
            iterator inorder_operatorPlus2(){
                iterator it = *this;
                if (stk.empty() && iterator::curr_node == nullptr){
                    iterator::curr_node = nullptr;
                }
                else{
                    if (iterator::curr_node->mRight == nullptr){
                        if (!stk.empty()){
                            iterator::curr_node = stk.top();
                            stk.pop();
                        }
                        else{
                            iterator::curr_node = nullptr;
                        }
                    }
                    else{
                        iterator::curr_node = iterator::curr_node->mRight;
                        while (iterator::curr_node != nullptr){
                            stk.push(iterator::curr_node);
                            iterator::curr_node = iterator::curr_node->mLeft;
                        }
                        iterator::curr_node = stk.top();
                        stk.pop();
                    }
                }
                return it;
            }

        };


        iterator begin_preorder() { return iterator{root, iterator_type::preOrder}; }
        iterator end_preorder() { return iterator{nullptr, iterator_type::preOrder}; }
        iterator begin_inorder() { return iterator{root, iterator_type::inOrder}; }
        iterator end_inorder() { return iterator{nullptr, iterator_type::inOrder}; }
        iterator begin() { return iterator{root, iterator_type::inOrder}; }
        iterator end() { return iterator{nullptr, iterator_type::inOrder}; }
        iterator begin_postorder() { return iterator{root, iterator_type::postOrder}; }
        iterator end_postorder() { return iterator{nullptr, iterator_type::postOrder}; }
    };

    template <typename T>
    BinaryTree<T> &BinaryTree<T>::add_root(const T &node){
        if (root == nullptr){
            root = new Node(node);
        }
        else{
            root->mValue = node;
        }
        return *this;
    }

    template <typename T>
    BinaryTree<T> &BinaryTree<T>::add_right(const T &pNode, const T &val){
        Node *temp = findNode(pNode);
        if (temp == nullptr){
            throw std::invalid_argument("Unable to add to a non-existing node in the tree!");
        }
        if (temp->mRight == nullptr){
            temp->mRight = new Node(val);
        }
        else{
            temp->mRight->mValue = val;
        }
        return *this;
    }

    template <typename T>
    BinaryTree<T> &BinaryTree<T>::add_left(const T &pNode, const T &val){
        Node *temp = findNode(pNode);
        if (temp == nullptr){
            throw std::invalid_argument("Unable to add to a non-existing node in the tree!");
        }
        if (temp->mLeft == nullptr){
            temp->mLeft = new Node(val);
        }
        else{
            temp->mLeft->mValue = val;
        }
        return *this;
    }

};