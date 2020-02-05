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
#include "upload.hpp"

#include <sys/select.h>
#include <curl/curl.h>

namespace web {

    size_t StringWrite(const char *contents, size_t size, size_t nmemb, std::string *userp) {
        userp->append(contents, size * nmemb);
        return size * nmemb;
    }

    std::string UploadImage(const CapsAlbumFileId &fileId) {
        u64 size = 0;
        Result rc = capsaGetAlbumFileSize(&fileId, &size);
        if (R_FAILED(rc))
            return "can't get size";

        void* imgBuffer = malloc(size);
        if (!imgBuffer)
            return "malloc failed";

        u64 actualSize = 0;
        rc = capsaLoadAlbumFile(&fileId, &actualSize, imgBuffer, size);
        if (R_FAILED(rc)) {
            free(imgBuffer);
            return "failed to load img";
        }

        CURL *curl = curl_easy_init();
        if (!curl)
            return "failed to start curl";

        curl_mime* mime = curl_mime_init(curl);
        curl_mimepart* file_part = curl_mime_addpart(mime);

        curl_mime_filename(file_part, "switch.jpg");
        curl_mime_name(file_part, "fileToUpload");
        curl_mime_data(file_part, (const char*)imgBuffer, actualSize);

        curl_mimepart* part = curl_mime_addpart(mime);
        curl_mime_name(part, "curl");
        curl_mime_data(part, "1", CURL_ZERO_TERMINATED);

        std::string urlresponse = std::string();

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, StringWrite);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&urlresponse);
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
        curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);
        curl_easy_setopt(curl, CURLOPT_URL, "https://lewd.pics/p/index.php");
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);

        CURLcode res = curl_easy_perform(curl);
        long http_code = 0;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

        if (res != CURLE_OK) {
            urlresponse = "curl failed";
        } else if (http_code != 200) {
            urlresponse = "failed with " + std::to_string(http_code);
        } else if (urlresponse.size() > 0x30) {
            urlresponse = "result too long";
        }

        free(imgBuffer);

        curl_easy_cleanup(curl);
        curl_mime_free(mime);

        return urlresponse;
    }

}