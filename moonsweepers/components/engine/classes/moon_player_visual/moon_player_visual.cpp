#include "moon_player_visual.hpp"

#include <cstdint>

#include "utilities/log/log.hpp"

namespace moon_player_visual
{
    namespace
    {
        il2cpp::il2cpp_class*      class_        = nullptr;
        il2cpp::il2cpp_field_info* field_brush_  = nullptr;
        std::int32_t               offset_brush_ = -1;
    }

    void resolve() noexcept
    {
        class_ = il2cpp::resolve("Assembly-CSharp.dll", "MoonCleaners", "MoonPlayerVisual");
        if (class_ == nullptr)
        {
            lg::warn("playervisual", "MoonPlayerVisual not found");

            return;
        }

        field_brush_ = il2cpp::class_get_field_from_name(class_, "brush");
        if (field_brush_ != nullptr)
        {
            offset_brush_ = il2cpp::field_get_offset(field_brush_);
        }

        lg::info("playervisual", "ready class=0x{:X} brush_off=0x{:X}",
            reinterpret_cast<std::uintptr_t>(class_),
            static_cast<std::uintptr_t>(offset_brush_));
    }

    bool ready() noexcept
    {
        return class_ != nullptr && offset_brush_ >= 0;
    }

    il2cpp::il2cpp_object* brush(il2cpp::il2cpp_object* visual) noexcept
    {
        if (visual == nullptr || offset_brush_ < 0)
        {
            return nullptr;
        }

        return *reinterpret_cast<il2cpp::il2cpp_object**>(
            reinterpret_cast<std::uint8_t*>(visual) + offset_brush_);
    }
}
