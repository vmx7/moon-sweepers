#include "moon_brush.hpp"

#include "utilities/log/log.hpp"

namespace moon_brush
{
    namespace
    {
        il2cpp::il2cpp_class* class_ = nullptr;

        il2cpp::il2cpp_field_info* f_cleaning_width_       = nullptr;
        il2cpp::il2cpp_field_info* f_cleaning_depth_       = nullptr;
        il2cpp::il2cpp_field_info* f_cleaning_edge_width_  = nullptr;
        il2cpp::il2cpp_field_info* f_removal_rate_         = nullptr;
        il2cpp::il2cpp_field_info* f_reach_                = nullptr;
        il2cpp::il2cpp_field_info* f_stroke_interval_      = nullptr;
        il2cpp::il2cpp_field_info* f_clear_in_one_pass_    = nullptr;
    }

    void resolve() noexcept
    {
        class_ = il2cpp::resolve("Assembly-CSharp.dll", "MoonCleaners", "MoonBrush");
        if (class_ == nullptr)
        {
            lg::warn("brush", "MoonBrush not found");

            return;
        }

        f_cleaning_width_      = il2cpp::class_get_field_from_name(class_, "cleaningWidth");
        f_cleaning_depth_      = il2cpp::class_get_field_from_name(class_, "cleaningDepth");
        f_cleaning_edge_width_ = il2cpp::class_get_field_from_name(class_, "cleaningEdgeWidth");
        f_removal_rate_        = il2cpp::class_get_field_from_name(class_, "removalRate");
        f_reach_               = il2cpp::class_get_field_from_name(class_, "reach");
        f_stroke_interval_     = il2cpp::class_get_field_from_name(class_, "strokeInterval");
        f_clear_in_one_pass_   = il2cpp::class_get_field_from_name(class_, "clearInOnePass");

        lg::info("brush", "ready class=0x{:X}", reinterpret_cast<std::uintptr_t>(class_));
    }

    bool ready() noexcept
    {
        return class_ != nullptr;
    }

    il2cpp::il2cpp_field_info* field_cleaning_width()      noexcept { return f_cleaning_width_; }
    il2cpp::il2cpp_field_info* field_cleaning_depth()      noexcept { return f_cleaning_depth_; }
    il2cpp::il2cpp_field_info* field_cleaning_edge_width() noexcept { return f_cleaning_edge_width_; }
    il2cpp::il2cpp_field_info* field_removal_rate()        noexcept { return f_removal_rate_; }
    il2cpp::il2cpp_field_info* field_reach()               noexcept { return f_reach_; }
    il2cpp::il2cpp_field_info* field_stroke_interval()     noexcept { return f_stroke_interval_; }
    il2cpp::il2cpp_field_info* field_clear_in_one_pass()   noexcept { return f_clear_in_one_pass_; }
}
