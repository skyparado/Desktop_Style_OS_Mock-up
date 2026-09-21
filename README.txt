CSOPESY Desktop - desktop role only
Author: [Add your name before submission]
Entry point: src/main.cpp, main()

Implemented: a full-client-area procedural wallpaper, live local date/time
with seconds, PWR shutdown, and background layering for teammates' windows.
No taskbar, task manager, boot sequence, or additional applications are included.

RUN (Windows x64)
  Double-click build\desktop.exe after building.
  Resize or maximize normally. Minimum client size is 320 x 240.
  Click PWR at the bottom right to shut down cleanly.
  Keyboard: Tab to focus PWR, then Space or Enter to activate it.
  Full-screen here means the entire application client area, as specified
  in the requirement. The native title bar remains available for resizing.

BUILD (from PowerShell in the repository folder)
  powershell -ExecutionPolicy Bypass -File scripts/setup.ps1
  powershell -ExecutionPolicy Bypass -File scripts/build.ps1
  .\build\desktop.exe

Setup needs internet once. It downloads checksum-pinned Dear ImGui 1.91.9b,
GLFW 3.4 and portable w64devkit 2.0.0 into .deps, without a system install.
Later builds work offline. Dependencies and build output are git-ignored.
The executable statically links GLFW and the C++ runtime. An OpenGL 3.0
capable Windows graphics driver is required.

TEST AND REGENERATE PPT SCREENSHOTS
  powershell -ExecutionPolicy Bypass -File scripts/verify.ps1

The separate verification executable opens a window and resizes it through
1280x720, 800x600, 640x360, 360x640, 320x240, and 1600x900. It captures real
OpenGL framebuffers, checks edge coverage and window layering, observes a
running clock change, clicks PWR through ImGui mouse input, and checks cleanup.
Keep the test window unminimized and avoid moving the mouse during this test.
Results: docs/ppt/test-results.txt
PNG screenshots: docs/ppt/screenshots/
Slide notes and code snippets: docs/ppt/desktop-slides.md
These screenshots show the application client area, without the native title bar.

GROUP INTEGRATION
Copy src/desktop.h and src/desktop.cpp into the group's Dear ImGui project.
Call DrawDesktop() after ImGui::NewFrame(). It returns true on PWR activation;
pass that result to glfwSetWindowShouldClose(window, GLFW_TRUE).
Render the group's ordinary ImGui windows after DrawDesktop() in main.cpp.
The wallpaper and clock use GetBackgroundDrawList(), which always draws below
ordinary windows and does not intercept clicks. PWR uses its own small window.
Avoid ImGui foreground draw lists for ordinary app windows.
If integrating a bottom taskbar, your group can move the PWR control into it
while preserving its close request and normal resource cleanup.

REFERENCES / THIRD-PARTY LICENSES
Dear ImGui (MIT): https://github.com/ocornut/imgui/tree/v1.91.9b
GLFW (zlib/libpng): https://www.glfw.org/docs/3.4/window_guide.html
w64devkit build tools: https://github.com/skeeto/w64devkit/releases/tag/v2.0.0
Downloaded packages retain their upstream license files in .deps.
The wallpaper is drawn by this project; no external wallpaper image is required.
