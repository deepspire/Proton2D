#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include "menu.hh"
#include "proton/proton.hh"
#include "ss.hh"


int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv)
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    Proton::Display display("Proton2D test application", 600, 500);
    display.setIcon("kachan.png");
    display.setScene(new Menu(display.getRenderer(), display.getNativeWindow()));
    // display.setRenderScale(2, 2);
    display.startRendering();
    return 0;
}
