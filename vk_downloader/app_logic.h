#ifndef APP_LOGIC_H
#define APP_LOGIC_H

#include <iostream>
#include "../vk/src/messages.h"
#include "../downloader/curl_downloader.h"
#include "../oop_solutions/readConfiguration/read_conf.h"


class AppLogic
{
private:
    static const std::string help_str;
    static const std::string ini_demo;
    static void callback_download(const DownloadInfo &info);

    VK::Messages vk;
    CurlDownloader *downloader;
    std::map<std::string, std::string> cfg;
    std::string time2string(const time_t time);
    void read_cfg();

    template<typename T> void save_attacments(int chat_id,
                                              const std::string &chat_title = "",
                                              const size_t count = 0);

    std::unordered_map<std::string, std::string> default_mask(const VK::Attachment::BaseAttachment *att,
                                                    const std::string &chat_title);

    bool save_dialogs_to_file(const std::string &filename, const size_t count);

    bool save_attacments_from_dialogs(const std::map<std::string, std::string> &dialogs,
                                      const int att_type);

public:
    AppLogic(const size_t thread_count = 20);
    int run(const std::string &login, const std::string &pass,
            const std::string &access_token = "");

    void wait_for_all_downloaded(){
        while(downloader){
            bool work_now = downloader->is_work_now();
            if(!work_now){
                break;
            }
            system("clear");
            std::cout << "Wait for download" << std::endl;
        }
    }

    ~AppLogic(){
        delete downloader;
    }
};


#endif // APP_LOGIC_H
