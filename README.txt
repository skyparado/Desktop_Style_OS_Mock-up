CSOPESY Desktop - desktop, taskbar and App 1
Author: [Add your name before submission]
Entry point: src/main.cpp, main()

Implemented: a full-client-area procedural wallpaper, live local date/time
with seconds, PWR shutdown, background layering for teammates' windows,
a fixed bottom taskbar with three buttons, and App 1 (Notes).
App 2 and the Task Manager are not included yet; their taskbar buttons
already toggle the shared flags they will read.

TASKBAR AND APP 1
  The taskbar is fixed to the bottom edge at any window size.
  Notes opens App 1, a small editable notes window with placeholder text.
  App 2 and Task Mgr toggle windows.showApp2 and windows.showTaskManager.
  A button stays highlighted while its window is open.
  Close App 1 with the same taskbar button or the X in its title bar.
  Files: src/taskbar.h, src/taskbar.cpp, src/app1.h, src/app1.cpp.
  Teammates add their windows in src/main.cpp, after DrawTaskbar(windows).

RUN FROM VS CODE (easiest)
  1. Open this folder in VS Code (File > Open Folder).
  2. Install the "C/C++" extension by Microsoft if VS Code asks for it.
  3. Press F5, or open Run and Debug and click the green Run button.
  The first run downloads the dependencies and compiles, which takes a few
  minutes. After that, Run starts the app right away unless src/ changed.

RUN (Windows x64)
  Double-click build\desktop.exe after building.
  Resize or maximize normally. Minimum client size is 320 x 240.
  Click PWR at the bottom right to shut down cleanly.
  Keyboard: Tab to focus PWR, then Space or Enter to activate it.
  Full-screen here means the entire application client area, as specified
  in the requirement. The native title bar remains available for resizing.

BUILD (from PowerShell in the repository folder)
  powershell -ExecutionPolicy Bypass -File scripts/build.ps1
  .\build\desktop.exe

The first build runs scripts/setup.ps1 automatically and needs internet once.
It downloads checksum-pinned Dear ImGui 1.91.9b, GLFW 3.4 and portable
w64devkit 2.0.0 into .deps, without a system install.
Later builds work offline. Dependencies and build output are git-ignored.
The executable statically links GLFW and the C++ runtime. An OpenGL 3.0
capable Windows graphics driver is required.

TEST AND REGENERATE PPT SCREENSHOTS
  powershell -ExecutionPolicy Bypass -File scripts/verify.ps1

The separate verification executable opens a window and resizes it through
1280x720, 800x600, 640x360, 360x640, 320x240, and 1600x900. It captures real
OpenGL framebuffers, checks edge coverage and window layering, observes a
running clock change, opens and closes App 1 from the taskbar, clicks PWR
through ImGui mouse input, and checks cleanup.
Keep the test window unminimized and avoid moving the mouse during this test.
Results: docs/ppt/test-results.txt
PNG screenshots: docs/ppt/screenshots/
Slide notes and code snippets: docs/ppt/desktop-slides.md,
docs/ppt/taskbar-slides.md
These screenshots show the application client area, without the native title bar.

GROUP INTEGRATION
Copy src/desktop.h and src/desktop.cpp into the group's Dear ImGui project.
Call DrawDesktop() after ImGui::NewFrame(). It returns true on PWR activation;
pass that result to glfwSetWindowShouldClose(window, GLFW_TRUE).
Render the group's ordinary ImGui windows after DrawDesktop() in main.cpp.
The wallpaper and clock use GetBackgroundDrawList(), which always draws below
ordinary windows and does not intercept clicks. PWR uses its own small window.
Avoid ImGui foreground draw lists for ordinary app windows.
The bottom taskbar leaves its right end clear, so PWR sits inside the bar.

REFERENCES / THIRD-PARTY LICENSES
Dear ImGui (MIT): https://github.com/ocornut/imgui/tree/v1.91.9b
GLFW (zlib/libpng): https://www.glfw.org/docs/3.4/window_guide.html
w64devkit build tools: https://github.com/skeeto/w64devkit/releases/tag/v2.0.0
Downloaded packages retain their upstream license files in .deps.
The wallpaper is drawn by this project; no external wallpaper image is required.
