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

template < typename Container >
void print( Container const & c )
{
    for ( std::size_t i = 0 ; i != c.size() ; ++i )
    {
        std::cout << c[i] ;
    }
}

void * memcpy(void * dest, void const * src, std::size_t n) {
    auto d = static_cast<std::byte *>(dest);
    auto s = static_cast<std::byte const *>(src);

    for (std::size_t i = 0; i != n; ++i){
        d[i] = s[i];
    }

    return dest;
}

template <typename To, typename From>
To bit_cast(From const & from) {
    To to;
    memcpy(&to, &from, sizeof(To));
    return to;
}

template <typename T, typename Allocator = std::allocator<T>>
class vector {
    private:
        // 先頭の要素へのポインター
        pointer first ;
        // 最後の要素の1つ前方のポインター
        pointer last ;
        // 確保したストレージの終端
        pointer reserved_last ;
        // アロケーターの値
        allocator_type alloc ;
    public:
        using value_type = T;
        using pointer = T *;
        using const_pointer = const pointer;
        using reference = value_type &;
        using const_reference = const value_type &;
        using allocator_type = Allocator;

        using iterator = pointer;
        using const_iterator = const_pointer;
        using reverse_interator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        vector(std::size_t n = 0, Allocator a = Allocator());
        ~vector();

        vector(const vector & x);
        vector & operator =(const vector & x);

        void push_back(const T & x);
        T & operator [](std::size_t i) noexcept;

        iterator begin() noexcept {
            return first;
        }
        iterator end() noexcept {
            return last;
        }

        const_iterator cbegin() const noexcept {
            return first;
        }

        const_iterator cend() const noexcept {
            return last;
        }

        reverse_interator rbegin() noexcept {
            return reverse_iterator{last};
        }

        reverse_interator rend() noexcept {
            return reverse_iterator{begin};
        }
};

int main()
{
    std::allocator<int> a;
    using traits = std::allocator_traits<decltype(a)>;

    std::string * p = traits::allocate(a, 1);
    std::string * s = traits::construct(a, p, "hello");

    traits::destroy(a, s);
    traits::deallocate(a, p, 1);
}