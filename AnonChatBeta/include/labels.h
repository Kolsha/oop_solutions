#pragma once
#include <QString>

#include "tg/attachment.h"



namespace Labels {

using ::std::string;
using ::std::vector;

const string man_message = "Привет. Я бот для поиска собеседника 🎭\n"
                           "На этом все ✋";

const QString start_cmd = "/start";

const string help = "Помощь 💁";
const QString help_cmd = "/help";

const string start_search = "Поиск 🔍";
const QString start_search_cmd = "/search";


const string stop_search = "Остановить 🔍";
const QString stop_search_cmd = "/stop";


const string leave = "Завершить ✋";
const QString leave_cmd = "/leave";




const string settings = "Настройки ⚙";
const QString settings_cmd = "/set";

const string search_settings = "Настройки поиска 🔎";
const QString search_settings_cmd = "/sset";





const string main_menu = "Меню 🌍";
const QString main_menu_cmd = "/menu";


const string department = "Факультет 🎓";
const string department_any = "Любой 🎓";


const string gender = "Пол 👽:";
const string gender_any = "Любой 🎭";




const string selected = "✅";

const std::vector<TgBot::Attachment::KeyboardButton> gender_list = {
    string("Мужской"), string("Женский")
};


const std::vector<TgBot::Attachment::KeyboardButton> department_list = {
    string("ГГФ"), string("ММФ"), string("ФЕН"), string("ФФ"), string("ФИТ"),
    string("ЭФ"), string("ГумФак"), string("ФилФак"), string("МедФак"), string("Другой")
};


}
