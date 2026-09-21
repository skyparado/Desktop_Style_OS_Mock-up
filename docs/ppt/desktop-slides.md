# Desktop section — ready-to-copy PPT material

Use these slides as your section of the group's presentation. Add your name.
The screenshots are captures of the running OpenGL application, not mockups.

## Slide 1 — Desktop base layer

**Image:** `screenshots/desktop-1280x720.png` (16:9).

- Full-window landscape wallpaper drawn with Dear ImGui.
- Current local date and time in the upper-right corner.
- PWR in the lower-right corner closes the application.

**Speaker notes:** The wallpaper uses a sky gradient, a sun and three landscape
layers. Its coordinates are derived from the current viewport dimensions every
frame, so it fills the client area when the application is resized or maximized.
It does not require loading an image texture.

## Slide 2 — Filling the application window

**Images:** `screenshots/desktop-800x600.png` and
`screenshots/desktop-360x640.png` side by side. Preserve aspect ratios.

**Code excerpt — `src/desktop.cpp`:**

```cpp
const ImGuiViewport* viewport = ImGui::GetMainViewport();
const ImVec2 p = viewport->Pos;
const ImVec2 size = viewport->Size;
const ImVec2 end(p.x + size.x, p.y + size.y);
ImDrawList* draw = ImGui::GetBackgroundDrawList();
draw->AddRectFilledMultiColor(p, end,
    IM_COL32(23, 53, 76, 255), IM_COL32(31, 66, 87, 255),
    IM_COL32(204, 195, 157, 255), IM_COL32(121, 162, 159, 255));
```

**Speaker notes:** Viewport size is queried each frame rather than stored at
startup. The OpenGL viewport also follows the framebuffer size, which can differ
from the logical window size on a scaled display. Minimum window size: 320×240.

## Slide 3 — Real-time clock

**Images:** Crop the clock region from `screenshots/clock-before.png` and
`screenshots/clock-after.png`. Keep the differing seconds visible.

**Code excerpt — Windows path in `src/desktop.cpp`:**

```cpp
const std::time_t now = std::time(nullptr);
std::tm local{};
localtime_s(&local, &now);
char timeText[32];
char dateText[64];
std::strftime(timeText, sizeof(timeText), "%I:%M:%S %p", &local);
std::strftime(dateText, sizeof(dateText), "%a, %b %d, %Y", &local);
```

**Speaker notes:** This code runs inside DrawDesktop on every rendered frame.
The displayed seconds change once per second. It reads the operating system's
local wall clock, so it also reflects date changes and system time adjustments.
The application waits while minimized and refreshes immediately on restoration.

## Slide 4 — PWR and clean shutdown

**Code excerpts — `src/desktop.cpp` and `src/main.cpp`:**

```cpp
shutdown = ImGui::Button("PWR", ImVec2(82, 46));
```

```cpp
while (!glfwWindowShouldClose(window)) {
    if (DrawDesktop()) glfwSetWindowShouldClose(window, GLFW_TRUE);
}
ImGui_ImplOpenGL3_Shutdown();
ImGui_ImplGlfw_Shutdown();
ImGui::DestroyContext();
glfwDestroyWindow(window);
glfwTerminate();
```

**Speaker notes:** PWR sets the GLFW close flag. The render loop ends normally,
then the application releases its renderer, ImGui context and native window.
It shuts down this application only. No forced process termination is used.

## Slide 5 — Layering and verification

**Image:** `screenshots/window-above-desktop.png`.

- Wallpaper and clock: ImGui background draw list.
- Teammates' windows: ordinary ImGui windows above the background.
- PWR: a small interactive ImGui window.

**Frame flow:** Poll events → begin ImGui frame → desktop → group windows →
render draw data → swap buffers. PWR → leave loop → clean up.

**Speaker notes:** The pictured window exists only in the verification build.
It demonstrates layering without adding another group member's application.
The desktop has no full-screen input window that could cover or focus over
other windows. The automated test confirms draw order, checks framebuffer edges
at six sizes, observes changing clock pixels, and sends a mouse press/release
to the actual PWR widget. See `test-results.txt` for the recorded results.

## Suggested live demonstration

1. Run `build/desktop.exe` from the IDE or terminal.
2. Point out the wallpaper, time and PWR. Let the seconds change.
3. Resize and maximize the window; show that the corners stay anchored.
4. Click PWR and show that the application closes normally.

Record a continuous walkthrough as required by the class instructions. These
still screenshots supplement that recording; they do not replace it.
