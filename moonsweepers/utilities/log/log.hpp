#pragma once

#include <format>
#include <string_view>
#include <utility>

namespace lg
{
    enum class level
    {
        debug,
        info,
        warn,
        error
    };

    namespace detail
    {
        void emit(level lvl, std::string_view tag, std::string_view message);
    }

    template <typename... args_t>
    void info(std::string_view tag, std::format_string<args_t...> fmt, args_t&&... args)
    {
        detail::emit(level::info, tag, std::format(fmt, std::forward<args_t>(args)...));
    }

    template <typename... args_t>
    void warn(std::string_view tag, std::format_string<args_t...> fmt, args_t&&... args)
    {
        detail::emit(level::warn, tag, std::format(fmt, std::forward<args_t>(args)...));
    }

    template <typename... args_t>
    void error(std::string_view tag, std::format_string<args_t...> fmt, args_t&&... args)
    {
        detail::emit(level::error, tag, std::format(fmt, std::forward<args_t>(args)...));
    }

#ifdef _DEBUG
    template <typename... args_t>
    void debug(std::string_view tag, std::format_string<args_t...> fmt, args_t&&... args)
    {
        detail::emit(level::debug, tag, std::format(fmt, std::forward<args_t>(args)...));
    }
#else
    template <typename... args_t>
    void debug(std::string_view, std::format_string<args_t...>, args_t&&...)
    {
    }
#endif
}
