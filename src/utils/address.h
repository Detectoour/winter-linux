#pragma once

template <typename ptr_type = uintptr_t>
struct address_base_t
{
    ptr_type m_ptr;

    address_base_t() : m_ptr {}
    {
    }

    address_base_t(ptr_type ptr) : m_ptr(ptr)
    {
    }

    address_base_t(ptr_type* ptr) : m_ptr(ptr_type(ptr))
    {
    }

    address_base_t(void* ptr) : m_ptr(ptr_type(ptr))
    {
    }

    address_base_t(const void* ptr) : m_ptr(ptr_type(ptr))
    {
    }

    ~address_base_t() = default;

    operator ptr_type() const
    {
        return m_ptr;
    }

    operator void*() const
    {
        return reinterpret_cast<void*>(m_ptr);
    }

    ptr_type get_inner() const
    {
        return m_ptr;
    }

    template <typename t = address_base_t<ptr_type>>
    bool compare(t in) const
    {
        return m_ptr == ptr_type(in);
    }

    address_base_t<ptr_type>& self_get(uint8_t in = 1)
    {
        m_ptr = get<ptr_type>(in);

        return *this;
    }

    address_base_t<ptr_type>& add(ptrdiff_t offset)
    {
        m_ptr += offset;

        return *this;
    }

    template <typename t = address_base_t<ptr_type>>
    address_base_t<ptr_type>& relative(ptrdiff_t offset = 0x1)
    {
        m_ptr = jmp(offset);

        return *this;
    }

    template <typename t = address_base_t<ptr_type>>
    address_base_t<ptr_type>& set(t in)
    {
        m_ptr = ptr_type(in);

        return m_ptr ? *this : t();
    }

    template <typename t = ptr_type>
    t cast()
    {
        return m_ptr ? t(m_ptr) : t();
    }

    template <typename t = address_base_t<ptr_type>>
    t get(uint8_t in = 1)
    {
        ptr_type dummy = m_ptr;

        while (in--)
            /// Check if pointer is still valid
            if (dummy)
                dummy = *reinterpret_cast<ptr_type*>(dummy);

        return m_ptr ? t(dummy) : t();
    }

    template <typename t = address_base_t<ptr_type>>
    t jmp(ptrdiff_t offset = 0x1)
    {
        ptr_type base     = m_ptr + offset;
        auto displacement = *reinterpret_cast<int32_t*>(base);
        base += sizeof(uint32_t);
        base += displacement;

        return m_ptr ? t(base) : t();
    }
};

using address_t = address_base_t<uintptr_t>;