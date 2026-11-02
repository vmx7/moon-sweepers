#pragma once

#include <cstdint>

namespace il2cpp
{
    using il2cpp_object       = void;
    using il2cpp_class        = void;
    using il2cpp_field_info   = void;
    using il2cpp_method_info  = void;
    using il2cpp_assembly     = void;
    using il2cpp_image        = void;
    using il2cpp_domain       = void;
    using il2cpp_type         = void;

    [[nodiscard]] bool init() noexcept;

    [[nodiscard]] il2cpp_domain* domain_get() noexcept;
    [[nodiscard]] const il2cpp_assembly* domain_assembly_open(il2cpp_domain* d, const char* name) noexcept;
    [[nodiscard]] const il2cpp_image*    assembly_get_image(const il2cpp_assembly* a) noexcept;

    [[nodiscard]] il2cpp_class* class_from_name(const il2cpp_image* image, const char* ns, const char* name) noexcept;
    [[nodiscard]] il2cpp_field_info*  class_get_field_from_name(il2cpp_class* k, const char* name) noexcept;
    [[nodiscard]] il2cpp_method_info* class_get_method_from_name(il2cpp_class* k, const char* name, int argc) noexcept;

    void field_static_get_value(il2cpp_field_info* f, void* out) noexcept;
    void field_static_set_value(il2cpp_field_info* f, void* in) noexcept;

    void field_get_value(il2cpp_object* o, il2cpp_field_info* f, void* out) noexcept;
    void field_set_value(il2cpp_object* o, il2cpp_field_info* f, void* in) noexcept;

    [[nodiscard]] std::int32_t field_get_offset(il2cpp_field_info* f) noexcept;

    il2cpp_object* object_new(il2cpp_class* k) noexcept;

    [[nodiscard]] il2cpp_object* runtime_invoke(const il2cpp_method_info* m, void* obj, void** params, il2cpp_object** exc) noexcept;

    void thread_attach(il2cpp_domain* d) noexcept;
    void thread_detach() noexcept;

    [[nodiscard]] il2cpp_class* resolve(const char* image_name, const char* ns, const char* name) noexcept;

    [[nodiscard]] il2cpp_class* resolve_in(const char* assembly_dll, const char* ns, const char* name) noexcept;

    [[nodiscard]] const il2cpp_type* class_get_type(il2cpp_class* k) noexcept;
    [[nodiscard]] il2cpp_object*     type_get_object(const il2cpp_type* t) noexcept;

    template <typename t>
    [[nodiscard]] inline t get_static(il2cpp_field_info* f) noexcept
    {
        t v{};
        field_static_get_value(f, &v);
        return v;
    }

    template <typename t>
    inline void set_static(il2cpp_field_info* f, const t& value) noexcept
    {
        field_static_set_value(f, const_cast<t*>(&value));
    }

    template <typename t>
    [[nodiscard]] inline t read_field(il2cpp_object* o, std::int32_t offset) noexcept
    {
        return *reinterpret_cast<t*>(reinterpret_cast<std::uint8_t*>(o) + offset);
    }

    template <typename t>
    inline void write_field(il2cpp_object* o, std::int32_t offset, const t& value) noexcept
    {
        *reinterpret_cast<t*>(reinterpret_cast<std::uint8_t*>(o) + offset) = value;
    }
}
