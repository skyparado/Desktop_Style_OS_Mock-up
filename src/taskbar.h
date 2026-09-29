#pragma once

// which app windows are open
struct WindowStates {
    bool showApp1 = false;
    bool showApp2 = false;
    bool showTaskManager = false;
};

// taskbar height in pixels
extern const float kTaskbarHeight;

// call after DrawDesktop()
void DrawTaskbar(WindowStates& windows);
