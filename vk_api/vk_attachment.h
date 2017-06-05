#ifndef VK_ATTACHMENT_H
#define VK_ATTACHMENT_H

#include "vk_api.h"

namespace VK {

namespace Attachment {


class BaseAttachment : public DataModel
{
protected:
    std::string parsed_type;
    bool parse_type(const json &data);
    bool parse_common(const json &data);
public:
    int id;
    int owner_id;
    size_t date;
    std::string direct_url;
};

class Audio : public BaseAttachment
{
public:
    static const std::string type;
    std::string artist;
    std::string title;
    size_t duration;
    bool parse(const json &data);
    std::string dump(){
        return artist + " - " + title + " : " + std::to_string(duration);
    }

};

class Photo : public BaseAttachment
{
public:
    static const std::string type;
    std::string text;
    bool parse(const json &data);
    std::string dump(){
        return text + " - " + direct_url + " : " + std::to_string(date);
    }

};

class Document : public BaseAttachment
{
public:
    static const std::string type;
    std::string title;
    std::string ext;
    int doc_type; /*
        Тип документа. Возможные значения:
        1 — текстовые документы;
        2 — архивы;
        3 — gif;
        4 — изображения;
        5 — аудио;
        6 — видео;
        7 — электронные книги;
        8 — неизвестно.
    */
    size_t size; /* in byte */

    bool parse(const json &data);
    std::string dump(){
        return title + " - " + ext + " : " + std::to_string(size);
    }

};

}

}
#endif // VK_ATTACHMENT_H
