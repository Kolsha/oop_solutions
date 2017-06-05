#ifndef messages_H
#define messages_H

#include "vk_api.h"

#include "vk_attachment.h"

namespace VK {

class DialogInfo: public DataModel
{
public:
    int chat_id;
    std::string body;
    std::string title;

    bool parse(const json &data);


    std::string dump(){
        return "[" + title + " | " + std::to_string(chat_id) + "] " + body;
    }

};


using vector_dialogs = std::vector< DialogInfo >;

class Messages : public Client
{

public:
    Messages();
    vector_dialogs get_dialogs( const size_t count = 20,
                                const size_t offset = 0);
    vector_dialogs get_all_dialogs();

    template<typename T> std::vector<T> get_attachments(const int chat_id, const size_t count = 0);
};

}
#endif
