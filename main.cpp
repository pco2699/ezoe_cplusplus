#include "all.h"

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

template <typename T, typename Allocator = std::allocator<T> >
class vector {
    public:
        using value_type = T;
        using pointer = T *;
        using const_pointer = const pointer;
        using reference = value_type &;
        using const_reference = const value_type &;
        using allocator_type = Allocator;

        using iterator = pointer;
        using const_iterator = const_pointer;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;
        using size_type = std::size_t;
    private:
        // 先頭の要素へのポインター
        pointer first = nullptr;
        // 最後の要素の1つ前方のポインター
        pointer last = nullptr;
        // 確保したストレージの終端
        pointer reserved_last = nullptr;
        // アロケーターの値
        allocator_type alloc;

        using traits = std::allocator_traits<allocator_type>;

        pointer allocate(size_type n) {
            return traits::allocate(alloc, n);
        }
        void deallocate() {
            traits::deallocate(alloc, first, capacity());
        }
        void destroy(pointer ptr) {
            traits::destroy(alloc, ptr);
        }

        void construct( pointer ptr )
        { traits::construct( alloc, ptr ) ; }
        void construct( pointer ptr, const_reference value )
        { traits::construct( alloc, ptr, value ) ; }
        // ムーブ用
        void construct( pointer ptr, value_type && value )
        { traits::construct( alloc, ptr, std::move(value) ) ; }

        void destroy_until(reverse_iterator rend) {
            for (auto riter = rbegin(); riter != rend; ++riter, --last) {
                destroy(&*riter);
            }
        }

        void clear() noexcept {
            destroy_until(rend());
        }

        public:
        void reserve(size_type sz) {
            if (sz <= capacity())
                return;

            auto ptr = allocate(sz);

            auto old_first = first;
            auto old_last = last;
            auto old_capacity = capacity();

            first = ptr;
            last = first;
            reserved_last = first + sz;


            for (auto old_iter = old_first; old_iter != old_last; ++old_iter, ++last) {
                construct(last, std::move(*old_iter));
            }

            for (auto riter = reverse_iterator(old_last), rend=reverse_iterator(old_first);
                riter != rend; ++riter) {
                    destroy(&*riter);
                }
            traits::deallocate(alloc, old_first, old_capacity);
        }

        ~vector(){
            clear();
            deallocate();
        };

        vector(const allocator_type & alloc) noexcept
        : alloc(alloc) {}

        vector(): vector(allocator_type()) {}
        vector(size_type size, const allocator_type & alloc = allocator_type())
            : vector(alloc) {
                resize(size);
            }

        vector(size_type size, const_reference value, const allocator_type & alloc = allocator_type())
            : vector(alloc) {
                resize(size, value);
            }

        template<typename InputIterator>
        vector(InputIterator first, InputIterator last, const Allocator & = Allocator()) {
            reserve(std::distance(first, last));
            for (auto i = first; i != last; ++i) {
                push_back(*i);
            }
        }

        vector(std::initializer_list<value_type> init, const Allocator & alloc = Allocator())
            : vector(std::begin(init), std::end(init), alloc) {}

        vector(const vector & r)
            : alloc( traits::select_on_container_copy_construction(r.alloc))
            {
                reserve(r.size());
                for (auto dest = first, src = r.cbegin(), last = r.cend();
                    src != last; ++dest, ++src) {
                        construct(dest, *src);
                }
                last = first + r.size();
            }
        vector & operator =(const vector & r){
            if (this == &r)
                return *this;
            
            if (size() == r.size()) {
                std::copy(r.cbegin(), r.cend(), begin());
            }
            else {
                if (capacity() >= r.size() ){
                    std::copy(r.cbegin(), r.cbegin() + size(), begin());
                    for (auto src_iter = r.cbegin() + size(), src_end = r.cend();
                    src_iter != src_end; ++src_iter, ++last) {
                        std::cout << "コピー構築！" << std::endl;
                        construct(last, *src_iter);
                    }
                } else {

                }
            }
            return *this;
        }

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

        reverse_iterator rbegin() noexcept {
            return reverse_iterator{last};
        }

        reverse_iterator rend() noexcept {
            return reverse_iterator{first};
        }

        const_reverse_iterator crbegin() const noexcept {
            return reverse_iterator{last};
        }

        const_reverse_iterator crend() const noexcept {
            return reverse_iterator{first};
        }

        size_type size() const noexcept {
            return last - first;
        }

        bool empty() const noexcept {
            return size() == 0;
        }

        size_type capacity() const noexcept {
            return reserved_last - first;
        }

        reference operator [](size_type i) {
            return first[i];
        }

        const_reference operator [](size_type i ) const {
            return first[i];
        }

        void resize(size_type sz) {
            if (sz < size()) {
                auto diff = size() - sz;
                destroy_until(rbegin() + diff);
                last = first + sz;
            }
            else if (sz > size()) {
                reserve(sz);
                for (; last != reserved_last; ++last) {
                    construct(last);
                }
            }
        }

        void resize(size_type sz, const_reference value) {
            if (sz < size()) {
                auto diff = size() - sz;
                destroy_until(rbegin() + diff);
                last = first + sz;
            }
            else if (sz > size()) {
                reserve(sz);
                for (; last != reserved_last; ++last) {
                    construct(last, value);
                }
            }
        }

        void push_back(const_reference value) {
            if (size() + 1 > capacity()) {
                auto c = size();
                if (c == 0)
                    c = 1;
                else
                    c *= 2;
                reserve(c);
            }
            construct(last, value);
            ++last;
        }
};

int main()
{
    vector<int> v = {1, 2, 3, 4, 5};
    vector<int> w(3);
    w.reserve(5);
    w = v;
    for (auto iter = w.begin(); iter != w.end(); ++iter) {
        std::cout << *iter << std::endl;
    }
    
}

