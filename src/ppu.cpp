#include "ppu.h"

#include "matgui/draw.h"

Ppu::Ppu() {
}

void Ppu::draw() {

    _testPaint.drawLine(0, 0, width(), height());
}
