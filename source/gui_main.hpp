#pragma once
#include <tesla.hpp>

class GuiMain : public tsl::Gui {
private:
    u8* img = nullptr;
    bool uploaded = false;
public:
    GuiMain();
    ~GuiMain();

    virtual tsl::Element* createUI();
};