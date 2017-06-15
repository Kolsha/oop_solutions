#include <iostream>

using namespace std;

#include <stdio.h>
#include <string.h>

#include <curl/curl.h>

#include "curl_downloader.h"

/* <DESC>
 * Checks a single file's size and mtime from an FTP server.
 * </DESC>
 */


void info_download(const DownloadInfo &info){
    cout << "size: " << info.total_size << endl << "url: " << info.url << endl;
    cout << "down: " << info.downloaded_size << endl;
    if(info.state == DS_OK){
        cout << "Download complete" << endl;
    }
    else if(info.state != DS_PROCESS){
        cout << "Error" << endl;
    }
}


int main(void)
{

//    std::unordered_map<string, string> params = {{"1", "test"}, {"2", "hello"}, {"3", "world"}};
//    cout << apply_mask("3 2 1", params) << endl;
//    return 1;
    size_t count = 5;
    CurlDownloader test(count, info_download);
    for(size_t i = 0; i < count; i++){
        test.add_download("http://download.qt.io/official_releases/online_installers/qt-unified-linux-x64-online.run?" + to_string(i),
                          "/home/kolsha/a.b" + to_string(i));
    }
    std::this_thread::sleep_for(std::chrono::seconds(2));
    while(test.is_work_now()){

    }
    return 1;

}
