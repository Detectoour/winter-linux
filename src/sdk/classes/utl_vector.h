#pragma once
inline int utl_memory_calc_new_allocation_count(int allocation_count, int grow_size, int new_size, int bytes_item)
{
    if (grow_size)
        allocation_count = ((1 + ((new_size - 1) / grow_size)) * grow_size);
    else
    {
        if (!allocation_count)
            allocation_count = (31 + bytes_item) / bytes_item;

        while (allocation_count < new_size)
            allocation_count *= 2;
    }

    return allocation_count;
}

template <class T, class N = int>
class utl_memory
{
public:
    T& operator[](N i)
    {
        return m_memory[i];
    }

    const T& operator[](N i) const
    {
        return m_memory[i];
    }

    T* base()
    {
        return m_memory;
    }

    int num_allocated() const
    {
        return m_allocation_count;
    }

    void grow(int iNum = 1)
    {
        if (is_externally_allocated())
            return;

        int old_allocation_count = m_allocation_count;
        int allocation_requested = m_allocation_count + iNum;
        int new_allocation_count = utl_memory_calc_new_allocation_count(m_allocation_count, m_grow_size, allocation_requested, sizeof(T));

        if (static_cast<int>(static_cast<N>(new_allocation_count)) < allocation_requested)
        {
            if (static_cast<int>(static_cast<N>(new_allocation_count)) == 0 && static_cast<int>(static_cast<N>(new_allocation_count - 1)) >= allocation_requested)
                --new_allocation_count;
            else
            {
                if (static_cast<int>(static_cast<N>(allocation_requested)) != allocation_requested)
                {
                    return;
                }

                while (static_cast<int>(static_cast<N>(new_allocation_count)) < allocation_requested)
                    new_allocation_count = (new_allocation_count + allocation_requested) / 2;
            }
        }

        m_allocation_count = new_allocation_count;

        if (m_memory != nullptr)
        {
            // pMemory = reinterpret_cast<T*>(I::MemAlloc->Realloc(pMemory, iAllocationCount * sizeof(T)));

            std::byte* pData = new std::byte[m_allocation_count * sizeof(T)];
            memcpy(pData, m_memory, old_allocation_count * sizeof(T));
            m_memory = reinterpret_cast<T*>(pData);
        }
        else
            // pMemory = reinterpret_cast<T*>(I::MemAlloc->Alloc(iAllocationCount * sizeof(T)));
            m_memory = reinterpret_cast<T*>(new std::byte[m_allocation_count * sizeof(T)]);
    }

    bool is_externally_allocated() const
    {
        return m_grow_size < 0;
    }

protected:
    T* m_memory;
    int m_allocation_count;
    int m_grow_size;
};

template <class T>
void destruct(T* memory)
{
    memory->~T();
}

template <class T>
T* construct(T* memory)
{
    return ::new (memory) T;
}

template <class T>
T* copy_construct(T* memory, T const& src)
{
    return ::new (memory) T(src);
}

template <class T, class A = utl_memory<T>>
class utl_vector
{
    using allocator = A;

public:
    auto begin() noexcept
    {
        return m_memory.base();
    }

    auto end() noexcept
    {
        return m_memory.base() + m_size;
    }

    auto begin() const noexcept
    {
        return m_memory.base();
    }

    auto end() const noexcept
    {
        return m_memory.base() + m_size;
    }

    T& operator[](int i)
    {
        return m_memory[i];
    }

    const T& operator[](int i) const
    {
        return m_memory[i];
    }

    T& element(int i)
    {
        return m_memory[i];
    }

    const T& element(int i) const
    {
        return m_memory[i];
    }

    T* base()
    {
        return m_memory.base();
    }

    [[nodiscard]] int count() const
    {
        return m_size;
    }

    int& size()
    {
        return m_size;
    }

    void grow_vector(int nCount = 1)
    {
        if (m_size + nCount > m_memory.num_allocated())
            m_memory.grow(m_size + nCount - m_memory.num_allocated());

        m_size += nCount;
    }

    void shift_elements_right(const int i_element, const int shift = 1)
    {
        const int nToMove = m_size - i_element - shift;

        if (nToMove > 0 && shift > 0)
            memmove(&element(i_element + shift), &element(i_element), nToMove * sizeof(T));
    }

    void shift_elements_left(const int i_element, const int shift = 1)
    {
        const int nToMove = m_size - i_element - shift;

        if (nToMove > 0 && shift > 0)
            memmove(&element(i_element), &element(i_element + shift), nToMove * sizeof(T));
    }

    int insert_before(const int i_element)
    {
        // can insert at the end
        grow_vector();
        shift_elements_right(i_element);
        construct(&element(i_element));
        return i_element;
    }

    int insert_before(int i_element, const T& src)
    {
        // reallocate if can't insert something that's in the list
        // can insert at the end
        grow_vector();
        shift_elements_right(i_element);
        copy_construct(&element(i_element), src);
        return i_element;
    }

    int add_to_tail()
    {
        return insert_before(m_size);
    }

    int add_to_tail(const T& src)
    {
        return insert_before(m_size, src);
    }

    int find(const T& src) const
    {
        for (int i = 0; i < count(); ++i)
        {
            if (element(i) == src)
                return i;
        }

        return -1;
    }

    void remove(const int i_element)
    {
        destruct(&element(i_element));
        shift_elements_left(i_element);
        --m_size;
    }

    void remove_all()
    {
        for (int i = m_size; --i >= 0;)
            destruct(&element(i));

        m_size = 0;
    }

    bool find_and_remove(const T& src)
    {
        if (const int nElement = find(src); nElement != -1)
        {
            remove(nElement);
            return true;
        }

        return false;
    }

protected:
    allocator m_memory;
    int m_size;
    T* m_elements;
};