#include "gui_main.hpp"
#include "gui_error.hpp"
#include "gui_upload.hpp"

#include "elm_button.hpp"

#include <string>

constexpr Result ResultNoScreenshotTaken = MAKERESULT(420,69);
constexpr Result ResultBadImage = MAKERESULT(420,70);
constexpr Result ResultOOM = MAKERESULT(420,71);

constexpr size_t thumbWidth = 320, thumbHeight = 180;
constexpr size_t jpgSize = 1024*1024;
constexpr size_t imgSize = thumbWidth*thumbHeight*4;

GuiMain::GuiMain() { }
GuiMain::~GuiMain() {
    if (this->img) {
        free(this->img);
        this->img = nullptr;
    }
}

tsl::Element* GuiMain::createUI() {
    tsl::element::Frame *rootFrame = new tsl::element::Frame();
    tsl::element::CustomDrawer* imgElm = nullptr;
    CapsOverlayThumbnailData data = {0};

    if (!img)
        img = (u8*)malloc(imgSize);

    if (!img) {
        tsl::Gui::changeTo(new ErrorGui(0, "Out of memory!"));
        return rootFrame;
    }

    Result rc = capsaGetLastOverlayScreenShotThumbnail(&data, this->img, imgSize);
    if (R_FAILED(rc) && data.size > 0 && data.file_id.application_id != 0) {
        tsl::Gui::changeTo(new ErrorGui(rc, "No screenshot taken!"));
        free(this->img);
        this->img = nullptr;
        return rootFrame;
    }

    u64 w, h;
    u8* jpg = (u8*)malloc(jpgSize);
    if (!jpg) {
        tsl::Gui::changeTo(new ErrorGui(0, "Out of memory!"));
        free(this->img);
        this->img = nullptr;
        return rootFrame;
    }
    
    rc = capsaLoadAlbumScreenShotThumbnailImage(&w, &h, &data.file_id, img, imgSize, jpg, jpgSize);
    free(jpg);

    if (R_FAILED(rc) || w != thumbWidth || h != thumbHeight) {
        tsl::Gui::changeTo(new ErrorGui(rc, "CapSrv error!"));
        free(this->img);
        this->img = nullptr;
        return rootFrame;
    }

    imgElm = new tsl::element::CustomDrawer(0,0,FB_WIDTH,FB_HEIGHT,[=](u16 x, u16 y, tsl::Screen *screen) {
        if (this->img)
            screen->drawRGBA8Image((FB_WIDTH - thumbWidth)/2,40,thumbWidth,thumbHeight,this->img);

        char appId[0x11] = {};
        snprintf(appId, 0x11, "%016lX", data.file_id.application_id);
        screen->drawString(appId, false, 95, 300, 25, tsl::a(0xFFFF));
        char date[0x20];
        std::snprintf(date, 0x20, "%04d:%02d:%02d %02d:%02d:%02d",
            data.file_id.datetime.year,
            data.file_id.datetime.month,
            data.file_id.datetime.day,
            data.file_id.datetime.hour,
            data.file_id.datetime.minute,
            data.file_id.datetime.second
        );
        screen->drawString(date, false, 95, 340, 25, tsl::a(0xFFFF));
    });

    rootFrame->addElement(imgElm);

    auto btn = new Button((FB_WIDTH-240)/2, 380, 240, 80, "Upload", [=](s64 key) {
        if (key & KEY_A) {
            free(this->img);
            this->img = nullptr;
            tsl::Gui::changeTo(new UploadGui(data.file_id));
            return true;
        }
        return false;
    });
    rootFrame->addElement(btn);

    return rootFrame;
}