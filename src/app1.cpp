#include "app1.h"
#include "imgui.h"
#include <cstdio>
#include <cstring>

namespace {
const char kSample[] =
    "Welcome to the CSOPESY Desktop!\n"
    "\n"
    "Quick tour:\n"
    "- Notes: type anything here, Clear or Reset it\n"
    "- Calc: add, subtract, multiply or divide two numbers\n"
    "- Task Mgr: view the running process list\n"
    "- PWR: shut down the desktop\n";

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
