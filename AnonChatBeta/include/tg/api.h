#pragma once
#include <string>
#include <unordered_map>
#include <vector>

#include "include/json.hpp"

#include "attachment.h"

namespace TgBot{

using callback_func = string(*)(const string &);

/* http params */
using params_map = std::unordered_map<string, string>;

class Client
{

private:
    string token;
    static const string api_url;
protected:
    string request(const string &url, const string &data) const;
    Attachment::User user;
public:
    explicit  Client() {}
    Client(const string &_token);

    bool check_token();

    string get_token();

    void set_token(const string &_token);

    const Attachment::User &get_user();

    json call(const string &method, const params_map &params) const;

    json call(const string &method, const string &params = "") const;



    Attachment::Message send_message(size_t chatId, const std::string& text,
                                     bool disableWebPagePreview = false, size_t replyToMessageId = 0,
                                     const json &replyMarkup = nullptr,
                                     const std::string& parseMode = "", bool disableNotification = false) const;

    Attachment::Message send_photo(size_t chatId, const std::string& photoId,
                                   const std::string& caption = "",
                                   size_t replyToMessageId = 0,
                                   const json &replyMarkup = nullptr, bool disableNotification = false) const;
    Attachment::Message send_sticker(size_t chatId, const std::string& stickerId,
                                     size_t replyToMessageId = 0,
                                     const json &replyMarkup = nullptr, bool disableNotification = false) const;

    Attachment::Message send_voice(size_t chatId, const std::string& voiceId, const std::string &caption = "",
                                   size_t duration = 0, size_t replyToMessageId = 0,
                                   const json &replyMarkup = nullptr, bool disableNotification = false) const;

    Attachment::Message send_message(const Attachment::Message &msg);

    void clear();

    virtual ~Client(){}

};


/* Network utils
 */
class Utils{
public:
    static string data2str(const params_map &data);
    static string urlencode(const string &url);
    static string char2hex(const char dec);
    static int CURL_WRITER(char *data, size_t size, size_t nmemb, string *buffer);
};


}


