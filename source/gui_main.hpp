#pragma once
#include <tesla.hpp>

class GuiMain : public tsl::Gui {
private:
    u8* img;
public:
    GuiMain();
    ~GuiMain();

    virtual tsl::Element* createUI();

    virtual void onDestroy();
};