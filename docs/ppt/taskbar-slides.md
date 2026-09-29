# Taskbar and App 1 section - ready-to-copy PPT material

Use these slides as your section of the group's presentation. Add your name.
The screenshot is a capture of the running OpenGL application, not a mockup.

## Slide 1 - Fixed taskbar

**Image:** `screenshots/taskbar-app1.png` (16:9). Crop the bottom strip for a
close-up of the bar if the slide needs one.

- Fixed panel pinned to the bottom of the application window.
- Three clickable buttons: Notes (App 1), App 2, Task Manager.
- The PWR button from the desktop layer sits at the right end of the same bar.
- A button stays highlighted while its window is open, so the bar also shows
  which applications are running.

**Speaker notes:** The bar is repositioned from the viewport size every frame,
so it stays anchored to the bottom edge at any window size. The bar's panel is
painted on the ImGui background draw list, which never intercepts mouse input;
only the buttons themselves are clickable.

## Slide 2 - Shared window state

**Code excerpt - `src/taskbar.h`:**

```cpp
struct WindowStates {
    bool showApp1 = false;
    bool showApp2 = false;
    bool showTaskManager = false;
};
```

**Code excerpt - `src/taskbar.cpp`:**

```cpp
void TaskbarButton(const char* label, bool& isOpen, float width) {
    const ImVec4 idle(0.16f, 0.28f, 0.31f, 1.0f);
    const ImVec4 running(0.31f, 0.51f, 0.52f, 1.0f);
    ImGui::PushStyleColor(ImGuiCol_Button, isOpen ? running : idle);
    if (ImGui::Button(label, ImVec2(width, 34.0f))) isOpen = !isOpen;
    ImGui::PopStyleColor();
    ImGui::SameLine();
}
```

**Speaker notes:** One struct holds the open or closed flag of every
application. The taskbar only flips a boolean; it never draws or owns an
application window. Each application reads its own flag, so adding App 2 and the
Task Manager did not require changing the taskbar code.

## Slide 3 - App 1: Notes

**Image:** `screenshots/taskbar-app1.png`, cropped to the Notes window.

**Code excerpt - `src/app1.cpp`:**

```cpp
void DrawApp1(bool& isOpen) {
    if (!isOpen) return;
    ImGui::SetNextWindowSize(ImVec2(440.0f, 320.0f), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(90.0f, 120.0f), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Notes - App 1", &isOpen)) {
        ...
        ImGui::InputTextMultiline("##note", gNote, sizeof(gNote), ImVec2(-1.0f, -1.0f));
    }
    ImGui::End();
}
```

**Speaker notes:** An early return keeps the window out of the frame entirely
while it is closed, so a closed application costs nothing. Passing the same flag
to `ImGui::Begin` gives the title bar an X that closes the window exactly like
the taskbar button does. `ImGuiCond_FirstUseEver` sets the opening position only
once, so the window stays where the user dragged it.

## Slide 4 - Not interfering with other windows

- App 1 is an ordinary ImGui window, so it can be moved, resized, focused and
  closed while other applications stay open.
- The desktop wallpaper and the taskbar panel use the background draw list, so
  they always render below every application window.
- Test evidence: `test-results.txt` lines "Taskbar button opens App 1" and
  "Taskbar button closes App 1 again", produced by clicking the real button
  through injected ImGui mouse input.

**Speaker notes:** Draw order per frame is desktop, then taskbar, then the
application windows. Nothing in the taskbar uses a foreground draw list, which
would otherwise paint over teammates' windows.
