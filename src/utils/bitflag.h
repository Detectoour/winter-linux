#pragma once
struct bitflag_t
{
    bitflag_t() = default;

    explicit bitflag_t(int flags) : m_flags(flags)
    {
    }

    ~bitflag_t() = default;

    int m_flags = 0;

    [[nodiscard]] bool has_flag(int flag) const
    {
        return m_flags & flag;
    }

    void add_flag(int flag)
    {
        m_flags |= flag;
    }

    void remove_flag(int flag)
    {
        m_flags &= ~flag;
    }

    [[nodiscard]] bool is_empty() const
    {
        return !m_flags;
    }
};