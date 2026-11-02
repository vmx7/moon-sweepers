#include "moon_night_vision.hpp"

#include "components/engine/il2cpp.hpp"
#include "utilities/log/log.hpp"

namespace moon_night_vision
{
    namespace
    {
        il2cpp::il2cpp_class*      class_        = nullptr;
        il2cpp::il2cpp_field_info* field_amount_ = nullptr;
    }

    void resolve() noexcept
    {
        class_ = il2cpp::resolve("Assembly-CSharp.dll", "MoonCleaners", "MoonNightVision");
        if (class_ == nullptr)
        {
            lg::warn("nightvision", "MoonNightVision not found");

            return;
        }

        field_amount_ = il2cpp::class_get_field_from_name(class_, "<Amount>k__BackingField");

        lg::info("nightvision", "ready class=0x{:X} Amount_field=0x{:X}",
            reinterpret_cast<std::uintptr_t>(class_),
            reinterpret_cast<std::uintptr_t>(field_amount_));
    }

    bool ready() noexcept
    {
        return class_ != nullptr && field_amount_ != nullptr;
    }

    void set_amount(float amount) noexcept
    {
        if (field_amount_ == nullptr)
        {
            return;
        }
        il2cpp::field_static_set_value(field_amount_, &amount);
    }

    float amount() noexcept
    {
        if (field_amount_ == nullptr)
        {
            return 0.0f;
        }
        float v = 0.0f;
        il2cpp::field_static_get_value(field_amount_, &v);

        return v;
    }
}
