#include "elm_qr.hpp"
#include "qrcodegen.h"

namespace {

constexpr auto QR_BUFFER_SIZE = qrcodegen_BUFFER_LEN_MAX;
constexpr auto SCALE = 4;

} // namespace

void ElmQr::generateQrCode(const std::string& text) {
    std::vector<u8> qr_buffer(QR_BUFFER_SIZE);
    std::vector<u8> temp_buffer(QR_BUFFER_SIZE);
    if (!qrcodegen_encodeText(text.c_str(), temp_buffer.data(), qr_buffer.data(), qrcodegen_Ecc_MEDIUM, qrcodegen_VERSION_MIN, qrcodegen_VERSION_MAX, qrcodegen_Mask_AUTO, true)) {
        return;
    }

    const auto size = qrcodegen_getSize(qr_buffer.data());
    if (size <= 0) {
        return;
    }

    qr_size = size;
    qr_data = qr_buffer;
    Element::invalidate();
}

tsl::elm::Element *ElmQr::requestFocus(tsl::elm::Element *oldFocus, tsl::FocusDirection direction) {
    return oldFocus;
}

void ElmQr::draw(tsl::gfx::Renderer *renderer) {
    if (!qr_size) {
        return;
    }

    const auto white = tsl::Color(0xFF, 0xFF, 0xFF, 0xFF);
    const auto black = tsl::Color(0x00, 0x00, 0x00, 0xFF);

    for (int y = 0; y < qr_size; y++) {
        for (int x = 0; x < qr_size; x++) {
            const auto c = qrcodegen_getModule(qr_data.data(), x, y) ? white : black;
            for (int yy = 0; yy < SCALE; yy++) {
                for (int xx = 0; xx < SCALE; xx++) {
                    renderer->setPixel(this->getX() + x * SCALE + xx, this->getY() + y * SCALE + yy, c);
                }
            }
        }
    }
}

void ElmQr::layout(u16 parentX, u16 parentY, u16 parentWidth, u16 parentHeight) {
    this->setBoundaries(tsl::cfg::FramebufferWidth / 2 - qr_size * SCALE / 2, this->getY() + 10, qr_size * SCALE, qr_size * SCALE);
}
