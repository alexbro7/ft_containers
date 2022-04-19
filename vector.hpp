#pragma once
#include <memory>
#include "utils.hpp"
#include "iterators.hpp"
#include "reverse_iterator.hpp"

namespace ft
{
	template <typename T, class Alloc = std::allocator<T> >
	class vector{
	public:
		typedef T                   									value_type;
        typedef Alloc             		  								allocator_type;
		typedef	typename allocator_type::reference						reference;
		typedef	typename allocator_type::const_reference				const_reference;
		typedef	typename allocator_type::pointer						pointer;
		typedef	typename allocator_type::const_pointer					const_pointer;
		typedef iterator_vector<random_access_iterator_tag, value_type>	iterator;
		typedef iterator_vector<random_access_iterator_tag, value_type>	const_iterator;
		typedef reverse_iterator<const_iterator>						const_reverse_iterator;
		typedef reverse_iterator<iterator>								reverse_iterator;
		typedef typename std::ptrdiff_t									difference_type;
		typedef size_t              									size_type;

	// CONSTRUCTOR
		explicit vector (const allocator_type& alloc = allocator_type())
		: _size(0), _capacity(0), _alloc(alloc)
			{ _arr = NULL; }

		explicit vector (size_type n, const value_type& val = value_type(),
                 const allocator_type& alloc = allocator_type())
		:  _size(n), _capacity(n),_alloc(alloc) {
			if (n < 1)
				_arr = NULL;
			else {
				_arr = _alloc.allocate(n);
				for (size_type i = 0; i < n; i++)
					_alloc.construct(&_arr[i], val);
			}
		}

		template <class InputIterator>
        vector(InputIterator first, InputIterator last, const allocator_type& alloc = allocator_type(),
		typename ft::enable_if<!ft::is_integral<InputIterator>::value>::type * = 0)
		: _size(0),  _alloc(alloc) {
			for (InputIterator it = first; it != last; it++)
				++_size;
			_capacity = _size;
			if (!_size)
				_arr = NULL;
			else {
				_arr = _alloc.allocate(_capacity);
				for (size_type i = 0; i < _size; i++) {
					_alloc.construct(&_arr[i], *first);
					first++;
				}
			}
		}

		vector (const vector& x)
		: _size(x._size), _capacity(x._size), _alloc(x._alloc) {
			_arr = _alloc.allocate(_capacity);
			for (size_type i = 0; i < x._size; i++)
				_alloc.construct(&_arr[i], x._arr[i]);
        }

		vector &operator=(const vector &x) {
			if (_arr){
            	clear();
				_alloc.deallocate(_arr, _capacity);
			}
			_capacity = 0;
			_size = 0;
			_arr = NULL;
			reserve(x.size());
			while (_size < x._size) {
				_alloc.construct(&_arr[_size], x[_size]);
				_size++;
			}
			return *this;
		}

		~vector() {
			clear();
			if (!_arr)
			_alloc.deallocate(_arr, _capacity);
		}

	//ITERATOR
		iterator begin()
			{ return iterator(_arr); }

		const_iterator begin() const
			{ return const_iterator(_arr); }

		iterator end()
			{ return iterator(_arr + _size); }

		const_iterator end() const
			{ return const_iterator(_arr + _size); }

		reverse_iterator rbegin()
			{ return reverse_iterator(iterator(_arr + _size)); }

		const_reverse_iterator rbegin() const
			{ return const_reverse_iterator(iterator(_arr + _size)); }

		reverse_iterator rend()
			{ return reverse_iterator(iterator(_arr)); }

		const_reverse_iterator rend() const
			{ return const_reverse_iterator(iterator(_arr)); }

	//CAPACITY
		size_type size() const
			{ return _size; }

		size_type max_size() const {
        	// return (std::min((size_type) std::numeric_limits<difference_type>::max(), std::numeric_limits<size_type>::max() / sizeof(value_type)));}
        	if ((size_type) std::numeric_limits<difference_type>::max() < std::numeric_limits<size_type>::max() / sizeof(value_type))
				return (size_type) std::numeric_limits<difference_type>::max();
			return std::numeric_limits<size_type>::max() / sizeof(value_type);
		}

		void resize (size_type n, value_type val = value_type()) {
			while (_size > n) 
				pop_back();
			if (n > _capacity * 2)
				reserve(n);
			while (_size < n) {
				push_back(val);
			}
		}

		size_type capacity() const
			{ return _capacity; }

		bool empty() const
			{ return _size == 0; }

		void reserve(size_type n) {
			if (n <= _capacity)
				return;
			pointer tmp = _alloc.allocate(n);
			for (size_type i = 0; i < _size; i++) {
				_alloc.construct(&tmp[i], _arr[i]);
				_alloc.destroy(&_arr[i]);
			}
			_alloc.deallocate(_arr, _capacity);
			_arr = tmp;
			_capacity = n;
		}

	//ELEMENT ACCESS
		reference operator[] (size_type n)
			{ return _arr[n]; }

		const_reference operator[] (size_type n) const
			{ return _arr[n]; }

		reference at (size_type n){
			if (n >= _size)
				throw std::out_of_range("Out of range");
			return _arr[n];
		}

		const_reference at (size_type n) const {
			if (n >= _size)
				throw std::out_of_range("Out of range");
			return _arr[n];
		}

		reference front()
			{ return _arr[0]; }

		const_reference front() const
			{ return _arr[0]; }

		reference back()
			{ return _arr[_size - 1]; }

		const_reference back() const
			{ return _arr[_size - 1]; }

	//MODIFIERS
		template <class InputIterator>
		void assign (InputIterator first, InputIterator last,
		typename ft::enable_if<!ft::is_integral<InputIterator>::value, InputIterator>::type* = NULL){
			difference_type	n = last - first;
			while (_size) {
				_alloc.destroy(&_arr[_size - 1]);
				_size--;	
			}
			reserve(n);
			for (difference_type i = 0; i < n; i++) {
				_alloc.construct(&_arr[i], first[i]);
				_size++;	
			}
		}
  
        void assign(size_type n, const value_type& val) {
			while (_size) {
				_alloc.destroy(&_arr[_size - 1]);
				_size--;	
			}
			reserve(n);
			while (_size < n) {
				_alloc.construct(&_arr[_size], val);
				_size++;
			}
		}

		void push_back (const value_type& val)
		{
			if (_capacity == 0){
				_capacity = 1;
				_arr = _alloc.allocate(_capacity);
				_alloc.construct(&_arr[0], val);
				_size++;
				return;
			}
			if (_capacity > _size)
            {
                _alloc.construct(&_arr[_size], val);
                _size++;
                return;
            }
			_capacity *= 2;
			pointer tmp = _alloc.allocate(_capacity);
			for (size_type i = 0; i < _size; i++) {
				_alloc.construct(&tmp[i], _arr[i]);
				_alloc.destroy(&_arr[i]);
			}
			_alloc.deallocate(_arr, _capacity);
			_arr = tmp;
			_alloc.construct(&_arr[_size], val);
			_size++;
			return;
		}

		void pop_back() {
			if (_size) {
				_alloc.destroy(&_arr[_size - 1]);
				_size--;
			}
		}

		iterator insert (iterator position, const value_type& val) {
			difference_type n = position - begin();
			insert(position, 1, val);
			return (begin() + n);
		}

		void insert (iterator position, size_type n, const value_type& val) {
			iterator	it = end();
			vector		tmp(position, it);

			if (_size + n > _capacity * 2)
				reserve(_size + n);
			while (it != position){
				pop_back();
				it--;
			}
			while (n){
				push_back(val);
				n--;
			}
			it = tmp.begin();
			while (it != tmp.end()){
				push_back(*it);
				it++;
			}
		}

		template <class InputIterator>
    	void insert (iterator position, InputIterator first, InputIterator last,
		typename ft::enable_if<!ft::is_integral<InputIterator>::value>::type * = 0) {
			iterator	it = end();
			vector		tmp(position, it);
			if ((last - first) + _size > _capacity * 2)
				reserve((last - first) + _size);
			while (it != position){
				pop_back();
				it--;
			}
			while (first != last){
				push_back(*first);
				first++;
			}
			it = tmp.begin();
			while (it != tmp.end()){
				push_back(*it);
				it++;
			}
		}

		iterator erase (iterator position) {
			iterator	it = end();
			vector		tmp(position + 1, it);
			while (it != position){
				pop_back();
				it--;
			}
			for (iterator ite = tmp.begin(); ite != tmp.end(); ite++)
				push_back(*ite);
			return position;
		}

		iterator erase (iterator first, iterator last){
			vector tmp(last, end());
			iterator it = end() - 1;
			while (it != first - 1){
				pop_back();
				it--;
			}
			for (iterator ite = tmp.begin(); ite != tmp.end(); ite++)
				push_back(*ite);
			return first;
		}

		void swap (vector& x){
			pointer			arr = _arr;
			size_type		size = _size;
			size_type		capacity = _capacity;
			allocator_type	alloc = _alloc;
			_arr = x._arr;
			_size = x._size;
			_capacity = x._capacity;
			_alloc = x._alloc;
			x._arr = arr;
			x._size = size;
			x._capacity = capacity;
			x._alloc = alloc;
		}

		void	clear(){
			while (_size)
				pop_back();
		}

	//ALLOCATOR
		allocator_type get_allocator() const
			{ return _alloc; }
	//RELATIONAL OPERATORS
		friend bool operator== (const ft::vector<T,Alloc>& lhs, const ft::vector<T,Alloc>& rhs) {
			// if (!lhs.size() && !rhs.size())
			// 	return true;
			// if (lhs.size() && rhs.size())
			// 	return ft::equal(lhs.begin(), lhs.end(), rhs.begin());
			if (lhs._size != rhs._size)
				return false;
			for (size_type i = 0; i < lhs._size && i < rhs._size; i++)
				if (lhs[i] != rhs[i])
					return false;
			return true;
		}

		friend bool operator!=(const ft::vector<T,Alloc>& lhs, const ft::vector<T,Alloc>& rhs)
			{ return !(lhs == rhs); }

		friend bool operator< (const ft::vector<T,Alloc>& lhs, const ft::vector<T,Alloc>& rhs)
			{ return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()); }

		friend bool operator<= (const ft::vector<T,Alloc>& lhs, const ft::vector<T,Alloc>& rhs)
			{ return lhs < rhs ? true : lhs == rhs ? true : false; }

		friend bool operator>  (const ft::vector<T,Alloc>& lhs, const ft::vector<T,Alloc>& rhs)
			{ return !(lhs <= rhs); }

		friend bool operator>= (const ft::vector<T,Alloc>& lhs, const ft::vector<T,Alloc>& rhs)
			{ return !(lhs < rhs); }


	private:
		pointer			_arr;
		size_t			_size;
		size_t			_capacity;
		allocator_type	_alloc;
	};

	//NON_MEMBER FONCTIONS
	template <class T, class Alloc>
	void swap (ft::vector<T,Alloc>& x, ft::vector<T,Alloc>& y)
		{ x.swap(y); }
}