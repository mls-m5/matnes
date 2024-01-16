
#include "matgui/application.h"
#include "matgui/window.h"
#include "ppuview.h"

using namespace std;
using namespace matgui;
using namespace matnes;

#ifndef TEST

int main(int argc, char **argv) {
    Application app(argc, argv);

    Window window("Lasersköld nes emulator");

    window.style.fill.color(.4, 0, 0);
    window.updateStyle();

    auto ppu = Ppu{};

    ppu.drawAll();

    window.createChild<PpuView>()->setPpu(&ppu);

    app.mainLoop();

    return 0;
}

#endif
