#include "moon_dev_tools.hpp"

#include "components/engine/il2cpp.hpp"
#include "utilities/log/log.hpp"

namespace moon_dev_tools
{
    namespace
    {
        il2cpp::il2cpp_class*       class_      = nullptr;
        il2cpp::il2cpp_field_info*  field_noclip_= nullptr;
        il2cpp::il2cpp_method_info* get_enabled_= nullptr;
    }

    void resolve() noexcept
    {
        class_ = il2cpp::resolve("Assembly-CSharp.dll", "MoonCleaners", "MoonDevTools");
        if (class_ == nullptr)
        {
            lg::warn("devtools", "MoonDevTools class not found");

            return;
        }

        field_noclip_ = il2cpp::class_get_field_from_name(class_, "Noclip");
        get_enabled_  = il2cpp::class_get_method_from_name(class_, "get_Enabled", 0);

        lg::info("devtools", "ready class=0x{:X} Noclip=0x{:X}",
            reinterpret_cast<std::uintptr_t>(class_),
            reinterpret_cast<std::uintptr_t>(field_noclip_));
    }

    bool ready() noexcept
    {
        return class_ != nullptr && field_noclip_ != nullptr;
    }

    bool noclip() noexcept
    {
        if (!ready())
        {
            return false;
        }

        return il2cpp::get_static<bool>(field_noclip_);
    }

    void set_noclip(bool v) noexcept
    {
        if (!ready())
        {
            return;
        }

        il2cpp::set_static<bool>(field_noclip_, v);
    }

    bool enabled() noexcept
    {
        if (get_enabled_ == nullptr)
        {
            return false;
        }

        il2cpp::il2cpp_object* exc = nullptr;
        auto* ret = il2cpp::runtime_invoke(get_enabled_, nullptr, nullptr, &exc);

        return ret != nullptr && *reinterpret_cast<bool*>(reinterpret_cast<std::uint8_t*>(ret) + 0x10);
    }
}
