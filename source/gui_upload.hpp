#pragma once
#include <tesla.hpp>

//#include "elm_text.hpp"
#include <sys/select.h>
#include <curl/curl.h>

class UploadGui : public tsl::Gui {
private:
    CapsAlbumFileId fileId;
public:
    UploadGui(const CapsAlbumFileId &file_id);
    ~UploadGui();

    virtual tsl::Element* createUI();
};