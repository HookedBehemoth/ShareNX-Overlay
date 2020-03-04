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

ErrorGui::ErrorGui(Result result, const char *message) : rc(result), msg(message) {
    snprintf(this->errorCode, 10, "%04d-%04d", 2000 + R_MODULE(result), R_DESCRIPTION(result));
}
ErrorGui::~ErrorGui() {}

tsl::elm::Element *ErrorGui::createUI() {
    auto rootFrame = new tsl::elm::OverlayFrame("ShareNX", "v1.0.1");

    auto error = new tsl::elm::CustomDrawer([&](tsl::gfx::Renderer *screen, u16 x, u16 y, u16 w, u16 h) {
        screen->drawString("\uE150", false, (w - 90) / 2, 300, 90, a(0xFFFF));
        screen->drawString(this->msg, false, 105, 380, 25, a(0xFFFF));
        if (this->rc != 0) {
            screen->drawString(this->errorCode, false, 120, 430, 25, a(0xFFFF));
        }
    });

    rootFrame->setContent(error);
    return rootFrame;
}

bool ErrorGui::handleInput(u64 down, u64 held, touchPosition pos, JoystickPosition left, JoystickPosition right) {
    if (down & KEY_B) {
        tsl::Overlay::get()->close();
        return true;
    }
    return false;
}
