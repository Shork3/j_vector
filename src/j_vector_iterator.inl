#include "j_vector.hpp"

namespace just
{
    template <typename T, typename Allocator>
    template <typename U> 
    j_vector<T, Allocator>::j_vector_iterator<U>::j_vector_iterator(U *ptr): m_current(ptr)
    {
    }

    template <typename T, typename Allocator> 
    template <typename U> 
    j_vector<T, Allocator>::j_vector_iterator<U> &j_vector<T, Allocator>::j_vector_iterator<U>::operator++()
    {
        ++m_current;
        return *this;
    }

    template <typename T, typename Allocator>
    template <typename U> 
    j_vector<T, Allocator>::j_vector_iterator<U> j_vector<T, Allocator>::j_vector_iterator<U>::operator++(int)
    {
        auto temp = *this;
        ++m_current;
        return temp;
    }

    template <typename T, typename Allocator>
    template <typename U> 
    j_vector<T, Allocator>::j_vector_iterator<U> &j_vector<T, Allocator>::j_vector_iterator<U>::operator--()
    {
        --m_current;
        return *this;
    }

    template <typename T, typename Allocator>
    template <typename U> 
    j_vector<T, Allocator>::j_vector_iterator<U>  j_vector<T, Allocator>::j_vector_iterator<U>::operator--(int)
    {
        auto temp = *this;
        --m_current;
        return temp;
    }

    template <typename T, typename Allocator>
    template <typename U>
    bool j_vector<T, Allocator>::j_vector_iterator<U>::operator==(const j_vector_iterator &itr2) const noexcept
    {
        return this->m_current == itr2.m_current;
    }

    template <typename T, typename Allocator>
    template <typename U>
    bool j_vector<T, Allocator>::j_vector_iterator<U>::operator!=(const j_vector_iterator &itr2) const noexcept
    {
        return !(*this == itr2);
    }

    template <typename T, typename Allocator>
    template <typename U>
    j_vector<T, Allocator>::j_vector_iterator<U> &j_vector<T, Allocator>::j_vector_iterator<U>::operator-(const int value)
    {
        m_current -= value;
        return *this;
    }

    template <typename T, typename Allocator>
    template <typename U>
    j_vector<T, Allocator>::j_vector_iterator<U> &j_vector<T, Allocator>::j_vector_iterator<U>::operator+(const int value)
    {
        m_current += value;
        return *this;
    }

    template <typename T, typename Allocator>
    template <typename U>
    U &j_vector<T, Allocator>::j_vector_iterator<U>::operator*()
    {
        return (*m_current);
    }

} // namespace just