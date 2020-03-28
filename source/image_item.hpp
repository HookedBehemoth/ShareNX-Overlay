#pragma once

#include <tesla.hpp>

constexpr size_t THUMB_WIDTH = 320, THUMB_HEIGHT = 180;
constexpr size_t JPG_SIZE = 0x80000;
constexpr size_t IMG_SIZE = THUMB_WIDTH * THUMB_HEIGHT * 4;

class ImageItem : public tsl::elm::ListItem {
  private:
    const u8 *buffer;
    s32 img_x, img_y;

  public:
    ImageItem(const u8 *image)
        : ListItem(""), buffer(image), img_x(), img_y() {}

    virtual void draw(tsl::gfx::Renderer *renderer) override {
        if (this->img_x == 0 || this->img_y == 0) {
            this->img_x = this->getX() + ((this->getWidth() - THUMB_WIDTH) / 2);
            this->img_y = this->getY() + ((this->getHeight() - THUMB_HEIGHT) / 2);
        }
        renderer->drawRect(this->img_x, this->img_y, THUMB_WIDTH, THUMB_HEIGHT, 0xf000);
        renderer->drawBitmap(this->img_x, this->img_y, THUMB_WIDTH, THUMB_HEIGHT, this->buffer);
    }

    virtual void layout(u16 parentX, u16 parentY, u16 parentWidth, u16 parentHeight) override {
        this->setBoundaries(this->getX(), this->getY(), this->getWidth(), THUMB_HEIGHT + 150);
    }

    virtual tsl::elm::Element *requestFocus(tsl::elm::Element *, tsl::FocusDirection) {
        return nullptr;
    }
};
