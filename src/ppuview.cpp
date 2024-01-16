#include "ppuview.h"
#include "matgui/draw.h"
#include "matgui/texture.h"

namespace matnes {

void PpuView::draw() {
    if (!_ppu) {
        return;
    }
    auto screenMemory = _ppu->_screenMemory;

    { // TODO: This is test code. Remove as fast as possible when working
        screenMemory.data.at(10) = {255, 255, 255, 255};
    }

    // TODO: Somday make this more efficient
    auto pixels = std::vector<matgui::Texture::Pixel>{};

    pixels.resize(screenMemory.data.size());
    for (size_t i = 0; i < screenMemory.data.size(); ++i) {
        auto p = screenMemory.data.at(i);
        pixels.at(i) = {p.r, p.g, p.b, p.a};
    }
    auto texture = matgui::Texture{};
    texture.createBitmap(pixels, ScreenMemory::width, ScreenMemory::height);
    texture.interpolation(matgui::Texture::Nearest);

    matgui::drawTextureRect(matgui::vec{0., 0.},
                            0.,
                            // static_cast<double>(ScreenMemory::width),
                            // static_cast<double>(ScreenMemory::height),
                            width(),
                            height(),
                            texture,
                            matgui::DrawStyle::OrigoTopLeft);
}

} // namespace matnes
