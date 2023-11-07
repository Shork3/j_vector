#pragma once
#include <initializer_list>
#include <memory>

class j_vector_test;
namespace just
{
    template <typename T, typename Allocator = std::allocator<T>>
    class j_vector
    {
    private:
        Allocator m_allocator;
        using AllocatorTraits = std::allocator_traits<Allocator>;
        int m_size = 0;
        int m_capacity = 0;
        T *m_array = nullptr;
        void check_capacity();
        void change_array_capacity(const int capacity);
        void change_array_size(const int size);
        template<typename U>
        class j_vector_iterator
        {
        private:
            U *m_current;

        public:
            j_vector_iterator(U *first);

            j_vector_iterator &operator++();
            j_vector_iterator operator++(int);
            j_vector_iterator &operator--();
            j_vector_iterator operator--(int);
            j_vector_iterator &operator+(const int value);
            j_vector_iterator &operator-(const int value);
            U &operator*();
            bool operator==(const j_vector_iterator &itr2) const noexcept;
            bool operator!=(const j_vector_iterator &itr2) const noexcept;
        };

    public:
        using iterator = j_vector_iterator<T>;
        using const_iterator = j_vector_iterator<const T>;

        j_vector(const int capacity = 0, const Allocator &alloc = Allocator());
        j_vector(const int size, const T &value, const Allocator &alloc = Allocator());
        j_vector(const std::initializer_list<T> &list, const Allocator &alloc = Allocator());
        j_vector(const j_vector<T> &copy_value, const Allocator &alloc = Allocator());
        j_vector(j_vector<T> &&copy_value, const Allocator &alloc = Allocator());
        ~j_vector();
        void clear();
        void resize(const int size, const T &value);
        void resize(const int size);
        void reserve(const int size);
        void push_back(const T &value);
        void push_back(T &&value);
        void pop_back();
        void insert(const int index, const T &value);
        bool empty() const noexcept;
        int size() const noexcept;
        int capacity() const noexcept;
        T &back();
        T &front();
        T &at(const int index);

        T &operator[](const int index);
        bool operator==(const j_vector<T> &compared_vector) const;
        j_vector<T> &operator=(const j_vector<T> &copy_vector);
        j_vector<T> &operator=(const std::initializer_list<T> &list);

        class j_vector_error : public std::exception
        {
        private:
            int m_error_code;
            std::string m_error_message;

        public:
            j_vector_error(const std::string &msg, const int error_code);
            int get_code() const noexcept;
            virtual const char *what() const noexcept override;
        };

        iterator begin();
        iterator end();
        const_iterator cbegin() const;
        const_iterator cend() const;
    };
}
#include "j_vector.inl"
#include "j_vector_iterator.inl"