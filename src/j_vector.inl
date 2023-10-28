#include "j_vector.hpp"

namespace just
{
    template <typename T, typename Allocator>
    j_vector<T, Allocator>::j_vector(const int capacity, const Allocator &alloc): m_allocator(alloc)
    {
        change_array_capacity(capacity);
        change_array_size(0);
    }

    template <typename T, typename Allocator>
    j_vector<T, Allocator>::j_vector(const int size, const T &value, const Allocator &alloc): m_allocator(alloc)
    {
        change_array_capacity(size);
        change_array_size(size);

        for (int i = 0; i < size; ++i)
        {
            AllocatorTraits::construct(m_allocator, m_array + i, std::move(value));
        }
    }

    template <typename T, typename Allocator>
    j_vector<T, Allocator>::j_vector(const j_vector<T> &value, const Allocator &alloc): m_allocator(alloc)
    {
        change_array_capacity(value.m_capacity);
        change_array_size(value.m_size);

        for (int i = 0; i < m_size; ++i)
        {
            AllocatorTraits::construct(m_allocator, m_array + i, std::move(value.m_array[i]));
        }
    }

    template <typename T, typename Allocator> 
    j_vector<T, Allocator>::j_vector(j_vector<T> &&copy_value, const Allocator &alloc): m_allocator(alloc)
    {
        clear();
        m_capacity = copy_value.m_capacity;
        m_size = copy_value.m_size;
        m_array = copy_value.m_array;

        copy_value.m_capacity = 0;
        copy_value.m_size = 0;
        copy_value.m_array = nullptr;
    }

    template <typename T, typename Allocator>
    j_vector<T, Allocator>::j_vector(const std::initializer_list<T> &list, const Allocator &alloc): m_allocator(alloc)
    {
        change_array_capacity(list.size());
        change_array_size(list.size());

        auto itr = list.begin();
        for (int i = 0; i < m_size; ++i)
        {
            AllocatorTraits::construct(m_allocator, m_array + i, std::move(*(itr + i)));
        }
    }

    template <typename T, typename Allocator>
    j_vector<T, Allocator>::~j_vector()
    {
        if (m_array != nullptr)
        {
            for (int i = 0; i < m_size; ++i)
            {
                AllocatorTraits::destroy(m_allocator, m_array + i);
            }
            AllocatorTraits::deallocate(m_allocator, m_array, m_capacity);
        }
    }

    template <typename T, typename Allocator>
    j_vector<T> &j_vector<T, Allocator>::operator=(const j_vector<T> &copy_vector)
    {
        clear();
        change_array_capacity(copy_vector.m_capacity);
        change_array_size(copy_vector.m_size);

        for (int i = 0; i < m_size; ++i)
        {
            AllocatorTraits::construct(m_allocator, m_array + i, std::move(copy_vector.m_array[i]));
        }

        return *this;
    }

    template <typename T, typename Allocator>
    j_vector<T> &j_vector<T, Allocator>::operator=(const std::initializer_list<T> &list)
    {
        clear();
        change_array_capacity(list.size());
        change_array_size(list.size());

        auto itr = list.begin();
        for (int i = 0; i < m_size; ++i)
        {
            AllocatorTraits::construct(m_allocator, m_array + i, std::move(*(itr + i)));
        }

        return *this;
    }

    template <typename T, typename Allocator>
    bool j_vector<T, Allocator>::operator==(const j_vector<T> &compared_vector) const
    {
        if (m_size != compared_vector.m_size)
            return false;

        for (int i = 0; i < m_size; ++i)
        {
            if (m_array[i] != compared_vector.m_array[i])
                return false;
        }
        return true;
    }

    template <typename T, typename Allocator>
    T &j_vector<T, Allocator>::operator[](const int index)
    {
        return m_array[index];
    }

    template <typename T, typename Allocator>
    bool j_vector<T, Allocator>::empty() const noexcept
    {
        return !(m_size > 0);
    }

    template <typename T, typename Allocator>
    void j_vector<T, Allocator>::clear()
    {
        if (m_array != nullptr)
        {
            for (int i = 0; i < m_size; ++i)
            {
                AllocatorTraits::destroy(m_allocator, m_array + i);
            }
        }
        m_size = 0;
        m_capacity = 0;
        m_array = nullptr;
    }

    template <typename T, typename Allocator>
    int j_vector<T, Allocator>::size() const noexcept
    {
        return m_size;
    }

    template <typename T, typename Allocator>
    void j_vector<T, Allocator>::reserve(const int size)
    {
        if (size > m_size)
            change_array_capacity(size);
        else
            change_array_capacity(m_size);
    }

    template <typename T, typename Allocator>
    void j_vector<T, Allocator>::resize(const int size, const T &value)
    {
        change_array_capacity(size);
        int start_index = m_size;
        change_array_size(size);

        for (int i = start_index; i < m_size; ++i)
        {
            AllocatorTraits::construct(m_allocator, m_array + i, std::move(value));
        }
    }

    template <typename T, typename Allocator>
    void j_vector<T, Allocator>::resize(const int size)
    {
        change_array_capacity(size);
    }

    template <typename T, typename Allocator>
    int j_vector<T, Allocator>::capacity() const noexcept
    {
        return m_capacity;
    }

    template <typename T, typename Allocator>
    void j_vector<T, Allocator>::check_capacity()
    {
        if (m_size == m_capacity)
        {
            if (m_capacity == 0)
                change_array_capacity(m_capacity + 1);
            else
                change_array_capacity(m_capacity * 2);
        }
    }

    template <typename T, typename Allocator>
    void j_vector<T, Allocator>::change_array_capacity(const int new_capacity)
    {
        if (new_capacity < 0)
            throw j_vector_error("capacity должен быть больше нуля", 1);
        if (m_size > new_capacity)
            change_array_size(new_capacity);

        T *temp_array = m_array;
        int temp_capacity = m_capacity;
        m_capacity = new_capacity;
        m_array = AllocatorTraits::allocate(m_allocator, new_capacity);

        if (temp_array != nullptr)
        {
            for (int i = 0; i < m_size; ++i)
            {
                AllocatorTraits::construct(m_allocator, m_array + i, std::move(temp_array[i]));
            }
            for (int i = 0; i < temp_capacity; ++i)
            {
                AllocatorTraits::destroy(m_allocator, temp_array + i);
            }

            AllocatorTraits::deallocate(m_allocator, temp_array, temp_capacity);
        }
    }

    template <typename T, typename Allocator>
    void j_vector<T, Allocator>::change_array_size(const int size)
    {
        if (size < 0)
            throw j_vector_error("size должне быть больше или равен нулю", 1);

        if (size > m_capacity)
            throw j_vector_error("size не может быть больше m_capacity", 1);

        if (m_size == size)
            return;

        if (m_size > size)
        {
            for (int i = size; i < m_size; ++i)
            {
                m_array[i].~T();
            }
            m_size = size;
        }
        else
        {
            m_size = size;
        }
    }

    template <typename T, typename Allocator>
    void j_vector<T, Allocator>::push_back(const T &value)
    {
        check_capacity();
        change_array_size(m_size + 1);
        AllocatorTraits::construct(m_allocator, m_array + (m_size - 1), std::move(value));
    }

    template <typename T, typename Allocator>
    void j_vector<T, Allocator>::push_back(T &&value)
    {
        check_capacity();
        change_array_size(m_size + 1);
        AllocatorTraits::construct(m_allocator, m_array + (m_size - 1), std::move(value));
    }

    template <typename T, typename Allocator>
    void j_vector<T, Allocator>::insert(const int index, const T &value)
    {
        int temp_capacity = sizeof(T) * (m_size - (index - 1));
        T *temp = AllocatorTraits::allocate(m_allocator, temp_capacity);

        for (int i = index, j = 0; i < m_size; ++i, ++j)
        {
            temp[j] = m_array[i];
            AllocatorTraits::construct(m_allocator, temp + j, std::move(m_array[i]));
        }

        check_capacity();
        m_array[index].~T();
        AllocatorTraits::construct(m_allocator, m_array + index, std::move(value));
        change_array_size(m_size + 1);

        for (int i = index + 1, j = 0; i < m_size; ++i, ++j)
        {
            AllocatorTraits::construct(m_allocator, m_array + i, std::move(temp[j]));
        }

        for (int i = 0; i < temp_capacity; ++i)
        {
            AllocatorTraits::destroy(m_allocator, temp + i);
        }
        AllocatorTraits::deallocate(m_allocator, temp, temp_capacity);
    }

    template <typename T, typename Allocator>
    void j_vector<T, Allocator>::pop_back()
    {
        if (m_size > 0)
            change_array_size(m_size - 1);
    }

    template <typename T, typename Allocator>
    T &j_vector<T, Allocator>::back()
    {
        return m_array[m_size - 1];
    }   

    template <typename T, typename Allocator>
    T &j_vector<T, Allocator>::front()
    {
        return m_array[0];
    }

    template <typename T, typename Allocator>
    T &j_vector<T, Allocator>::at(const int index)
    {
        if (-1 < index && index < m_size)
            return m_array[index];
        throw j_vector_error("Индекс вышел за границы вектора.", 1);
    }

    template <typename T, typename Allocator>
    typename j_vector<T, Allocator>::iterator j_vector<T, Allocator>::begin()
    {
        return iterator(m_array);
    }

    template <typename T, typename Allocator>
    typename j_vector<T, Allocator>::iterator j_vector<T, Allocator>::end()
    {
        return iterator(&m_array[m_size]);
    }

    template <typename T, typename Allocator>
    typename j_vector<T, Allocator>::const_iterator j_vector<T, Allocator>::cbegin() const
    {
        return const_iterator(m_array);
    }

    template <typename T, typename Allocator>
    typename j_vector<T, Allocator>::const_iterator j_vector<T, Allocator>::cend() const
    {
        return const_iterator(&m_array[m_size]);
    }

    template <typename T, typename Allocator>
    j_vector<T, Allocator>::iterator::iterator(T *first) : m_current(first)
    {
    }

    template <typename T, typename Allocator>
    T &j_vector<T, Allocator>::iterator::operator++()
    {
        return *(++m_current);
    }

    template <typename T, typename Allocator>
    T &j_vector<T, Allocator>::iterator::operator++(int)
    {
        T *temp = m_current;
        m_current += 1;
        return *temp;
    }

    template <typename T, typename Allocator>
    T &j_vector<T, Allocator>::iterator::operator--()
    {
        return *(m_current -= 1);
    }

    template <typename T, typename Allocator>
    T &j_vector<T, Allocator>::iterator::operator--(int)
    {
        T *temp = m_current;
        m_current -= 1;
        return *temp;
    }

    template <typename T, typename Allocator>
    bool j_vector<T, Allocator>::iterator::operator==(const iterator &itr2) const noexcept
    {
        return this->m_current == itr2.m_current;
    }

    template <typename T, typename Allocator>
    bool j_vector<T, Allocator>::iterator::operator!=(const iterator &itr2) const noexcept
    {
        return !(*this == itr2);
    }

    template <typename T, typename Allocator>
    T &j_vector<T, Allocator>::iterator::operator-(const int value)
    {
        return *(m_current -= value);
    }

    template <typename T, typename Allocator>
    T &j_vector<T, Allocator>::iterator::operator+(const int value)
    {
        return *(m_current += value);
    }

    template <typename T, typename Allocator>
    T &j_vector<T, Allocator>::iterator::operator*()
    {
        return (*m_current);
    }

    template <typename T, typename Allocator>
    j_vector<T, Allocator>::const_iterator::const_iterator(const T *first) : m_current(first)
    {
    }

    template <typename T, typename Allocator>
    const T &j_vector<T, Allocator>::const_iterator::operator++()
    {
        return *(++m_current);
    }

    template <typename T, typename Allocator>
    const T &j_vector<T, Allocator>::const_iterator::operator++(int)
    {
        const T *temp = m_current;
        m_current += 1;
        return *temp;
    }

    template <typename T, typename Allocator>
    const T &j_vector<T, Allocator>::const_iterator::operator--()
    {
        return *(m_current -= 1);
    }

    template <typename T, typename Allocator>
    const T &j_vector<T, Allocator>::const_iterator::operator--(int)
    {
        T *temp = m_current;
        m_current -= 1;
        return *temp;
    }

    template <typename T, typename Allocator>
    const T &j_vector<T, Allocator>::const_iterator::operator-(const int value)
    {
        return *(m_current -= value);
    }

    template <typename T, typename Allocator>
    const T &j_vector<T, Allocator>::const_iterator::operator+(const int value)
    {
        return *(m_current += value);
    }

    template <typename T, typename Allocator>
    const T &j_vector<T, Allocator>::const_iterator::operator*()
    {
        return (*m_current);
    }

    template <typename T, typename Allocator>
    bool j_vector<T, Allocator>::const_iterator::operator==(const const_iterator &itr2) const noexcept
    {
        return this->m_current == itr2.m_current;
    }

    template <typename T, typename Allocator>
    bool j_vector<T, Allocator>::const_iterator::operator!=(const const_iterator &itr2) const noexcept
    {
        return !(*this == itr2);
    }

    template <typename T, typename Allocator>
    j_vector<T, Allocator>::j_vector_error::j_vector_error(const std::string& msg, const int error_code) : m_error_code(error_code), m_error_message(msg)
    {
        
    }

    template <typename T, typename Allocator>
    const char *j_vector<T, Allocator>::j_vector_error::what() const noexcept
    {
        return m_error_message.c_str();
    }

    template <typename T, typename Allocator>
    int j_vector<T, Allocator>::j_vector_error::get_code() const noexcept
    {
        return m_error_code;
    }
} // namespace just