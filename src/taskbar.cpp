#include "taskbar.h"
#include "imgui.h"

const float kTaskbarHeight = 48.0f;

// leave room on the right for the PWR button
static const float kPowerReserve = 110.0f;

namespace {
// button stays highlighted while its app is open
void TaskbarButton(const char* label, bool& isOpen, float width) {
    const ImVec4 idle(0.16f, 0.28f, 0.31f, 1.0f);
    const ImVec4 running(0.31f, 0.51f, 0.52f, 1.0f);
    ImGui::PushStyleColor(ImGuiCol_Button, isOpen ? running : idle);
    if (ImGui::Button(label, ImVec2(width, 34.0f))) isOpen = !isOpen;
    ImGui::PopStyleColor();
    ImGui::SameLine();
}
}

void DrawTaskbar(WindowStates& windows) {
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    const ImVec2 p = viewport->Pos;
    const ImVec2 size = viewport->Size;
    const float top = p.y + size.y - kTaskbarHeight;

    // drawn on the background list so it doesn't block clicks
    ImDrawList* draw = ImGui::GetBackgroundDrawList();
    draw->AddRectFilled(ImVec2(p.x, top), ImVec2(p.x + size.x, p.y + size.y), IM_COL32(9, 26, 37, 232));
    draw->AddLine(ImVec2(p.x, top), ImVec2(p.x + size.x, top), IM_COL32(218, 239, 231, 45));

    ImGui::SetNextWindowPos(ImVec2(p.x, top));
    ImGui::SetNextWindowSize(ImVec2(size.x - kPowerReserve, kTaskbarHeight));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10.0f, 7.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 8.0f);
    constexpr ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoBringToFrontOnFocus |
        ImGuiWindowFlags_NoScrollWithMouse;
    if (ImGui::Begin("##Taskbar", nullptr, flags)) {
        // split the width evenly so the buttons fit on small windows
        float width = (ImGui::GetContentRegionAvail().x - 2.0f * ImGui::GetStyle().ItemSpacing.x) / 3.0f;
        if (width > 130.0f) width = 130.0f;
        if (width < 40.0f) width = 40.0f;
        TaskbarButton("Notes", windows.showApp1, width);
        TaskbarButton("Calc", windows.showApp2, width);
        TaskbarButton("Task Mgr", windows.showTaskManager, width);
    }
    ImGui::End();
    ImGui::PopStyleVar(2);
}
