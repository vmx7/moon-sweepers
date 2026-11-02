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
        il2cpp::il2cpp_method_info* method_dev_add_money_      = nullptr;
        il2cpp::il2cpp_method_info* method_dev_set_dust_       = nullptr;
        il2cpp::il2cpp_method_info* method_dev_complete_       = nullptr;
        il2cpp::il2cpp_method_info* method_dev_finish_all_     = nullptr;
        il2cpp::il2cpp_method_info* method_dev_reset_dust_     = nullptr;
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
        method_dev_add_money_      = il2cpp::class_get_method_from_name(class_, "DevAddMoney",         2);
        method_dev_set_dust_       = il2cpp::class_get_method_from_name(class_, "DevSetCarriedDust",   2);
        method_dev_complete_       = il2cpp::class_get_method_from_name(class_, "DevCompleteRegions",  2);
        method_dev_finish_all_     = il2cpp::class_get_method_from_name(class_, "DevFinishAllButOne",  2);
        method_dev_reset_dust_     = il2cpp::class_get_method_from_name(class_, "DevResetDust",        0);

        lg::info("world",
            "ready IsLandmarkRaised=0x{:X} DevAddMoney=0x{:X} DevSetDust=0x{:X} DevComplete=0x{:X} DevReset=0x{:X}",
            reinterpret_cast<std::uintptr_t>(method_is_landmark_raised_),
            reinterpret_cast<std::uintptr_t>(method_dev_add_money_),
            reinterpret_cast<std::uintptr_t>(method_dev_set_dust_),
            reinterpret_cast<std::uintptr_t>(method_dev_complete_),
            reinterpret_cast<std::uintptr_t>(method_dev_reset_dust_));
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

    namespace
    {
        struct il2cpp_decimal
        {
            std::int32_t  flags;
            std::int32_t  hi;
            std::uint32_t lo;
            std::uint32_t mid;
        };

        [[nodiscard]] il2cpp_decimal make_decimal_from_double(double v) noexcept
        {
            il2cpp_decimal d{};
            if (v < 0.0)
            {
                d.flags = static_cast<std::int32_t>(0x80000000);
                v = -v;
            }
            const auto whole = static_cast<std::uint64_t>(v);
            d.lo  = static_cast<std::uint32_t>(whole & 0xFFFFFFFFULL);
            d.mid = static_cast<std::uint32_t>((whole >> 32) & 0xFFFFFFFFULL);

            return d;
        }
    }

    void dev_add_money(il2cpp::il2cpp_object* world_instance,
                       il2cpp::il2cpp_object* player_string,
                       double                 dollars) noexcept
    {
        if (world_instance == nullptr || method_dev_add_money_ == nullptr || player_string == nullptr)
        {
            lg::warn("dev", "add_money early-out world=0x{:X} fn=0x{:X} player=0x{:X}",
                reinterpret_cast<std::uintptr_t>(world_instance),
                reinterpret_cast<std::uintptr_t>(method_dev_add_money_),
                reinterpret_cast<std::uintptr_t>(player_string));

            return;
        }

        auto d = make_decimal_from_double(dollars);
        void* args[] = { player_string, &d };
        il2cpp::il2cpp_object* exc = nullptr;
        auto* ret = il2cpp::runtime_invoke(method_dev_add_money_, world_instance, args, &exc);
        lg::info("dev", "add_money invoked ret=0x{:X} exc=0x{:X}",
            reinterpret_cast<std::uintptr_t>(ret),
            reinterpret_cast<std::uintptr_t>(exc));
    }

    void dev_set_carried_dust(il2cpp::il2cpp_object* world_instance,
                              il2cpp::il2cpp_object* player_string,
                              double                 kilograms) noexcept
    {
        if (world_instance == nullptr || method_dev_set_dust_ == nullptr || player_string == nullptr)
        {
            return;
        }

        double kg = kilograms;
        void* args[] = { player_string, &kg };
        il2cpp::il2cpp_object* exc = nullptr;
        (void)il2cpp::runtime_invoke(method_dev_set_dust_, world_instance, args, &exc);
    }

    std::int32_t dev_complete_regions(il2cpp::il2cpp_object* world_instance,
                                      const sdk::vector3&    point,
                                      std::int32_t           rings) noexcept
    {
        if (world_instance == nullptr || method_dev_complete_ == nullptr)
        {
            return 0;
        }

        sdk::vector3 p = point;
        std::int32_t r = rings;
        void* args[] = { &p, &r };
        il2cpp::il2cpp_object* exc = nullptr;
        auto* boxed = il2cpp::runtime_invoke(method_dev_complete_, world_instance, args, &exc);
        if (boxed == nullptr)
        {
            return 0;
        }

        return *reinterpret_cast<const std::int32_t*>(
            reinterpret_cast<const std::uint8_t*>(boxed) + 0x10);
    }

    std::int32_t dev_finish_all_but_one(il2cpp::il2cpp_object* world_instance,
                                        const sdk::vector3&    point,
                                        sdk::vector3&          last_out) noexcept
    {
        if (world_instance == nullptr || method_dev_finish_all_ == nullptr)
        {
            return 0;
        }

        sdk::vector3 p = point;
        sdk::vector3 last{};
        void* args[] = { &p, &last };
        il2cpp::il2cpp_object* exc = nullptr;
        auto* boxed = il2cpp::runtime_invoke(method_dev_finish_all_, world_instance, args, &exc);
        last_out = last;
        if (boxed == nullptr)
        {
            return 0;
        }

        return *reinterpret_cast<const std::int32_t*>(
            reinterpret_cast<const std::uint8_t*>(boxed) + 0x10);
    }

    bool dev_reset_dust(il2cpp::il2cpp_object* world_instance) noexcept
    {
        if (world_instance == nullptr || method_dev_reset_dust_ == nullptr)
        {
            return false;
        }

        il2cpp::il2cpp_object* exc = nullptr;
        auto* boxed = il2cpp::runtime_invoke(method_dev_reset_dust_, world_instance, nullptr, &exc);
        if (boxed == nullptr)
        {
            return false;
        }

        return *reinterpret_cast<const std::uint8_t*>(
            reinterpret_cast<const std::uint8_t*>(boxed) + 0x10) != 0;
    }
}
