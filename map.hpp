#pragma once
#include "iterators.hpp"
#include "reverse_iterator.hpp"
#include "utils.hpp"
#include "AVLTree.hpp"
#include <memory>

namespace ft
{
    template <class Key, class T, class Compare = std::less<Key>,
              class Alloc = std::allocator<const pair<Key, T> > >
    class map
    {

    public:
    //                ----------------  TypeDEF ----------------
        typedef Key                                                         key_type;
        typedef T                                                           mapped_type;
        typedef ft::pair< key_type,mapped_type>                             value_type;
        typedef	Compare										                key_compare;
        typedef	Alloc										                allocator_type;
        typedef AVLTree<value_type, key_compare,  Alloc>			            tree;
        typedef AVLTree<const value_type, key_compare,  Alloc>		            const_tree;
        typedef typename tree::Node                                         Node;
        typedef typename tree::value_compare                                value_compare;
        typedef typename allocator_type::reference                          reference;
        typedef typename allocator_type::const_reference                    const_reference;
        typedef typename allocator_type::pointer                            pointer;
        typedef typename allocator_type::const_pointer                      const_pointer;
        typedef iterator_map<bidirectional_iterator_tag, tree>              iterator;
        typedef iterator_map<bidirectional_iterator_tag, const_tree, tree>  const_iterator;
        typedef reverse_iterator<const_iterator>                            const_reverse_iterator;
        typedef reverse_iterator<iterator>                                  reverse_iterator;
        typedef ptrdiff_t                                                   difference_type;
        typedef size_t                                                      size_type;

        //               ----------------  Constructor ----------------
        explicit map(const key_compare& comp = key_compare(),
		const allocator_type& alloc = allocator_type())
        : _tree(comp, alloc) {}

        template <class InputIterator>
        map (InputIterator first, InputIterator last,
        const key_compare& comp = key_compare(),
        const allocator_type& alloc = allocator_type())
        : _tree(comp, alloc) { insert(first, last); }

        map(const map& x) { *this = x; }

        map& operator= (const map& x) {
            _tree.clear();
            insert(x.begin(), x.end());
            return *this;
        }

        //               ----------------  Destructor ----------------
        ~map() {}

        //              ----------------  iterator ----------------
        iterator begin()
            { return iterator((_tree.size() ? _tree.minimum() : _tree.end_node()), _tree.end_node()); }

        const_iterator begin() const
            { return iterator((_tree.size() ? _tree.minimum() : _tree.end_node()), _tree.end_node()); }

        iterator end()
            { return iterator(_tree.end_node(), _tree.end_node()); }
        
        const_iterator end() const
            { return iterator(_tree.end_node(), _tree.end_node()); }
        
        reverse_iterator rbegin()
            { return reverse_iterator(end()); }

        const_reverse_iterator rbegin() const
            { return const_reverse_iterator(end()); }
        
        reverse_iterator rend()
            { return reverse_iterator(iterator(begin())); }
        
        const_reverse_iterator rend() const
            { return const_reverse_iterator(iterator(begin())); }
        

        //              ----------------  Capacity ----------------
        bool empty() const { return _tree.size() == 0; }
        size_type size() const { return _tree.size(); }
        size_type max_size() const { return _tree.max_size(); }

        //             ----------------  element access ----------------
        mapped_type& operator[] (const key_type& k) {
            iterator it = find(k);
            if (it != end())
                return (*it).second;
            else
            {
                insert(ft::make_pair(k, mapped_type()));
                return (*find(k)).second;
            }
        }
        //             ----------------  Modifiers ----------------
        pair<iterator,bool> insert(const value_type& val) {
			size_type n = size();
			_tree.insert(val);
			pair<iterator,bool> ret;
			ret.second = (n != size());
			ret.first = iterator(_tree.searchTree(val.first), _tree.end_node());
			return ret;
		}

        iterator insert (iterator position, const value_type& val)
        {
            (void)position;                       
            _tree.insert(val);
			pair<iterator,bool> ret;
            ret.first = iterator(this->_tree.searchTree(val.first), this->_tree.end_node());
            return ret.first;
        }

        template <class InputIterator>
        void insert (InputIterator first, InputIterator last){
            InputIterator it = first;
            while (it != last){
                // std::cout << "inserting " << (*it).first << std::endl;
                insert(*it);
                it++;
                // sleep(1);
            }
        }


        void swap (map& x){
            map tmp = *this;
            *this = x;
            x = tmp;
        }

        void erase (iterator position){
            _tree.deleteNode((*position).first);
        }

        void erase (iterator first, iterator last){
            iterator tmp;
            while (first != last){
                tmp = first;
                ++first;
                erase(tmp);
            }
        }

        size_type	erase(const key_type& k) {
                if (!count(k))
                    return 0;
                value_type data = *(find(k));
				_tree.deleteNode(data.first);
                return(1);
		}

        void clear(){
            _tree.clear();
        }
        //            ----------------  Observers ----------------
        key_compare key_comp() const { return _tree.key_comp(); }
        value_compare	value_comp() const { return _tree.value_comp(); }
        //            ----------------  Operations ----------------
        iterator find(const key_type& k)
        {
            Node *tmp = _tree.searchTree(k);
            if (tmp)
                return iterator(tmp, _tree.end_node());
            return end();
        }

        size_type count (const key_type& k) const
            { return _tree.searchTree(k) != NULL ? 1 : 0; }

        iterator    lower_bound(const key_type& k) {
				for (iterator it = begin(); it != end(); it++)
					if (!_tree.key_comp()((*it).first, k))
						return it;
				return end();
		}

        iterator    upper_bound(const key_type& k) {
				for (iterator it = begin(); it != end(); it++)
					if (_tree.key_comp()(k, (*it).first))
						return it;
				return end();
		}

        pair<iterator,iterator>	equal_range(const key_type& k) {
			for (iterator it = begin(); it != end(); it++)
				if (!_tree.key_comp()(k, (*it).first) && !_tree.key_comp()((*it).first, k))
					return pair<iterator, iterator>(it++, it);
			return pair<iterator, iterator>(lower_bound(k), lower_bound(k));
		}

        //          ----------------  Allocator ----------------
        // allocator_type get_allocator() const
        //     { return _tree.get_allocator(); }
 
        void print() const
        { _tree.prettyPrint(); }

    private:
        tree _tree;
    };
}