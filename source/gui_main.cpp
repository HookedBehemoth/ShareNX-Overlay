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
#include "elm_qr.hpp"
#include "upload.hpp"

MainGui::MainGui(const CapsAlbumFileId &file_id, const u8 *rgba_buffer)
    : fileId(file_id) {
    img = new ImageItem(file_id, rgba_buffer);
}

MainGui::~MainGui() {}

tsl::elm::Element *MainGui::createUI() {
    auto rootFrame = new tsl::elm::OverlayFrame("ShareNX \uE134", VERSION);

    auto list = new tsl::elm::List();

    list->addItem(this->img);

    auto qr = new ElmQr();

    auto button = new tsl::elm::ListItem("Upload");
    button->setClickListener([this, qr](u64 keys) {
        if (keys & HidNpadButton_A && !this->uploaded) {
            std::string url = web::UploadImage(this->fileId);
            this->uploaded = true;
            this->img->setUrl(url);
            qr->generateQrCode(url);
            return true;
        }
        return false;
    });
    list->addItem(button);
    list->addItem(qr);

    rootFrame->setContent(list);

    return rootFrame;
}
