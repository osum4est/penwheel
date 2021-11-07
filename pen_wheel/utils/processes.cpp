#include "processes.h"

#if WIN32

#include <Windows.h>
#include <WinUser.h>
#include <Psapi.h>
#include <filesystem>

process processes::get_active_process() {
    HWND foreground_window = GetForegroundWindow();

    DWORD pid;
    if (!GetWindowThreadProcessId(foreground_window, &pid))
        return { };

    HANDLE handle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
    if (!handle)
        return { };

    WCHAR buffer[MAX_PATH];
    if (!GetModuleFileNameEx(handle, nullptr, buffer, MAX_PATH))
        return { };

    std::wstring wide_string(buffer);
    std::filesystem::path path(wide_string);
    CloseHandle(handle);
    return {
            .pid = pid,
            .name = path.stem().string()
    };
}

#endif
