#include "moon_world.hpp"

#include <cstdint>
#include <cstring>

#include "utilities/log/log.hpp"

namespace moon_world
{
    namespace
    {
        il2cpp::il2cpp_class*       class_                     = nullptr;
        il2cpp::il2cpp_method_info* method_gentle_ground_      = nullptr;
        il2cpp::il2cpp_method_info* method_surface_at_         = nullptr;
        il2cpp::il2cpp_method_info* method_is_landmark_raised_ = nullptr;
        il2cpp::il2cpp_method_info* method_crew_raise_         = nullptr;
        il2cpp::il2cpp_field_info*  field_local_player_        = nullptr;
    }

    void resolve() noexcept
    {
        class_ = il2cpp::resolve("Assembly-CSharp.dll", "MoonCleaners", "MoonWorld");
        if (class_ == nullptr)
        {
            lg::warn("world", "MoonWorld not found");

            return;
        }

        method_gentle_ground_      = il2cpp::class_get_method_from_name(class_, "GentleGround",      1);
        method_surface_at_         = il2cpp::class_get_method_from_name(class_, "SurfaceAt",         3);
        method_is_landmark_raised_ = il2cpp::class_get_method_from_name(class_, "IsLandmarkRaised",  1);
        method_crew_raise_         = il2cpp::class_get_method_from_name(class_, "CrewRaiseLandmark", 2);
        field_local_player_        = il2cpp::class_get_field_from_name(class_, "LocalPlayer");

        lg::info("world",
            "ready class=0x{:X} IsLandmarkRaised=0x{:X} CrewRaise=0x{:X} LocalPlayer_field=0x{:X}",
            reinterpret_cast<std::uintptr_t>(class_),
            reinterpret_cast<std::uintptr_t>(method_is_landmark_raised_),
            reinterpret_cast<std::uintptr_t>(method_crew_raise_),
            reinterpret_cast<std::uintptr_t>(field_local_player_));
    }

    bool ready() noexcept
    {
        return class_ != nullptr
            && (method_gentle_ground_ != nullptr || method_surface_at_ != nullptr);
    }

    bool gentle_ground(il2cpp::il2cpp_object* world_instance,
                       const sdk::vector3&    direction,
                       sdk::vector3&          out) noexcept
    {
        if (world_instance == nullptr)
        {
            return false;
        }

        sdk::vector3 dir = direction;

        if (method_surface_at_ != nullptr)
        {
            sdk::vector3 up{};
            std::uint8_t on_structure = 0;
            void* args[] = { &dir, &up, &on_structure };
            il2cpp::il2cpp_object* exc = nullptr;
            auto* boxed = il2cpp::runtime_invoke(method_surface_at_, world_instance, args, &exc);
            if (boxed != nullptr)
            {
                std::memcpy(&out, reinterpret_cast<std::uint8_t*>(boxed) + 0x10, sizeof(out));

                return true;
            }
        }

        return false;
    }

    bool is_landmark_raised(il2cpp::il2cpp_object* world_instance,
                            std::int32_t          id) noexcept
    {
        if (world_instance == nullptr || method_is_landmark_raised_ == nullptr)
        {
            return false;
        }

        std::int32_t value = id;
        void* args[] = { &value };
        il2cpp::il2cpp_object* exc = nullptr;
        auto* boxed = il2cpp::runtime_invoke(method_is_landmark_raised_, world_instance, args, &exc);
        if (boxed == nullptr)
        {
            return false;
        }

        return *reinterpret_cast<const std::uint8_t*>(
            reinterpret_cast<const std::uint8_t*>(boxed) + 0x10) != 0;
    }

    il2cpp::il2cpp_object* local_player() noexcept
    {
        if (field_local_player_ == nullptr)
        {
            return nullptr;
        }

        il2cpp::il2cpp_object* str = nullptr;
        il2cpp::field_static_get_value(field_local_player_, &str);

        return str;
    }

    bool crew_raise_landmark(il2cpp::il2cpp_object* world_instance,
                             il2cpp::il2cpp_object* player_string,
                             std::int32_t          index) noexcept
    {
        if (world_instance == nullptr || method_crew_raise_ == nullptr || player_string == nullptr)
        {
            return false;
        }

        std::int32_t idx = index;
        void* args[] = { player_string, &idx };
        il2cpp::il2cpp_object* exc = nullptr;
        auto* boxed = il2cpp::runtime_invoke(method_crew_raise_, world_instance, args, &exc);
        if (boxed == nullptr)
        {
            return false;
        }

        return *reinterpret_cast<const std::uint8_t*>(
            reinterpret_cast<const std::uint8_t*>(boxed) + 0x10) != 0;
    }
}
