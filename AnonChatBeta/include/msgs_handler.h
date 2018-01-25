#pragma once

#include <QCoreApplication>
#include <QDebug>
#include <QThreadPool>


#include "include/tg/attachment.h"
#include "QSafeQueue.h"
#include "db_utils.h"


using namespace TgBot;
using namespace Attachment;

using message_queue_ptr = std::shared_ptr<QSafeQueue<Attachment::Message>>;


enum CMD {CMD_NONE, CMD_START,
          CMD_SEARCH, CMD_STOP,
          CMD_SET_SEARCH, CMD_SET,
          CMD_MAIN_MENU,
          CMD_LEAVE,
          CMD_HELP
         };

enum MenuState {MS_MAIN, MS_SET, MS_SET_SEARCH};


class MsgsHandler : public QRunnable
{
protected:


    DbUser user;
    message_queue_ptr inputMsgs;
    message_queue_ptr outputMsgs;

    Attachment::ReplyKeyboardMarkup generate_menu(const MenuState state = MS_MAIN);

    CMD parse_cmd(QString &text);

    void user_leave_chat(Attachment::Message &msg);
    bool check_last_msg(Attachment::Message &msg);

    void run();
public:
    MsgsHandler(message_queue_ptr in, message_queue_ptr out);

    virtual ~MsgsHandler(){}
};

