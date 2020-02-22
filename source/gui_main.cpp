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

#include "elm_button.hpp"
#include "elm_text.hpp"
#include "upload.hpp"

constexpr size_t THUMB_WIDTH = 320, THUMB_HEIGHT = 180;
constexpr size_t JPG_SIZE = 1024 * 1024;
constexpr size_t IMG_SIZE = THUMB_WIDTH * THUMB_HEIGHT * 4;

class Frame : public tsl::elm::Element {
public:
	void addElement(std::shared_ptr<tsl::elm::Element> elm) {
		m_elements.push_back(elm);
	}

	virtual void draw(tsl::gfx::Renderer *renderer) override {
		for (auto &elm : m_elements)
			elm->draw(renderer);
	}

	virtual void layout(u16 parentX, u16 parentY, u16 parentWidth, u16 parentHeight) override {
	}

private:
	std::vector<std::shared_ptr<tsl::elm::Element>> m_elements;
};

class ErrorFrame : public tsl::elm::CustomDrawer {
public:
	ErrorFrame(Result result, const std::string &message)
		: tsl::elm::CustomDrawer(
			  [&](tsl::gfx::Renderer *renderer, u16 x, u16 y, u16 w, u16 h) {
				  renderer->drawString("\uE150", false, (tsl::cfg::FramebufferWidth - 90) / 2, 300, 90, a(0xFFFF));
				  renderer->drawString(m_message.c_str(), false, 105, 380, 25, a(0xFFFF));
				  if (this->m_result != 0) {
					  char errorCode[10];
					  snprintf(errorCode, 10, "%04d-%04d", 2000 + R_MODULE(m_result), R_DESCRIPTION(m_result));
					  renderer->drawString(errorCode, false, 120, 430, 25, a(0xFFFF));
				  }
			  }),
		  m_result(result),
		  m_message(message) {};

private:
	Result m_result;
	std::string m_message;
};

GuiMain::~GuiMain() {
	if (this->img) {
		free(this->img);
		this->img = nullptr;
	}
}

tsl::elm::Element *GuiMain::createUI() {
	auto rootFrame = new tsl::elm::OverlayFrame("ShareNX", "Behemoth, v1.0.0");
	auto main = new Frame();

	CapsAlbumFileId file_id = {0};
	u64 size = 0;

	if (!this->img)
		this->img = (u8 *)malloc(IMG_SIZE);

	if (!img) {
		rootFrame->setContent(new ErrorFrame(0, "Out of memory!"));
		return rootFrame;
	}

	Result rc = capsaGetLastOverlayScreenShotThumbnail(&file_id, &size, this->img, IMG_SIZE);
	if (R_FAILED(rc) || size == 0 || file_id.application_id == 0) {
		rootFrame->setContent(new ErrorFrame(rc, "No screenshot taken!"));
		free(this->img);
		this->img = nullptr;
		return rootFrame;
	}

	u64 w, h;
	u8 *jpg = (u8 *)malloc(JPG_SIZE);
	if (!jpg) {
		rootFrame->setContent(new ErrorFrame(0, "Out of memory!"));
		free(this->img);
		this->img = nullptr;
		return rootFrame;
	}

	rc = capsaLoadAlbumScreenShotThumbnailImage(&w, &h, &file_id, this->img, IMG_SIZE, jpg, JPG_SIZE);
	free(jpg);

	if (R_FAILED(rc) || w != THUMB_WIDTH || h != THUMB_HEIGHT) {
		rootFrame->setContent(new ErrorFrame(rc, "CapSrv error!"));
		free(this->img);
		this->img = nullptr;
		return rootFrame;
	}

	snprintf(this->appId, 0x11, "%016lX", file_id.application_id);

	std::snprintf(this->date, 0x20, "%04d:%02d:%02d %02d:%02d:%02d",
				  file_id.datetime.year,
				  file_id.datetime.month,
				  file_id.datetime.day,
				  file_id.datetime.hour,
				  file_id.datetime.minute,
				  file_id.datetime.second);

	main->addElement(std::make_shared<tsl::elm::CustomDrawer>([=](tsl::gfx::Renderer *renderer, u16 x, u16 y, u16 w, u16 h) {
		if (this->img) {
			const u8 *bmp = this->img;
			for (s32 y1 = 0; y1 < h; y1++)
				for (s32 x1 = 0; x1 < w; x1++) {
					const tsl::gfx::Color color = {static_cast<u8>(bmp[1] >> 4), static_cast<u8>(bmp[2] >> 4), static_cast<u8>(bmp[3] >> 4), static_cast<u8>(bmp[0] >> 4)};
					renderer->setPixelBlendSrc(x + x1, y + y1, a(color));
					bmp += 4;
				}
		}
		renderer->drawString(this->appId, false, 95, 340, 25, a(0xFFFF));
		renderer->drawString(this->date, false, 95, 380, 25, a(0xFFFF));
	}));

	auto txt = std::make_shared<Text>(0, 540, tsl::cfg::FramebufferWidth, 80, "");
	main->addElement(txt);

	main->addElement(std::make_shared<Button>((tsl::cfg::FramebufferWidth - 240) / 2, 430, 240, 80, "Upload", [=](u64 key) -> bool {
		if (key & KEY_A && !this->uploaded) {
			auto url = web::UploadImage(file_id);
			txt->setText(url);
			this->uploaded = true;
			return true;
		}
		return false;
	}));

	return rootFrame;
}