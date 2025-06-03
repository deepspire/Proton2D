#include "proton/proton.hh"
#include "ss.hh"
#include "menu.hh"
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

int main(int argc, char **argv)
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    Proton::Display display("Proton2D test application");
    display.setScene(new Menu(&display));
    display.startRendering();
    return 0;
}
