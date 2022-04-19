#pragma once
#include "vector.hpp"
namespace ft{
    template <class T, class Container = vector<T> >
    class stack
    {
    public:
        typedef Container                            container_type;
        typedef typename Container::value_type       value_type;
        typedef typename Container::size_type        size_type;
        typedef typename Container::reference        reference;
        typedef typename Container::const_reference  const_reference;

        explicit stack(const container_type& cont = container_type()) : c(cont) {}

        ~stack() {}

        reference top()
            { return c.back(); }

        const_reference top() const
            { return c.back(); }

        bool        empty() const
            { return c.empty(); }

        size_type   size() const
            { return c.size(); }

        void push(const value_type &value)
            { c.push_back(value); }

        void pop()
            { c.pop_back(); }

       friend bool operator==( const stack<T,Container>& lhs, const stack<T,Container>& rhs )
        { return lhs.c == rhs.c ;}

        friend bool operator!=( const stack<T,Container>& lhs, const stack<T,Container>& rhs )
            { return lhs.c != rhs.c ;}

        friend bool operator<( const stack<T,Container>& lhs, const stack<T,Container>& rhs )
            { return lhs.c < rhs.c ;}

        friend bool operator<=( const stack<T,Container>& lhs, const stack<T,Container>& rhs )
            { return lhs.c <= rhs.c ;}

        friend bool operator>( const stack<T,Container>& lhs, const stack<T,Container>& rhs )
            { return lhs.c > rhs.c ;}

        friend bool operator>=( const stack<T,Container>& lhs, const stack<T,Container>& rhs )
            { return lhs.c >= rhs.c ;}
    protected:
        container_type   c;
    };
}