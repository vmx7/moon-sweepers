#include "moon_steam_lobby.hpp"

#include "components/engine/il2cpp.hpp"
#include "utilities/log/log.hpp"

namespace moon_steam_lobby
{
    namespace
    {
        il2cpp::il2cpp_class*      class_         = nullptr;
        il2cpp::il2cpp_field_info* field_is_host_ = nullptr;
    }

    void resolve() noexcept
    {
        class_ = il2cpp::resolve("Assembly-CSharp.dll", "MoonCleaners", "MoonSteamLobby");
        if (class_ == nullptr)
        {
            lg::warn("lobby", "MoonSteamLobby not found");

            return;
        }

        field_is_host_ = il2cpp::class_get_field_from_name(class_, "<IsHost>k__BackingField");

        lg::info("lobby", "ready class=0x{:X} IsHost_field=0x{:X}",
            reinterpret_cast<std::uintptr_t>(class_),
            reinterpret_cast<std::uintptr_t>(field_is_host_));
    }

    bool ready() noexcept
    {
        return class_ != nullptr && field_is_host_ != nullptr;
    }

    bool is_host() noexcept
    {
        if (field_is_host_ == nullptr)
        {
            return false;
        }
        bool v = false;
        il2cpp::field_static_get_value(field_is_host_, &v);

        return v;
    }

    void set_is_host(bool v) noexcept
    {
        if (field_is_host_ == nullptr)
        {
            return;
        }
        bool b = v;
        il2cpp::field_static_set_value(field_is_host_, &b);
    }
}
