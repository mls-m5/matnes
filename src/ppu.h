#pragma once

#include "matgui/paint.h"
#include "matgui/view.h"

class Ppu : public matgui::View {
public:
    Ppu();

    void draw() override;

private:
    matgui::Paint _testPaint;
};
