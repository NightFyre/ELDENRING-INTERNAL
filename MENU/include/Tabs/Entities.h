#pragma once
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_dx12.h>
#include <imgui_impl_win32.h>

namespace ER::Tabs {
    class Entities {
    public:
        void static Reset();
        void static Draw();
    };
}