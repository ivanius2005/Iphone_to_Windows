/*
 * ═══════════════════════════════════════════════════════════
 *  IVN_05 · Mirror Launcher
 * ───────────────────────────────────────────────────────────
 *  Lanza UxPlay con el entorno MinGW64 configurado, para
 *  arrancar el receptor AirPlay con doble clic sin abrir
 *  la terminal de MSYS2.
 *
 *  Compilar (desde MSYS2 MINGW64):
 *    gcc ivn_mirror_launcher.c -o IVN_Mirror.exe
 *
 *  ⚠ Edita TU_USUARIO en la ruta de uxplay.exe antes de
 *    compilar si tu instalación difiere.
 *
 *  Licencia: MIT
 * ═══════════════════════════════════════════════════════════
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

int main(void)
{
    /* Añadir mingw64\bin al PATH para que uxplay encuentre sus DLLs */
    char newpath[32768];
    const char *mingw = "C:\\msys64\\mingw64\\bin";
    char *oldpath = getenv("PATH");

    snprintf(newpath, sizeof(newpath), "%s;%s",
             mingw, oldpath ? oldpath : "");
    SetEnvironmentVariableA("PATH", newpath);

    /* Lanzar UxPlay con el nombre IVN_05 Mirror */
    const char *cmd =
        "C:\\msys64\\home\\TU_USUARIO\\UxPlay\\build\\uxplay.exe "
        "-n \"Iphone_to_Windows\"";

    printf("=========================================\n");
    printf("   Iphone_to_Windows - AirPlay Receiver\n");
    printf("=========================================\n");
    printf(" En tu iPhone:\n");
    printf(" Centro de Control -> Duplicar pantalla\n");
    printf(" -> Iphone_to_Windows\n");
    printf("=========================================\n\n");

    return system(cmd);
}
