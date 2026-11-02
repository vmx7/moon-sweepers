#include "moon_run_over.hpp"

#include "utilities/log/log.hpp"

namespace moon_run_over
{
    namespace
    {
        il2cpp::il2cpp_class*       class_             = nullptr;
        il2cpp::il2cpp_method_info* get_local_walker_  = nullptr;
    }

    void resolve() noexcept
    {
        class_ = il2cpp::resolve("Assembly-CSharp.dll", "MoonCleaners", "MoonRunOver");
        if (class_ == nullptr)
        {
            lg::warn("runover", "MoonRunOver not found");

            return;
        }

        get_local_walker_ = il2cpp::class_get_method_from_name(class_, "get_LocalWalker", 0);

        lg::info("runover", "ready class=0x{:X} get_LocalWalker=0x{:X}",
            reinterpret_cast<std::uintptr_t>(class_),
            reinterpret_cast<std::uintptr_t>(get_local_walker_));
    }

    bool ready() noexcept
    {
        return class_ != nullptr && get_local_walker_ != nullptr;
    }

    il2cpp::il2cpp_object* local_walker() noexcept
    {
        if (get_local_walker_ == nullptr)
        {
            return nullptr;
        }

        il2cpp::il2cpp_object* exc = nullptr;

        return il2cpp::runtime_invoke(get_local_walker_, nullptr, nullptr, &exc);
    }
}
