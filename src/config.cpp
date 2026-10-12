#include "config.h"
#include <cstdio>
#include <fstream>
#include <sstream>

namespace {
bool ParseBool(const std::string& text, bool fallback) {
    if (text == "true" || text == "1") return true;
    if (text == "false" || text == "0") return false;
    return fallback;
}

int ParseInt(const std::string& text, int fallback, int low, int high) {
    try {
        const int value = std::stoi(text);
        return value < low ? low : value > high ? high : value;
    } catch (...) { return fallback; }
}
}

Config LoadConfig() {
    Config config;
    std::ifstream file("config.txt");
    if (!file) file.open("../config.txt");
    if (!file) {
        std::fprintf(stderr, "config.txt not found, using defaults\n");
        return config;
    }

    // one "key value" pair per line, # starts a comment
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream in(line);
        std::string key, value;
        if (!(in >> key) || key[0] == '#') continue;
        std::getline(in >> std::ws, value);
        while (!value.empty() && (value.back() == '\r' || value.back() == ' ')) value.pop_back();
        if (value.size() >= 2 && value.front() == '"' && value.back() == '"')
            value = value.substr(1, value.size() - 2);

        if (key == "window-width") config.windowWidth = ParseInt(value, config.windowWidth, 320, 3840);
        else if (key == "window-height") config.windowHeight = ParseInt(value, config.windowHeight, 240, 2160);
        else if (key == "clock-24h") config.clock24h = ParseBool(value, config.clock24h);
        else if (key == "show-seconds") config.showSeconds = ParseBool(value, config.showSeconds);
        else if (key == "desktop-title") config.desktopTitle = value.substr(0, 24);
        else if (key == "font-size") config.fontSize = float(ParseInt(value, int(config.fontSize), 10, 32));
        else std::fprintf(stderr, "config.txt: unknown key '%s'\n", key.c_str());
    }
    return config;
}
