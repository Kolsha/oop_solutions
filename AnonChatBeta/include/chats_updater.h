#pragma once


#include <QCoreApplication>
#include <QDebug>
#include <QThreadPool>


#include "include/tg/api.h"
#include "QSafeQueue.h"

using namespace TgBot;

class ChatsUpdater : public QRunnable
{
protected:
    std::shared_ptr<QSafeQueue<Attachment::Message>> outputMsgs;
    void run();
public:
    ChatsUpdater(std::shared_ptr<QSafeQueue<Attachment::Message>> queue);

    virtual ~ChatsUpdater(){
    }


};
