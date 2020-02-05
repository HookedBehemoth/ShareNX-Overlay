/*
 * Copyright (c) 2020 Behemoth
 *
 * This file is part of ShareNX.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
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
    this->setTitle("ShareNX");
    this->setSubtitle("Behemoth, v1.0.0");

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