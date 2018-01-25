#include "include/msgs_handler.h"

#include "include/labels.h"


using  ::std::vector;
using  ::std::string;

using Attachment::KeyboardButton;


MsgsHandler::MsgsHandler(message_queue_ptr in, message_queue_ptr out)
{
    inputMsgs = in;
    outputMsgs = out;
}

void MsgsHandler::run()
{

    for(;;){
        Attachment::Message msg;

        QThread::usleep(qrand() % 100);
        if(!inputMsgs->dequeue(msg))
            continue;

        if(!this->user.parse(msg.from)){
            continue;
        }

        QString text = QString(msg.text.c_str());
        QString ltxt = text.toLower();

        CMD curr_cmd = parse_cmd(text);
        switch(curr_cmd)
        {
        case CMD_MAIN_MENU:
            msg.text = "Менюшка подъехала)0)";
            user.last_msg = Labels::main_menu_cmd;
            msg.flags.reply_markup = generate_menu(MS_MAIN).serialize();
            break;
        case CMD_SET:
            msg.text = "Раскройте себя";
            user.last_msg = Labels::settings_cmd;
            msg.flags.reply_markup = generate_menu(MS_SET).serialize();
            break;
        case CMD_SET_SEARCH:
            msg.text = "Кого будем искать?";
            user.last_msg = Labels::search_settings_cmd;

            msg.flags.reply_markup = generate_menu(MS_SET_SEARCH).serialize();
            break;
        case CMD_LEAVE:{
            user.last_msg = Labels::leave_cmd;
            user_leave_chat(msg);
            msg.flags.reply_markup = generate_menu(MS_MAIN).serialize();
        }
            break;
        case CMD_SEARCH:
            user_leave_chat(msg);
            msg.text = (user.now_search) ? "Мы уже ищем;)" : "Поиск начался;)";
            user.last_msg = Labels::start_search_cmd;
            user.now_search = true;
            msg.flags.reply_markup = generate_menu(MS_MAIN).serialize();

            break;
        case CMD_STOP:{
            msg.text = (user.now_search) ? "Поиск остановлен" : "Вы никого и не искали)";
            msg.text += "\nДля поиска используйте " + Labels::start_search_cmd.toStdString();
            user.now_search = false;
            user.last_msg = Labels::stop_search_cmd;
            msg.flags.reply_markup = generate_menu(MS_MAIN).serialize();

        }
            break;
        case CMD_START:
        case CMD_HELP:{
            msg.text = Labels::man_message;
            msg.flags.reply_markup = generate_menu(MS_MAIN).serialize();
            user.last_msg = Labels::help_cmd;
        }
            break;
        default:{

        }
        }

        if(curr_cmd != CMD_NONE){
            user.update();
            outputMsgs->enqueue(msg);
            continue;
        }

        if(check_last_msg(msg)){
            user.update();
            outputMsgs->enqueue(msg);
            continue;
        }

        if(text.toStdString() == Labels::department ||
                text.toStdString() == Labels::gender){
            continue;
        }


        if(!user.chat_with){
            user.last_msg = text;
            user.update();

            if(user.now_search){
                msg.text = "Мы все еще ищем;)";
            }else{
                msg.text = "Возможно стоит начать искать, а не спамить меня\n"
                           "/search ;)";
            }
            msg.flags.reply_markup = generate_menu(MS_MAIN).serialize();
            outputMsgs->enqueue(msg);
        }
        else{
            msg.chat.id = user.chat_with;
            msg.flags.reply_markup = generate_menu(MS_MAIN).serialize();
            outputMsgs->enqueue(msg);
        }

    }
}

void select_menu_item(vector<KeyboardButton> &items, const string &text){
    for(auto &item : items){
        if(item.text == text){
            item.text = Labels::selected + item.text;
            break;
        }
    }
}

ReplyKeyboardMarkup MsgsHandler::generate_menu(const MenuState state)
{

    ReplyKeyboardMarkup res;
    switch(state){
    case MS_MAIN:
    {
        string search_btn = ((user.is_parsed() && user.now_search) ? Labels::stop_search : Labels::start_search);
        if(user.chat_with > 0)
            search_btn = Labels::leave;

        res.keyboard = {
            {search_btn},
            {Labels::settings},
            {Labels::search_settings},
            {Labels::help}
        };

    }

        break;
    case MS_SET:
    {
        vector<KeyboardButton> gender_list = Labels::gender_list;
        vector<KeyboardButton> department_list = Labels::department_list;

        if(user.is_parsed()){
            select_menu_item(gender_list, user.params["gender"].first.toStdString());
            select_menu_item(department_list, user.params["department"].first.toStdString());
        }

        res.one_time_keyboard = true;
        res.keyboard = {

            {Labels::main_menu, Labels::gender},
            gender_list,

            {Labels::department},

            vector<KeyboardButton>(department_list.begin(),
            department_list.begin() + department_list.size() / 2),

            vector<KeyboardButton>(department_list.begin() + department_list.size() / 2,
            department_list.end()),

        };
    }
        break;
    case MS_SET_SEARCH:
    {
        res.one_time_keyboard = true;
        string any = Labels::gender_any;
        vector<KeyboardButton> gender_list = Labels::gender_list;

        if(user.params["gender"].second.isEmpty()){
            any = Labels::selected + any;
        }
        else{
            select_menu_item(gender_list, user.params["gender"].second.toStdString());
        }
        gender_list.push_back(any);

        vector<KeyboardButton> department_list = Labels::department_list;

        any = Labels::department_any;
        if(user.params["department"].second.isEmpty()){
            any = Labels::selected + any;
        }
        else{
            select_menu_item(department_list, user.params["department"].second.toStdString());
        }

        department_list.push_back(any);


        res.keyboard = {

            {Labels::main_menu, Labels::gender},
            gender_list,

            {Labels::department},

            vector<KeyboardButton>(department_list.begin(),
            department_list.begin() + department_list.size() / 2),

            vector<KeyboardButton>(department_list.begin() + department_list.size() / 2,
            department_list.end()),

        };


    }

        break;
    }

    return res;

}

CMD MsgsHandler::parse_cmd(QString &text)
{
    QString ltxt = text.toLower();
    if(ltxt.startsWith(Labels::start_search_cmd) ||
            text.startsWith(Labels::start_search.c_str())){
        return CMD_SEARCH;
    }


    if(ltxt.startsWith(Labels::stop_search_cmd) ||
            text.startsWith(Labels::stop_search.c_str())){
        return CMD_STOP;
    }


    if(ltxt.startsWith(Labels::settings_cmd) ||
            text.startsWith(Labels::settings.c_str())){
        return CMD_SET;
    }


    if(ltxt.startsWith(Labels::search_settings_cmd) ||
            text.startsWith(Labels::search_settings.c_str())){
        return CMD_SET_SEARCH;
    }

    if(ltxt.startsWith(Labels::main_menu_cmd) ||
            text.startsWith(Labels::main_menu.c_str())){
        return CMD_MAIN_MENU;
    }


    if(ltxt.startsWith(Labels::leave_cmd) ||
            text.startsWith(Labels::leave.c_str())){
        return CMD_LEAVE;
    }


    if(ltxt.startsWith(Labels::help_cmd) ||
            text.startsWith(Labels::help.c_str())){
        return CMD_HELP;
    }

    if(ltxt.startsWith(Labels::start_cmd)){
        return CMD_START;
    }

    return CMD_NONE;
}

void MsgsHandler::user_leave_chat(Message &msg)
{
    if(user.chat_with < 1){
        msg.text = "Вы не были в диалоге";
        return ;
    }

    Message to_talker = msg;
    to_talker.chat.id = user.chat_with;

    user.chat_with = 0;
    user.now_search = false;
    msg.text = "Диалог завершен";

    DbUser talker;

    if(talker.load(to_talker.chat.id)){
        talker.chat_with = 0;
        talker.now_search = false;
        talker.update();
        to_talker.text = "Собеседник завершил диалог;(";
        outputMsgs->enqueue(to_talker);

    }


}

bool MsgsHandler::check_last_msg(Message &msg)
{

    QString text(msg.text.c_str());

    MenuState ms = MS_MAIN;
    if(user.last_msg == Labels::settings_cmd){

        if(std::find(Labels::gender_list.begin(), Labels::gender_list.end(), msg.text) != Labels::gender_list.end()){
            user.params["gender"].first = text;
            //updated = true;
            ms = MS_SET;
        }

        if(std::find(Labels::department_list.begin(), Labels::department_list.end(), msg.text) != Labels::department_list.end()){
            user.params["department"].first = text;
            ms = MS_SET;
        }
    }




    if(user.last_msg == Labels::search_settings_cmd){

        if(msg.text == Labels::gender_any){
            user.params["gender"].second = "";
            ms = MS_SET_SEARCH;
        }
        else if(msg.text == Labels::department_any){
            user.params["department"].second = "";
            ms = MS_SET_SEARCH;
        }
        else if(std::find(Labels::gender_list.begin(), Labels::gender_list.end(), msg.text) != Labels::gender_list.end()){
            user.params["gender"].second = text;
            ms = MS_SET_SEARCH;
        }
        else if(std::find(Labels::department_list.begin(), Labels::department_list.end(), msg.text) != Labels::department_list.end()){
            user.params["department"].second = text;
            ms = MS_SET_SEARCH;
        }
    }


    if(ms != MS_MAIN){
        msg.text = "Настройки обновлены!)";
        msg.flags.reply_markup = generate_menu(ms).serialize();
        return true;
    }

    return false;


}
