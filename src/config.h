#pragma once
#include <string>

// settings read from config.txt; these defaults are used when a line is missing
struct Config {
    int windowWidth = 1280;
    int windowHeight = 720;
    bool clock24h = false;
    bool showSeconds = true;
    std::string desktopTitle = "CSOPESY";
    float fontSize = 18.0f;
};

// looks for config.txt in the working folder, then one folder up (when run from build/)
Config LoadConfig();
