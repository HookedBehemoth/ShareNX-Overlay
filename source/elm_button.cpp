#include "elm_button.hpp"

Button::Button(u16 x, u16 y, u16 w, u16 h, const std::string &txt, std::function<bool(s64)> cb) : m_text(txt), m_onClick(cb) {
    this->setPosition(x, y);
    this->setSize(w, h);
}

Button::~Button() { }

tsl::Element* Button::requestFocus(tsl::Element *oldFocus, FocusDirection direction) {
    return this;
}

void Button::draw(tsl::Screen *screen, u16 x1, u16 y1) {
    const auto [x, y] = this->getPosition();
    const auto [w, h] = this->getSize();

    screen->drawRect(x, y, w, 1, a({ 0x5, 0x5, 0x5, 0xF }));
    screen->drawRect(x, y + h - 1, w, 1, a({ 0x5, 0x5, 0x5, 0xF }));

    screen->drawString(this->m_text.c_str(), false, x + 20, y + 45, 23, a({ 0xF, 0xF, 0xF, 0xF }));
}

void Button::layout() {

}

bool Button::onClick(s64 key) {
    return m_onClick(key);
}