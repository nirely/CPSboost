#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
#include <windows.h>
#include <filesystem>

using namespace std;
namespace fs = filesystem;

void createDefaultCFG() {
    ofstream cfg_file("config.cfg");

    if (!cfg_file.is_open()) { cerr << "failed to open file!"; return; }

    cfg_file << "average_cps_left=10\n";
    cfg_file << "average_cps_right=10\n";
    cfg_file << "keybind=M\n";

    cfg_file.close();
}

map<string, string> readCFG() {
    map<string, string> config_data;
    ifstream config_file("config.cfg");
    string line;

    if (!config_file.is_open()) { cerr << "failed to open file!"; return config_data; }

    while (getline(config_file, line)) {
        if (line.empty()) { continue; }

        size_t delimiterPos = line.find("=");
        if (delimiterPos != string::npos) {
            string key = line.substr(0, delimiterPos);
            string valueStr = line.substr(delimiterPos + 1);
            config_data[key] = valueStr;
        }
    }

    config_file.close();
    return config_data;
}

USHORT charToVirtualKey(char c) {
    if (c >= 'A' && c <= 'Z') { return static_cast<USHORT>(toupper(c)); }
    else if (c >= '0' && c <= '9') { return static_cast<USHORT>(c); }
    else if (c == ' ') { return VK_SPACE; }
    else if (c == ',') { return VK_OEM_COMMA; }
    else if (c == '.') { return VK_OEM_PERIOD; }

    return 0;
}

int getRandomCPS(double averageCPS, double deviationPercentage = 0.2) {
    double deviation = averageCPS * deviationPercentage;
    return static_cast<int>(averageCPS - deviation + rand() % static_cast<int>(2 * deviation + 1));
}

void simulateClick(bool isLeftButton) {
    INPUT input = { 0 };
    input.type = INPUT_MOUSE;

    if (isLeftButton) {
        input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
        SendInput(1, &input, sizeof(INPUT));
        input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
        SendInput(1, &input, sizeof(INPUT));
    }
    else {
        input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
        SendInput(1, &input, sizeof(INPUT));
        input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
        SendInput(1, &input, sizeof(INPUT));
    }
}

int main() {
    ifstream checkFile("config.cfg");
    if (!checkFile) { createDefaultCFG(); }
    checkFile.close();

    bool leftButtonClicked = false;
    bool rightButtonClicked = false;
    bool isProgramActive = false;

    map<string, string> config_data = readCFG();
    srand(static_cast<unsigned int>(time(nullptr)));

    double average_cps_left = stod(config_data["average_cps_left"]);
    double average_cps_right = stod(config_data["average_cps_right"]);
    char keybindChar = toupper(config_data["keybind"][0]);
    USHORT toggleKeyCode = charToVirtualKey(keybindChar);

    if (toggleKeyCode == 0) { cerr << "Invalid keybind in config file! Defaulting to 'M'" << endl; toggleKeyCode = 0x4D; }

    fs::file_time_type lastWriteTime = fs::last_write_time("config.cfg");

    HWND hwnd = CreateWindowEx(
        0,
        "STATIC",
        "RawInputWindow",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL, NULL, NULL, NULL
    );

    if (!hwnd) { cerr << "Failed to create window! Error: " << GetLastError() << endl; return 1; }

    RAWINPUTDEVICE rid;
    rid.usUsagePage = 0x01;
    rid.usUsage = 0x06;
    rid.dwFlags = RIDEV_INPUTSINK | RIDEV_NOLEGACY;
    rid.hwndTarget = hwnd;

    if (!RegisterRawInputDevices(&rid, 1, sizeof(rid))) { cerr << "Failed to register raw input device! Error: " << GetLastError() << endl; return 1; }

    MSG msg;
    while (true) {
        fs::file_time_type currentWriteTime = fs::last_write_time("config.cfg");

        if (currentWriteTime != lastWriteTime) {
            config_data = readCFG();
            lastWriteTime = currentWriteTime;

            average_cps_left = stod(config_data["average_cps_left"]);
            average_cps_right = stod(config_data["average_cps_right"]);
            keybindChar = toupper(config_data["keybind"][0]);
            toggleKeyCode = charToVirtualKey(keybindChar);

            if (toggleKeyCode == 0) { cerr << "Invalid keybind in config file! Defaulting to 'M'" << endl; toggleKeyCode = 0x4D; }
        }

        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_INPUT) {
                UINT dwSize = 40;
                BYTE lpb[40];
                GetRawInputData((HRAWINPUT)msg.lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER));
                RAWINPUT* raw = (RAWINPUT*)lpb;
                if (raw->header.dwType == RIM_TYPEKEYBOARD) {
                    USHORT keyCode = raw->data.keyboard.VKey;
                    USHORT flags = raw->data.keyboard.Flags;
                    if (keyCode == toggleKeyCode && !(flags & RI_KEY_BREAK)) { isProgramActive = !isProgramActive; }
                }
            }
        }

        if (!isProgramActive) { this_thread::sleep_for(chrono::milliseconds(10)); continue; }

        if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
            if (!leftButtonClicked) {
                leftButtonClicked = true;
                int extraClicks = getRandomCPS(average_cps_left);
                for (int i = 0; i < extraClicks; ++i) { simulateClick(true); }
            }
        }
        else { leftButtonClicked = false; }

        if (GetAsyncKeyState(VK_RBUTTON) & 0x8000) {
            if (!rightButtonClicked) {
                rightButtonClicked = true;
                int extraClicks = getRandomCPS(average_cps_right);
                for (int i = 0; i < extraClicks; ++i) { simulateClick(false); }
            }
        }
        else { rightButtonClicked = false; }

        this_thread::sleep_for(chrono::milliseconds(10));
    }

    return 0;
}