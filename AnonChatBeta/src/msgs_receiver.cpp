#include "include/msgs_receiver.h"

MsgsReceiver::MsgsReceiver(const QString &token, std::shared_ptr<QSafeQueue<Attachment::Message>> queue)
{
    bot = new LongPollBot(token.toStdString());
    inputMsgs = queue;
}

void MsgsReceiver::run()
{
    for(;;){
        auto msgs = bot->get_messages();
        for(auto &msg : msgs){
            if(msg.chat.type != Attachment::ct_private)
                continue;
            qDebug() << msg.dump().c_str();
            //api.send_message(msg, false, 0, keys.serialize());
            //qDebug() << (msg.dump().c_str());
            inputMsgs->enqueue(std::move(msg));
            //outputMsgs.enqueue(std::move(msg));

        }

        QThread::usleep(qrand() % 100);

    }
}
