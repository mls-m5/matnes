#pragma once

#include "matgui/view.h"
#include "ppu.h"

namespace matnes {

class PpuView : public matgui::View {
public:
    void draw() override;
    void setPpu(const Ppu *ppu) {
        _ppu = ppu;
    }

private:
    const Ppu *_ppu = nullptr;
};

} // namespace matnes
