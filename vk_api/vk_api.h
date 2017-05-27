#ifndef VK_API_H
#define VK_API_H
#include <string>
#include <unordered_map>
#include <vector>

#include "json.hpp"

namespace VK{

using callback_func = std::string(*)(const std::string);

using json = nlohmann::json;

class DataModel{
public:
    DataModel parse();
};

class DialogInfo: public DataModel{
public:
    int chat_id;
    std::string title;
    static DialogInfo parse(json jres);
};








class API
{
private:

    static const std::string app_id;
    static const std::string app_secret;
    static const std::string scope;

    static const std::string auth_url;

    std::unordered_map<std::string, std::string> tmp_params;

    std::string lang;
    std::string version;

    std::string a_t;
    size_t user_id;

    std::string captcha_sid;
    std::string captcha_key;
    callback_func captcha_callback;
    bool check_access();
    std::string get_captcha_key(const std::string captcha_sid){
        if(captcha_callback != nullptr){
            return captcha_callback(captcha_sid);
        }
        return "";
    }

public:
    static const std::string api_url;
    API(const callback_func callback = nullptr,
        const std::string version = "5.64", const std::string lang = "ru");

    std::string access_token(){
        return a_t;
    }

    bool auth(const std::string login, const std::string pass,
              const std::string access_token = "");

    static std::string request(const std::string url, const std::string data);

    json call(const std::string method, std::unordered_map<std::string, std::string>& params);

    void clear(){
        a_t.clear();
        user_id = 0;
    }

    /* Added methods */

    /* count = 0 : get all dialogs; */
    std::vector< DialogInfo > get_dialogs(size_t offset = 0,
                                         size_t count = 200);

};

class Utils{
public:
    static std::string data2str(const std::unordered_map<std::string, std::string>);
    static std::string urlencode(const std::string &c);
    static std::string char2hex(const char dec);
    static int CURL_WRITER(char *data, size_t size, size_t nmemb, std::string *buffer);
};


}



#endif // VK_API_H
