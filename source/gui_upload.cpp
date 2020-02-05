#include "gui_upload.hpp"

size_t StringWrite(const char *contents, size_t size, size_t nmemb, std::string *userp) {
	userp->append(contents, size * nmemb);
	return size * nmemb;
}

UploadGui::UploadGui(const CapsAlbumFileId &file_id) : fileId(file_id) { }
UploadGui::~UploadGui() { }

tsl::Element* UploadGui::createUI() {
    auto root = new tsl::element::Frame();
    this->setTitle("Uploading");

    //Text *text_view = new Text(100, 380, 240, 80, "");
    //root->addElement(text_view);

    CURL *curl = curl_easy_init();
    if (!curl) {
        this->setTitle("failed to start curl");
        return root;
    }
    curl_mime* mime = curl_mime_init(curl);
    curl_mimepart* file_part = curl_mime_addpart(mime);

    curl_mime_name(file_part, "fileToUpload");
    void* imgBuffer = malloc(40*1024);
    if (!imgBuffer) {
        curl_easy_cleanup(curl);
        curl_mime_free(mime);
        this->setTitle("malloc failed");
        return root;
    }

    u64 size;
    Result rc = capsaLoadAlbumFileThumbnail(&this->fileId, &size, imgBuffer, 40*1024);
    if (R_FAILED(rc)) {
        free(imgBuffer);
        curl_easy_cleanup(curl);
        curl_mime_free(mime);
        this->setTitle("failed to load img");
        return root;
    }
    curl_mime_data(file_part, (const char*)imgBuffer, size);

    curl_mimepart* part = curl_mime_addpart(mime);
    curl_mime_name(part, "curl");
    curl_mime_data(part, "1", CURL_ZERO_TERMINATED);

    std::string urlresponse;

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, StringWrite);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&urlresponse);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);
    curl_easy_setopt(curl, CURLOPT_URL, "https://lewd.pics/p/index.php");
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);

    CURLcode res = curl_easy_perform(curl);
    long http_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

    if (res != CURLE_OK)
        this->setTitle("curl failed");
    else
        //this->setTitle(std::to_string(http_code));
        this->setTitle(urlresponse);

    free(imgBuffer);

    curl_easy_cleanup(curl);
    curl_mime_free(mime);
    
    return root;
}