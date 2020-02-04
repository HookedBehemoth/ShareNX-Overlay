#pragma once
#include <tesla.hpp>

#include "elm_text.hpp"

struct UploadThreadContext {
    UploadThreadContext(const CapsAlbumFileId &file_id) : fileId(file_id) {}
    CapsAlbumFileId fileId;
    bool finished = false;
    float progress = 0;
    std::string message;
};

class UploadGui : public tsl::Gui {
private:
    CapsAlbumFileId fileId;
    Text *text_view = nullptr;
    UploadThreadContext *ctx = nullptr;
    Thread upload_thread;
public:
    UploadGui(const CapsAlbumFileId &file_id);
    ~UploadGui();

    virtual tsl::Element* createUI();

    virtual void update();
    virtual void onDestroy();
};