#include <QCoreApplication>
#include <QDebug>

#include "include/chat_bot.h"

void usage(const char *app){
    std::cout << "./" << app << " token [sender_count]" << std::endl;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    size_t sender_count = 5;
    QString token = "";
    if(argc < 2){
        usage(a.applicationName().toStdString().c_str());
        return -1;
    }

    token = argv[1];

    if(argc > 2){
        sender_count = QString(argv[2]).toUInt();
    }



    ChatBot chat_bot(token, sender_count);

    if(!chat_bot.init()){
        qDebug() << "Failed init";
        return -2;
    }

    chat_bot.start();

    qDebug() << "started";
    return a.exec();
}
