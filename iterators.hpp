#pragma once
#include "utils.hpp"
#include <memory>

namespace ft
{
	struct random_access_iterator_tag {};
	struct bidirectional_iterator_tag {};

    template <class Category, class T>
    class iterator_vector 
    {
        public:
            typedef T						value_type;
        	typedef value_type&				reference;
        	typedef const value_type&		const_reference;
        	typedef value_type*				pointer;
         	typedef const value_type*		const_pointer;
			typedef Category				iterator_category;
	        typedef typename std::ptrdiff_t difference_type;

            iterator_vector(pointer ptr = NULL) : _arr(ptr) {}

            iterator_vector(const iterator_vector &src) : _arr(src._arr) {}

            virtual ~iterator_vector(void) {}

            iterator_vector  &operator=(const iterator_vector &rhs) {
                _arr = rhs._arr;
                return *this;
            }

            pointer base() const 
                { return _arr; }

            bool    operator==(const iterator_vector &ref) const {
                return (_arr == ref._arr);
            }

            bool    operator!=(const iterator_vector &ref) const  {
                return (_arr != ref._arr);
            }

            reference   operator*(void) const {
                return *_arr;
            }
 
            pointer     operator->(void) const {
                return _arr;
            }
 
            iterator_vector operator++() {
                _arr++;
                return *this;
            }
 
            iterator_vector operator--() {
                _arr--;
                return *this;
            }
 
            iterator_vector operator++(int) {
                _arr++;
                return iterator_vector(_arr - 1);
            }
 
            iterator_vector operator--(int) {
                _arr--;
                return iterator_vector(_arr + 1);
            }
 
            iterator_vector operator +(difference_type n) {
                return iterator_vector(_arr + n);
            }
	
            iterator_vector operator -(difference_type n) {
                return iterator_vector(_arr - n);
            }

	        difference_type operator +(iterator_vector n) {
                return _arr + n._ptr;
            }

	        difference_type operator -(iterator_vector n) {
                return _arr - n._arr;
            }

            bool    operator<(const iterator_vector &ref) const {
                return (_arr < ref._arr);
            }

            bool    operator>(const iterator_vector &ref) const {
                return (_arr > ref._arr);
            }

            bool    operator<=(const iterator_vector &ref) const {
                return (_arr <= ref._arr);
            }

            bool    operator>=(const iterator_vector &ref) const {
                return (_arr >= ref._arr);
            }

            reference operator [](difference_type i) {
                return *(_arr + i);
            }

            iterator_vector  &operator+=(difference_type n) {
                _arr += n;
                return *this;
            }

            iterator_vector  &operator-=(difference_type n) {
                _arr -= n;
                return *this;
            }

        private:
            pointer _arr;
    };

	template <class Category, class T, class not_const_T = T>
	class iterator_map 
	{
	public:
		typedef typename T::value_type		value_type;
		typedef typename not_const_T::Node	Node;
		typedef typename T::key_type		key_type;
		typedef typename T::mapped_type		mapped_type;
		typedef typename T::key_compare		key_compare;
		typedef typename T::reference		reference;
		typedef typename T::pointer			pointer;
		typedef ptrdiff_t 					difference_type;
		typedef Category					iterator_category;

		iterator_map() {}
		iterator_map(const iterator_map<Category, not_const_T> &src) : _node(src.base()), _end(src.end()) {}
		iterator_map(Node *node, Node *end)
		{
			_node = node;
			_end = end;
		}
		virtual ~iterator_map() {}

		iterator_map&	operator=(const iterator_map<Category, not_const_T> &src){
			_node = src._node;
			_end = src._end;
			return *this;
		}

		friend bool		operator== (const iterator_map& lhs, const iterator_map& rhs) {
			return lhs._node == rhs._node; }
		friend bool 	operator!= (const iterator_map& lhs, const iterator_map& rhs) {
			return lhs._node != rhs._node; }

		reference operator*() const { return _node->data; }

		pointer operator->() const { return &(operator*()); }

		iterator_map operator--(int)
		{
			iterator_map tmp = *this;
			--*this;
			return tmp;
		}

		iterator_map &operator--()
		{
			if (_node == _end)
			{
				_node = _node->parent;
				return *this;
			}
			else if (_node->left)
			{
				_node = _node->left->max();
				return *this;
			}
			else if (_node->parent)
			{
				key_type key = _node->data.first;
				Node *tmp = _node->parent;
				while (tmp && _key_comp(key, tmp->data.first))
					tmp = tmp->parent;
				if (tmp)
				{
					_node = tmp;
					return *this;
				}
			}
			return *this;
		}

		iterator_map operator++(int)
		{
			iterator_map tmp = *this;
			++*this;
			return tmp;
		}

		iterator_map &operator++()
		{
			if (_node->right)
			{
				_node = _node->right->min();
				return *this;
			}
			else if (_node->parent)
			{
				key_type key = _node->data.first;
				Node *tmp = _node->parent;
				while (tmp && _key_comp(tmp->data.first, key))
					tmp = tmp->parent;
				if (tmp)
				{
					_node = tmp;
					return *this;
				}
			}
			_node = _end;
			return *this;
		}

		Node*	base() const { return _node; }
		Node*	end() const { return _end; }

	private:
		Node *_node;
		Node *_end;
		key_compare _key_comp;
	};
}