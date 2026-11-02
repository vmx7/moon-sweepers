#include "log.hpp"

#include <mutex>

#include <windows.h>

namespace lg::detail
{
    namespace
    {
        std::mutex sink_mutex;

        [[nodiscard]] constexpr std::string_view tag_of(level lvl) noexcept
        {
            switch (lvl)
            {
                case level::debug: return "debug";
                case level::info:  return "info ";
                case level::warn:  return "warn ";
                case level::error: return "error";
            }

            return "     ";
        }
    }

    void emit(level lvl, std::string_view tag, std::string_view message)
    {
        SYSTEMTIME st{};
        GetLocalTime(&st);

        const auto line = std::format(
            "[moon][{:02}:{:02}:{:02}.{:03}][{}][{}] {}\n",
            st.wHour, st.wMinute, st.wSecond, st.wMilliseconds,
            tag_of(lvl), tag, message);

        const std::scoped_lock lock{ sink_mutex };
        OutputDebugStringA(line.c_str());
    }
}
