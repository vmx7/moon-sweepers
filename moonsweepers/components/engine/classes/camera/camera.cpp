#include "camera.hpp"

#include <cstring>

#include "components/engine/classes/moon_walker/moon_walker.hpp"
#include "components/engine/engine.hpp"
#include "components/engine/il2cpp.hpp"
#include "utilities/log/log.hpp"

#include "imgui/imgui.h"

namespace camera
{
    namespace
    {
        il2cpp::il2cpp_class*       class_camera_        = nullptr;
        il2cpp::il2cpp_class*       class_transform_     = nullptr;
        il2cpp::il2cpp_method_info* method_get_main_     = nullptr;
        il2cpp::il2cpp_method_info* method_wts_          = nullptr;
        il2cpp::il2cpp_method_info* method_get_transform_= nullptr;
        il2cpp::il2cpp_method_info* method_get_position_ = nullptr;
        il2cpp::il2cpp_object*      main_camera_         = nullptr;

        sdk::vector3 position_{};
        bool         have_main_{ false };
    }

    void resolve() noexcept
    {
        class_camera_ = il2cpp::resolve_in("UnityEngine.CoreModule.dll", "UnityEngine", "Camera");
        if (class_camera_ == nullptr)
        {
            class_camera_ = il2cpp::resolve_in("UnityEngine.dll", "UnityEngine", "Camera");
        }

        class_transform_ = il2cpp::resolve_in("UnityEngine.CoreModule.dll", "UnityEngine", "Transform");
        if (class_transform_ == nullptr)
        {
            class_transform_ = il2cpp::resolve_in("UnityEngine.dll", "UnityEngine", "Transform");
        }

        if (class_camera_ == nullptr)
        {
            lg::warn("camera", "UnityEngine.Camera not found");

            return;
        }

        method_get_main_      = il2cpp::class_get_method_from_name(class_camera_,    "get_main",            0);
        method_wts_           = il2cpp::class_get_method_from_name(class_camera_,    "WorldToScreenPoint",  1);
        method_get_transform_ = il2cpp::class_get_method_from_name(class_camera_,    "get_transform",       0);

        if (class_transform_ != nullptr)
        {
            method_get_position_ = il2cpp::class_get_method_from_name(class_transform_, "get_position", 0);
        }

        lg::info("camera", "class=0x{:X} get_main=0x{:X} wts=0x{:X} get_transform=0x{:X} get_position=0x{:X}",
            reinterpret_cast<std::uintptr_t>(class_camera_),
            reinterpret_cast<std::uintptr_t>(method_get_main_),
            reinterpret_cast<std::uintptr_t>(method_wts_),
            reinterpret_cast<std::uintptr_t>(method_get_transform_),
            reinterpret_cast<std::uintptr_t>(method_get_position_));
    }

    void refresh() noexcept
    {
        il2cpp::il2cpp_object* exc = nullptr;

        main_camera_ = moon_walker::view_camera(engine::local_walker());
        if (main_camera_ == nullptr && method_get_main_ != nullptr)
        {
            main_camera_ = il2cpp::runtime_invoke(method_get_main_, nullptr, nullptr, &exc);
        }

        if (main_camera_ == nullptr)
        {
            have_main_ = false;

            return;
        }
        have_main_ = true;

        if (method_get_transform_ != nullptr && method_get_position_ != nullptr)
        {
            auto* t = il2cpp::runtime_invoke(method_get_transform_, main_camera_, nullptr, &exc);
            if (t != nullptr)
            {
                auto* p = il2cpp::runtime_invoke(method_get_position_, t, nullptr, &exc);
                if (p != nullptr)
                {
                    std::memcpy(&position_, reinterpret_cast<std::uint8_t*>(p) + 0x10, sizeof(position_));
                }
            }
        }
    }

    bool ready() noexcept
    {
        return have_main_ && method_wts_ != nullptr;
    }

    const sdk::vector3& position() noexcept
    {
        return position_;
    }

    sdk::vector2 viewport() noexcept
    {
        const auto size = ImGui::GetIO().DisplaySize;

        return { size.x, size.y };
    }

    bool world_to_screen(const sdk::vector3& world, sdk::vector2& out) noexcept
    {
        if (!have_main_ || method_wts_ == nullptr)
        {
            return false;
        }

        sdk::vector3 w = world;
        void* args[] = { &w };
        il2cpp::il2cpp_object* exc = nullptr;
        auto* boxed = il2cpp::runtime_invoke(method_wts_, main_camera_, args, &exc);
        if (boxed == nullptr)
        {
            return false;
        }

        sdk::vector3 s{};
        std::memcpy(&s, reinterpret_cast<std::uint8_t*>(boxed) + 0x10, sizeof(s));

        if (s.z <= 0.0f)
        {
            return false;
        }

        const auto vp = viewport();
        out.x = s.x;
        out.y = vp.y - s.y;

        return true;
    }

    const sdk::matrix4x4& view_projection() noexcept
    {
        static sdk::matrix4x4 ident{};

        return ident;
    }
}
