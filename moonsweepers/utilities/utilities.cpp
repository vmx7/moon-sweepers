#include "utilities.hpp"

#include <cwctype>

#include <windows.h>
#include <winternl.h>

namespace mem
{
    namespace
    {
        struct ldr_entry
        {
            LIST_ENTRY     in_load_order_links;
            LIST_ENTRY     in_memory_order_links;
            LIST_ENTRY     in_init_order_links;
            PVOID          dll_base;
            PVOID          entry_point;
            ULONG          size_of_image;
            UNICODE_STRING full_dll_name;
            UNICODE_STRING base_dll_name;
        };

        [[nodiscard]] bool iequal(std::wstring_view a, std::wstring_view b) noexcept
        {
            if (a.size() != b.size())
            {
                return false;
            }
            for (std::size_t i = 0; i < a.size(); ++i)
            {
                if (std::towlower(a[i]) != std::towlower(b[i]))
                {
                    return false;
                }
            }
            return true;
        }
    }

    module_info module_of(std::wstring_view name) noexcept
    {
        const auto peb = reinterpret_cast<PPEB>(__readgsqword(0x60));
        if (peb == nullptr || peb->Ldr == nullptr)
        {
            return {};
        }
        const auto head = &peb->Ldr->InMemoryOrderModuleList;
        for (auto cur = head->Flink; cur != head; cur = cur->Flink)
        {
            const auto entry = CONTAINING_RECORD(cur, ldr_entry, in_memory_order_links);
            const std::wstring_view base_name{
                entry->base_dll_name.Buffer,
                entry->base_dll_name.Length / sizeof(wchar_t)
            };
            if (iequal(base_name, name))
            {
                return { reinterpret_cast<std::uintptr_t>(entry->dll_base), entry->size_of_image };
            }
        }
        return {};
    }

    void* get_export(const module_info& mod, const char* name) noexcept
    {
        if (!mod.valid() || name == nullptr)
        {
            return nullptr;
        }
        return reinterpret_cast<void*>(
            GetProcAddress(reinterpret_cast<HMODULE>(mod.base), name));
    }

    bool wait_module(std::wstring_view name, std::uint32_t timeout_ms) noexcept
    {
        const auto start = GetTickCount64();
        while (GetTickCount64() - start < timeout_ms)
        {
            const auto m = module_of(name);
            if (m.valid())
            {
                return true;
            }
            Sleep(100);
        }
        return false;
    }
}
