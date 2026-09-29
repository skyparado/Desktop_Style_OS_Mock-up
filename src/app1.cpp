#include "app1.h"
#include "imgui.h"
#include <cstdio>
#include <cstring>

namespace {
const char kSample[] =
    "To-do - Week 5\n"
    "\n"
    "- Finish the desktop and taskbar\n"
    "- Test all apps together\n"
    "- Record the demo video\n"
    "- Submit the PPT\n";

char gNote[2048];
bool gLoaded = false;
}

void DrawApp1(bool& isOpen) {
    if (!isOpen) return;
    if (!gLoaded) { std::snprintf(gNote, sizeof(gNote), "%s", kSample); gLoaded = true; }

    ImGui::SetNextWindowSize(ImVec2(440.0f, 320.0f), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(90.0f, 120.0f), ImGuiCond_FirstUseEver);
    // &isOpen adds a close (X) button to the title bar
    if (ImGui::Begin("Notes - App 1", &isOpen)) {
        if (ImGui::Button("Clear")) gNote[0] = '\0';
        ImGui::SameLine();
        if (ImGui::Button("Reset")) std::snprintf(gNote, sizeof(gNote), "%s", kSample);
        ImGui::SameLine();
        ImGui::Text("%d characters", int(std::strlen(gNote)));
        ImGui::Separator();
        ImGui::InputTextMultiline("##note", gNote, sizeof(gNote), ImVec2(-1.0f, -1.0f));
    }
    ImGui::End();
}
