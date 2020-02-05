#pragma once
#include <tesla.hpp>

class GuiMain : public tsl::Gui {
private:
    u8* img = nullptr;
    char appId[0x11];
    char date[0x20];
    bool uploaded = false;
public:
    GuiMain();
    ~GuiMain();

    virtual tsl::Element* createUI();
};