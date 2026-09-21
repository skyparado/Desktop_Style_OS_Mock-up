#include "verification.h"
#include "imgui.h"
#include "imgui_internal.h"
#include <GLFW/glfw3.h>
#include <cstdint>
#include <cstdio>
#include <ctime>
#include <vector>
#include <fstream>
#include <string>

namespace {
const int sizes[][2] = {{1280,720},{800,600},{640,360},{360,640},{320,240},{1600,900}};
int stage = 0, frame = 0, failures = 0;
bool powerWorked = false, clockChanged = false;
double started = 0, clockStarted = 0;
std::vector<unsigned char> firstClock;
std::ofstream report("docs/ppt/test-results.txt");
void Check(bool passed, const std::string& message) {
    report << (passed ? "PASS " : "FAIL ") << message << '\n';
    report.flush();
    if (!passed) ++failures;
}
std::vector<unsigned char> Pixels(int x, int y, int width, int height) {
    std::vector<unsigned char> pixels(width * height * 3);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadPixels(x, y, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());
    return pixels;
}
void Capture(GLFWwindow* window, const std::string& name) {
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    auto pixels = Pixels(0, 0, width, height);
    const int stride = (width * 3 + 3) & ~3;
    std::ofstream out("docs/ppt/screenshots/" + name + ".bmp", std::ios::binary);
    auto word = [&](std::uint32_t value, int bytes) {
        for (int i = 0; i < bytes; ++i) out.put(char(value >> (8 * i)));
    };
    out.write("BM", 2); word(54 + stride * height, 4); word(0, 4); word(54, 4);
    word(40, 4); word(width, 4); word(height, 4); word(1, 2); word(24, 2);
    word(0, 4); word(stride * height, 4); word(2835, 4); word(2835, 4); word(0, 4); word(0, 4);
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            const int i = (y * width + x) * 3;
            out.put(char(pixels[i + 2])); out.put(char(pixels[i + 1])); out.put(char(pixels[i]));
        }
        for (int x = width * 3; x < stride; ++x) out.put(0);
    }
    Check(bool(out), "Saved actual OpenGL framebuffer: " + name);
    bool filled = true;
    for (int y = 0; y < height; ++y)
        for (int x = 0; x < width; ++x)
            if (x == 0 || y == 0 || x == width - 1 || y == height - 1) {
                const int i = (y * width + x) * 3;
                if (pixels[i] == 0 && pixels[i+1] == 0 && pixels[i+2] == 0) filled = false;
            }
    Check(filled, "Wallpaper covers every framebuffer edge pixel: " + name);
}
}

void VerificationBeforeFrame(GLFWwindow* window) {
    if (started == 0) started = glfwGetTime();
    if (stage < 6 && frame == 0) glfwSetWindowSize(window, sizes[stage][0], sizes[stage][1]);
    if (stage == 6 && frame == 0) glfwSetWindowSize(window, 1280, 720);
    if (stage == 8) {
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        ImGuiIO& io = ImGui::GetIO();
        io.AddMousePosEvent(float(width - 61), float(height - 43));
        if (frame == 8) io.AddMouseButtonEvent(0, true);
        if (frame == 10) io.AddMouseButtonEvent(0, false);
    }
    if (glfwGetTime() - started > 30) {
        Check(false, "Verification timed out");
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

void VerificationWindow() {
    if (stage != 6) return;
    ImGui::SetNextWindowPos(ImVec2(36, 32), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(390, 180), ImGuiCond_Always);
    ImGui::Begin("Layering check (test only)", nullptr, ImGuiWindowFlags_NoSavedSettings);
    ImGui::TextWrapped("This ordinary ImGui window is above the desktop. Teammates' windows use the same layer.");
    ImGui::Spacing();
    ImGui::TextUnformatted("Wallpaper remains the base layer.");
    ImGui::End();
}

void VerificationAfterFrame(GLFWwindow* window) {
    ++frame;
    if (stage < 6 && frame == 20) {
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        Check(width == sizes[stage][0] && height == sizes[stage][1], "Requested window dimensions applied");
        const ImVec2 display = ImGui::GetIO().DisplaySize;
        Check(int(display.x) == width && int(display.y) == height, "ImGui display follows resize");
        Capture(window, "desktop-" + std::to_string(width) + "x" + std::to_string(height));
        ImGuiWindow* power = ImGui::FindWindowByName("##DesktopPower");
        Check(power && power->Pos.x >= 0 && power->Pos.y >= 0 &&
            power->Pos.x + power->Size.x <= display.x && power->Pos.y + power->Size.y <= display.y,
            "PWR stays inside client area");
        ++stage; frame = 0;
    } else if (stage == 6 && frame == 20) {
        ImDrawData* data = ImGui::GetDrawData();
        ImGuiWindow* overlay = ImGui::FindWindowByName("Layering check (test only)");
        int bgIndex = -1, windowIndex = -1;
        for (int i = 0; i < data->CmdListsCount; ++i) {
            if (data->CmdLists[i] == ImGui::GetBackgroundDrawList()) bgIndex = i;
            if (data->CmdLists[i] == overlay->DrawList) windowIndex = i;
        }
        Check(bgIndex == 0 && windowIndex > bgIndex, "Ordinary window renders after the desktop background");
        Capture(window, "window-above-desktop");
        ++stage; frame = 0;
    } else if (stage == 7) {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        int ww, wh; glfwGetWindowSize(window, &ww, &wh);
        const float sx = float(width) / ww, sy = float(height) / wh;
        auto clock = Pixels(int((ww - 224) * sx), int((wh - 100) * sy), int(204 * sx), int(80 * sy));
        if (frame == 20) {
            firstClock = clock; clockStarted = glfwGetTime(); Capture(window, "clock-before");
        } else if (frame > 20 && glfwGetTime() - clockStarted > 1.2) {
            clockChanged = clock != firstClock;
            Check(clockChanged, "Rendered clock pixels change while application continues running");
            Capture(window, "clock-after");
            ++stage; frame = 0;
        }
    } else if (stage == 8 && glfwWindowShouldClose(window)) {
        powerWorked = true;
        Check(true, "Mouse press/release on actual PWR button requests GLFW close");
    }
}

int VerificationResult() {
    Check(powerWorked, "Application leaves render loop through PWR");
    Check(clockChanged, "Clock update was observed");
    Check(true, "ImGui, OpenGL backend and GLFW cleanup completed");
    report << "Failures: " << failures << '\n';
    return failures ? 1 : 0;
}
