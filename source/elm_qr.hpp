#pragma once

#include <tesla.hpp>
#include <vector>
#include <string>

class ElmQr final : public tsl::elm::Element {
  private:
    std::vector<u8> qr_data;
    int qr_size{};

  public:
    void generateQrCode(const std::string& text);

    tsl::elm::Element *requestFocus(tsl::elm::Element *oldFocus, tsl::FocusDirection direction) override;
    void draw(tsl::gfx::Renderer *renderer) override;
    void layout(u16 parentX, u16 parentY, u16 parentWidth, u16 parentHeight) override;
};
