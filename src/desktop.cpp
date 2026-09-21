#include "desktop.h"
#include "imgui.h"
#include <cmath>
#include <ctime>

namespace {
void Hill(ImDrawList* draw, ImVec2 origin, ImVec2 size, float level,
          float amplitude, float phase, ImU32 top, ImU32 bottom) {
    // Adjacent gradient quads form a smooth landscape at any aspect ratio.
    constexpr int segments = 160;
    for (int i = 0; i < segments; ++i) {
        const float u = float(i) / segments;
        const float v = float(i + 1) / segments;
        auto ridge = [&](float x) {
            return origin.y + size.y * (level + amplitude *
                (std::sin(x * 5.0f + phase) + 0.3f * std::sin(x * 9.0f + phase)));
        };
        const ImVec2 a(origin.x + u * size.x, ridge(u));
        const ImVec2 b(origin.x + v * size.x, ridge(v));
        const ImVec2 c(b.x, origin.y + size.y);
        const ImVec2 d(a.x, c.y);
        draw->PrimReserve(6, 4);
        const unsigned int index = draw->_VtxCurrentIdx;
        draw->PrimWriteIdx(ImDrawIdx(index));
        draw->PrimWriteIdx(ImDrawIdx(index + 1));
        draw->PrimWriteIdx(ImDrawIdx(index + 2));
        draw->PrimWriteIdx(ImDrawIdx(index));
        draw->PrimWriteIdx(ImDrawIdx(index + 2));
        draw->PrimWriteIdx(ImDrawIdx(index + 3));
        const ImVec2 uv = ImGui::GetFontTexUvWhitePixel();
        draw->PrimWriteVtx(a, uv, top);
        draw->PrimWriteVtx(b, uv, top);
        draw->PrimWriteVtx(c, uv, bottom);
        draw->PrimWriteVtx(d, uv, bottom);
    }
}
}

bool DrawDesktop() {
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    const ImVec2 p = viewport->Pos;
    const ImVec2 size = viewport->Size;
    const ImVec2 end(p.x + size.x, p.y + size.y);
    // The background draw list is always behind regular ImGui windows,
    // even after clicking the wallpaper. No full-screen window steals input.
    ImDrawList* draw = ImGui::GetBackgroundDrawList();
    draw->AddRectFilledMultiColor(p, end,
        IM_COL32(23, 53, 76, 255), IM_COL32(31, 66, 87, 255),
        IM_COL32(204, 195, 157, 255), IM_COL32(121, 162, 159, 255));
    const ImVec2 sun(p.x + size.x * 0.73f, p.y + size.y * 0.35f);
    const float radius = (size.x < size.y ? size.x : size.y) * 0.075f;
    draw->AddCircleFilled(sun, radius * 1.5f, IM_COL32(241, 226, 176, 12), 96);
    draw->AddCircleFilled(sun, radius * 1.2f, IM_COL32(241, 226, 176, 18), 96);
    draw->AddCircleFilled(sun, radius, IM_COL32(241, 226, 187, 255), 96);
    Hill(draw, p, size, 0.62f, 0.08f, 2.0f, IM_COL32(86, 132, 133, 255), IM_COL32(55, 103, 112, 255));
    Hill(draw, p, size, 0.72f, 0.09f, 4.3f, IM_COL32(44, 105, 110, 255), IM_COL32(26, 69, 83, 255));
    Hill(draw, p, size, 0.88f, 0.07f, 1.2f, IM_COL32(25, 73, 79, 255), IM_COL32(12, 36, 49, 255));

    // Query wall-clock time every frame, including after suspend or time changes.
    const std::time_t now = std::time(nullptr);
    std::tm local{};
#ifdef _WIN32
    localtime_s(&local, &now);
#else
    localtime_r(&now, &local);
#endif
    char timeText[32];
    char dateText[64];
    std::strftime(timeText, sizeof(timeText), "%I:%M:%S %p", &local);
    std::strftime(dateText, sizeof(dateText), "%a, %b %d, %Y", &local);
    const float margin = 20.0f;
    const ImVec2 clockStart(end.x - 224.0f, p.y + margin);
    draw->AddRectFilled(clockStart, ImVec2(end.x - margin, p.y + 103.0f), IM_COL32(12, 30, 43, 170), 14.0f);
    draw->AddRect(clockStart, ImVec2(end.x - margin, p.y + 103.0f), IM_COL32(218, 239, 231, 35), 14.0f);
    draw->AddText(ImGui::GetFont(), 25.0f, ImVec2(clockStart.x + 17, clockStart.y + 13), IM_COL32(240, 247, 239, 255), timeText);
    draw->AddText(ImGui::GetFont(), 15.0f, ImVec2(clockStart.x + 17, clockStart.y + 48), IM_COL32(174, 199, 200, 255), dateText);
    draw->AddText(ImGui::GetFont(), 20.0f, ImVec2(p.x + margin, end.y - 65), IM_COL32(230, 242, 229, 255), "CSOPESY");
    draw->AddText(ImGui::GetFont(), 12.0f, ImVec2(p.x + margin, end.y - 37), IM_COL32(140, 174, 177, 255), "DESKTOP ENVIRONMENT");

    ImGui::SetNextWindowPos(ImVec2(end.x - 102, end.y - 66));
    ImGui::SetNextWindowSize(ImVec2(82, 46));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.16f, 0.28f, 0.31f, 1));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.51f, 0.29f, 0.27f, 1));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.65f, 0.32f, 0.28f, 1));
    constexpr ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoBringToFrontOnFocus |
        ImGuiWindowFlags_NoScrollWithMouse;
    bool shutdown = false;
    if (ImGui::Begin("##DesktopPower", nullptr, flags)) {
        shutdown = ImGui::Button("PWR", ImVec2(82, 46));
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Shut down the application");
    }
    ImGui::End();
    ImGui::PopStyleColor(3);
    ImGui::PopStyleVar(3);
    return shutdown;
}
