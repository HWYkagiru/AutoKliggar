#pragma once
#include <fstream>
#include <sstream>
#include <Windows.h>
#include <vector>
#include <map>
#include <string>

struct Config {
    int cps = 250; 
    int keybind = 'C'; 
    bool admin = true;
    bool toggle = false;
    std::map<std::string, int> supportedKeys = {
        {"A", 'A'}, {"B", 'B'}, {"C", 'C'}, {"D", 'D'}, {"E", 'E'}, {"F", 'F'},
        {"G", 'G'}, {"H", 'H'}, {"I", 'I'}, {"J", 'J'}, {"K", 'K'}, {"L", 'L'},
        {"M", 'M'}, {"N", 'N'}, {"O", 'O'}, {"P", 'P'}, {"Q", 'Q'}, {"R", 'R'},
        {"S", 'S'}, {"T", 'T'}, {"U", 'U'}, {"V", 'V'}, {"W", 'W'}, {"X", 'X'},
        {"Y", 'Y'}, {"Z", 'Z'},
        {"0", '0'}, {"1", '1'}, {"2", '2'}, {"3", '3'}, {"4", '4'},
        {"5", '5'}, {"6", '6'}, {"7", '7'}, {"8", '8'}, {"9", '9'},
        {"XButton1", VK_XBUTTON1}, {"XButton2", VK_XBUTTON2},
        {"LMB", VK_LBUTTON}, {"RMB", VK_RBUTTON}, {"MMB", VK_MBUTTON},
        {"Space", VK_SPACE}, {"Shift", VK_SHIFT}, {"Ctrl", VK_CONTROL},
        {"Alt", VK_MENU}, {"Tab", VK_TAB}, {"Enter", VK_RETURN},
        {"Backspace", VK_BACK}, {"Escape", VK_ESCAPE},
        {"F1", VK_F1}, {"F2", VK_F2}, {"F3", VK_F3}, {"F4", VK_F4},
        {"F5", VK_F5}, {"F6", VK_F6}, {"F7", VK_F7}, {"F8", VK_F8},
        {"F9", VK_F9}, {"F10", VK_F10}, {"F11", VK_F11}, {"F12", VK_F12},
        {"CapsLock", VK_CAPITAL}, {"NumLock", VK_NUMLOCK}, {"ScrollLock", VK_SCROLL},
        {"Insert", VK_INSERT}, {"Delete", VK_DELETE}, {"Home", VK_HOME},
        {"End", VK_END}, {"PageUp", VK_PRIOR}, {"PageDown", VK_NEXT},
        {"Left", VK_LEFT}, {"Right", VK_RIGHT}, {"Up", VK_UP}, {"Down", VK_DOWN},
        {"PrintScreen", VK_SNAPSHOT}, {"Pause", VK_PAUSE},
        {"NumPad0", VK_NUMPAD0}, {"NumPad1", VK_NUMPAD1}, {"NumPad2", VK_NUMPAD2},
        {"NumPad3", VK_NUMPAD3}, {"NumPad4", VK_NUMPAD4}, {"NumPad5", VK_NUMPAD5},
        {"NumPad6", VK_NUMPAD6}, {"NumPad7", VK_NUMPAD7}, {"NumPad8", VK_NUMPAD8},
        {"NumPad9", VK_NUMPAD9}, {"Multiply", VK_MULTIPLY}, {"Add", VK_ADD},
        {"Subtract", VK_SUBTRACT}, {"Decimal", VK_DECIMAL}, {"Divide", VK_DIVIDE},
        {"OEM1", VK_OEM_1}, {"OEMPlus", VK_OEM_PLUS}, {"OEMComma", VK_OEM_COMMA},
        {"OEMMinus", VK_OEM_MINUS}, {"OEMPeriod", VK_OEM_PERIOD}, {"OEM2", VK_OEM_2},
        {"OEM3", VK_OEM_3}, {"OEM4", VK_OEM_4}, {"OEM5", VK_OEM_5},
        {"OEM6", VK_OEM_6}, {"OEM7", VK_OEM_7}, {"OEM8", VK_OEM_8},
        {"LeftWindows", VK_LWIN}, {"RightWindows", VK_RWIN}, {"Apps", VK_APPS},
        {"Sleep", VK_SLEEP}, {"VolumeMute", VK_VOLUME_MUTE},
        {"VolumeDown", VK_VOLUME_DOWN}, {"VolumeUp", VK_VOLUME_UP},
        {"MediaNextTrack", VK_MEDIA_NEXT_TRACK}, {"MediaPrevTrack", VK_MEDIA_PREV_TRACK},
        {"MediaStop", VK_MEDIA_STOP}, {"MediaPlayPause", VK_MEDIA_PLAY_PAUSE},
        {"BrowserBack", VK_BROWSER_BACK}, {"BrowserForward", VK_BROWSER_FORWARD},
        {"BrowserRefresh", VK_BROWSER_REFRESH}, {"BrowserStop", VK_BROWSER_STOP},
        {"BrowserSearch", VK_BROWSER_SEARCH}, {"BrowserFavorites", VK_BROWSER_FAVORITES},
        {"BrowserHome", VK_BROWSER_HOME}
    };

    void load(const std::string& f) {
        std::ifstream file(f);
        if (file.is_open()) {
            std::string l, k, v;
            while (std::getline(file, l)) {
                std::istringstream iss(l);
                if (std::getline(iss, k, ':') && std::getline(iss, v)) {
                    v.erase(0, v.find_first_not_of(" \t")); v.erase(v.find_last_not_of(" \t") + 1);
                    if (k == "cps") cps = std::stoi(v);
                    else if (k == "keybind") {
                        auto it = supportedKeys.find(v);
                        if (it != supportedKeys.end()) {
                            keybind = it->second;
                        } else {
                            keybind = v[0];
                        }
                    }
                    else if (k == "admin") admin = (v == "true");
                    else if (k == "toggle") toggle = (v == "true");
                }
            }
        } else save(f);
    }

    void save(const std::string& f) {
        std::ofstream file(f);
        if (file.is_open()) {
            file << "cps: " << cps << "\nkeybind: ";
            for (const auto& pair : supportedKeys) {
                if (pair.second == keybind) {
                    file << pair.first;
                    break;
                }
            }
            file << "\nadmin: " << (admin ? "true" : "false");
            file << "\ntoggle: " << (toggle ? "true" : "false");
            
            file << "\n\nSupported keys:\n\n";
            file << "### Letters\nA, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z\n";
            
            file << "\n### Numbers\n0, 1, 2, 3, 4, 5, 6, 7, 8, 9\n";
            
            file << "\n### Function Keys\nF1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12\n";
            
            file << "\n### Mouse Buttons\nLMB, RMB, MMB, XButton1, XButton2\n";
            
            file << "\n### Other Keys\n";
            std::vector<std::string> otherKeys;
            for (const auto& pair : supportedKeys) {
                if (!(pair.first.size() == 1 && pair.first[0] >= 'A' && pair.first[0] <= 'Z') &&
                    !(pair.first.size() == 1 && pair.first[0] >= '0' && pair.first[0] <= '9') &&
                    !(pair.first.size() == 2 && pair.first[0] == 'F') &&
                    pair.first != "LMB" && pair.first != "RMB" && pair.first != "MMB" &&
                    pair.first != "XButton1" && pair.first != "XButton2") {
                    otherKeys.push_back(pair.first);
                }
            }
            for (size_t i = 0; i < otherKeys.size(); ++i) {
                file << otherKeys[i];
                if (i < otherKeys.size() - 1) file << ", ";
                if ((i + 1) % 8 == 0) file << "\n";
            }
            file << "\n";
        }
    }
};