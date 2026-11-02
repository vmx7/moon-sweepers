#include "moon_remote_player.hpp"

#include <algorithm>
#include <cstdint>
#include <cstring>

#include "utilities/log/log.hpp"

namespace moon_remote_player
{
    namespace
    {
        il2cpp::il2cpp_class*       class_remote_             = nullptr;
        il2cpp::il2cpp_class*       class_unity_object_       = nullptr;
        il2cpp::il2cpp_class*       class_component_          = nullptr;
        il2cpp::il2cpp_class*       class_transform_          = nullptr;
        il2cpp::il2cpp_method_info* method_find_objects_      = nullptr;
        il2cpp::il2cpp_method_info* method_get_transform_     = nullptr;
        il2cpp::il2cpp_method_info* method_get_position_      = nullptr;
        il2cpp::il2cpp_field_info*  field_display_name_       = nullptr;

        [[nodiscard]] std::string read_il2cpp_string(il2cpp::il2cpp_object* str) noexcept
        {
            std::string out;
            if (str == nullptr)
            {
                return out;
            }
            const auto length = *reinterpret_cast<const std::int32_t*>(
                reinterpret_cast<const std::uint8_t*>(str) + 0x10);
            const auto* chars = reinterpret_cast<const wchar_t*>(
                reinterpret_cast<const std::uint8_t*>(str) + 0x14);
            if (length <= 0 || length > 1024)
            {
                return out;
            }
            out.reserve(static_cast<std::size_t>(length));
            for (std::int32_t c = 0; c < length; ++c)
            {
                const auto ch = chars[c];
                out.push_back((ch >= 0x20 && ch < 0x7F) ? static_cast<char>(ch) : '?');
            }

            return out;
        }
    }

    void resolve() noexcept
    {
        class_remote_       = il2cpp::resolve("Assembly-CSharp.dll", "MoonCleaners", "MoonRemotePlayer");
        class_unity_object_ = il2cpp::resolve_in("UnityEngine.CoreModule.dll", "UnityEngine", "Object");
        class_component_    = il2cpp::resolve_in("UnityEngine.CoreModule.dll", "UnityEngine", "Component");
        class_transform_    = il2cpp::resolve_in("UnityEngine.CoreModule.dll", "UnityEngine", "Transform");

        if (class_remote_ == nullptr || class_unity_object_ == nullptr)
        {
            lg::warn("remote", "MoonRemotePlayer or UnityEngine.Object not found");

            return;
        }

        method_find_objects_ = il2cpp::class_get_method_from_name(class_unity_object_, "FindObjectsOfType", 1);
        if (method_find_objects_ == nullptr)
        {
            method_find_objects_ = il2cpp::class_get_method_from_name(class_unity_object_, "FindObjectsByType", 2);
        }

        if (class_component_ != nullptr)
        {
            method_get_transform_ = il2cpp::class_get_method_from_name(class_component_, "get_transform", 0);
        }
        if (class_transform_ != nullptr)
        {
            method_get_position_ = il2cpp::class_get_method_from_name(class_transform_, "get_position", 0);
        }

        field_display_name_ = il2cpp::class_get_field_from_name(class_remote_, "<DisplayName>k__BackingField");

        lg::info("remote",
            "ready class=0x{:X} FindObjects=0x{:X} get_transform=0x{:X} DisplayName=0x{:X}",
            reinterpret_cast<std::uintptr_t>(class_remote_),
            reinterpret_cast<std::uintptr_t>(method_find_objects_),
            reinterpret_cast<std::uintptr_t>(method_get_transform_),
            reinterpret_cast<std::uintptr_t>(field_display_name_));
    }

    bool ready() noexcept
    {
        return class_remote_ != nullptr
            && method_find_objects_ != nullptr
            && method_get_transform_ != nullptr
            && method_get_position_ != nullptr;
    }

    std::vector<snapshot> find_all() noexcept
    {
        std::vector<snapshot> out;
        if (!ready())
        {
            return out;
        }

        const auto* type_obj = il2cpp::type_get_object(il2cpp::class_get_type(class_remote_));
        if (type_obj == nullptr)
        {
            return out;
        }

        il2cpp::il2cpp_object* exc = nullptr;
        il2cpp::il2cpp_object* array = nullptr;
        if (il2cpp::class_get_method_from_name(class_unity_object_, "FindObjectsOfType", 1) != nullptr)
        {
            void* args[] = { const_cast<void*>(static_cast<const void*>(type_obj)) };
            array = il2cpp::runtime_invoke(method_find_objects_, nullptr, args, &exc);
        }
        else
        {
            std::int32_t sort_mode = 1;
            void* args[] = { const_cast<void*>(static_cast<const void*>(type_obj)), &sort_mode };
            array = il2cpp::runtime_invoke(method_find_objects_, nullptr, args, &exc);
        }
        if (array == nullptr)
        {
            return out;
        }

        const auto count = *reinterpret_cast<std::int32_t*>(
            reinterpret_cast<std::uint8_t*>(array) + 0x18);
        auto** items = reinterpret_cast<il2cpp::il2cpp_object**>(
            reinterpret_cast<std::uint8_t*>(array) + 0x20);

        if (count <= 0 || count > 64)
        {
            return out;
        }

        out.reserve(static_cast<std::size_t>(count));
        for (std::int32_t i = 0; i < count; ++i)
        {
            auto* rp = items[i];
            if (rp == nullptr)
            {
                continue;
            }

            snapshot s{};
            s.object = rp;

            auto* t = il2cpp::runtime_invoke(method_get_transform_, rp, nullptr, &exc);
            if (t != nullptr)
            {
                auto* pos = il2cpp::runtime_invoke(method_get_position_, t, nullptr, &exc);
                if (pos != nullptr)
                {
                    std::memcpy(&s.position,
                                reinterpret_cast<std::uint8_t*>(pos) + 0x10,
                                sizeof(s.position));
                }
            }

            if (field_display_name_ != nullptr)
            {
                il2cpp::il2cpp_object* name_str = nullptr;
                il2cpp::field_get_value(rp, field_display_name_, &name_str);
                s.name = read_il2cpp_string(name_str);
            }

            out.push_back(std::move(s));
        }

        return out;
    }
}
