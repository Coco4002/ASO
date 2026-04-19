/*Să se proiecteze o aplicație (powershell, cmd, MSVC/C++ preferabil) care să identifice toate serviciile sistem 
care rulează la modul curent pe mașină*/

#include <windows.h>
#include <iostream>
#include <vector>
#include <string>

int main() {
    SC_HANDLE hSCM = OpenSCManager(nullptr, nullptr, SC_MANAGER_ENUMERATE_SERVICE);
    if (hSCM == nullptr) {
        std::cerr << "Eroare la OpenSCManager. Cod: " << GetLastError() << std::endl;
        return 1;
    }

    DWORD bytesNeeded = 0;
    DWORD servicesCount = 0;
    DWORD resumeHandle = 0;

    EnumServicesStatusEx(
        hSCM,
        SC_ENUM_PROCESS_INFO,
        SERVICE_WIN32,
        SERVICE_STATE_ALL,
        nullptr,
        0,
        &bytesNeeded,
        &servicesCount,
        &resumeHandle,
        nullptr
    );

    if (GetLastError() != ERROR_MORE_DATA) {
        std::cerr << "Eroare la prima apelare EnumServicesStatusEx. Cod: " << GetLastError() << std::endl;
        CloseServiceHandle(hSCM);
        return 1;
    }

    std::vector<BYTE> buffer(bytesNeeded);

    if (!EnumServicesStatusEx(
        hSCM,
        SC_ENUM_PROCESS_INFO,
        SERVICE_WIN32,
        SERVICE_STATE_ALL,
        buffer.data(),
        bytesNeeded,
        &bytesNeeded,
        &servicesCount,
        &resumeHandle,
        nullptr)) {
        std::cerr << "Eroare la a doua apelare EnumServicesStatusEx. Cod: " << GetLastError() << std::endl;
        CloseServiceHandle(hSCM);
        return 1;
    }

    LPENUM_SERVICE_STATUS_PROCESS services =
        reinterpret_cast<LPENUM_SERVICE_STATUS_PROCESS>(buffer.data());

    std::cout << "Servicii care ruleaza in acest moment:\n\n";

    for (DWORD i = 0; i < servicesCount; i++) {
        if (services[i].ServiceStatusProcess.dwCurrentState == SERVICE_RUNNING) {
            std::wcout << L"Nume serviciu: " << services[i].lpServiceName << std::endl;
            std::wcout << L"Nume afisat : " << services[i].lpDisplayName << std::endl;
            std::wcout << L"PID         : " << services[i].ServiceStatusProcess.dwProcessId << std::endl;
            std::wcout << L"-----------------------------" << std::endl;
        }
    }

    CloseServiceHandle(hSCM);
    return 0;
}