#pragma once
#include <memory>
#include "utils.hpp"

namespace ft
{
    template <class T, class Compare = std::less<typename T::first_type>, class Alloc = std::allocator<T> >
    class AVLTree 
    {
    public:
        typedef Alloc allocator_type;
        typedef T value_type;
        typedef typename T::first_type key_type;
        typedef typename T::second_type mapped_type;
        typedef Compare key_compare;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;
        typedef T &reference;
        typedef T *pointer;

        // data structure that represents a node in the tree
        struct Node
        {
            value_type data; // holds the key
            Node *parent;    // pointer to the parent
            Node *left;      // pointer to left child
            Node *right;     // pointer to right child
            int height;          // balance factor of the node

            Node *min()
            {
                Node *cur = this;
                while (cur->left != nullptr)
                    cur = cur->left;
                return cur;
            }

            Node *max()
            {
                Node *cur = this;
                while (cur->right != nullptr)
                    cur = cur->right;
                return cur;
            }
        };

        class value_compare
		{
			friend class  AVLTree;
			protected:
				Compare comp;
				value_compare (key_compare c) : comp(c) {}
			public:
				typedef bool		result_type;
				typedef value_type	first_argument_type;
				typedef value_type	second_argument_type;
				bool operator() (const value_type& x, const value_type& y) const {
					return comp(x.first, y.first); }
	    };

        value_compare	value_comp() const { return value_compare(_comp); }

    private:
        Node*                   _root;
        Node*                   _end_node;
        allocator_type          _allocValue;
        std::allocator<Node>    _allocNode;
        size_type               _size;
        key_compare             _comp; 

        // A utility function to get height
        // of the tree
        int height(Node *N) const
        {
            if (N == NULL)
                return 0;
            return N->height;
        }

        /* Helper function that allocates a
           new node with the given key and
           NULL left and right pointers. */
        Node *newNode(value_type key, Node* parent)
        {
            // Node *node = new Node();
            Node* node = _allocNode.allocate(1);
            // node->key = key;
            _allocValue.construct(&node->data, key);
            node->left = NULL;
            node->right = NULL;
            node->height = 1; // new node is initially
                              // added at leaf
            node->parent = parent;
            return (node);
        }

        // A utility function to right
        // rotate subtree rooted with y
        // See the diagram given above.
        Node *rightRotate(Node *y)
        {
            Node *x = y->left;
            Node *T2 = x->right;

            x->parent = y->parent;
            y->parent = x;
            if (T2 != NULL)
                T2->parent = y;
            // Perform rotation
            x->right = y;
            // x->right->parent = x;
            y->left = T2;
            // y->left->parent = y;

            // Update heights
            // y->height = std::max(height(y->left), height(y->right)) + 1;
            y->height = (height(y->left) > height(y->right) ? height(y->left) + 1 : height(y->right)) + 1;
            // x->height = std::max(height(x->left), height(x->right)) + 1;
            x->height = (height(x->left) > height(x->right) ? height(x->left) + 1 : height(x->right)) + 1;

            // Return new_root 
            return x;
        }

        // A utility function to left
        // rotate subtree rooted with x
        // See the diagram given above.
        Node *leftRotate(Node *x)
        {
            Node *y = x->right;
            Node *T2 = y->left;

            y->parent = x->parent;
            x->parent = y;
            if (T2 != NULL)
                T2->parent = x;
            // Perform rotation
            y->left = x;
            x->right = T2;

            // Update heights
            // x->height = std::max(height(x->left), height(x->right)) + 1;
            x->height = (height(x->left) > height(x->right) ? height(x->left) + 1 : height(x->right)) + 1;
            // y->height = std::max(height(y->left), height(y->right)) + 1;
            y->height = (height(y->left) > height(y->right) ? height(y->left) + 1 : height(y->right)) + 1;
            // Return new_root 
            return y;
        }

        // Get Balance factor of node N
        int getBalance(Node *N) const
        {
            if (N == NULL)
                return 0;
            return height(N->left) -
                   height(N->right);
        }

        Node* search(key_type key) const
        {
            Node* cur = _root;
            while (cur != NULL)
            {
                if (_comp(key, cur->data.first))
                    cur = cur->left;
                else if (_comp(cur->data.first, key))
                    cur = cur->right;
                else
                    return cur;
            }
            return NULL;
        }

        Node *insertp(Node *node, value_type key, Node* parent = NULL)
        {
            /* 1. Perform the normal BST rotation */
            if (node == NULL){
                _size++;
                return (newNode(key, parent));
            }

            if (_comp(key.first, node->data.first))
                node->left = insertp(node->left, key, node);
            else if (_comp(node->data.first, key.first))
                node->right = insertp(node->right, key, node);
            else // Equal keys not allowed
                return node;

            /* 2. Update height of this ancestor node */
            // node->height = 1 + std::max(height(node->left), height(node->right));
            node->height = (height(node->left) > height(node->right) ? height(node->left) : height(node->right)) + 1;

            /* 3. Get the balance factor of this
                ancestor node to check whether
                this node became unbalanced */
            int balance = getBalance(node);

            // If this node becomes unbalanced,
            // then there are 4 cases

            // Left Left Case
            if (balance > 1 && _comp(key.first, node->left->data.first))
                return rightRotate(node);

            // Right Right Case
            if (balance < -1 && _comp(node->right->data.first, key.first))
                return leftRotate(node);

            // Left Right Case
            if (balance > 1 && _comp(node->left->data.first, key.first))
            {
                node->left = leftRotate(node->left);
                return rightRotate(node);
            }

            // Right Left Case
            if (balance < -1 && _comp(key.first, node->right->data.first))
            {
                node->right = rightRotate(node->right);
                return leftRotate(node);
            }
            set_end_node();
            /* return the (unchanged) node pointer */
            return node;
        }

        /* Given a non-empty binary search tree,
        return the node with minimum key value
        found in that tree. Note that the entire
        tree does not need to be searched. */
        Node *minValueNode(Node *node) const
        {
            if (!node)
                return NULL;
            Node *current = node;

            /* loop down to find the leftmost leaf */
            while (current->left != NULL)
                current = current->left;

            return current;
        }

        Node *maxValueNode(Node *node) const
        {
            if (!node)
                return NULL;
            Node *current = node;

            /* loop down to find the leftmost leaf */
            while (current->right != NULL)
                current = current->right;

            return current;
        }

        // Recursive function to delete a node
        // with given key from subtree with
        // given _root. It returns _root of the
        // modified subtree.
        Node *deleteNodep(Node *node, key_type key)
        {

            // STEP 1: PERFORM STANDARD BST DELETE
            if (node == NULL)
                return node;

            // If the key to be deleted is smaller
            // than the _root's key, then it lies
            // in left subtree
            if (_comp(key, node->data.first))
                node->left = deleteNodep(node->left, key);

            // If the key to be deleted is greater
            // than the _root's key, then it lies
            // in right subtree
            else if (_comp(node->data.first, key))
                node->right = deleteNodep(node->right, key);

            // if key is same as _root's key, then
            // This is the node to be deleted
            else
            {
                // node with only one child or no child
                if ((node->left == NULL) ||
                    (node->right == NULL))
                {
                    Node *temp = node->left ? node->left : node->right;

                    // No child case
                    if (temp == NULL)
                    {
                        temp = node;
                        node = NULL;
                    }
                    else               // One child case
                        *node = *temp; // Copy the contents of
                                       // the non-empty child
                    // free(temp);
                    if(node)
                        node->parent = temp->parent;


                    _allocValue.destroy(&temp->data);
                    _allocNode.deallocate(temp, 1);
                    _size--;
                    set_end_node();
                }
                else
                {
						// find the greatest smaller
						Node *tmp = minValueNode(node->right);

						// switch them
						if (tmp != node->right)
						{
							tmp->right = node->right;
							node->right->parent = tmp;
						}
						if (tmp != node->left)
						{
							tmp->left = node->left;
							node->left->parent = tmp;
						}
						tmp->parent->left = 0;
						tmp->parent = node->parent;
						// destroy it
						this->_allocValue.destroy(&node->data);
						this->_allocNode.deallocate(node, 1);
                        _size--;
						node = tmp;
                        set_end_node();
					}
            }

            // If the tree had only one node
            // then return
            if (node == NULL)
                return node;

            // STEP 2: UPDATE HEIGHT OF THE CURRENT NODE
            // node->height = 1 + std::max(height(node->left), height(node->right));
            node->height = (height(node->left) > height(node->right) ? height(node->left) : height(node->right)) + 1;

            // STEP 3: GET THE BALANCE FACTOR OF
            // THIS NODE (to check whether this
            // node became unbalanced)
            int balance = getBalance(node);

            // If this node becomes unbalanced,
            // then there are 4 cases

            // Left Left Case
            if (balance > 1 &&
                getBalance(node->left) >= 0)
                return rightRotate(node);

            // Left Right Case
            if (balance > 1 &&
                getBalance(node->left) < 0)
            {
                node->left = leftRotate(node->left);
                return rightRotate(node);
            }

            // Right Right Case
            if (balance < -1 &&
                getBalance(node->right) <= 0)
                return leftRotate(node);

            // Right Left Case
            if (balance < -1 &&
                getBalance(node->right) > 0)
            {
                node->right = rightRotate(node->right);
                return leftRotate(node);
            }

            return node;
        }

        // size_type max_size(void) const
        // {
        //     return ft::numeric_limits<size_type>::max();
        // }

        // A utility function to print preorder
        // traversal of the tree.
        // The function also prints height
        // of every node
        // void preOrder(Node *_root)
        // {
        //     if (_root != NULL)
        //     {
        //         std::cout << _root->key.first << " ";
        //         preOrder(_root->left);
        //         preOrder(_root->right);
        //     }
        // }

        // void padding(char c, int n) const
        // {
        //     for (int i = 0; i < n; i++)
        //         std::cout << c;
        // }
            
        // void print_tree(Node *_root, int lvl = 0) const 
        // {
        //     if (_root == NULL)
        //     {
        //         padding('\t', lvl);
        //         std::cout << "~";
        //     }
        //     else
        //     {
        //         print_tree(_root->left, lvl + 1);
        //         padding('\t', lvl);
        //         std::cout << _root->data.first << ":" << _root->data.second << ";";
        //         // if (_root->parent)

        //         // std::cout  << (_root->parent->data.first) << ";";
        //         if (_root->parent)
        //             std::cout << _root->parent->data.first;
        //         else
        //             std::cout << "NULL";
        //         std::cout << std::endl;
        //         print_tree(_root->right, lvl + 1);
        //     }
        //     std::cout << std::endl;
        // }
                
        void set_end_node(void)
        {
            if (_size)
                _end_node->parent = maxValueNode(_root);
            else
                _end_node->parent = nullptr;
            // _end_node->parent = (_root != nullptr ? maximum(getRoot()) : nullptr);
            _end_node->left = _end_node->right = nullptr;
        }

    public :

         AVLTree(const key_compare &comp = key_compare(),
             const allocator_type &alloc = allocator_type())
            : _allocValue(alloc), _comp(comp)
        {
            _root = NULL;
            _size = 0;
            _end_node = _allocNode.allocate(1);
            set_end_node();
        }

         AVLTree(const  AVLTree &x) { *this = x; }
        
        ~ AVLTree() { clear(); _allocNode.deallocate(_end_node, 1); }

        void        clear() { 
            while(_root)
                _root = deleteNodep(_root, _root->data.first);
            set_end_node();
        }

        size_type   size(void) const { return _size; }
        Node*       minimum(void) const { return (_size ? minValueNode(_root) : NULL); }
        Node*       maximum(void) const { return (_size ? maxValueNode(_root) : NULL); }
        Node*       end_node(void) const { return _end_node; }
        void        insert(value_type v) { _root = insertp(_root, v); }
        Node*       searchTree(key_type key) const{ return search(key); }
        key_compare key_comp() const { return _comp; }
        void        prettyPrint() const { print_tree(this->_root); }
        void        deleteNode(key_type key) { _root = deleteNodep(_root, key); }
        size_type   max_size() const { return _allocNode.max_size(); }
    };
}