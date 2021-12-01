#pragma once
#include <sys/mman.h>
#include <unistd.h>

// Main purpose:
// Place longjump bytes at the beginning of the target function
// Save the original bytes in order to reset ==> call original function
// ==> detour again

class c_detour
{
private:
    std::vector<uint8_t> m_original_bytes {};
    std::vector<uint8_t> m_long_jmp_bytes {
        0x48, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // mov rax, hook_func_address
        0xFF, 0xE0                                                  // jmp rax
    };
    void* m_original_function {};
    void* m_target_function {};

    uintptr_t m_page_base {};
    size_t m_page_size {};

public:
    c_detour() = default;
    c_detour(void* orig, void* target)
    {
        m_original_function = orig;
        m_target_function   = target;

        m_page_size = sysconf(_SC_PAGE_SIZE);

        // https://stackoverflow.com/a/6387879
        // In Linux (in other OSes too, I guess), pages are aligned at
        // page boundaries. Thus, if you know your PAGE_SIZE, then you can
        // simply get the start of the page by masking the lower address
        // bits.
        m_page_base = reinterpret_cast<uintptr_t>(m_original_function) & -m_page_size;

        // Initialize the size
        m_original_bytes.resize(m_long_jmp_bytes.size());

        // Save the original bytes
        memcpy(m_original_bytes.data(), m_original_function, m_original_bytes.size());

        // Set the address we need to jump to
        // +2 in order to skip first 2 bytes (0x48, 0x8B)
        // the size of pointer in x64 is 8
        memcpy(reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(m_long_jmp_bytes.data()) + 2), &m_target_function, sizeof(void*));
    }

    bool create(void* orig, void* target)
    {
        if (!orig || !target)
            return false;
        m_original_function = orig;
        m_target_function   = target;

        m_page_size = sysconf(_SC_PAGE_SIZE);

        // https://stackoverflow.com/a/6387879
        // In Linux (in other OSes too, I guess), pages are aligned at
        // page boundaries. Thus, if you know your PAGE_SIZE, then you can
        // simply get the start of the page by masking the lower address
        // bits.
        m_page_base = reinterpret_cast<uintptr_t>(m_original_function) & -m_page_size;

        // Initialize the size
        m_original_bytes.resize(m_long_jmp_bytes.size());

        // Save the original bytes
        memcpy(m_original_bytes.data(), m_original_function, m_original_bytes.size());

        // Set the address we need to jump to
        // +2 in order to skip first 2 jmp bytes (0x48, 0x8B)
        // the size of pointer in x64 is 8, and we only need to copy 8 bytes (same in x86 but 4 bytes)
        memcpy(reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(m_long_jmp_bytes.data()) + 2), &m_target_function, sizeof(void*));
        return true;
    }

    auto hook()
    {
        if (!m_original_function || !m_target_function)
            return false;

        if (mprotect(reinterpret_cast<void*>(m_page_base), m_page_size, PROT_READ | PROT_EXEC | PROT_WRITE) == -1)
            return false;

        // Replace original bytes to our owns
        memcpy(m_original_function, m_long_jmp_bytes.data(), m_long_jmp_bytes.size());

        if (mprotect(reinterpret_cast<void*>(m_page_base), m_page_size, PROT_READ | PROT_EXEC) == -1)
            return false;

        return true;
    }

    auto unhook()
    {
        if (!m_original_function || !m_target_function)
            return false;

        if (mprotect(reinterpret_cast<void*>(m_page_base), m_page_size, PROT_READ | PROT_EXEC | PROT_WRITE) == -1)
            return false;

        // Replace the jmp bytes to original byes
        memcpy(m_original_function, m_original_bytes.data(), m_original_bytes.size());

        if (mprotect(reinterpret_cast<void*>(m_page_base), m_page_size, PROT_READ | PROT_EXEC) == -1)
            return false;

        return true;
    }

    template <typename t = void, typename... args>
    auto call_original(args... arg) -> t
    {
        unhook();
        auto ret = call_function<t, args...>(arg...);
        hook();
        return ret;
    }

    ~c_detour()
    {
        unhook();
        m_original_function = m_target_function = nullptr;
        m_original_bytes.clear();
    }

private:
    template <typename t = void, typename... args>
    constexpr inline auto call_function(args... arg) -> t
    {
        return reinterpret_cast<t (*)(args...)>(m_original_function)(arg...);
    }
};

namespace hook_funcs
{
    namespace paint
    {
        inline c_detour detour;
        void hook(void* ptr, int mode);
    }

    namespace clientmode_createmove
    {
        inline c_detour detour;
        bool hook(void* ptr, float frame_time, user_cmd* cmd);
    }

    namespace level_init_post
    {
        inline c_detour detour;
        void hook(void* ptr);
    }

    namespace draw_model
    {
        inline c_detour detour;
        void hook(void* ptr, void* result, const draw_model_info_t* info, matrix3x4_t* bone_to_world, float* weight, float* delayed_weight, const vec3_t* origin, int flags);
    }

    namespace post_screen_effects
    {
        inline c_detour detour;
        void hook(void* ptr, void* view_setup);
    }

    namespace color_modulate
    {
        inline c_detour detour;
        void hook(void* ptr, float* r, float* g, float* b);
    }

    namespace level_shutdown
    {
        inline c_detour detour;
        void hook(void* ptr);
    }

    namespace frame_stage_notify
    {
        inline c_detour detour;
        void hook(void* ptr, client_frame_stage_t stage);
    }

    namespace get_whitelist_spew_flags
    {
        inline c_detour detour;
        int hook(void* ptr);
    }

    namespace get_fov
    {
        inline c_detour detour;
        float hook(cs_player* ptr);
    }
}

class hooks
{
public:
    static bool hook();
    static void unhook();

private:
    static void create(c_detour& detour, address_t address, void* func, const std::string& name);
    static void destroy(c_detour& detour, const std::string& name);
};
