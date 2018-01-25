#pragma once

#include <QObject>
#include <QtSql>

#include "tg/api.h"
#include "QSafeQueue.h"
#include "msgs_handler.h"


using namespace TgBot;

class ChatBot : public QObject
{
    Q_OBJECT
protected:

    Client bot;
    QString token;
    size_t sender_count = 5;
    message_queue_ptr inputMsgs,outputMsgs;
    QSqlDatabase dbase;
    QThreadPool pool;
    bool initialized = false;
    bool init_database();
public:
    explicit ChatBot(const QString &_token,
                     const size_t _sender_count = 5,
                     QObject *parent = 0);

    bool init(const QString _token = "");

    void start();

};
