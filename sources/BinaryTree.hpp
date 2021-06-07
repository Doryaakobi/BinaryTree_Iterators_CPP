#pragma once
#include <iostream>
#include <list>
#include <map>
#include <stack>
#include <vector>
#include <unordered_set>

namespace ariel
{

    template <class T>

    class BinaryTree
    {
    private:
        typedef struct node
        {
            T mValue;
            struct node *mRight, *mLeft;

            node(T data) : mValue(data), mRight(nullptr), mLeft(nullptr) {}
        } Node;

        Node *root;

    public:
        BinaryTree() { root = nullptr; }

        ~BinaryTree() { clearTree(); }

        BinaryTree(const BinaryTree &someTree)
        {
            clearTree();
            createTree(someTree);
        }

        BinaryTree(BinaryTree &&someTree) noexcept
        {
            root = someTree.root;
            someTree.root = nullptr;
        }

        BinaryTree &operator=(const BinaryTree &someTree)
        {
            if (this != &someTree)
            {
                clearTree();
                createTree(someTree);
            }
            return *this;
        }

        BinaryTree &operator=(BinaryTree &&other) noexcept
        {
            root = other.root;
            other.root = nullptr;
            return *this;
        }

        BinaryTree &add_root(const T &node);
        BinaryTree &add_left(const T &pNode, const T &val);
        BinaryTree &add_right(const T &pNode, const T &val);

        void clearTree()
        {
            if (root)
            {
                return;
            }
            for (auto it = begin(); it != end(); ++it)
            {
                Node *temp = it.getCurr();
                delete temp;
            }
        }

        void copyTree(Node *nRoot, Node *subTree)
        {
            if (subTree->mLeft)
            {
                nRoot->mLeft = new Node(subTree->mLeft->mValue);
                copyTree(nRoot->mLeft, subTree->mLeft);
            }
            if (subTree->mRight)
            {
                nRoot->mRight = new Node(subTree->mRight->mValue);
                copyTree(nRoot->mRight, subTree->mRight);
            }
        }

        void createTree(const BinaryTree &tree)
        {
            if (!tree.root)
            {
                return;
            }
            root = new Node(tree.root->mValue);
            copyTree(root, tree.root);
        }

        Node *findNode(T value)
        {
            if (root)
            {
                for (auto it = begin_inorder(); it != end_inorder(); ++it)
                {
                    if (*it == value)
                    {
                        return it.getCurr();
                    }
                }
            }
            return nullptr;
        }

        /**
             * printTree - Function for printing the tree
             * @return: std::string - A string that represents all the vertices in the graph and their children
            */
        std::string printTree()
        {
            printBinaryTreeVisualy(this->root);
            print(root);
            return "";
        }

        /**
             * print - A recursive function that prints each vertex and its two children
             * @param: Node* node - Starts at the parent vertex of the tree and changes as it runs
             */
        void print(Node *node)
        {
            if (node == nullptr)
            {
                return;
            }
            std::cout << "(" << node->mValue << ") -> ";
            if (node->mLeft == nullptr)
            {
                std::cout << "(L-null,";
            }
            else
            {
                std::cout << "(" << node->mRight->mValue << ", ";
            }
            if (node->mRight == nullptr)
            {
                std::cout << "R-null)" << std::endl;
            }
            else
            {
                std::cout << node->mLeft->mValue << ")" << std::endl;
            }
            print(node->mRight);
            print(node->mLeft);
        }

        /**
             * printBinaryTreeVisualy - A recursive function that graphically prints the tree
             * @param: Node* _root - Begins at the parent vertex of the tree
             */
        void printBinaryTreeVisualy(Node *_root)
        {
            std::cout << "================================" << std::endl;
            printVisualyTree(_root, 0);
            std::cout << "================================" << std::endl;
        }

        /**
             * printVisualyTree - A recursive function that prints the tree graphically
             * @param: Node* root - Starts at the parent vertex of the tree and changes as it runs
             * @param: int level - The level of the tree where we are
             */
        void printVisualyTree(Node *root, int level)
        {
            if (root == nullptr)
            {
                return;
            }
            printVisualyTree(root->mRight, level + 1);
            if (level != 0)
            {
                for (int i = 0; i < level - 1; i++)
                {
                    std::cout << "|\t";
                }
                std::cout << "|-------" << root->mValue << std::endl;
            }
            else
            {
                std::cout << root->mValue << std::endl;
            }
            printVisualyTree(root->mLeft, level + 1);
        }

        friend std::ostream &operator<<(std::ostream &os, BinaryTree &tree)
        {
            return os << tree.printTree() << std::endl;
        }

        /**
         * iterator - main iterator class which contains the basic funcionality of the an iterator,
         * other BinaryTree iterators function differently when trasversing over a tree therefore
         * it would be much easier to implement different derrived iterators from this main one.
        */
        class iterator
        {
        public:
            Node *curr_node;

            T &operator*() const
            {
                return curr_node->mValue;
            }

            T *operator->() const
            {
                return &(curr_node->mValue);
            }

            bool operator==(const iterator &itr) const
            {
                return (curr_node == itr.curr_node);
            }

            bool operator!=(const iterator &itr) const
            {
                return !(curr_node == itr.curr_node);
            }

            Node *getCurr()
            {
                return curr_node;
            }
        };

        class preorder_iterator : public iterator
        {
        private:
            std::stack<Node *> stk;

        public:
            preorder_iterator(Node *root = nullptr)
            {
                if (root == nullptr)
                {
                    iterator::curr_node = nullptr;
                    return;
                }
                stk.push(root);
                iterator::curr_node = stk.top();
            }

            /**
             * Trasversal algorithm taken from GeeksforGeeks
             * https://www.geeksforgeeks.org/iterative-preorder-traversal/
            */
            preorder_iterator &operator++()
            {
                if (stk.empty())
                {
                    iterator::curr_node = nullptr;
                }
                else
                {
                    Node *node = iterator::curr_node;
                    stk.pop();
                    if (node->mRight != nullptr)
                    {
                        stk.push(node->mRight);
                    }
                    if (node->mLeft != nullptr)
                    {
                        stk.push(node->mLeft);
                    }
                    if (!(stk.empty()))
                    {
                        iterator::curr_node = stk.top();
                    }
                    else
                    {
                        iterator::curr_node = nullptr;
                    }
                }
                return *this;
            }
            
            /**
             * Trasversal algorithm taken from GeeksforGeeks
             * https://www.geeksforgeeks.org/iterative-preorder-traversal/
            */
            preorder_iterator operator++(int)
            {
                preorder_iterator it = *this;
                if (stk.empty())
                {
                    iterator::curr_node = nullptr;
                }
                else
                {
                    Node *node = iterator::curr_node;
                    stk.pop();
                    if (node->mRight != nullptr)
                    {
                        stk.push(node->mRight);
                    }
                    if (node->mLeft != nullptr)
                    {
                        stk.push(node->mLeft);
                    }
                    if (!(stk.empty()))
                    {
                        iterator::curr_node = stk.top();
                    }
                    else
                    {
                        iterator::curr_node = nullptr;
                    }
                }
                return it;
            }
        };



        /*
        Trasversal algorithm taken from GeeksforGeeks
        https://www.geeksforgeeks.org/inorder-tree-traversal-without-recursion/
        */
        class inorder_iterator : public iterator
        {
        private:
            std::stack<Node *> stk;

        public:
            inorder_iterator(Node *root = nullptr)
            {
                while (root != nullptr)
                {
                    stk.push(root);
                    root = root->mLeft;
                }
                if (stk.empty())
                {
                    iterator::curr_node = nullptr;
                }
                else
                {
                    iterator::curr_node = stk.top();
                    stk.pop();
                }
            }

            inorder_iterator &operator++()
            {
                if (stk.empty() && iterator::curr_node == nullptr)
                {
                    iterator::curr_node = nullptr;
                }
                else
                {
                    if (iterator::curr_node->mRight == nullptr)
                    {
                        if (!stk.empty())
                        {
                            iterator::curr_node = stk.top();
                            stk.pop();
                        }
                        else
                        {
                            iterator::curr_node = nullptr;
                        }
                    }
                    else
                    {
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

            inorder_iterator operator++(int)
            {
                inorder_iterator it = *this;
                if (stk.empty() && iterator::curr_node == nullptr)
                {
                    iterator::curr_node = nullptr;
                }
                else
                {
                    if (iterator::curr_node->mRight == nullptr)
                    {
                        if (!stk.empty())
                        {
                            iterator::curr_node = stk.top();
                            stk.pop();
                        }
                        else
                        {
                            iterator::curr_node = nullptr;
                        }
                    }
                    else
                    {
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
                return it;
            }
        };


        /*
        Trasversal algorithm taken from GeeksforGeeks
        https://www.geeksforgeeks.org/postorder-traversal-binary-tree-without-recursion-without-stack/ 
        */
        class postorder_iterator : public iterator
        {
        private:
            Node *temp;
            std::unordered_set<Node *> ordered;

        public:
            postorder_iterator(Node *root = nullptr)
            {
                if (root == nullptr)
                {
                    iterator::curr_node = nullptr;
                }
                else
                {
                    temp = root;
                    postOrdered(root);
                }
            }

            postorder_iterator &operator++()
            {
                Node *node = temp;
                if (temp && ordered.find(temp) == ordered.end())
                {
                    postOrdered(node);
                }
                else
                {
                    iterator::curr_node = nullptr;
                }
                return *this;
            }

            postorder_iterator operator++(int)
            {
                postorder_iterator it = *this;
                Node *node = temp;
                if (temp && ordered.find(temp) == ordered.end())
                {
                    postOrdered(node);
                }
                else
                {
                    iterator::curr_node = nullptr;
                }
                return it;
            }

            void postOrdered(Node *node)
            {
                while (temp != nullptr && ordered.find(temp) == ordered.end())
                {
                    if (temp->mLeft != nullptr && ordered.find(temp->mLeft) == ordered.end())
                    {
                        temp = temp->mLeft;
                    }
                    else if (temp->mRight && ordered.find(temp->mRight) == ordered.end())
                    {
                        temp = temp->mRight;
                    }
                    else
                    {
                        iterator::curr_node = temp;
                        ordered.insert(temp);
                        temp = node;
                        break;
                    }
                }
            }
        };

        preorder_iterator begin_preorder() { return preorder_iterator{root}; }
        preorder_iterator end_preorder() { return preorder_iterator{nullptr}; }

        inorder_iterator begin_inorder() { return inorder_iterator{root}; }
        inorder_iterator end_inorder() { return inorder_iterator{nullptr}; }
        inorder_iterator begin() { return inorder_iterator{root}; }
        inorder_iterator end() { return inorder_iterator{nullptr}; }

        postorder_iterator begin_postorder() { return postorder_iterator{root}; }
        postorder_iterator end_postorder() { return postorder_iterator{nullptr}; }
    };

    template <typename T>
    BinaryTree<T> &BinaryTree<T>::add_root(const T &node)
    {
        if (root == nullptr)
        {
            root = new Node(node);
        }
        else
        {
            root->mValue = node;
        }
        return *this;
    }

    template <typename T>
    BinaryTree<T> &BinaryTree<T>::add_right(const T &pNode, const T &val)
    {
        Node *temp = findNode(pNode);
        if (temp == nullptr)
        {
            throw std::invalid_argument("Unable to add to a non-existing node in the tree!");
        }
        if (temp->mRight == nullptr)
        {
            temp->mRight = new Node(val);
        }
        else
        {
            temp->mRight->mValue = val;
        }
        return *this;
    }

    template <typename T>
    BinaryTree<T> &BinaryTree<T>::add_left(const T &pNode, const T &val)
    {
        Node *temp = findNode(pNode);
        if (temp == nullptr)
        {
            throw std::invalid_argument("Unable to add to a non-existing node in the tree!");
        }
        if (temp->mLeft == nullptr)
        {
            temp->mLeft = new Node(val);
        }
        else
        {
            temp->mLeft->mValue = val;
        }
        return *this;
    }

};