#include "il2cpp.hpp"

#include "components/engine/engine.hpp"
#include "utilities/log/log.hpp"
#include "utilities/utilities.hpp"

namespace il2cpp
{
    namespace
    {
        using fn_domain_get                 = il2cpp_domain* (*)();
        using fn_domain_assembly_open       = const il2cpp_assembly* (*)(il2cpp_domain*, const char*);
        using fn_assembly_get_image         = const il2cpp_image* (*)(const il2cpp_assembly*);
        using fn_class_from_name            = il2cpp_class* (*)(const il2cpp_image*, const char*, const char*);
        using fn_class_get_field_from_name  = il2cpp_field_info* (*)(il2cpp_class*, const char*);
        using fn_class_get_method_from_name = il2cpp_method_info* (*)(il2cpp_class*, const char*, int);
        using fn_field_static_get_value     = void (*)(il2cpp_field_info*, void*);
        using fn_field_static_set_value     = void (*)(il2cpp_field_info*, void*);
        using fn_field_get_value            = void (*)(il2cpp_object*, il2cpp_field_info*, void*);
        using fn_field_set_value            = void (*)(il2cpp_object*, il2cpp_field_info*, void*);
        using fn_field_get_offset           = std::int32_t (*)(il2cpp_field_info*);
        using fn_object_new                 = il2cpp_object* (*)(il2cpp_class*);
        using fn_runtime_invoke             = il2cpp_object* (*)(const il2cpp_method_info*, void*, void**, il2cpp_object**);
        using fn_thread_attach              = void* (*)(il2cpp_domain*);
        using fn_thread_detach              = void  (*)(void*);

        fn_domain_get                 p_domain_get                 = nullptr;
        fn_domain_assembly_open       p_domain_assembly_open       = nullptr;
        fn_assembly_get_image         p_assembly_get_image         = nullptr;
        fn_class_from_name            p_class_from_name            = nullptr;
        fn_class_get_field_from_name  p_class_get_field_from_name  = nullptr;
        fn_class_get_method_from_name p_class_get_method_from_name = nullptr;
        fn_field_static_get_value     p_field_static_get_value     = nullptr;
        fn_field_static_set_value     p_field_static_set_value     = nullptr;
        fn_field_get_value            p_field_get_value            = nullptr;
        fn_field_set_value            p_field_set_value            = nullptr;
        fn_field_get_offset           p_field_get_offset           = nullptr;
        fn_object_new                 p_object_new                 = nullptr;
        fn_runtime_invoke             p_runtime_invoke             = nullptr;
        fn_thread_attach              p_thread_attach              = nullptr;
        fn_thread_detach              p_thread_detach              = nullptr;

        il2cpp_domain*       domain_        = nullptr;
        const il2cpp_image*  assembly_csharp_image_ = nullptr;
        void*                attach_handle_ = nullptr;
    }

    bool init() noexcept
    {
        const auto& ga = engine::game_assembly();
        if (!ga.valid())
        {
            lg::error("il2cpp", "game assembly not ready");
            return false;
        }

#define BIND(name) p_##name = reinterpret_cast<fn_##name>(mem::get_export(ga, "il2cpp_" #name))

        BIND(domain_get);
        BIND(domain_assembly_open);
        BIND(assembly_get_image);
        BIND(class_from_name);
        BIND(class_get_field_from_name);
        BIND(class_get_method_from_name);
        BIND(field_static_get_value);
        BIND(field_static_set_value);
        BIND(field_get_value);
        BIND(field_set_value);
        BIND(field_get_offset);
        BIND(object_new);
        BIND(runtime_invoke);
        BIND(thread_attach);
        BIND(thread_detach);

#undef BIND

        if (p_domain_get == nullptr || p_class_from_name == nullptr)
        {
            lg::error("il2cpp", "required exports missing");
            return false;
        }

        domain_ = p_domain_get();
        if (domain_ == nullptr)
        {
            lg::error("il2cpp", "domain_get returned null");
            return false;
        }

        attach_handle_ = p_thread_attach(domain_);

        const auto a = p_domain_assembly_open(domain_, "Assembly-CSharp.dll");
        if (a == nullptr)
        {
            lg::error("il2cpp", "Assembly-CSharp.dll not loaded in domain");
            return false;
        }
        assembly_csharp_image_ = p_assembly_get_image(a);
        if (assembly_csharp_image_ == nullptr)
        {
            lg::error("il2cpp", "assembly_get_image returned null");
            return false;
        }
        lg::info("il2cpp", "runtime ready domain=0x{:X} image=0x{:X}",
            reinterpret_cast<std::uintptr_t>(domain_),
            reinterpret_cast<std::uintptr_t>(assembly_csharp_image_));
        return true;
    }

    il2cpp_domain* domain_get() noexcept
    {
        return domain_;
    }

    const il2cpp_assembly* domain_assembly_open(il2cpp_domain* d, const char* n) noexcept
    {
        if (p_domain_assembly_open == nullptr)
        {
            return nullptr;
        }
        return p_domain_assembly_open(d, n);
    }

    const il2cpp_image* assembly_get_image(const il2cpp_assembly* a) noexcept
    {
        if (p_assembly_get_image == nullptr || a == nullptr)
        {
            return nullptr;
        }
        return p_assembly_get_image(a);
    }

    il2cpp_class* class_from_name(const il2cpp_image* img, const char* ns, const char* name) noexcept
    {
        if (p_class_from_name == nullptr || img == nullptr)
        {
            return nullptr;
        }
        return p_class_from_name(img, ns, name);
    }

    il2cpp_field_info* class_get_field_from_name(il2cpp_class* k, const char* n) noexcept
    {
        if (p_class_get_field_from_name == nullptr || k == nullptr)
        {
            return nullptr;
        }
        return p_class_get_field_from_name(k, n);
    }

    il2cpp_method_info* class_get_method_from_name(il2cpp_class* k, const char* n, int a) noexcept
    {
        if (p_class_get_method_from_name == nullptr || k == nullptr)
        {
            return nullptr;
        }
        return p_class_get_method_from_name(k, n, a);
    }

    void field_static_get_value(il2cpp_field_info* f, void* out) noexcept
    {
        if (p_field_static_get_value != nullptr && f != nullptr)
        {
            p_field_static_get_value(f, out);
        }
    }

    void field_static_set_value(il2cpp_field_info* f, void* in) noexcept
    {
        if (p_field_static_set_value != nullptr && f != nullptr)
        {
            p_field_static_set_value(f, in);
        }
    }

    void field_get_value(il2cpp_object* o, il2cpp_field_info* f, void* out) noexcept
    {
        if (p_field_get_value != nullptr && o != nullptr && f != nullptr)
        {
            p_field_get_value(o, f, out);
        }
    }

    void field_set_value(il2cpp_object* o, il2cpp_field_info* f, void* in) noexcept
    {
        if (p_field_set_value != nullptr && o != nullptr && f != nullptr)
        {
            p_field_set_value(o, f, in);
        }
    }

    std::int32_t field_get_offset(il2cpp_field_info* f) noexcept
    {
        if (p_field_get_offset == nullptr || f == nullptr)
        {
            return 0;
        }
        return p_field_get_offset(f);
    }

    il2cpp_object* object_new(il2cpp_class* k) noexcept
    {
        if (p_object_new == nullptr || k == nullptr)
        {
            return nullptr;
        }
        return p_object_new(k);
    }

    il2cpp_object* runtime_invoke(const il2cpp_method_info* m, void* o, void** p, il2cpp_object** e) noexcept
    {
        if (p_runtime_invoke == nullptr || m == nullptr)
        {
            return nullptr;
        }
        return p_runtime_invoke(m, o, p, e);
    }

    void thread_attach(il2cpp_domain* d) noexcept
    {
        if (p_thread_attach != nullptr)
        {
            p_thread_attach(d);
        }
    }

    void thread_detach() noexcept
    {
        if (p_thread_detach != nullptr && attach_handle_ != nullptr)
        {
            p_thread_detach(attach_handle_);
        }
    }

    il2cpp_class* resolve(const char*, const char* ns, const char* name) noexcept
    {
        return class_from_name(assembly_csharp_image_, ns, name);
    }

    il2cpp_class* resolve_in(const char* assembly_dll, const char* ns, const char* name) noexcept
    {
        if (domain_ == nullptr || assembly_dll == nullptr)
        {
            return nullptr;
        }
        const auto a = p_domain_assembly_open(domain_, assembly_dll);
        if (a == nullptr)
        {
            return nullptr;
        }
        const auto img = p_assembly_get_image(a);
        if (img == nullptr)
        {
            return nullptr;
        }

        return class_from_name(img, ns, name);
    }
}
