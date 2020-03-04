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
#include "gui_main.hpp"

#include "gui_error.hpp"
#include "upload.hpp"

#include <string>

MainGui::MainGui() {}
MainGui::~MainGui() {}

tsl::elm::Element *MainGui::createUI() {
    auto rootFrame = new tsl::elm::OverlayFrame("ShareNX", "v1.0.1");

    u64 size;

    Result rc = capsaGetLastOverlayScreenShotThumbnail(&this->fileId, &size, this->img, IMG_SIZE);
    if (R_FAILED(rc) || size == 0 || this->fileId.application_id == 0) {
        tsl::changeTo<ErrorGui>(rc, "No screenshot taken!");
        return rootFrame;
    }

    u64 w, h;
    void *jpg = malloc(JPG_SIZE);
    if (!jpg) {
        tsl::changeTo<ErrorGui>(0, "Out of memory!");
        return rootFrame;
    }

    rc = capsaLoadAlbumScreenShotThumbnailImage(&w, &h, &this->fileId, this->img, IMG_SIZE, jpg, JPG_SIZE);
    free(jpg);

    if (R_FAILED(rc) || w != THUMB_WIDTH || h != THUMB_HEIGHT) {
        tsl::changeTo<ErrorGui>(rc, "CapSrv error!");
        return rootFrame;
    }

    snprintf(this->appId, 0x11, "%016lX", this->fileId.application_id);

    std::snprintf(this->date, 0x20, "%4d:%02d:%02d %02d:%02d:%02d",
                  this->fileId.datetime.year,
                  this->fileId.datetime.month,
                  this->fileId.datetime.day,
                  this->fileId.datetime.hour,
                  this->fileId.datetime.minute,
                  this->fileId.datetime.second);

    auto *custElm = new tsl::elm::CustomDrawer([=](tsl::gfx::Renderer *renderer, u16 x, u16 y, u16 w, u16 h) {
        /* Thumbnail */
        if (this->img) {
            u16 tmb_x = (w - THUMB_WIDTH) / 2;
            u16 tmp_y = 110;
            renderer->drawRect(tmb_x, tmp_y, THUMB_WIDTH, THUMB_HEIGHT, a(0xF000));
            renderer->drawBitmap(tmb_x, tmp_y, THUMB_WIDTH, THUMB_HEIGHT, this->img);
        }

        /* Meta */
        renderer->drawString(this->appId, false, 95, 340, 25, a(0xFFFF));
        renderer->drawString(this->date, false, 95, 380, 25, a(0xFFFF));

        /* Button */
        {
            static float counter = 0;
            const float progress = (std::sin(counter) + 1) / 2;
            tsl::gfx::Color highlightColor = {static_cast<u8>((0x2 - 0x8) * progress + 0x8),
                                              static_cast<u8>((0x8 - 0xF) * progress + 0xF),
                                              static_cast<u8>((0xC - 0xF) * progress + 0xF),
                                              0xF};
            counter += 0.1F;

            u16 btn_x = (w - 240) / 2;
            u16 btn_y = 430;
            u16 btn_w = 240;
            u16 btn_h = 80;

            renderer->drawRect(btn_x, btn_y, btn_w, btn_h, a(0xF000));

            renderer->drawRect(btn_x - 4, btn_y - 4, btn_w + 8, 4, a(highlightColor));
            renderer->drawRect(btn_x - 4, btn_y + btn_h, btn_w + 8, 4, a(highlightColor));
            renderer->drawRect(btn_x - 4, btn_y, 4, btn_h, a(highlightColor));
            renderer->drawRect(btn_x + btn_w, btn_y, 4, btn_h, a(highlightColor));

            renderer->drawString("Upload", false, btn_x + 75, btn_y + 45, 25, a(0xFFFF));
        }

        /* URL */
        renderer->drawString(this->url.c_str(), false, 10, 540, 20, a(0xFFFF));
    });

    rootFrame->setContent(custElm);

    return rootFrame;
}

bool MainGui::handleInput(u64 down, u64 held, touchPosition pos, JoystickPosition left, JoystickPosition right) {
    if (down & KEY_A && !this->uploaded) {
        this->url = std::move(web::UploadImage(this->fileId));
        this->uploaded = true;
        return true;
    }
    return false;
}
