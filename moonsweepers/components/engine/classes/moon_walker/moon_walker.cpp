#include "moon_walker.hpp"

#include "utilities/log/log.hpp"

namespace moon_walker
{
    namespace
    {
        il2cpp::il2cpp_class*       class_                   = nullptr;
        il2cpp::il2cpp_method_info* set_flying_2_            = nullptr;
        il2cpp::il2cpp_method_info* set_night_vision_        = nullptr;
        il2cpp::il2cpp_method_info* set_gravity_boots_       = nullptr;
        il2cpp::il2cpp_method_info* unstick_                 = nullptr;
        il2cpp::il2cpp_method_info* set_fly_speed_           = nullptr;
        il2cpp::il2cpp_method_info* set_dev_speed_mult_      = nullptr;
        il2cpp::il2cpp_method_info* set_equipment_speed_mult_= nullptr;
        il2cpp::il2cpp_field_info*  field_view_camera_       = nullptr;
        std::int32_t                offset_view_camera_      = -1;
        il2cpp::il2cpp_field_info*  field_world_             = nullptr;
        std::int32_t                offset_world_            = -1;
        il2cpp::il2cpp_method_info* teleport_                = nullptr;
        il2cpp::il2cpp_field_info*  field_nv_on_             = nullptr;
        std::int32_t                offset_nv_on_            = -1;
        il2cpp::il2cpp_field_info*  field_gb_on_             = nullptr;
        std::int32_t                offset_gb_on_            = -1;
        il2cpp::il2cpp_field_info*  field_climb_speed_       = nullptr;
        std::int32_t                offset_climb_speed_      = -1;
        il2cpp::il2cpp_field_info*  field_earth_gravity_     = nullptr;
        std::int32_t                offset_earth_gravity_    = -1;

        void write_float_at(il2cpp::il2cpp_object* walker, std::int32_t off, float v) noexcept
        {
            if (walker == nullptr || off < 0)
            {
                return;
            }
            auto* dst = reinterpret_cast<std::uint8_t*>(walker) + off;
            *reinterpret_cast<float*>(dst) = v;
        }
    }

    void resolve() noexcept
    {
        class_ = il2cpp::resolve("Assembly-CSharp.dll", "MoonCleaners", "MoonWalker");
        if (class_ == nullptr)
        {
            lg::warn("walker", "MoonWalker not found");

            return;
        }

        set_flying_2_             = il2cpp::class_get_method_from_name(class_, "SetFlying",                     2);
        set_night_vision_         = il2cpp::class_get_method_from_name(class_, "SetNightVision",                1);
        set_gravity_boots_        = il2cpp::class_get_method_from_name(class_, "SetGravityBoots",               1);
        unstick_                  = il2cpp::class_get_method_from_name(class_, "Unstick",                       0);
        set_fly_speed_            = il2cpp::class_get_method_from_name(class_, "set_FlySpeed",                  1);
        set_dev_speed_mult_       = il2cpp::class_get_method_from_name(class_, "set_DevSpeedMultiplier",        1);
        set_equipment_speed_mult_ = il2cpp::class_get_method_from_name(class_, "set_EquipmentSpeedMultiplier",  1);
        field_view_camera_        = il2cpp::class_get_field_from_name(class_, "viewCamera");
        if (field_view_camera_ != nullptr)
        {
            offset_view_camera_ = il2cpp::field_get_offset(field_view_camera_);
        }
        field_world_              = il2cpp::class_get_field_from_name(class_, "world");
        if (field_world_ != nullptr)
        {
            offset_world_ = il2cpp::field_get_offset(field_world_);
        }
        teleport_                 = il2cpp::class_get_method_from_name(class_, "Teleport", 2);
        field_nv_on_              = il2cpp::class_get_field_from_name(class_, "<NightVisionOn>k__BackingField");
        if (field_nv_on_ != nullptr)
        {
            offset_nv_on_ = il2cpp::field_get_offset(field_nv_on_);
        }
        field_gb_on_              = il2cpp::class_get_field_from_name(class_, "<GravityBootsOn>k__BackingField");
        if (field_gb_on_ != nullptr)
        {
            offset_gb_on_ = il2cpp::field_get_offset(field_gb_on_);
        }

        const auto bind_float = [](const char* name,
                                   il2cpp::il2cpp_field_info*& field,
                                   std::int32_t&               off) noexcept
        {
            field = il2cpp::class_get_field_from_name(class_, name);
            if (field != nullptr)
            {
                off = il2cpp::field_get_offset(field);
            }
        };

        bind_float("ClimbSpeed",   field_climb_speed_,   offset_climb_speed_);
        bind_float("EarthGravity", field_earth_gravity_, offset_earth_gravity_);

        lg::info("walker",
            "ready class=0x{:X} SetFlying=0x{:X} SetNightVision=0x{:X} viewCamera_off=0x{:X} world_off=0x{:X}",
            reinterpret_cast<std::uintptr_t>(class_),
            reinterpret_cast<std::uintptr_t>(set_flying_2_),
            reinterpret_cast<std::uintptr_t>(set_night_vision_),
            static_cast<std::uintptr_t>(offset_view_camera_),
            static_cast<std::uintptr_t>(offset_world_));
    }

    bool ready() noexcept
    {
        return class_ != nullptr;
    }

    il2cpp::il2cpp_method_info* method_set_flying() noexcept         { return set_flying_2_; }
    il2cpp::il2cpp_method_info* method_set_night_vision() noexcept   { return set_night_vision_; }
    il2cpp::il2cpp_method_info* method_set_gravity_boots() noexcept  { return set_gravity_boots_; }
    il2cpp::il2cpp_method_info* method_unstick() noexcept            { return unstick_; }
    il2cpp::il2cpp_method_info* set_fly_speed() noexcept             { return set_fly_speed_; }
    il2cpp::il2cpp_method_info* set_dev_speed_multiplier() noexcept  { return set_dev_speed_mult_; }
    il2cpp::il2cpp_method_info* set_equipment_speed_multiplier() noexcept { return set_equipment_speed_mult_; }

    il2cpp::il2cpp_object* view_camera(il2cpp::il2cpp_object* walker) noexcept
    {
        if (walker == nullptr || offset_view_camera_ < 0)
        {
            return nullptr;
        }

        return *reinterpret_cast<il2cpp::il2cpp_object**>(
            reinterpret_cast<std::uint8_t*>(walker) + offset_view_camera_);
    }

    il2cpp::il2cpp_object* world(il2cpp::il2cpp_object* walker) noexcept
    {
        if (walker == nullptr || offset_world_ < 0)
        {
            return nullptr;
        }

        return *reinterpret_cast<il2cpp::il2cpp_object**>(
            reinterpret_cast<std::uint8_t*>(walker) + offset_world_);
    }

    void teleport(il2cpp::il2cpp_object* walker, float x, float y, float z) noexcept
    {
        if (walker == nullptr || teleport_ == nullptr)
        {
            return;
        }

        float pos[3]{ x, y, z };
        bool  refresh = true;
        void* args[]  = { pos, &refresh };
        il2cpp::il2cpp_object* exc = nullptr;
        (void)il2cpp::runtime_invoke(teleport_, walker, args, &exc);
    }

    void set_night_vision_field(il2cpp::il2cpp_object* walker, bool on) noexcept
    {
        if (walker == nullptr || offset_nv_on_ < 0)
        {
            return;
        }
        auto* dst = reinterpret_cast<std::uint8_t*>(walker) + offset_nv_on_;
        *dst = on ? 1 : 0;
    }

    void set_gravity_boots_field(il2cpp::il2cpp_object* walker, bool on) noexcept
    {
        if (walker == nullptr || offset_gb_on_ < 0)
        {
            return;
        }
        auto* dst = reinterpret_cast<std::uint8_t*>(walker) + offset_gb_on_;
        *dst = on ? 1 : 0;
    }

    void call_unstick(il2cpp::il2cpp_object* walker) noexcept
    {
        if (walker == nullptr || unstick_ == nullptr)
        {
            return;
        }
        il2cpp::il2cpp_object* exc = nullptr;
        (void)il2cpp::runtime_invoke(unstick_, walker, nullptr, &exc);
    }

    void set_climb_speed(il2cpp::il2cpp_object* walker, float v) noexcept   { write_float_at(walker, offset_climb_speed_,   v); }
    void set_earth_gravity(il2cpp::il2cpp_object* walker, float v) noexcept { write_float_at(walker, offset_earth_gravity_, v); }
}
