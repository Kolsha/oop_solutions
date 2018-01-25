#pragma once

#include "api.h"
#include "attachment.h"



namespace TgBot {





class LongPollBot : public Client
{
protected:
    size_t last_update_id = 0;
public:
    LongPollBot(const string &_token) : Client(_token){}
    std::vector<Attachment::Message> get_messages();
    virtual ~LongPollBot() {}
};


}


