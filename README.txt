===============================================================================
  CSOPESY DESKTOP - Desktop-Style OS Mock-up
  CSOPESY Semi-Major Output 2
===============================================================================

  GROUP MEMBERS
    Garcia, Theodore Rodolfo III, C.
    Parado, Sky Hannah, G.
    Magbatoc, Ethan Daniel, B.
    Villorente, Khyle Raeka, B.

  ENTRY POINT
    src/main.cpp  ->  main()

  A desktop-style OS shell built on GLFW + OpenGL + Dear ImGui.


-------------------------------------------------------------------------------
 1. FEATURES
-------------------------------------------------------------------------------

  Desktop    Full-window image wallpaper (assets/bg.jpg) loaded as an OpenGL
             texture, a live clock with date in the top-right corner, and a
             PWR button that shuts the application down cleanly. If the image
             is missing, a landscape drawn with ImGui commands is shown.

  Taskbar    Fixed to the bottom edge at any window size, with three buttons.
             A button stays highlighted while its window is open.

  Notes      App 1. An editable notes window with placeholder text, a
             character count, and Clear / Reset buttons.

  Calc       App 2. Adds, subtracts, multiplies or divides two numbers.
             Clear resets both inputs and the result.

  Task Mgr   A Windows-style table of processes with CPU and memory usage
             (dummy values).

  Notes:
    - Close an app with its taskbar button or the X in its title bar.
    - Text and numbers stay while the program runs, but are not saved to disk.


-------------------------------------------------------------------------------
 2. HOW TO RUN - VS CODE (recommended)
-------------------------------------------------------------------------------

  1. Open this folder in VS Code (File > Open Folder), not a single file.
  2. Install the "C/C++" extension by Microsoft if VS Code asks for it.
  3. Open Run and Debug (Ctrl+Shift+D) and choose "Run Desktop".
  4. Press the green Run button or F5.

  IMPORTANT
    - Do not use the small Run button above the editor ("Run C/C++ File").
      It compiles only the open file and will fail.
    - The first run downloads the dependencies and compiles, which takes a
      few minutes. After that, Run starts right away unless src/ changed.


-------------------------------------------------------------------------------
 3. HOW TO RUN - POWERSHELL (Windows x64)
-------------------------------------------------------------------------------

  Build:   powershell -ExecutionPolicy Bypass -File scripts/build.ps1
  Run:     .\build\desktop.exe

  - The first build runs scripts/setup.ps1 automatically and needs internet
    once. It downloads checksum-pinned Dear ImGui 1.91.9b, GLFW 3.4,
    stb_image 2.30 and portable w64devkit 2.0.0 into .deps, without a
    system install.
  - Later builds work offline. Dependencies and build output are git-ignored.
  - The executable statically links GLFW and the C++ runtime.
  - Requires a Windows graphics driver that supports OpenGL 3.0.


-------------------------------------------------------------------------------
 4. USING THE DESKTOP
-------------------------------------------------------------------------------

  - Resize or maximize normally. Minimum window size is 320 x 240.
  - Click the taskbar buttons to open and close Notes, Calc and Task Mgr.
  - Click PWR at the bottom right to shut down.
  - Keyboard: press Tab to focus PWR, then Space or Enter to activate it.

  IMPORTANT
    Always close the program with PWR, not the window's X or the debugger's
    Stop button.


-------------------------------------------------------------------------------
 5. CONFIGURATION (config.txt)
-------------------------------------------------------------------------------

  config.txt in the project folder is read once at startup, so changing it
  does not require recompiling. Edit it, save, then press Run again.

  Format: one "key value" pair per line. Lines starting with # are comments.

  KEY             DEFAULT     DESCRIPTION
  --------------  ----------  ---------------------------------------------
  window-width    1280        Starting window width (320 - 3840)
  window-height   720         Starting window height (240 - 2160)
  clock-24h       false       true = 15:25:42, false = 03:25:42 PM
  show-seconds    true        Show or hide seconds on the clock
  desktop-title   "CSOPESY"   Label at the bottom left (max 24 characters)
  font-size       18          Text size in the app windows (10 - 32)

  - A missing file, missing line, or invalid value uses the default.
  - Out-of-range numbers are clamped to the nearest allowed value.


-------------------------------------------------------------------------------
 6. AUTOMATED TESTS
-------------------------------------------------------------------------------

  Run:     powershell -ExecutionPolicy Bypass -File scripts/verify.ps1

  The verification executable opens a window and:
    - resizes it through 1280x720, 800x600, 640x360, 360x640, 320x240 and
      1600x900, capturing real OpenGL framebuffers at each size
    - checks wallpaper edge coverage and window layering
    - confirms the clock keeps updating
    - opens and closes Notes from the taskbar
    - clicks PWR through ImGui mouse input and checks cleanup

  Before running:
    - Keep the test window unminimized and do not move the mouse.
    - Keep show-seconds true in config.txt, or the clock check may fail.

  Output:
    Results       docs/ppt/test-results.txt
    Screenshots   docs/ppt/screenshots/
    Slide notes   docs/ppt/desktop-slides.md, docs/ppt/taskbar-slides.md


-------------------------------------------------------------------------------
 7. PROJECT FILES
-------------------------------------------------------------------------------

  src/main.cpp              Entry point: window setup and the render loop
  src/config.h/.cpp         Reads config.txt
  src/wallpaper.h/.cpp      Loads assets/bg.jpg and draws it full-window
  src/desktop.h/.cpp        Wallpaper, clock and PWR button
  src/taskbar.h/.cpp        Bottom taskbar and its three buttons
  src/app1.h/.cpp           Notes (App 1)
  src/app2.h/.cpp           Calculator (App 2)
  src/task_manager.h/.cpp   Task Manager
  tests/verification.*      Automated checks used by scripts/verify.ps1
  scripts/                  Setup, build and verify scripts
  assets/bg.jpg             Wallpaper image

  Render order: each frame, main.cpp draws the desktop first, then the
  taskbar, then the app windows. The wallpaper, clock and taskbar background
  use ImGui's background draw list, so they always sit below app windows and
  never block clicks.


-------------------------------------------------------------------------------
 8. REFERENCES / THIRD-PARTY LICENSES
-------------------------------------------------------------------------------

  Dear ImGui (MIT)        https://github.com/ocornut/imgui/tree/v1.91.9b
  GLFW (zlib/libpng)      https://www.glfw.org/docs/3.4/window_guide.html
  stb_image (public dom.) https://github.com/nothings/stb
  w64devkit build tools   https://github.com/skeeto/w64devkit/releases/tag/v2.0.0

  - Downloaded packages keep their upstream license files in .deps.
  - Wallpaper image: assets/bg.jpg (add the artist credit here).
