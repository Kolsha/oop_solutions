#include "include/msgs_sender.h"


MsgsSender::MsgsSender(const QString &token, std::shared_ptr<QSafeQueue<Attachment::Message>> queue)
{
    bot = new Client(token.toStdString());
    outputMsgs = queue;
}

void MsgsSender::run()
{

    for(;;){
        Attachment::Message msg;
        if(!outputMsgs->dequeue(msg)){
            QThread::usleep(qrand() % 100);
            continue;
        }
        bot->send_message(msg);
    }
}
