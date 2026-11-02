#include "moon_landmark.hpp"

#include <algorithm>
#include <cstdint>
#include <cstring>

#include <cmath>

#include "components/engine/classes/camera/camera.hpp"
#include "components/engine/classes/moon_walker/moon_walker.hpp"
#include "components/engine/classes/moon_world/moon_world.hpp"
#include "components/engine/engine.hpp"
#include "utilities/log/log.hpp"

namespace moon_landmark
{
    namespace
    {
        il2cpp::il2cpp_class*       class_landmarks_ = nullptr;
        il2cpp::il2cpp_class*       class_landmark_  = nullptr;
        il2cpp::il2cpp_method_info* method_get_all_  = nullptr;
        il2cpp::il2cpp_method_info* method_direction_= nullptr;
        il2cpp::il2cpp_field_info*  field_latitude_  = nullptr;
        il2cpp::il2cpp_field_info*  field_longitude_ = nullptr;
        il2cpp::il2cpp_field_info*  field_kind_      = nullptr;
        il2cpp::il2cpp_field_info*  field_name_      = nullptr;
        il2cpp::il2cpp_field_info*  field_id_        = nullptr;
    }

    void resolve() noexcept
    {
        class_landmarks_ = il2cpp::resolve("Assembly-CSharp.dll", "MoonCleaners", "MoonLandmarks");
        class_landmark_  = il2cpp::resolve("Assembly-CSharp.dll", "MoonCleaners", "MoonLandmark");

        if (class_landmarks_ != nullptr)
        {
            method_get_all_   = il2cpp::class_get_method_from_name(class_landmarks_, "get_All",   0);
            method_direction_ = il2cpp::class_get_method_from_name(class_landmarks_, "Direction", 2);
        }

        if (class_landmark_ != nullptr)
        {
            field_latitude_  = il2cpp::class_get_field_from_name(class_landmark_, "latitude");
            field_longitude_ = il2cpp::class_get_field_from_name(class_landmark_, "longitude");
            field_kind_      = il2cpp::class_get_field_from_name(class_landmark_, "kind");
            field_name_      = il2cpp::class_get_field_from_name(class_landmark_, "name");
            field_id_        = il2cpp::class_get_field_from_name(class_landmark_, "index");
        }

        lg::info("landmark", "ready landmarks=0x{:X} landmark=0x{:X} get_All=0x{:X} Direction=0x{:X}",
            reinterpret_cast<std::uintptr_t>(class_landmarks_),
            reinterpret_cast<std::uintptr_t>(class_landmark_),
            reinterpret_cast<std::uintptr_t>(method_get_all_),
            reinterpret_cast<std::uintptr_t>(method_direction_));
    }

    bool ready() noexcept
    {
        return class_landmarks_ != nullptr
            && class_landmark_  != nullptr
            && method_get_all_  != nullptr
            && method_direction_ != nullptr
            && field_latitude_  != nullptr
            && field_longitude_ != nullptr;
    }

    std::vector<marker> snapshot() noexcept
    {
        std::vector<marker> out;

        if (!ready())
        {
            return out;
        }

        il2cpp::il2cpp_object* exc = nullptr;
        auto* list = il2cpp::runtime_invoke(method_get_all_, nullptr, nullptr, &exc);
        if (list == nullptr)
        {
            return out;
        }

        const auto count = *reinterpret_cast<std::int32_t*>(
            reinterpret_cast<std::uint8_t*>(list) + 0x18);
        auto** items = reinterpret_cast<il2cpp::il2cpp_object**>(
            reinterpret_cast<std::uint8_t*>(list) + 0x20);

        if (count <= 0 || count > 100000)
        {
            return out;
        }

        out.reserve(static_cast<std::size_t>(count));

        auto* world_inst = moon_walker::world(engine::local_walker());

        for (std::int32_t i = 0; i < count; ++i)
        {
            auto* obj = items[i];
            if (obj == nullptr)
            {
                continue;
            }

            float latitude  = 0.0f;
            float longitude = 0.0f;
            std::uint8_t raw_kind = 0xFF;

            il2cpp::field_get_value(obj, field_latitude_,  &latitude);
            il2cpp::field_get_value(obj, field_longitude_, &longitude);
            if (field_kind_ != nullptr)
            {
                il2cpp::field_get_value(obj, field_kind_, &raw_kind);
            }

            void* dir_args[] = { &latitude, &longitude };
            il2cpp::il2cpp_object* d_exc = nullptr;
            auto* dir_boxed = il2cpp::runtime_invoke(method_direction_, nullptr, dir_args, &d_exc);
            if (dir_boxed == nullptr)
            {
                continue;
            }

            sdk::vector3 dir{};
            std::memcpy(&dir, reinterpret_cast<std::uint8_t*>(dir_boxed) + 0x10, sizeof(dir));

            const auto& cam = camera::position();
            const float cam_mag = std::sqrt(cam.x * cam.x + cam.y * cam.y + cam.z * cam.z);
            const float radius = cam_mag > 100.0f ? cam_mag : 2000.0f;
            sdk::vector3 pos{ dir.x * radius, dir.y * radius, dir.z * radius };

            std::int32_t id = -1;
            if (field_id_ != nullptr)
            {
                il2cpp::field_get_value(obj, field_id_, &id);
            }

            marker m{};
            m.world  = pos;
            m.kind   = static_cast<sdk::landmark_kind>(raw_kind);
            m.handle = reinterpret_cast<std::uintptr_t>(obj);
            m.id     = id;
            m.raised = (world_inst != nullptr && id >= 0)
                         ? moon_world::is_landmark_raised(world_inst, id)
                         : false;

            if (field_name_ != nullptr)
            {
                il2cpp::il2cpp_object* name_str = nullptr;
                il2cpp::field_get_value(obj, field_name_, &name_str);
                if (name_str != nullptr)
                {
                    const auto length = *reinterpret_cast<const std::int32_t*>(
                        reinterpret_cast<const std::uint8_t*>(name_str) + 0x10);
                    const auto* chars = reinterpret_cast<const wchar_t*>(
                        reinterpret_cast<const std::uint8_t*>(name_str) + 0x14);
                    const auto n = std::min<std::int32_t>(length, static_cast<std::int32_t>(sizeof(m.name) - 1));
                    for (std::int32_t c = 0; c < n; ++c)
                    {
                        const auto ch = chars[c];
                        m.name[c] = (ch >= 0x20 && ch < 0x7F) ? static_cast<char>(ch) : '?';
                    }
                    m.name[n] = '\0';
                }
            }

            out.push_back(m);
        }

        return out;
    }
}
