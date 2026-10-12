#include "wallpaper.h"
#include "imgui.h"
#include <GLFW/glfw3.h>
#include <cstdint>
#include <cstdio>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_JPEG
#define STBI_ONLY_PNG
#include "stb_image.h"

#ifndef GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_EDGE 0x812F
#endif

namespace {
GLuint gTexture = 0;
float gAspect = 1.0f;
}

bool LoadWallpaper() {
    // run from the project folder (VS Code) or from build/ (double-click)
    int width = 0, height = 0;
    unsigned char* pixels = stbi_load("assets/bg.jpg", &width, &height, nullptr, 4);
    if (!pixels) pixels = stbi_load("../assets/bg.jpg", &width, &height, nullptr, 4);
    if (!pixels) {
        std::fprintf(stderr, "assets/bg.jpg not loaded, using the drawn wallpaper\n");
        return false;
    }

    glGenTextures(1, &gTexture);
    glBindTexture(GL_TEXTURE_2D, gTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    stbi_image_free(pixels);
    gAspect = float(width) / float(height);
    return true;
}

bool DrawWallpaper(ImDrawList* draw, const ImVec2& pos, const ImVec2& size) {
    if (!gTexture) return false;
    // crop the side that overflows so the image always fills the window
    ImVec2 uv0(0.0f, 0.0f), uv1(1.0f, 1.0f);
    const float windowAspect = size.x / size.y;
    if (windowAspect > gAspect) {
        const float visible = gAspect / windowAspect;
        uv0.y = (1.0f - visible) * 0.5f;
        uv1.y = 1.0f - uv0.y;
    } else {
        const float visible = windowAspect / gAspect;
        uv0.x = (1.0f - visible) * 0.5f;
        uv1.x = 1.0f - uv0.x;
    }
    draw->AddImage(ImTextureID(std::uintptr_t(gTexture)), pos,
        ImVec2(pos.x + size.x, pos.y + size.y), uv0, uv1);
    return true;
}

void FreeWallpaper() {
    if (gTexture) glDeleteTextures(1, &gTexture);
    gTexture = 0;
}
