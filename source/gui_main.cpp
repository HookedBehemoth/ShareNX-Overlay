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

#include <string>

#include "gui_error.hpp"
#include "image_item.hpp"
#include "upload.hpp"

MainGui::MainGui(const CapsAlbumFileId &file_id, const u8 *rgba_buffer)
    : fileId(file_id), buffer(rgba_buffer) {}

MainGui::~MainGui() {}

tsl::elm::Element *MainGui::createUI() {
    auto rootFrame = new tsl::elm::OverlayFrame("ShareNX", "v1.0.1");

    std::snprintf(this->appId, 0x11, "%016lX", this->fileId.application_id);

    std::snprintf(this->date, 0x20, "%4d:%02d:%02d %02d:%02d:%02d",
                  this->fileId.datetime.year,
                  this->fileId.datetime.month,
                  this->fileId.datetime.day,
                  this->fileId.datetime.hour,
                  this->fileId.datetime.minute,
                  this->fileId.datetime.second);

    auto *list = new tsl::elm::List(2);

    list->addItem(new ImageItem(this->buffer));

    auto *button = new tsl::elm::ListItem("Upload");
    button->setClickListener([&](u64 keys) {
        if (keys && KEY_A && !this->uploaded) {
            this->url = web::UploadImage(this->fileId);
            this->uploaded = true;
            list->addItemPostponed(new tsl::elm::ListItem(url));
            return true;
        }
        return false;
    });
    list->addItem(button);
    list->addItem(new tsl::elm::ListItem(this->appId));
    list->addItem(new tsl::elm::ListItem(this->date));

    rootFrame->setContent(list);

    return rootFrame;
}

bool MainGui::handleInput(u64, u64, touchPosition, JoystickPosition, JoystickPosition) {
    return false;
}
