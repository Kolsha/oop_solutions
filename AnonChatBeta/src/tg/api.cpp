#include <curl/curl.h>
#include "include/tg/api.h"
#include "include/tg/attachment.h"



namespace TgBot {


const string Client::api_url = "https://api.telegram.org/bot";


Client::Client(const std::string &_token){

    token = _token;
}

std::string Client::get_token()
{
    return this->token;
}

void Client::set_token(const std::string &_token)
{
    this->clear();
    token = _token;
}

const Attachment::User &Client::get_user()
{
    return this->user;
}


bool Client::check_token(){

    json jres = call("getMe");
    try{

        bool ok = get_or_default<bool>(jres, string("ok"), false); //
        if(!ok){
            this->clear();
            return false;
        }
        json info = jres.at("result").get<json>();
        //info = info.begin().value();
        user.parse(info);
        return true;
    }
    catch(...){
        this->clear();
    }

    return false;
}


json Client::call(const string &method, const std::string &params) const{
    if(method.empty()){
        return nullptr;
    }
    string url = api_url + token + "/" + method;




    string res = request(url, params);
    if(res.empty()){
        return nullptr;
    }


    try{
        json jres = json::parse(res);
        return jres;
    }
    catch(...){

    }

    return nullptr;
}

Attachment::Message Client::send_message(size_t chatId, const std::string &text,
                                         bool disableWebPagePreview, size_t replyToMessageId,
                                         const json &replyMarkup, const std::string &parseMode,
                                         bool disableNotification) const
{
    Attachment::Message res;

    params_map params = {
        {"chat_id", std::to_string(chatId)},
        {"text", text},

    };
    if (disableWebPagePreview) {
        params.insert({"disable_web_page_preview", "true"});
    }
    if (disableNotification){
        params.insert({"disable_notification", "true"});
    }
    if (replyToMessageId) {
        params.insert({"reply_to_message_id", std::to_string(replyToMessageId)});
    }
    if (replyMarkup != nullptr) {
        params.insert({"reply_markup", replyMarkup.dump()});
    }
    if (!parseMode.empty()) {
        params.insert({"parse_mode", parseMode});
    }
    json jres = call("sendMessage", params);

    //std::cout << jres.dump() << std::endl;

    bool ok = get_or_default<bool>(jres, "ok", false);
    if(!ok)
        return std::move(res);

    res.parse(get_or_default<json>(jres, "result", nullptr));


    return std::move(res);
}

Attachment::Message Client::send_photo(size_t chatId, const std::string &photoId,
                                       const std::string &caption, size_t replyToMessageId,
                                       const json &replyMarkup, bool disableNotification) const
{
    Attachment::Message res;

    params_map params = {
        {"chat_id", std::to_string(chatId)},
        {"photo", photoId},

    };
    if (!caption.empty()) {
        params.insert({"caption", caption});
    }
    if (disableNotification){
        params.insert({"disable_notification", "true"});
    }
    if (replyToMessageId) {
        params.insert({"reply_to_message_id", std::to_string(replyToMessageId)});
    }
    if (replyMarkup != nullptr) {
        params.insert({"reply_markup", replyMarkup.dump()});
    }

    json jres = call("sendPhoto", params);

    bool ok = get_or_default<bool>(jres, "ok", false);
    if(!ok)
        return std::move(res);

    res.parse(get_or_default<json>(jres, "result", nullptr));


    return std::move(res);
}

Attachment::Message Client::send_sticker(size_t chatId, const std::string &stickerId, size_t replyToMessageId, const json &replyMarkup, bool disableNotification) const
{
    Attachment::Message res;

    params_map params = {
        {"chat_id", std::to_string(chatId)},
        {"sticker", stickerId},

    };
    if (disableNotification){
        params.insert({"disable_notification", "true"});
    }
    if (replyToMessageId) {
        params.insert({"reply_to_message_id", std::to_string(replyToMessageId)});
    }
    if (replyMarkup != nullptr) {
        params.insert({"reply_markup", replyMarkup.dump()});
    }

    json jres = call("sendSticker", params);

    bool ok = get_or_default<bool>(jres, "ok", false);
    if(!ok)
        return std::move(res);

    res.parse(get_or_default<json>(jres, "result", nullptr));


    return std::move(res);
}

Attachment::Message Client::send_voice(size_t chatId, const std::string &voiceId, const std::string &caption,
                                       size_t duration, size_t replyToMessageId, const json &replyMarkup, bool disableNotification) const
{
    Attachment::Message res;

    params_map params = {
        {"chat_id", std::to_string(chatId)},
        {"voice", voiceId},

    };
    if (!caption.empty()) {
        params.insert({"caption", caption});
    }
    if(duration){
        params.insert({"duration", std::to_string(duration)});
    }
    if (disableNotification){
        params.insert({"disable_notification", "true"});
    }
    if (replyToMessageId) {
        params.insert({"reply_to_message_id", std::to_string(replyToMessageId)});
    }
    if (replyMarkup != nullptr) {
        params.insert({"reply_markup", replyMarkup.dump()});
    }

    json jres = call("sendVoice", params);

    bool ok = get_or_default<bool>(jres, "ok", false);
    if(!ok)
        return std::move(res);

    res.parse(get_or_default<json>(jres, "result", nullptr));


    return std::move(res);
}

Attachment::Message Client::send_message(const Attachment::Message &msg)
{
    Attachment::Message res;
    if(!msg.is_parsed() || !msg.chat.is_parsed()){
        return std::move(res);
    }

    if(msg.sticker.is_parsed()){
        return send_sticker(msg.chat.id, msg.sticker.file_id, msg.flags.reply_to_message_id,
                            msg.flags.reply_markup, msg.flags.disable_notification);
    }

    if(msg.photo.size() > 0 && msg.photo.rbegin()->is_parsed()){
        return send_photo(msg.chat.id, msg.photo.rbegin()->file_id, msg.caption, msg.flags.reply_to_message_id,
                          msg.flags.reply_markup, msg.flags.disable_notification);
    }

    if(msg.voice.is_parsed()){
        return send_voice(msg.chat.id, msg.voice.file_id, msg.caption, msg.voice.duration,
                          msg.flags.reply_to_message_id, msg.flags.reply_markup, msg.flags.disable_notification);
    }

    return send_message(msg.chat.id, msg.text, msg.flags.disable_web_page_preview, msg.flags.reply_to_message_id,
                        msg.flags.reply_markup, msg.flags.parse_mode, msg.flags.disable_notification);

}

void Client::clear()
{
    this->token.clear();
    this->user.clear();

}

json Client::call(const string &method, const params_map &params) const{

    if(method.empty()){
        return nullptr;
    }

    string data;
    if(params.size()){
        data = Utils::data2str(params);
    }

    return this->call(method, data);
}









string Utils::char2hex(const char dec){
    char dig1 = (dec & 0xF0) >> 4;
    char dig2 = (dec & 0x0F);

    if (0  <= dig1 && dig1 <=  9) dig1 += 48;
    if (10 <= dig1 && dig1 <= 15) dig1 += 87;
    if (0  <= dig2 && dig2 <=  9) dig2 += 48;
    if (10 <= dig2 && dig2 <= 15) dig2 += 87;

    string r;
    r.append(&dig1, 1);
    r.append(&dig2, 1);
    return r;
}

string Utils::urlencode(const string &url){

    string escaped;
    for(const char &c : url) {
        if ( (48 <= c && c <= 57)  ||
             (65 <= c && c <= 90)  ||
             (97 <= c && c <= 122) ||
             (c =='~' || c =='!' || c =='*' || c =='(' || c ==')' || c =='\'')
             )
            escaped.append( &c, 1);
        else{
            escaped.append("%");
            escaped.append( char2hex(c) );
        }
    }

    return escaped;
}

string Utils::data2str(const params_map &data){
    string result;
    for(auto &kv:data){
        result += kv.first + "=" + urlencode(kv.second)+ "&";
    }

    return result;
}


int Utils::CURL_WRITER(char *data, size_t size, size_t nmemb, string *buffer){
    int result = 0;
    if (buffer != NULL){
        buffer->append(data, size * nmemb);
        result = size * nmemb;
    }

    return result;
}

string Client::request(const string &url, const string &data) const{
    char errorBuffer[CURL_ERROR_SIZE];

    string curl_buffer;

    CURLcode result;
    CURL *curl = curl_easy_init();

    if (curl){
        curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errorBuffer);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "VK API Client");
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Utils::CURL_WRITER);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &curl_buffer);
        result = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        if (result == CURLE_OK)
        {
            return curl_buffer;
        }
        else
        {
            return errorBuffer;
        }

        return (result == CURLE_OK) ? curl_buffer : errorBuffer;
    }
    curl_easy_cleanup(curl);

    return "";
}



}
