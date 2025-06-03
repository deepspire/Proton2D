#include "proton/proton.hh"
#include "ss.hh"
#include "menu.hh"

int main(int argc, char **argv)
{
    Proton::Display display("Proton2D test application");
    display.setScene(new Menu(&display));
    display.startRendering();
    return 0;
}