#include "gui_main.hpp"
#include "gui_error.hpp"
#include "upload.hpp"

#include "elm_button.hpp"
#include "elm_text.hpp"

#include <string>

constexpr Result ResultNoScreenshotTaken = MAKERESULT(420,69);
constexpr Result ResultBadImage = MAKERESULT(420,70);
constexpr Result ResultOOM = MAKERESULT(420,71);

constexpr size_t THUMB_WIDTH = 320, THUMB_HEIGHT = 180;
constexpr size_t JPG_SIZE = 1024*1024;
constexpr size_t IMG_SIZE = THUMB_WIDTH * THUMB_HEIGHT * 4;

GuiMain::GuiMain() { }
GuiMain::~GuiMain() {
    if (this->img) {
        free(this->img);
        this->img = nullptr;
    }
}

tsl::Element* GuiMain::createUI() {
    this->setTitle("ShareNX");
    this->setSubtitle("Behemoth, v1.0.0");

    tsl::element::Frame *rootFrame = new tsl::element::Frame();
    tsl::element::CustomDrawer* imgElm = nullptr;
    CapsOverlayThumbnailData data = {0};

    if (!this->img)
        this->img = (u8*)malloc(IMG_SIZE);

    if (!img) {
        tsl::Gui::changeTo(new ErrorGui(0, "Out of memory!"));
        return rootFrame;
    }

    Result rc = capsaGetLastOverlayScreenShotThumbnail(&data, this->img, IMG_SIZE);
    if (R_FAILED(rc) && data.size > 0 && data.file_id.application_id != 0) {
        tsl::Gui::changeTo(new ErrorGui(rc, "No screenshot taken!"));
        free(this->img);
        this->img = nullptr;
        return rootFrame;
    }

    u64 w, h;
    u8* jpg = (u8*)malloc(JPG_SIZE);
    if (!jpg) {
        tsl::Gui::changeTo(new ErrorGui(0, "Out of memory!"));
        free(this->img);
        this->img = nullptr;
        return rootFrame;
    }
    
    rc = capsaLoadAlbumScreenShotThumbnailImage(&w, &h, &data.file_id, this->img, IMG_SIZE, jpg, JPG_SIZE);
    free(jpg);

    if (R_FAILED(rc) || w != THUMB_WIDTH || h != THUMB_HEIGHT) {
        tsl::Gui::changeTo(new ErrorGui(rc, "CapSrv error!"));
        free(this->img);
        this->img = nullptr;
        return rootFrame;
    }

    snprintf(this->appId, 0x11, "%016lX", data.file_id.application_id);

    std::snprintf(this->date, 0x20, "%04d:%02d:%02d %02d:%02d:%02d",
        data.file_id.datetime.year,
        data.file_id.datetime.month,
        data.file_id.datetime.day,
        data.file_id.datetime.hour,
        data.file_id.datetime.minute,
        data.file_id.datetime.second
    );

    imgElm = new tsl::element::CustomDrawer(0, 0, FB_WIDTH, FB_HEIGHT, [=](u16 x, u16 y, tsl::Screen *screen) {
        if (this->img)
            screen->drawRGBA8Image((FB_WIDTH - THUMB_WIDTH)/2, 110, THUMB_WIDTH, THUMB_HEIGHT, this->img);
        screen->drawString(this->appId, false, 95, 340, 25, tsl::a(0xFFFF));
        screen->drawString(this->date, false, 95, 380, 25, tsl::a(0xFFFF));
    });
    rootFrame->addElement(imgElm);

    auto txt = new Text(0, 540, FB_WIDTH, 80, "");
    rootFrame->addElement(txt);

    auto btn = new Button((FB_WIDTH-240)/2, 430, 240, 80, "Upload", [=](s64 key) {
        if (key & KEY_A && !this->uploaded) {
            auto url = web::UploadImage(data.file_id);
            txt->setText(url);
            this->uploaded = true;
            return true;
        }
        return false;
    });
    rootFrame->addElement(btn);

    return rootFrame;
}