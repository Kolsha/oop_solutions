#pragma once

#include <QCoreApplication>
#include <QDebug>
#include <QThreadPool>


#include "include/tg/longpollbot.h"
#include "QSafeQueue.h"

using namespace TgBot;



class MsgsReceiver : public QRunnable
{
protected:
    LongPollBot *bot = nullptr;
    std::shared_ptr<QSafeQueue<Attachment::Message>> inputMsgs;
    void run();
public:
    MsgsReceiver(const QString &token, std::shared_ptr<QSafeQueue<Attachment::Message>> queue);

    virtual ~MsgsReceiver(){
        delete bot;
    }




};
