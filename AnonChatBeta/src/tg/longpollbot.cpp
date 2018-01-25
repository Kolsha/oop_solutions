#include "include/tg/longpollbot.h"

namespace TgBot {



std::vector<Attachment::Message> LongPollBot::get_messages()
{

    std::vector<Attachment::Message> result;
    params_map params = {
        {"offset", std::to_string(last_update_id)},
        {"allowed_updates", "message"},
    };

    json jres = call("getUpdates", params);

    bool ok = get_or_default<bool>(jres, "ok", false);
    if(!ok)
        return std::move(result);

    //std::cout << jres.dump() << std::endl;

    try{

        json items = jres.at("result").get<json>();

        for (json::iterator it = items.begin(); it != items.end(); ++it) {
            json item = it.value();
            if(item.find("message") == item.end())
                continue;

            last_update_id = 1 + get_or_default<size_t>(item, "update_id", last_update_id);
            item = item.at("message").get<json>();

            Attachment::Message msg;
            if(msg.parse(item)){
                result.push_back(std::move(msg));
            }
        }
    }
    catch(...){

    }

    return std::move(result);

}


}
