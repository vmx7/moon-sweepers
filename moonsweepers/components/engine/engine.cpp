#include "engine.hpp"

#include <atomic>

#include "components/engine/classes/camera/camera.hpp"
#include "components/engine/classes/moon_brush/moon_brush.hpp"
#include "components/engine/classes/moon_crew_session/moon_crew_session.hpp"
#include "components/engine/classes/moon_dev_tools/moon_dev_tools.hpp"
#include "components/engine/classes/moon_dust_hand/moon_dust_hand.hpp"
#include "components/engine/classes/moon_landmark/moon_landmark.hpp"
#include "components/engine/classes/moon_night_vision/moon_night_vision.hpp"
#include "components/engine/classes/moon_player_visual/moon_player_visual.hpp"
#include "components/engine/classes/moon_run_over/moon_run_over.hpp"
#include "components/engine/classes/moon_walker/moon_walker.hpp"
#include "components/engine/classes/moon_world/moon_world.hpp"
#include "components/engine/il2cpp.hpp"
#include "utilities/log/log.hpp"

namespace engine
{
    namespace
    {
        std::atomic<bool>      ready_{ false };
        mem::module_info       game_assembly_{};
        mem::module_info       unity_player_{};
        il2cpp::il2cpp_object* local_walker_{ nullptr };
    }

    bool init() noexcept
    {
        game_assembly_ = mem::module_of(L"GameAssembly.dll");
        unity_player_  = mem::module_of(L"UnityPlayer.dll");

        if (!game_assembly_.valid() || !unity_player_.valid())
        {
            lg::error("engine", "required modules not mapped");

            return false;
        }

        if (!il2cpp::init())
        {
            lg::error("engine", "il2cpp bind failed");

            return false;
        }

        camera::resolve();
        moon_brush::resolve();
        moon_walker::resolve();
        moon_run_over::resolve();
        moon_world::resolve();
        moon_dev_tools::resolve();
        moon_dust_hand::resolve();
        moon_landmark::resolve();
        moon_crew_session::resolve();
        moon_player_visual::resolve();
        moon_night_vision::resolve();

        ready_.store(true, std::memory_order_release);
        lg::info("engine", "engine ready game=0x{:X} unity=0x{:X}",
            game_assembly_.base, unity_player_.base);

        return true;
    }

    void shutdown() noexcept
    {
        ready_.store(false, std::memory_order_release);
    }

    void tick() noexcept
    {
        if (!ready_.load(std::memory_order_acquire))
        {
            return;
        }

        thread_local bool attached = false;
        if (!attached)
        {
            il2cpp::thread_attach(il2cpp::domain_get());
            attached = true;
            lg::info("engine", "render thread attached to il2cpp domain");
        }

        if (moon_run_over::ready())
        {
            local_walker_ = moon_run_over::local_walker();
        }

        camera::refresh();
    }

    const mem::module_info& game_assembly() noexcept
    {
        return game_assembly_;
    }

    const mem::module_info& unity_player() noexcept
    {
        return unity_player_;
    }

    il2cpp::il2cpp_object* local_walker() noexcept
    {
        return local_walker_;
    }
}
