#pragma once
#include <tesla.hpp>

class Text : public tsl::Element {
public:
    Text(u16 x, u16 y, u16 w, u16 h, const std::string &txt);
    ~Text();

    tsl::Element* requestFocus(Element *oldFocus, FocusDirection direction) {
        return nullptr;
    }

    void draw(tsl::Screen *screen, u16 x, u16 y) override;
    void layout() override;

    void setText(const std::string &txt);

private:
    std::string m_text;
};