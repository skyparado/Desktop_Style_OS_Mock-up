#include "desktop.h"
#include "taskbar.h"
#include "app1.h"
#include "app2.h"
#include "task_manager.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <cstdio>
#ifdef DESKTOP_VERIFY
#include "verification.h"
#endif

int main() {
    glfwSetErrorCallback([](int code, const char* message) {
        std::fprintf(stderr, "GLFW %d: %s\n", code, message);
    });
    if (!glfwInit()) return 1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    GLFWwindow* window = glfwCreateWindow(1280, 720, "CSOPESY Desktop", nullptr, nullptr);
    if (!window) { glfwTerminate(); return 1; }
    glfwSetWindowSizeLimits(window, 320, 240, GLFW_DONT_CARE, GLFW_DONT_CARE);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    ImFontConfig font;
    font.SizePixels = 18.0f;
    io.Fonts->AddFontDefault(&font);
    ImGui::StyleColorsDark();
    if (!ImGui_ImplGlfw_InitForOpenGL(window, true)) {
        ImGui::DestroyContext(); glfwDestroyWindow(window); glfwTerminate(); return 1;
    }
    if (!ImGui_ImplOpenGL3_Init("#version 130")) {
        ImGui_ImplGlfw_Shutdown(); ImGui::DestroyContext();
        glfwDestroyWindow(window); glfwTerminate(); return 1;
    }

    WindowStates windows;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        if (glfwGetWindowAttrib(window, GLFW_ICONIFIED)) {
            glfwWaitEventsTimeout(0.05);
            continue;
        }
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
#ifdef DESKTOP_VERIFY
        VerificationBeforeFrame(window);
#endif
        ImGui::NewFrame();
        if (DrawDesktop()) glfwSetWindowShouldClose(window, GLFW_TRUE);
        DrawTaskbar(windows);
        DrawApp1(windows.showApp1);
        DrawApp2(windows.showApp2);
        DrawTaskManager(windows.showTaskManager);

#ifdef DESKTOP_VERIFY
        VerificationWindow();
#endif
        ImGui::Render();
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#ifdef DESKTOP_VERIFY
        VerificationAfterFrame(window);
#endif
        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
#ifdef DESKTOP_VERIFY
    return VerificationResult();
#else
    return 0;
#endif
}
