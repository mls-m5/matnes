
#include "matgui/application.h"
#include "matgui/button.h"
#include "matgui/window.h"
#include "ppu.h"

using namespace std;
using namespace matgui;

int main(int argc, char **argv) {
    Application app(argc, argv);

    Window window("Lasersköld nes emulator");

    window.style.fill.color(.4, 0, 0);
    window.updateStyle();

    window.createChild<Ppu>();

    app.mainLoop();

    return 0;
}
