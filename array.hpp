#include "all.h"

template < typename Array >
struct array_iterator
{
    Array & a ;
    std::size_t i ;

    array_iterator( Array & a, std::size_t i )
        : a( a ), i( i ) { }

    typename Array::reference operator * ()
    {
        return a[i] ;
    }

    array_iterator & operator ++ () {
        ++i;
        return *this;
    }

    array_iterator & operator ++ (int) {
        array_iterator copy = *this;
        ++*this;
        return copy;
    }

    array_iterator & operator --(){
        --i;
        return *this;
    }

    array_iterator & operator--(int){
        array_iterator copy = *this;
        --*this;
        return copy;
    }

    bool operator == (array_iterator const & right) {
        return  i == right.i;
    }
    bool operator != (array_iterator const & right) {
        return i != right.i;
    }
    

} ;

template < typename T, std::size_t N >
struct array
{
    T storage[N] ;

    using reference = T & ;
    using const_reference = T const & ;
    using size_type = std::size_t;
    using iterator = array_iterator<array>;
    iterator begin() {
        return iterator(*this, 0);
    }

    iterator end() {
        return iterator(*this, N);
    }

    size_type size() const {
        return N;
    }

    reference front() {
        return storage[0];
    }

    const_reference front() const {
        return storage[0];
    }

    reference back() {
        return storage[N-1];
    }

    const_reference back() const {
        return storage[N-1];
    }

    // 非const版
    reference operator [] ( std::size_t i )
    {
        return storage[i] ;
    }
    // const版
    const_reference operator [] ( std::size_t i ) const
    {
        return storage[i] ;
    }

    reference at(std::size_t n) {
        if (n >= size()) throw std::out_of_range("Error: Out of Range");
        return storage[n];
    }

};