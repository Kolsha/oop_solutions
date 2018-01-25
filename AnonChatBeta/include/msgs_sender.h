#pragma once

#include <QCoreApplication>
#include <QDebug>
#include <QThreadPool>


#include "include/tg/api.h"
#include "QSafeQueue.h"

using namespace TgBot;



class MsgsSender : public QRunnable
{
protected:
    std::shared_ptr<QSafeQueue<Attachment::Message>> outputMsgs;
    Client *bot = nullptr;
    void run();
public:
    MsgsSender(const QString &token, std::shared_ptr<QSafeQueue<Attachment::Message>> queue);

    virtual ~MsgsSender(){
        delete bot;
    }


};
