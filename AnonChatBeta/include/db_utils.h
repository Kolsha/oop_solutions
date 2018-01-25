#pragma once

#include <QtSql>

#include "include/tg/attachment.h"


class DbUser
{
protected:
    QSqlQuery query;
    bool parsed = false;
    void load();
    bool insert_or_update_user(const TgBot::Attachment::User &tg_user);

    void update_params();
public:
    DbUser(){}
    DbUser(const TgBot::Attachment::User &tg_user);

    bool parse(const TgBot::Attachment::User &tg_user);

    bool load(const size_t _user_id);

    bool load_params();

    void clear();

    size_t user_id = 0;

    QString first_name;
    QString username;

    size_t chat_with = 0;
    QString nickname;
    QString last_msg;
    size_t last_msg_ts = 0;
    QMap<QString, QPair<QString, QString>> params;

    bool now_search = false;

    void update();
    inline bool is_parsed(){
        return (user_id > 0) && !first_name.isEmpty();
    }

};


struct PossiblePair{
   size_t UID1 = 0;
   size_t UID2 = 0;
   QString name = "";
};


std::vector<PossiblePair> get_possible_pairs();

std::vector<QString> get_all_names();
