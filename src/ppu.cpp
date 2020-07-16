#include "ppu.h"

#include "matgui/draw.h"

Ppu::Ppu() {
    _testPaint.line.color(1, 1, 1);
}

void Ppu::draw() {
    _testPaint.drawLine(0, 0, width(), height());
}
