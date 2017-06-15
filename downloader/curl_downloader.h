#ifndef CURL_DOWNLOADER_H
#define CURL_DOWNLOADER_H

#define DELTA_SIZE_TO_INFORM 30.0

#include <iostream>
#include <thread>
#include <mutex>
#include <unordered_map>


enum DownloadState { DS_OK, DS_ERROR, DS_FILTER, DS_PROCESS};

struct DownloadInfo{
    std::string url = "";
    std::string path = "";
    size_t min_size = 0;
    size_t max_size = 0;
    size_t downloaded_size = 0;
    size_t total_size= 0;
    DownloadState state = DS_PROCESS;

};

using download_callback_func = void(*)(const DownloadInfo &info);


/* Class to multi threading download files by url
 * consntructor thread_count and callback
 * function to inform about download change
 */

class CurlDownloader
{
private:
    bool terminated = false;
    size_t thread_count;
    size_t real_thread;

    std::mutex g_lock_queue;
    std::mutex g_lock_in_process;
    std::mutex g_lock_downloaded;
    std::mutex g_lock_info;
    std::unordered_map < std::string, DownloadInfo> downloaded, queue, in_process;

    download_callback_func callback_download = nullptr;

protected:
    void call_callback_download(const DownloadInfo &info){
        if(callback_download != nullptr){
            callback_download(info);
        }
    }

    void change_thread_count(int count);

    bool queue_pop(DownloadInfo &info);
    void execute();

public:
    /* used once, dont use it, if you don't understand */
    void update_download(const DownloadInfo &info);


    CurlDownloader(const size_t _thread_count = 10,
                   download_callback_func _callback_download = nullptr);

    /* add download in queue */
    void add_download(const std::string &url, const std::string &path,
                      const size_t min_size = 0, const size_t max_size = 0);

    /* terminate all threads
     * may take a time
     */
    void terminate(){
        std::unique_lock<std::mutex> locker(g_lock_info);
        terminated = true;
    }

    bool is_terminated(){
        std::unique_lock<std::mutex> locker(g_lock_info);
        return terminated;
    }

    /* */
    bool is_work_now(){
        std::unique_lock<std::mutex> locker1(g_lock_in_process);
        std::unique_lock<std::mutex> locker2(g_lock_queue);
        return (in_process.size() + queue.size()) > 0;
    }

    void clear_downloaded(){
        std::unique_lock<std::mutex> locker(g_lock_downloaded);
        downloaded.clear();
    }

    ~CurlDownloader(){

        while(real_thread){
            terminate();
        }
    }

};


/* e.g mask "Hello %test%!" and params {"%test", "world"}
 * return "Hello world!"
 */
std::string apply_mask(const std::string &mask, const std::unordered_map<std::string, std::string> &params);


/* creates dirs by path, e.g.: "/home/test/kolsha/ab/cd/ef"
 */
bool mkpath(const std::string &path);


/* return file dir. e.g.: "home/test/abcd.ef"
 * return "home/test/";
 */
std::string file_dir(const std::string &filepath);

/* return file name. e.g.: "home/test/abcd.ef"
 * return "abcd.ef";
 */
std::string file_name(const std::string &filepath);

/* add backslash to end, if need */
void normalize_path(std::string &path);

/* check file exists */
bool file_exists(const std::string &name);

#endif // CURL_DOWNLOADER_H
