#pragma once
#include "../../libs/ImGui/imgui.h"
#include "../../libs/ImGui/imgui_internal.h"
#include "../../libs/ImGui/imgui_impl_dx12.h"
#include "../../libs/ImGui/imgui_impl_win32.h"

namespace ER::Tabs {
    class Entities {
    public:
        void static Reset();
        void static Draw();
    };
}