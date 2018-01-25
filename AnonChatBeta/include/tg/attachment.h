#pragma once

#include "include/json.hpp"




template<typename T>
T get_or_default(const nlohmann::json &data, const std::string &field, const T def);



namespace TgBot {

using json = nlohmann::json;
using ::std::string;






namespace Attachment {





/* class for store data
 */
class DataModel{
protected:
    bool parsed = false;
public:
    /* parse json and fill self fileds
     * if all is ok returned true
     */
    virtual bool parse(const json &data) = 0;

    /* return class fields in std::string
     */
    virtual string dump() const = 0 ;

    virtual void clear() = 0;

    virtual bool is_parsed() const{
        return parsed;
    }

    virtual ~DataModel(){}
};


class BaseFile : public DataModel
{
protected:
    bool parse_common(const json &data);
public:
    string file_id;
    size_t file_size = 0;
    virtual void clear();
    virtual string dump() const;
    bool is_parsed() const;
    virtual ~BaseFile(){}
};


class PhotoSize : public BaseFile
{
public:
    size_t width = 0;
    size_t height = 0;
    bool parse(const json &data);
    void clear();
    string dump() const;
    virtual ~PhotoSize(){}
};


class Sticker : public BaseFile
{
public:
    size_t width = 0;
    size_t height = 0;
    PhotoSize thumb;
    string emoji;
    string set_name;

    bool parse(const json &data);
    void clear();
    string dump() const;
    virtual ~Sticker(){}
};



class Voice : public BaseFile
{
public:
    size_t duration = 0;
    string mime_type;
    bool parse(const json &data);
    void clear();
    string dump() const;
    virtual ~Voice(){}
};


/* User Info
 */
class User : public DataModel
{
public:

    string first_name;
    string last_name;
    string username;
    size_t id = 0;
    bool parse(const json &data);
    void clear();
    string dump() const;
    bool is_parsed() const;

    virtual ~User(){}
};


enum ChatType {ct_none, ct_private, ct_group, ct_supergroup, ct_channel};

class Chat : public DataModel
{
public:

    string first_name;
    string last_name;
    string username;
    string title;
    ChatType type = ct_none;
    size_t id = 0;
    bool parse(const json &data);
    void clear();
    string dump() const;

    bool is_parsed() const;

    virtual ~Chat(){}
};


struct SendMessageFlags{
    string parse_mode;
    bool disable_web_page_preview = false;
    bool disable_notification = false;
    size_t reply_to_message_id = 0;
    json reply_markup;
};


class Message : public DataModel
{
protected:
public:
    size_t message_id = 0;
    User from;
    size_t date;
    size_t edit_date;


    string caption;
    string text;
    Chat chat;

    std::vector<PhotoSize> photo;
    Sticker sticker;
    Voice voice;

    SendMessageFlags flags;


    bool parse(const json &data);
    void clear();
    string dump() const;

    bool is_parsed() const;

//    inline void set_parsed(bool _parsed = true){
//        parsed = _parsed;
//    }

    virtual ~Message(){
        //delete flags;
    }
};


class KeyboardButton
{
public:

    KeyboardButton(const string _text = "");
    string text;
    json serialize();
    /* requset contact/location */


    //friend bool operator==(const KeyboardButton& left, const KeyboardButton& right);

//    bool operator==(const KeyboardButton &other) {
//        return text == other.text;
//    }

//    bool operator==(const string &_text) {
//        return text == _text;
//    }

    virtual ~KeyboardButton(){}
};

inline bool operator==(const KeyboardButton& lhs, const string& rhs){

    return lhs.text == rhs;
}




class ReplyKeyboardMarkup
{
public:
    ReplyKeyboardMarkup(){}
    ReplyKeyboardMarkup(std::initializer_list<std::vector<KeyboardButton>> v);

    std::vector<std::vector<KeyboardButton>> keyboard;

    /**
         * Optional. Requests clients to resize the keyboard vertically for optimal fit (e.g., make the keyboard smaller if there are just two rows of buttons). Defaults to false, in which case the custom keyboard is always of the same height as the app's standard keyboard.
         */
    bool resize_keyboard = true;

    /**
         * Optional. Requests clients to hide the keyboard as soon as it's been used. Defaults to false.
         */
    bool one_time_keyboard = false;

    /**
         * Optional. Use this parameter if you want to show the keyboard to specific users only. Targets: 1) users that are @mentioned in the text of the Message object; 2) if the bot's message is a reply (has reply_to_message_id), sender of the original message.
         * Example: A user requests to change the bot‘s language, bot replies to the request with a keyboard to select the new language. Other users in the group don’t see the keyboard.
         */
    bool selective = false;

    bool remove_keyboard = false;

    json serialize();

    void resize_vector(const std::vector<size_t> &each_row);


    virtual ~ReplyKeyboardMarkup(){}
};

} // namespace Attachment


/*todo voice */
}

