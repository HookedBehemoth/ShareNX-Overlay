#include "gui_error.hpp"

class LastFrame : public tsl::element::Frame {
    public:
        virtual bool onClick(s64 key) {
            if (key == KEY_B) {
                tsl::Gui::closeGui();
                return true;
            }

            return false;
        }
};

ErrorGui::ErrorGui(Result result, const std::string &message) : rc(result), msg(message) { }
ErrorGui::~ErrorGui() { }

tsl::Element* ErrorGui::createUI() {
    auto root = new LastFrame();
    auto error = new tsl::element::CustomDrawer(0, 0, 100, FB_WIDTH, [&](u16 x, u16 y, tsl::Screen *screen) {
        screen->drawString("\uE150", false, (FB_WIDTH - 90) / 2, 300, 90, tsl::a(0xFFFF));
        screen->drawString(msg.c_str(), false, 105, 380, 25, tsl::a(0xFFFF));
        if (rc != 0) {
            char errorCode[10];
            snprintf(errorCode, 10, "%04d-%04d", 2000 + R_MODULE(rc), R_DESCRIPTION(rc));
            screen->drawString(errorCode, false, 120, 430, 25, tsl::a(0xFFFF));
        }
    });
    root->addElement(error);
    return root;
}