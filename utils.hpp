#pragma once
namespace ft
{
//ITERATOR_TRAITS
	template <class Iterator>
	class iterator_traits
	{
		public:
			typedef typename Iterator::difference_type		difference_type;
			typedef typename Iterator::value_type			value_type;
			typedef typename Iterator::pointer				pointer;
			typedef typename Iterator::reference			reference;
			typedef typename Iterator::iterator_category	iterator_category;
	};

//ENABLE_IF	
	template< bool condition, class T = void>
	struct enable_if{}; //nothing defined when not true.
	template<class T>
	struct enable_if<true, T>
	{
		typedef T type;
	};

//IS_INTEGRAL
	template <class T>
	struct is_integral { static const bool value = false; };
	
	template <>
	struct is_integral<bool> { static const bool value = true; };
	template <>
	struct is_integral<char> { static const bool value = true; };
	template <>
	struct is_integral<wchar_t> { static const bool value = true; };
	template <>
	struct is_integral<signed char> { static const bool value = true; };
	template <>
	struct is_integral<short int> { static const bool value = true; };
	template <>
	struct is_integral<int> { static const bool value = true; };
	template <>
	struct is_integral<long int> { static const bool value = true; };
	template <>
	struct is_integral<long long int> { static const bool value = true; };
	template <>
	struct is_integral<unsigned char> { static const bool value = true; };
	template <>
	struct is_integral<unsigned short int> { static const bool value = true; };
	template <>
	struct is_integral<unsigned int> { static const bool value = true; };
	template <>
	struct is_integral<unsigned long int> { static const bool value = true; };
	template <>
	struct is_integral<unsigned long long int> { static const bool value = true; };

//LEXICOGRAPHICAL_COMPARE
	template <class InputIterator1, class InputIterator2>
  	bool lexicographical_compare (InputIterator1 first1, InputIterator1 last1,
                                InputIterator2 first2, InputIterator2 last2) {
		while (first1!=last1)
  		{
 			if (first2==last2 || *first2<*first1) return false;
   			else if (*first1<*first2) return true;
    		++first1; ++first2;
  		}
  		return (first2!=last2);
	}

//PAIR
    template <class T, class U>
    class pair
    {
        public:
            typedef T first_type;
            typedef U second_type;
            pair() : first(), second() {}
            pair(const T &first, const U &second) : first(first), second(second) {}
            pair(const pair &other) : first(other.first), second(other.second) {}
            pair &operator=(const pair &other)
            {
                if (this == &other)
                    return *this;
                first = other.first;
                second = other.second;
                return *this;
            }
            template<class V, class W>
            pair(const pair<V, W> &other) : first(other.first), second(other.second) {}

            bool operator==(const pair &other) const { return (first == other.first && second == other.second); }
            bool operator!=(const pair &other) const { return !(*this == other); }
            bool operator<(const pair &other) const { return (first < other.first || (first == other.first && second < other.second)); }
            bool operator>(const pair &other) const { return (first > other.first || (first == other.first && second > other.second)); }
            bool operator<=(const pair &other) const { return !(*this > other); }
            bool operator>=(const pair &other) const { return !(*this < other); }
        
        first_type first;
        second_type second;
    };

	template <class T1,class T2>
	pair<T1,T2> make_pair (T1 x, T2 y)
	{
		  return ( pair<T1,T2>(x,y) );
	}
}










