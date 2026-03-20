/*Să se citească toate subcheile unei chei (la alegere) din Registry și să se afișeze la ieșirea standard.*/

#include <stdio.h>
#include <windows.h>

int main() {
    char keyPath[] = "SOFTWARE";

    HKEY hkey;
    LSTATUS result;

    result = RegOpenKeyExA(HKEY_LOCAL_MACHINE, keyPath, 0, KEY_READ, &hkey);
    if (result != ERROR_SUCCESS) {
        printf("Cheia nu a fost deschisa cu succes");
        return FALSE;
    }

    DWORD index = 0;
    char lpName[256];
    LPDWORD lpcchName = sizeof(lpName);

    while (1) {
        result = RegEnumKeyExA(hkey, index++, lpName, &lpcchName, NULL, NULL, NULL, NULL);

        if (result == ERROR_NO_MORE_ITEMS) {
            break;
        }

        if (result == ERROR_SUCCESS) {
            printf("Subcheia:%s\n", lpName);
        }

        lpcchName = sizeof(lpName);
    }

    RegCloseKey(hkey);

    return 0;
}