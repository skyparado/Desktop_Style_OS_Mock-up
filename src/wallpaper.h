#pragma once
struct ImDrawList;
struct ImVec2;

// load assets/bg.jpg into an OpenGL texture; call after the OpenGL backend is ready
bool LoadWallpaper();

// draws the image over the whole area, cropped to keep its aspect ratio;
// returns false when no image is loaded
bool DrawWallpaper(ImDrawList* draw, const ImVec2& pos, const ImVec2& size);

void FreeWallpaper();
