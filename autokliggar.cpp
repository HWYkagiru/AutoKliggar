/*
Copyright 2025 Developed by Kagiru.
This Tool is freely available on my GitHub: https://github.com/HWYkagiru
If you modify and distribute this tool, giving credits would be greatly appreciated.
*/

#include <Windows.h>
#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>
#include "config.h"

Config cfg;
std::atomic<bool> run(false), exitFlag(false);
std::atomic<bool> toggleState(false);

void Click() {
    INPUT in = {0}; in.type = INPUT_MOUSE; in.mi.dwFlags = MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP;
    auto nxt = std::chrono::steady_clock::now();
    while (!exitFlag.load(std::memory_order_relaxed)) {
        if (run.load(std::memory_order_relaxed)) {
            auto now = std::chrono::steady_clock::now();
            if (now >= nxt) {
                SendInput(1, &in, sizeof(INPUT));
                nxt += std::chrono::nanoseconds(1000000000 / cfg.cps);
                if (nxt < now) nxt = now + std::chrono::nanoseconds(1000000000 / cfg.cps);
            }
            std::this_thread::yield();
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            nxt = std::chrono::steady_clock::now();
        }
    }
}

bool IsAdmin() {
    BOOL a = FALSE; PSID ag; SID_IDENTIFIER_AUTHORITY Na = SECURITY_NT_AUTHORITY;
    if (AllocateAndInitializeSid(&Na, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &ag)) {
        if (!CheckTokenMembership(NULL, ag, &a)) a = FALSE;
        FreeSid(ag);
    }
    return a;
}

bool ReqAdmin() {
    wchar_t p[MAX_PATH];
    if (GetModuleFileNameW(NULL, p, ARRAYSIZE(p))) {
        SHELLEXECUTEINFOW s = {sizeof(s)}; s.lpVerb = L"runas"; s.lpFile = p; s.nShow = SW_NORMAL;
        return ShellExecuteExW(&s);
    }
    return false;
}

int main() {
    cfg.load("config.txt");
    bool a = false;
    if (cfg.admin && !IsAdmin()) {
        if (ReqAdmin()) return 0;
        MessageBoxA(NULL, "Running with limited permissions.", "Warning", MB_ICONWARNING);
    } else if (cfg.admin) {
        a = true;
        SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
    }
    
    std::string keyName;
    if (cfg.keybind == VK_XBUTTON1) keyName = "XButton1";
    else if (cfg.keybind == VK_XBUTTON2) keyName = "XButton2";
    else keyName = std::string(1, static_cast<char>(cfg.keybind));
    
    std::cout << "Loaded Config: CPS = " << cfg.cps << ", Keybind = " << keyName << " (" << cfg.keybind << "), Toggle = " << (cfg.toggle ? "On" : "Off") << std::endl;
    
    std::thread t(Click);
    bool prevKeyState = false;
    while (!exitFlag.load(std::memory_order_relaxed)) {
        bool currentKeyState = (GetAsyncKeyState(cfg.keybind) & 0x8000) != 0;
        
        if (cfg.toggle) {
            if (currentKeyState && !prevKeyState) {
                toggleState.store(!toggleState.load(std::memory_order_relaxed), std::memory_order_relaxed);
            }
            run.store(toggleState.load(std::memory_order_relaxed), std::memory_order_relaxed);
        } else {
            run.store(currentKeyState, std::memory_order_relaxed);
        }
        
        prevKeyState = currentKeyState;
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    t.join();
    return 0;
}