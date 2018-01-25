#include "include/tg/attachment.h"





template<typename T>
T get_or_default(const nlohmann::json &data, const std::string &field, const T def)
{
    if(data.find(field) == data.end()){
        return def;
    }

    return data.at(field).get<T>();
}






namespace TgBot {


using ::std::string;





namespace Attachment {

bool User::parse(const json &data){
    if(data == nullptr){
        return false;
    }

    this->clear();

    try{

        get_or_default<bool>(data, "ok", false); // this need to remove
        this->first_name = get_or_default<string>(data, "first_name", "");
        this->last_name = get_or_default<string>(data, "last_name", "");
        this->username = get_or_default<string>(data, "username", "");
        this->id = get_or_default<size_t>(data, "id", 0);
        parsed = true;

    }
    catch(...){

    }

    return parsed;

}

void User::clear()
{
    this->first_name.clear();
    this->last_name.clear();
    this->username.clear();
    this->id = 0;
    parsed = false;
}

string User::dump() const
{
    return std::to_string(id) + " - " + first_name + " " + last_name + "(@" + username + ")";
}

bool User::is_parsed() const
{
    return (id > 0 && !first_name.empty());
}



bool Message::parse(const json &data)
{
    if(data == nullptr){
        return false;
    }
    this->clear();

    //std::cout << data.dump() << std::endl;
    try{


        this->text = get_or_default<string>(data, "text", "");
        this->caption = get_or_default<string>(data, "caption", "");

        this->message_id = get_or_default<size_t>(data, "message_id", 0);
        this->date = get_or_default<size_t>(data, "date", 0);
        this->edit_date = get_or_default<size_t>(data, "edit_date", 0);

        this->chat.parse(get_or_default<json>(data, "chat", nullptr));

        this->from.parse(get_or_default<json>(data, "from", nullptr));

        this->sticker.parse(get_or_default<json>(data, "sticker", nullptr));

        this->voice.parse(get_or_default<json>(data, "voice", nullptr));

        json phts = get_or_default<json>(data, "photo", nullptr);
        if(phts != nullptr){
            for (json::iterator it = phts.begin(); it != phts.end(); ++it) {
                json item = it.value();

                PhotoSize pht;
                if(pht.parse(item)){
                    photo.push_back(std::move(pht));
                }
            }
        }


        parsed = (!text.empty() || sticker.is_parsed() || photo.size() > 0 || voice.is_parsed()) &&
                (from.is_parsed() && chat.is_parsed());

    }
    catch(...){

    }

    return parsed;
}

void Message::clear()
{
    parsed = false;
    this->caption.clear();
    this->chat.clear();
    this->date = 0;
    this->edit_date = 0;
    this->from.clear();
    this->message_id = 0;
    this->photo.clear();
    this->sticker.clear();
    this->text.clear();

}

std::string Message::dump() const
{
    string res;
    if(!text.empty()){
        res = text;
    }
    else if(voice.is_parsed()){
        res = voice.dump();
    }
    else if(sticker.is_parsed()){
        res = sticker.is_parsed();
    }

    if(photo.size()){
        res += " " + photo.rbegin()->dump();
    }
    res += " from " + chat.dump() + "; User: " + from.dump();

    return res;
}

bool Message::is_parsed() const
{
    return (message_id > 0 && chat.is_parsed());
}

bool Chat::parse(const json &data)
{
    if(data == nullptr){
        return false;
    }
    this->clear();
    try{


        this->first_name = get_or_default<string>(data, "first_name", "");
        this->last_name = get_or_default<string>(data, "last_name", "");
        this->username = get_or_default<string>(data, "username", "");
        this->username = get_or_default<string>(data, "title", "");
        this->id = get_or_default<size_t>(data, "id", 0);

        string tp = get_or_default<string>(data, "type", "");
        if(tp == "private"){
            this->type = ct_private;
        }else{
            this->type = ct_none;
        }
        parsed = true;

    }
    catch(...){

    }

    return parsed;
}

void Chat::clear()
{
    this->first_name.clear();
    this->last_name.clear();
    this->username.clear();
    this->id = 0;
    this->title.clear();
    this->type = ct_none;
    DataModel::parsed = false;
}

std::string Chat::dump() const
{
    return "Chat " + std::to_string(id) + " with " + first_name + " " + last_name;
}

bool Chat::is_parsed() const
{
    return id > 0;
}




bool BaseFile::parse_common(const json &data)
{
    if(data == nullptr){
        return false;
    }
    this->clear();
    try{


        this->file_id = get_or_default<string>(data, "file_id", "");
        this->file_size = get_or_default<size_t>(data, "file_size", 0);

        return !file_id.empty();
    }
    catch(...){

    }

    return false;
}

void BaseFile::clear()
{
    this->file_id.clear();
    this->file_size = 0;
}

std::string BaseFile::dump() const
{
    return "file " + file_id + " size " + std::to_string(file_size);
}

bool BaseFile::is_parsed() const
{
    return !file_id.empty();
}








bool PhotoSize::parse(const json &data)
{
    if(data == nullptr){
        return false;
    }
    this->clear();
    try{

        if(!parse_common(data)){
            return false;
        }
        this->width = get_or_default<size_t>(data, "width", 0);
        this->height = get_or_default<size_t>(data, "height", 0);
        parsed = true;
    }
    catch(...){

    }

    return parsed;
}

void PhotoSize::clear()
{
    BaseFile::clear();
    this->width = 0;
    this->height = 0;
}

std::string PhotoSize::dump() const
{
    return BaseFile::dump() + "; PhotoSize " + std::to_string(height) + "X" + std::to_string(width);
}





bool Sticker::parse(const json &data)
{
    if(data == nullptr){
        return false;
    }
    this->clear();
    try{

        if(!parse_common(data)){
            return false;
        }
        this->width = get_or_default<size_t>(data, "width", 0);
        this->height = get_or_default<size_t>(data, "height", 0);
        this->emoji = get_or_default<string>(data, "emoji", "");
        this->set_name = get_or_default<string>(data, "set_name", "");
        this->thumb.parse(get_or_default<json>(data, "thumb", nullptr));

        parsed = true;
    }
    catch(...){

    }

    return parsed;
}

void Sticker::clear()
{
    this->width = 0;
    this->height = 0;
    this->emoji.clear();
    this->set_name.clear();
    this->thumb.clear();
}

std::string Sticker::dump() const
{
    return BaseFile::dump() + "; Sticker " +emoji + " from " + set_name;
}


bool Voice::parse(const json &data)
{
    if(data == nullptr){
        return false;
    }
    this->clear();
    try{

        if(!parse_common(data)){
            return false;
        }
        this->duration = get_or_default<size_t>(data, "duration", 0);
        this->mime_type = get_or_default<string>(data, "mime_type", "");

        parsed = true;
    }
    catch(...){

    }

    return parsed;
}

void Voice::clear()
{
    duration = 0;
    mime_type.clear();
    parsed = false;
}

std::string Voice::dump() const
{
    return BaseFile::dump() + "; voice duration " + std::to_string(duration);
}








KeyboardButton::KeyboardButton(const std::string _text)
{
    text = _text;
}

json KeyboardButton::serialize()
{
    return json({{"text", text}});
}



ReplyKeyboardMarkup::ReplyKeyboardMarkup(std::initializer_list<std::vector<KeyboardButton> > v)
{
    keyboard = v;
}

json ReplyKeyboardMarkup::serialize()
{
    json result;

    if(selective)
        result["selective"] = selective;

    if(remove_keyboard){
        result["remove_keyboard"] = remove_keyboard;
        return std::move(result);
    }

    if(resize_keyboard)
        result["resize_keyboard"] = resize_keyboard;

    if(one_time_keyboard)
        result["one_time_keyboard"] = one_time_keyboard;

    result["keyboard"] = json::array();

    for (std::vector<KeyboardButton> &item : keyboard) {
        json arr = json::array();
        for (KeyboardButton &innerItem : item) {
            arr.push_back(innerItem.serialize());
        }
        result["keyboard"].push_back(std::move(arr));
    }

    return std::move(result);
}

void ReplyKeyboardMarkup::resize_vector(const std::vector<size_t> &each_row)
{
    keyboard.resize(each_row.size());
    for(size_t i = 0; i < each_row.size(); i++)
        keyboard[i].resize(each_row[i]);
}










}

}
