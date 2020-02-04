#include "gui_upload.hpp"

#include <sys/select.h>
#include <curl/curl.h>

class UploadFrame : public tsl::element::Frame {
    private:
        UploadThreadContext *ctx;
    public:
        UploadFrame(UploadThreadContext *context) : Frame(), ctx(context) { }

        virtual bool onClick(s64 key) {
            if (key == KEY_B && ctx->finished) {
                tsl::Gui::closeGui();
                return true;
            }

            return false;
        }
};

static size_t StringWrite(void *contents, size_t size, size_t nmemb, void *userdata) {
	((std::string*)userdata)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

void upload_thread_func(void *user) {
    UploadThreadContext *ctx = (UploadThreadContext *)user;

    /*CURL *curl = curl_easy_init();
    if (!curl) {
        ctx->finished = true;
        ctx->message = "failed to start curl";
        return;
    }
    curl_mime* mime = curl_mime_init(curl);
    curl_mimepart* file_part = curl_mime_addpart(mime);

    curl_mime_name(file_part, "fileToUpload");
    void* imgBuffer = malloc(40*1024);
    u64 size;
    Result rc = capsaLoadAlbumFileThumbnail(&ctx->fileId, &size, imgBuffer, 40*1024);
    if (R_FAILED(rc)) {
        free(imgBuffer);
        imgBuffer = nullptr;
        curl_easy_cleanup(curl);
        ctx->finished = true;
        ctx->message = "failed to load img";
        return;
    }
    curl_mime_data(file_part, (const char*)imgBuffer, size);

    curl_mimepart* part = curl_mime_addpart(mime);
    curl_mime_name(part, "curl");
    curl_mime_data(part, "1", CURL_ZERO_TERMINATED);

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, StringWrite);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ctx->message);
    curl_easy_setopt(curl, CURLOPT_URL, "https://lewd.pics/p/index.php");
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);

    curl_easy_perform(curl);

    free(imgBuffer);
    imgBuffer = nullptr;
    curl_easy_cleanup(curl);*/

    ctx->finished = true;
}

UploadGui::UploadGui(const CapsAlbumFileId &file_id) : fileId(file_id) { }
UploadGui::~UploadGui() { }

tsl::Element* UploadGui::createUI() {
    auto root = new tsl::element::Frame();
    tsl::Gui::setTitle("Uploading");

    this->text_view = new Text(100, 380, 240, 80, "");
    root->addElement(this->text_view);

    ctx = new UploadThreadContext(this->fileId);
    
    threadCreate(&upload_thread, upload_thread_func, ctx, nullptr, 0x10000, 0x2C, 3);
    threadStart(&upload_thread);
    
    return root;
}

void UploadGui::update() {
    if (this->ctx->finished)
        text_view->setText(this->ctx->message.c_str());
}

void UploadGui::onDestroy() {
    threadWaitForExit(&upload_thread);
    threadClose(&upload_thread);
    delete this->ctx;
}