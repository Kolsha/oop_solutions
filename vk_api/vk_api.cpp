#include <curl/curl.h>
#include "vk_api.h"

using namespace std;
using json = nlohmann::json;

const string VK::API::api_url = "https://api.vk.com/method/";
const string VK::API::app_id = "2274003";
const string VK::API::app_secret = "hHbZxrka2uZ6jB1inYsH";
const string VK::API::scope = "offline,groups,messages,friends,audio";

const string VK::API::auth_url = "https://oauth.vk.com/token?";


VK::API::API(const callback_func callback,
             const string version, const string lang){

    this->captcha_callback = callback;
    this->lang = lang;
    this->version = version;
}


bool VK::API::check_access(){
    tmp_params.clear();
    json jres = call("users.get", tmp_params);
    return (jres.find("error") == jres.end());
}

bool VK::API::auth(const std::string login, const std::string pass,
                   const std::string access_token){

    if(!access_token.empty()){
        this->a_t = access_token;
        if(check_access()){
            return true;
        }
    }

    this->a_t.clear();

    if(login.empty() || pass.empty()){
        return false;
    }

    string url = auth_url;
    unordered_map<string, string> params = {
        {"client_id", app_id},
        {"grant_type", "password"},
        {"client_secret", app_secret},
        {"scope", scope},
        {"username", login},
        {"password", pass},
        {"captcha_sid", captcha_sid},
        {"captcha_key", captcha_key}
    };

    string data = VK::Utils::data2str(params);

    string res = request(url, data);

    cout << res << endl;

    captcha_sid.clear();
    captcha_key.clear();

    try{
        json jres = json::parse(res);
        if(jres.find("error") == jres.end() ||
                jres.find("access_token") != jres.end()){

            this->a_t = jres.at("access_token").get<string>();
            this->user_id = jres.at("user_id").get<size_t>();


            return true;
        }
        else{
            string error = jres.at("error").get<string>();
            if(error == "invalid_client"){
                return false;
            }
            if(error == "invalid_request"){
                return false;
            }
            if(error == "need_captcha"){
                captcha_sid = jres.at("captcha_sid").get<string>();
                captcha_key = get_captcha_key(captcha_sid);
                if(!captcha_key.empty()){
                    return this->auth(login, pass);
                }
                captcha_sid.clear();
                captcha_key.clear();
            }

        }

        return false;
    }
    catch(...){
        cout << "Catch in auth" << endl;
        return false;
    }

    return false;
}


json VK::API::call(const string method, std::unordered_map<string, string> &params){

    if(method.empty()){
        return nullptr;
    }

    string url = api_url + method;

    params.insert({"captcha_sid", captcha_sid});
    params.insert({"captcha_key", captcha_key});
    params.insert({"v", version});
    params.insert({"lang", lang});
    if(!a_t.empty()){
        params.insert({"access_token", a_t});
    }

    string data = VK::Utils::data2str(params);

    string res = request(url, data);

    captcha_sid.clear();
    captcha_key.clear();

    try{
        json jres = json::parse(res);
        if(jres.find("error") == jres.end()){
            return jres;
        }
        else{
            string error = jres.at("error").get<string>();
            if(error == "need_captcha"){
                captcha_sid = jres.at("captcha_sid").get<string>();
                captcha_key = get_captcha_key(captcha_sid);
                if(!captcha_key.empty()){
                    return this->call(method, params);
                }
                captcha_sid.clear();
                captcha_key.clear();
            }

        }
    }
    catch(...){

    }

    return nullptr;
}


std::vector< VK::DialogInfo > VK::API::get_dialogs(size_t offset,
                                     size_t count){
    std::vector< VK::DialogInfo > res;
    bool load_all = true;
    if(count == 0){
        count = 200;
    }
    tmp_params.clear();

    json jres = call("users.get", tmp_params);
    //return (jres.find("error") == jres.end());
}








string VK::Utils::char2hex(const char dec){
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

string VK::Utils::urlencode(const string &c){

    string escaped;
    for(size_t i = 0; i < c.length(); i++){
        if ( (48 <= c[i] && c[i] <= 57) ||
             (65 <= c[i] && c[i] <= 90) ||
             (97 <= c[i] && c[i] <= 122) ||
             (c[i]=='~' || c[i]=='!' || c[i]=='*' || c[i]=='(' || c[i]==')' || c[i]=='\'')
             )
            escaped.append( &c[i], 1);
        else{
            escaped.append("%");
            escaped.append( char2hex(c[i]) );
        }
    }

    return escaped;
}

string VK::Utils::data2str(const unordered_map<string, string> data){
    string result;
    unordered_map<string, string>::iterator curr, end;
    /*for(curr = data.begin(), end = data.end(); curr != end; curr++){
        result += curr->first + "=" + urlencode(curr->second)+ "&";
    }*/
    for(auto &kv:data){
        result += kv.first + "=" + urlencode(kv.second)+ "&";
    }
    return result;
}


int VK::Utils::CURL_WRITER(char *data, size_t size, size_t nmemb, string *buffer){
    int result = 0;
    if (buffer != NULL){
        buffer->append(data, size * nmemb);
        result = size * nmemb;
    }
    return result;
}

string VK::API::request(const string url, const string data){
    static char errorBuffer[CURL_ERROR_SIZE];
    static string buffer;

    CURL *curl;
    CURLcode result;
    curl = curl_easy_init();
    if (curl){
        curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errorBuffer);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "VK API Client");
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, VK::Utils::CURL_WRITER);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
        result = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        if (result == CURLE_OK)
        {
            return buffer;
        }
        else
        {
            return errorBuffer;
        }
    }

    curl_easy_cleanup(curl);
    return "";
}


VK::DialogInfo VK::DialogInfo::parse(json jres){
    VK::DialogInfo info;

    info.chat_id = - 1;
    info.title = "test";

    return info;
}
