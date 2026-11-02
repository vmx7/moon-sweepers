#include "moon_crew_session.hpp"

#include <cstdint>

#include "utilities/log/log.hpp"

namespace moon_crew_session
{
    namespace
    {
        il2cpp::il2cpp_class*       class_               = nullptr;
        il2cpp::il2cpp_method_info* method_get_instance_ = nullptr;
        il2cpp::il2cpp_field_info*  field_local_visual_  = nullptr;
        std::int32_t                offset_local_visual_ = -1;
    }

    void resolve() noexcept
    {
        class_ = il2cpp::resolve("Assembly-CSharp.dll", "MoonCleaners", "MoonCrewSession");
        if (class_ == nullptr)
        {
            lg::warn("crewsession", "MoonCrewSession not found");

            return;
        }

        method_get_instance_ = il2cpp::class_get_method_from_name(class_, "get_Instance", 0);
        field_local_visual_  = il2cpp::class_get_field_from_name(class_, "localVisual");
        if (field_local_visual_ != nullptr)
        {
            offset_local_visual_ = il2cpp::field_get_offset(field_local_visual_);
        }

        lg::info("crewsession", "ready class=0x{:X} get_Instance=0x{:X} localVisual_off=0x{:X}",
            reinterpret_cast<std::uintptr_t>(class_),
            reinterpret_cast<std::uintptr_t>(method_get_instance_),
            static_cast<std::uintptr_t>(offset_local_visual_));
    }

    bool ready() noexcept
    {
        return class_ != nullptr && method_get_instance_ != nullptr && offset_local_visual_ >= 0;
    }

    il2cpp::il2cpp_object* instance() noexcept
    {
        if (method_get_instance_ == nullptr)
        {
            return nullptr;
        }

        il2cpp::il2cpp_object* exc = nullptr;

        return il2cpp::runtime_invoke(method_get_instance_, nullptr, nullptr, &exc);
    }

    il2cpp::il2cpp_object* local_visual() noexcept
    {
        auto* inst = instance();
        if (inst == nullptr || offset_local_visual_ < 0)
        {
            return nullptr;
        }

        return *reinterpret_cast<il2cpp::il2cpp_object**>(
            reinterpret_cast<std::uint8_t*>(inst) + offset_local_visual_);
    }
}
