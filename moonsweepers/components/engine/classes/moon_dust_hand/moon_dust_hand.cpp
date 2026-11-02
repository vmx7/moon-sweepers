#include "moon_dust_hand.hpp"

#include "utilities/log/log.hpp"

namespace moon_dust_hand
{
    namespace
    {
        il2cpp::il2cpp_class*      class_         = nullptr;
        il2cpp::il2cpp_field_info* f_reach_       = nullptr;
        il2cpp::il2cpp_field_info* f_inventory_   = nullptr;
    }

    void resolve() noexcept
    {
        class_ = il2cpp::resolve("Assembly-CSharp.dll", "MoonCleaners", "MoonDustHand");
        if (class_ == nullptr)
        {
            lg::warn("hand", "MoonDustHand not found");

            return;
        }

        f_reach_     = il2cpp::class_get_field_from_name(class_, "reach");
        f_inventory_ = il2cpp::class_get_field_from_name(class_, "inventory");

        lg::info("hand", "ready class=0x{:X}", reinterpret_cast<std::uintptr_t>(class_));
    }

    bool ready() noexcept
    {
        return class_ != nullptr;
    }

    il2cpp::il2cpp_field_info* field_reach()     noexcept { return f_reach_; }
    il2cpp::il2cpp_field_info* field_inventory() noexcept { return f_inventory_; }
}
