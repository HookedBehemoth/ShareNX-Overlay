#pragma once
#include <tesla.hpp>

class Button : public tsl::Element {
public:
    Button(u16 x, u16 y, u16 w, u16 h, const std::string &txt, std::function<bool(s64)> cb);
    ~Button();

    tsl::Element* requestFocus(tsl::Element *oldFocus, FocusDirection direction) override;

    void draw(tsl::Screen *screen, u16 x, u16 y) override;
    void layout() override;
    bool onClick(s64 key) override;

private:
    std::string m_text;
    std::function<bool(s64)> m_onClick;
};