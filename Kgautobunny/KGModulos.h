#pragma once

// Funci�n para obtener la direcci�n base de un m�dulo en un proceso
uintptr_t kg_obtener_modulo(DWORD processID, const wchar_t* nombre_del_modulo) {
    uintptr_t moduleBase = 0;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processID);
    if (hSnapshot != INVALID_HANDLE_VALUE) {
        MODULEENTRY32W moduleEntry;
        moduleEntry.dwSize = sizeof(MODULEENTRY32W);
        if (Module32FirstW(hSnapshot, &moduleEntry)) {
            do {
                if (!_wcsicmp(moduleEntry.szModule, nombre_del_modulo)) {
                    moduleBase = (uintptr_t)moduleEntry.modBaseAddr;
                    break;
                }
            } while (Module32NextW(hSnapshot, &moduleEntry));
        }
        CloseHandle(hSnapshot);
    }
    return moduleBase;
}

HANDLE kg_abrir_proceso(const char* nombre, DWORD acceso) {
    PROCESSENTRY32 proceso;
    proceso.dwSize = sizeof(PROCESSENTRY32);
    HANDLE capturar = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    if (Process32First(capturar, &proceso) == FALSE) {
        CloseHandle(capturar);
        return INVALID_HANDLE_VALUE;
    }
    while (Process32Next(capturar, &proceso) == TRUE) {
        if (strcmp(proceso.szExeFile, nombre) == 0) {
            HANDLE processHandle = OpenProcess(acceso, FALSE, proceso.th32ProcessID);
            CloseHandle(capturar);
            pid = proceso.th32ProcessID;
            return processHandle;
        }
    }

    CloseHandle(capturar);
    return INVALID_HANDLE_VALUE;
}