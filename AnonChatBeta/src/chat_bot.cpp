#include "include/chat_bot.h"
#include "include/msgs_sender.h"
#include "include/msgs_receiver.h"
#include "include/msgs_handler.h"
#include "include/chats_updater.h"


ChatBot::ChatBot(const QString &_token,
                 const size_t _sender_count,
                 QObject *parent) : QObject(parent), token(_token), sender_count(_sender_count)
{

    if(sender_count < 1)
        sender_count = 5;
    pool.setMaxThreadCount(30);

    inputMsgs = message_queue_ptr(new QSafeQueue<Attachment::Message>);
    outputMsgs = message_queue_ptr(new QSafeQueue<Attachment::Message>);
}

bool ChatBot::init(const QString _token)
{
    initialized = false;

    if(!_token.isEmpty())
        token = _token;

    bot.set_token(token.toStdString());

    if(!bot.check_token()){
        qDebug() << "Failed to check token";
        return false;
    }

    if(!init_database())
        return false;

    initialized = true;
    return true;
}

void ChatBot::start()
{
    if(!initialized){
        qDebug() << "initialized = false;";
        return ;
    }
    pool.start(new MsgsReceiver(token, inputMsgs));

    pool.start(new MsgsHandler(inputMsgs, outputMsgs));

    for(size_t i = 0; i < sender_count; i++)
        pool.start(new MsgsSender(token, outputMsgs));

    pool.start(new ChatsUpdater(outputMsgs));
}



bool ChatBot::init_database()
{
    const QMap<QString, QString> tables = {

        {"criteria_table",
         "CREATE TABLE `criteria` ( "
         "`id`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, "
         "`user_id`	INTEGER NOT NULL DEFAULT 0, "
         "`name`	varchar(255) NOT NULL DEFAULT \"\", "
         "`value_own`	varchar(255) NOT NULL DEFAULT \"\", "
         "`value_search`	varchar(255) NOT NULL DEFAULT \"\" ); "
        },

        {"criteria_table_index",
         "CREATE UNIQUE INDEX `user_id_name` ON `criteria` (`user_id` ,`name` )"
        },

        {"users_table",
         "CREATE TABLE `users` ( "
         "`id`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, "
         "`user_id`	INTEGER NOT NULL UNIQUE, "
         "`chat_with`	INTEGER DEFAULT 0, "
         "`now_search`	INTEGER DEFAULT 0, "
         "`last_msg`	TEXT DEFAULT \"\", "
         "`last_msg_ts`	INTEGER DEFAULT 0,"
         "`first_name`	NUMERIC DEFAULT \"No Name\", "
         "`username`	TEXT DEFAULT \"\", "
         "`nickname`	TEXT DEFAULT \"Belka\" ); "
        },

    };

    dbase = QSqlDatabase::addDatabase("QSQLITE");
    QString file_name = QString("%1.sqlite").arg(qHash(token));
    dbase.setDatabaseName(file_name);
    if (!dbase.open()) {
        qDebug() << "Failed to open bd: " << file_name;
        return false;
    }

    QSqlQuery query(dbase);


    for(auto &row : tables.toStdMap()){
        if(!query.exec(row.second)){
            QString err = query.lastError().text();
            if(!err.contains("exists", Qt::CaseInsensitive)){
                qDebug() << "Failed to create " << row.first << "; " << err;
                return false;
            }
        }
    }




    return true;
}
