#include <fstream>
#include <algorithm>
#include "app_logic.h"

using std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::string;
using std::ifstream;

const size_t BYTES_IN_MB = 1000000;


const std::string AppLogic::ini_demo = "#mask settigns\n"
                                       "audio_mask  '%audio_artist% - %audio_title%.mp3'\n"
                                       "photo_mask  '%att_date%.jpg'\n"
                                       "doc_mask    '%doc_title%.%doc_ext%'\n"
                                       "#dir settings\n"
                                       "audio_dir   'home/%chat_title%/audios'\n"
                                       "photo_dir   'home/%chat_title%/photos'\n"
                                       "doc_dir     'home/%chat_title%/docs'\n"
                                       "#size settigns\n"
                                       "min_size    0.1 # MB, if == 0 - ignore\n"
                                       "max_size    0   # MB, if == 0 - ignore\n"
                                       "#other settings\n"
                                       "date_format '%F %R'"
                                       "#Also you can use mask:\n"
                                       "# %chat_title%    -\n"
                                       "# %att_date%      -\n"
                                       "# %cur_date%      -\n"
                                       "# %audio_artist%  -\n"
                                       "# %audio_title%   -\n"
                                       "# %photo_text%    -\n"
                                       "# %doc_title%     -\n"
                                       "# %doc_ext%       - .jpg, .doc\n"
                                       "# %doc_type%      -\n";



void AppLogic::callback_download(const DownloadInfo &info)
{
    if(info.state == DS_PROCESS){
        return ;
    }
    cout << file_name(info.path) << " ";
    if(info.state == DS_OK){
        cout << "OK" << endl;
    }
    else{
        cout << "ERROR" << endl;
    }
}

string AppLogic::time2string(const time_t time)
{
    try{
        std::tm * ptm = std::localtime(&time);
        char buffer[256];
        std::strftime(buffer, 255, (cfg["date_format"]).c_str(), ptm);
        return string(buffer);
    }
    catch(...){

    }
    return "";
}

void AppLogic::read_cfg()
{
    std::ifstream ini("config.ini");
    if(ini.is_open()){
        cfg = readConfiguration(ini);
    }
    ini.close();
}


template<typename T>
void AppLogic::save_attacments(int chat_id,
                               const std::string &chat_title,
                               const size_t count)
{
    vector<T> atts = vk.get_attachments<T>(chat_id, count);
    vector<string> name_collision;
    string type_name = T::type;

    double min_size = stod(cfg["min_size"]) * BYTES_IN_MB;
    double max_size = stod(cfg["max_size"]) * BYTES_IN_MB;

    for(auto &kv:atts){
        T att = kv;



        string path = cfg[type_name + "_dir"];
        std::unordered_map<string, string> masks = default_mask(&att, chat_title);

        path = apply_mask(path, masks);
        normalize_path(path);
        int try_to_mkpath = 0;
        while (!mkpath(path) && try_to_mkpath < 3){
            try_to_mkpath++;
        }

        if(try_to_mkpath > 3){
            cout << "Bad "<< type_name << " path" << endl;
            break;
        }

        string filename = cfg[type_name + "_mask"];
        filename = apply_mask(filename, masks);
        string tmp_name = filename;
        size_t i = 0;
        while(std::find(name_collision.begin(), name_collision.end(), (path + tmp_name)) != name_collision.end() ){
            tmp_name = "(" + std::to_string(i) + ")" + filename;
            i++;
        }

        path += tmp_name;
        name_collision.push_back(path);

        downloader->add_download(att.direct_url, path,
                                 size_t(min_size), size_t(max_size));

    }

}

std::unordered_map<string, string> AppLogic::default_mask(const VK::Attachment::BaseAttachment *att,
                                                          const string &chat_title)
{
    string cur_date = time2string(time(NULL));
    string att_date = time2string(att->date);
    std::unordered_map<string, string> masks = {
        {"%chat_title%", chat_title},
        {"%att_date%", att_date},
        {"%cur_date%", cur_date}
    };

    const VK::Attachment::Audio *audio = dynamic_cast<const VK::Attachment::Audio*>(att);
    if(audio != nullptr){
        masks.insert({
                         {"%audio_artist%", audio->artist},
                         {"%audio_title%", audio->title},
                     });
    }

    const VK::Attachment::Photo *photo = dynamic_cast<const VK::Attachment::Photo*>(att);
    if(photo != nullptr){
        masks.insert({"%photo_text%", photo->text});
    }

    const VK::Attachment::Document *doc = dynamic_cast<const VK::Attachment::Document*>(att);
    if(doc != nullptr){
        string ext = "." + doc->ext ;
        ext = (doc->title.find(ext) == string::npos) ? ext : "";
        masks.insert({
                         {"%doc_title%", doc->title},
                         {"%doc_ext%", ext},
                         {"%doc_type%", doc->doc_type_str(doc->doc_type)}
                     });
    }

    return std::move(masks);
}

bool AppLogic::save_dialogs_to_file(const string &filename, const size_t count)
{
    std::ofstream out(filename);
    if(!out.is_open()){
        cout << "Can't open file" << endl;
        return false;
    }

    VK::vector_dialogs dialogs = vk.get_all_dialogs(count);
    for(auto &it:dialogs){
        VK::DialogInfo d = it;
        d.title = (d.title.empty()) ? "unknown" : d.title;
        out << "'" << d.title << "' " << d.chat_id << endl;
    }
    out.close();
    return true;
}

bool AppLogic::save_attacments_from_dialogs(const std::map<string, string> &dialogs, const int att_type)
{

    if(att_type < 0 || att_type > 3){
        system("clear");
        cout << "Bad type" << endl;
        return false;
    }
    for(auto &dlg:dialogs){
        int chat_id = std::stoi(dlg.second);
        string chat_title = dlg.first;
        if(att_type == 1 || att_type == 0){

            save_attacments<VK::Attachment::Audio>(chat_id, chat_title);

        }
        if(att_type == 2 || att_type == 0){
            save_attacments<VK::Attachment::Photo>(chat_id, chat_title);
        }

        if(att_type == 3 || att_type == 0){
            save_attacments<VK::Attachment::Document>(chat_id, chat_title);
        }
    }
    while(downloader->is_work_now()){

    }
    cout << "Downloaded" << endl;

    return true;

}

AppLogic::AppLogic(const size_t thread_count)
{
    downloader = new CurlDownloader(thread_count, AppLogic::callback_download);
    read_cfg();
    if(!cfg.size()){
        std::ofstream ini_example("config.ini");
        if(ini_example.is_open()){
            ini_example << "#Demo ini file, you can change it" << endl;
            ini_example << ini_demo << endl;
        }
        ini_example.close();
    }
    read_cfg();
    if((cfg["date_format"]).empty()){
        cfg["date_format"] = "%F %R";
    }

}

int AppLogic::run(const std::string &login, const std::string &pass, const std::string &access_token)
{
    if(!vk.auth(login, pass, access_token)){
        cout << "Auth failed. Exit." << endl;
        return 401;
    }
    cout << "Auth ok. Hello " << vk.first_name() << endl;
    cout << "Access token: '" << vk.access_token() << "'" << endl << endl;




    int action = -1;
    while (action != 9){

        cout << help_str << endl;
        cin >> action;
        switch (action) {
        case 0:
        {
            cout << "Ok, give me filename" << endl;
            string filename;
            int count = 0;
            cin >> filename;
            cout << "Ok, you need max count ? 0 - not need" << endl;
            cin >> count;
            save_dialogs_to_file(filename, count);
            system("clear");
            cout << "Save dialogs complete" << endl;
        }
            break;
        case 2:{
            cout << "Ok, give me chat_id" << endl;
            int chat_id = 0;
            cin >> chat_id;
            if(chat_id == 0){
                system("clear");
                cout << "Bad chat_id" << endl;
                break;
            }
            string chat_title;
            cout << "Chat title now:" << endl;
            cin >> chat_title;
            int att_type = 0;
            cout << "Attacment type: \n 0 - all \n 1 - audio \n 2 - photo \n 3 - document \n";
            cin >> att_type;
            if(att_type < 0 || att_type > 3){
                system("clear");
                cout << "Bad type" << endl;
                break;
            }
            std::map<string, string> dialogs = {
                {chat_title, std::to_string(chat_id)}
            };
            save_attacments_from_dialogs(dialogs, att_type);
        }
            break;
        case 1:{
            cout << "Ok, give me filename" << endl;
            string filename;
            cin >> filename;
            ifstream dialogs_file(filename);
            if(!dialogs_file.is_open()){
                system("clear");
                cout << "Bad filename" << endl;
                break;
            }

            std::map<string, string> dialogs = readConfiguration(dialogs_file);
            dialogs_file.close();
            int att_type = 0;
            cout << "Attacment type: \n 0 - all \n 1 - audio \n 2 - photo \n 3 - document \n";
            cin >> att_type;
            if(att_type < 0 || att_type > 3){
                system("clear");
                cout << "Bad type" << endl;
                break;
            }

            save_attacments_from_dialogs(dialogs, att_type);


        }
            break;

        }

    }


    return 0;
}


const std::string AppLogic::help_str = "What do you want:\n"
                                       "0 - save dialogs to file\n"
                                       "1 - save attachments from dialogs from file\n"
                                       "2 - save attacments from chat_id\n"
                                       "9 - exit\n"
        ;
