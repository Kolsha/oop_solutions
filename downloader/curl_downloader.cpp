#include <curl/curl.h>
#include <sys/stat.h>
#include "curl_downloader.h"

using std::string;
using std::cout;
using std::endl;


namespace{

struct DownloadProgress{
    CurlDownloader *downloader;
    DownloadInfo *info;
    FILE *stream;
};

size_t write_data(void *ptr, size_t size, size_t nmemb, DownloadProgress *progress)
{
    size_t written = 0;

    if(progress == nullptr){
        return written;
    }

    if(progress->stream != nullptr){
        written = fwrite(ptr, size, nmemb, progress->stream);
    }

    if(progress->downloader != nullptr
            && progress->info != nullptr){

        progress->info->downloaded_size += written;
        double dsize = 100;
        if(progress->info->total_size > 0){
            dsize = written;
            dsize /= progress->info->total_size;
            dsize *= 100;
        }
        if (dsize >= DELTA_SIZE_TO_INFORM){
            progress->downloader->update_download(*progress->info);
        }

    }

    return written;
}

bool download_file(DownloadProgress &progress)
{

    if(progress.info->url.empty() || progress.info->path.empty()){
        return false;
    }

    CURL *curl;
    FILE *fp;
    CURLcode res = CURLE_FAILED_INIT;

    curl = curl_easy_init();
    if (curl)
    {
        fp = fopen(progress.info->path.c_str(),"wb");

        try{
            if(fp != NULL){
                progress.stream = fp;
                curl_easy_setopt(curl, CURLOPT_URL, progress.info->url.c_str());
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, &progress);
                curl_easy_setopt (curl, CURLOPT_VERBOSE, 0L);
                curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
                res = curl_easy_perform(curl);
                fclose(fp);
            }
            curl_easy_cleanup(curl);

            return (CURLE_OK == res);
        }
        catch(...){
            curl_easy_cleanup(curl);
            fclose(fp);
        }
    }
    return false;
}

static size_t throw_away(void *ptr, size_t size, size_t nmemb, void *data)
{
    (void)ptr;
    (void)data;
    return (size_t)(size * nmemb);
}


size_t http_filesize(const string &url){
    CURL *curl;
    CURLcode res;
    double filesize = 0;

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
        curl_easy_setopt(curl, CURLOPT_FILETIME, 1L);
        curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, throw_away);
        curl_easy_setopt(curl, CURLOPT_HEADER, 0L);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        res = curl_easy_perform(curl);

        if(CURLE_OK == res) {

            res = curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD,
                                    &filesize);

            if((CURLE_OK == res) && (filesize > 0)){
                return size_t(filesize);
            }

        }
        else {
            //error
        }


        curl_easy_cleanup(curl);
    }
    return 0;
}


}

void CurlDownloader::update_download(const DownloadInfo &info){
    std::unique_lock<std::mutex> locker1(g_lock_in_process);
    call_callback_download(info);
    switch (info.state) {
    case DS_OK:
    case DS_ERROR:
    case DS_FILTER:
    {
        std::unique_lock<std::mutex> locker2(g_lock_downloaded);
        downloaded[info.url] = info;
        in_process.erase(info.url);
    }
        break;
    default:
        in_process[info.url] = info;
        break;
    }

}


void CurlDownloader::execute(){

    change_thread_count(1);
    try{
        while(!is_terminated()){
            std::this_thread::sleep_for(std::chrono::seconds(1));
            DownloadInfo info;
            if(!queue_pop(info)){
                continue;
            }
            info.total_size = http_filesize(info.url);

            if((info.max_size > 0 && info.total_size > info.max_size) ||
                    (info.min_size > 0 && info.total_size < info.min_size)){
                info.state = DS_FILTER;
                update_download(info);
                continue;
            }

            update_download(info);

            DownloadProgress progress = {this, &info, nullptr};

            if(download_file(progress)){
                info.state = DS_OK;
            }else{
                info.state = DS_ERROR;
            }

            update_download(info);
        }
    }
    catch(...){

    }

    change_thread_count(-1);
}





void CurlDownloader::change_thread_count(int count){

    std::unique_lock<std::mutex> locker(g_lock_info);

    if(count == 0){
        return ;
    }

    if(count){
        real_thread += count;
    }
    else{
        if((size_t)abs(count) >= real_thread){
            real_thread = 0;
        }
        else{
            real_thread -= count;
        }
    }
}

CurlDownloader::CurlDownloader(const size_t _thread_count, download_callback_func _callback_download)
{
    real_thread = 0;
    terminated = false;
    callback_download = _callback_download;
    if(_thread_count >= 1)
    {
        this->thread_count = _thread_count;
    }
    else{
        this->thread_count = 10;
    }

    for(size_t i = 0; i < this->thread_count; i++){
        std::thread th(&CurlDownloader::execute, this);
        th.detach();
    }


}


void CurlDownloader::add_download(const std::string &url, const std::string &path,
                                  const size_t min_size, const size_t max_size){

    if(is_terminated()){
        return ;
    }

    std::unique_lock<std::mutex> locker(g_lock_queue);
    if(url.empty() || path.empty()){
        return ;
    }

    DownloadInfo info;
    info.url = url;
    info.path = path;
    info.max_size = max_size;
    info.min_size = min_size;
    queue.insert({url, std::move(info)});

}

bool CurlDownloader::queue_pop(DownloadInfo &info){
    if(is_terminated()){
        return false;
    }
    std::unique_lock<std::mutex> locker(g_lock_queue);
    if(queue.size() < 1){
        return false;
    }

    string tmp_url =  queue.begin()->first;
    DownloadInfo tmp = queue.begin()->second;
    info = tmp;

    queue.erase(tmp_url);
    in_process.insert({tmp_url, std::move(tmp)});

    return true;
}


std::string apply_mask(const std::string &mask, const std::unordered_map<std::string, std::string> &params){
    if(mask.empty() || params.empty()){
        return "";
    }

    std::string res = mask;

    for(auto &kv:params){

        int pos = -1;
        do
        {
            pos = res.find(kv.first);
            if (pos!=-1){
                res.replace(pos, kv.first.length(), kv.second);
            }
        }
        while (pos!=-1);

    }


    return res;
}

bool mkpath(const std::string &path)
{
    bool bSuccess = false;
    int nRC = ::mkdir( path.c_str(), 0775 );
    if( nRC == -1 )
    {
        switch( errno )
        {
        case ENOENT:
            if( mkpath( path.substr(0, path.find_last_of('/')) ) )
            {
                bSuccess = 0 == ::mkdir( path.c_str(), 0775 );
            }
            else{
                bSuccess = false;
            }

            break;
        case EEXIST:
            bSuccess = true;
            break;
        default:
            bSuccess = false;
            break;
        }
    }
    else{
        bSuccess = true;
    }

    return bSuccess;
}

std::string file_dir(const std::string &filepath)
{
    int it = filepath.find_last_of("/\\");
    return filepath.substr(0, it);
}

std::string file_name(const std::string &filepath)
{
    int it = filepath.find_last_of("/\\");
    return filepath.substr(it + 1);
}

void normalize_path(std::string &path)
{
    path += (path[path.length()-1] == '/') ? "" : "/";
}

bool file_exists(const std::string &name)
{

    struct stat buffer;
    return (stat (name.c_str(), &buffer) == 0);

}
