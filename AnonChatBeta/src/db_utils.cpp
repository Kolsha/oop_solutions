#include "include/db_utils.h"


DbUser::DbUser(const TgBot::Attachment::User &tg_user)
{
    parse(tg_user);
}

bool DbUser::parse(const TgBot::Attachment::User &tg_user)
{
    if(!tg_user.is_parsed())
        return false;

    clear();
    if(!insert_or_update_user(tg_user))
        return false;

    return load(tg_user.id);
}

bool DbUser::load(const size_t _user_id)
{
    clear();
    const QString sql = "SELECT * FROM users WHERE user_id = :user_id LIMIT 1";
    if(!query.prepare(sql)){
        qDebug() << "Failed sql prepare; DbUser::load";
        return false;
    }
    query.bindValue(":user_id", QString("%1").arg(_user_id));

    try{
        if(!query.exec() || !query.next())
            return false;
        chat_with = query.value("chat_with").toUInt();
        first_name = query.value("first_name").toString();
        username = query.value("username").toString();
        last_msg = query.value("last_msg").toString();

        nickname = query.value("nickname").toString();
        last_msg_ts = query.value("last_msg_ts").toUInt();
        now_search = query.value("now_search").toBool();
        this->user_id = _user_id;

        parsed = true;
    }
    catch(...){

    }

    if(parsed)
        load_params();

    return parsed;
}



void DbUser::clear()
{
    user_id = 0;
    first_name.clear();
    username.clear();
    chat_with = 0;
    nickname.clear();
    last_msg.clear();
    last_msg_ts = 0;
    now_search = false;
    parsed = 0;
    params.clear();
}
void DbUser::update()
{
    const QString update_sql = "UPDATE users SET "
                               "first_name = :first_name, username = :username, "
                               "last_msg = :last_msg, last_msg_ts = :last_msg_ts, "
                               "now_search = :now_search, chat_with = :chat_with, "
                               "nickname = :nickname "
                               "WHERE user_id = :user_id LIMIT 1";
    /*QString sql = update_sql.arg(first_name).arg(username).
            arg(last_msg).arg(last_msg_ts).
            arg(now_search).arg(chat_with).arg(nickname).arg(user_id);*/

    if(!query.prepare(update_sql)){
        qDebug() << "Failed sql prepare; DbUser::update";
        return ;
    }
    query.bindValue(":first_name", first_name);
    query.bindValue(":username", username);
    query.bindValue(":last_msg", last_msg);
    query.bindValue(":last_msg_ts", QString("%1").arg(last_msg_ts));
    query.bindValue(":now_search", now_search);
    query.bindValue(":chat_with", QString("%1").arg(chat_with));
    query.bindValue(":nickname", nickname);
    query.bindValue(":user_id", QString("%1").arg(user_id));

    query.exec();

    update_params();
}

bool DbUser::insert_or_update_user(const TgBot::Attachment::User &tg_user)
{

    if(!tg_user.is_parsed())
        return false;
    const QString insert_sql = "INSERT INTO users "
                               "(user_id, first_name, username) VALUES"
                               "(:user_id, :first_name, :username)";


    const QString update_sql = "UPDATE users SET first_name = :first_name, username = :username WHERE user_id = :user_id";

    if(!query.prepare(insert_sql))
        return false;
    query.bindValue(":first_name", first_name);
    query.bindValue(":username", username);
    query.bindValue(":user_id", QString("%1").arg(user_id));


    if(!query.exec()){

        if(!query.prepare(update_sql))
            return false;
        query.bindValue(":first_name", first_name);
        query.bindValue(":username", username);
        query.bindValue(":user_id", QString("%1").arg(user_id));

        return query.exec();
    }

    return true;

}



bool DbUser::load_params()
{
    params.clear();

    const QString select_sql = "SELECT * FROM criteria WHERE user_id = :user_id";

    if(!query.prepare(select_sql))
        return false;

    query.bindValue(":user_id", QString("%1").arg(user_id));

    try{
        if(!query.exec())
            return false;

        while(query.next()){
            QString name = query.value("name").toString();

            QString value_own = query.value("value_own").toString();
            QString value_search = query.value("value_search").toString();

            params.insert(name, QPair<QString, QString>(value_own, value_search));

        }
    }
    catch(...){
        return false;
    }
    return true;
}

void DbUser::update_params()
{
    const QString insert_sql = "INSERT INTO criteria "
                               "(value_own, value_search, user_id, name) VALUES"
                               "(:value_own, :value_search, :user_id, :name)";

    const QString update_sql = "UPDATE criteria SET value_own = :value_own, "
                               "value_search = :value_search WHERE user_id = :user_id AND name = :name ";

    if(params.find("gender") == params.end())
        params["gender"];
    if(params.find("department") == params.end())
        params["department"];

    for(auto &row : params.toStdMap()){

        QString name = row.first;
        QString value_own = row.second.first;
        QString value_search = row.second.second;

        if(!query.prepare(insert_sql))
            continue;

        query.bindValue(":value_own", value_own);
        query.bindValue(":value_search", value_search);
        query.bindValue(":name", name);
        query.bindValue(":user_id", QString("%1").arg(user_id));


        if(!query.exec()){
            //qDebug() << query.lastError() << sql;
            if(!query.prepare(update_sql))
                continue;

            query.bindValue(":value_own", value_own);
            query.bindValue(":value_search", value_search);
            query.bindValue(":name", name);
            query.bindValue(":user_id", QString("%1").arg(user_id));

            query.exec();
            //qDebug() << query.lastError() << sql;
        }

    }

}

std::vector<PossiblePair> get_possible_pairs()
{
    std::vector<PossiblePair> result;
    QSqlQuery query;
    QString sql = "SELECT t1.user_id AS UID1, t2.user_id AS UID2, t1.name FROM criteria t1 "
                  "INNER JOIN criteria t2 ON t1.name = t2.name AND t2.value_search IN ('', t1.value_own) "
                  "AND t1.value_search IN ('', t2.value_own) AND t1.user_id <> t2.user_id "
                  "WHERE t1.user_id IN (SELECT user_id FROM users WHERE now_search <> 0 AND chat_with = 0) "
                  "GROUP BY MIN(t1.id, t2.id), MAX(t1.id, t2.id) ORDER BY t2.user_id %1";

    sql = sql.arg((qrand() % 1) == 1 ? "ASC" : "DESC");
    if (!query.exec(sql)) {
        qDebug() << "get_possible_pairs sql exec fail" << query.lastError() << " ;";
        return std::move(result);
    }


    size_t  uid1 = 0,
            uid2 = 0;
    QString name = "";

    while (query.next()) {
        uid1 = query.value("UID1").toUInt();
        uid2 = query.value("UID2").toUInt();
        name = query.value("name").toString();

        PossiblePair pair;// = {uid1, uid2, name}; // brace doent work
        pair.name = name;
        pair.UID1 = uid1;
        pair.UID2 = uid2;

        result.push_back(pair);

        qDebug() << "UID1 is " << uid1
                 << ". UID2 is " << uid2
                 << ". name" << name;
    }

    return std::move(result);

}

std::vector<QString> get_all_names()
{
    std::vector<QString> result;
    QSqlQuery query;
    QString sql = "SELECT name FROM criteria GROUP BY name";
    if (query.exec(sql)) {
        qDebug() << "get_possible_pairs sql exec fail";
        return std::move(result);
    }
    QString name = "";
    while (query.next()) {
        name = query.value("name").toString();
        result.push_back(name);
    }

    return std::move(result);
}
