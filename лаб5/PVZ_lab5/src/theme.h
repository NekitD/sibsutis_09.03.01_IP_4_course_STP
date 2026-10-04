#pragma once
#include <FL/Fl.H>
#include <FL/Enumerations.H>
#include <string>

namespace Theme {
    inline Fl_Color BG()          { return fl_rgb_color(245, 247, 250); }
    inline Fl_Color SURFACE()     { return FL_WHITE; }
    inline Fl_Color BORDER()      { return fl_rgb_color(222, 226, 230); }
    inline Fl_Color TEXT()        { return fl_rgb_color(33, 37, 41); }
    inline Fl_Color TEXT_MUTED()  { return fl_rgb_color(108, 117, 125); }
    inline Fl_Color ACCENT()      { return fl_rgb_color(44, 123, 229); }
    inline Fl_Color SUCCESS()     { return fl_rgb_color(40, 167, 69); }
    inline Fl_Color DANGER()      { return fl_rgb_color(220, 53, 69); }
    inline Fl_Color WARNING()     { return fl_rgb_color(255, 152, 0); }
    inline Fl_Color HEADER_BG()   { return fl_rgb_color(248, 249, 251); }

    inline void apply() {
        Fl::scheme("gtk+");
        Fl::background(245, 247, 250);
        Fl::foreground(33, 37, 41);
    }

    inline Fl_Color statusColor(const std::string& s) {
        if (s == "ready")     return ACCENT();
        if (s == "issued")    return SUCCESS();
        if (s == "cancelled") return DANGER();
        return TEXT_MUTED();
    }

    inline const char* statusRu(const std::string& s) {
        if (s == "ready")     return "К выдаче";
        if (s == "issued")    return "Выдан";
        if (s == "cancelled") return "Отменён";
        return s.c_str();
    }
}