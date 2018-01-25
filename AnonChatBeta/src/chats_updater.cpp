#include "include/chats_updater.h"
#include "include/db_utils.h"
#include "include/tg/attachment.h"

ChatsUpdater::ChatsUpdater(std::shared_ptr<QSafeQueue<Attachment::Message>> queue)
{
    outputMsgs = queue;
}



void ChatsUpdater::run()
{
    std::vector<QString> all_names = get_all_names();
    QMap<QString, size_t> pairs;
    std::vector<PossiblePair> possible_pairs;
    for(;;){
        pairs.clear();
        QThread::usleep(qrand() % 100);

        possible_pairs = get_possible_pairs();
        if(!possible_pairs.size()){
            QThread::sleep(qrand() % 3 + 3);
            continue;
        }

        PossiblePair right_pair;
        for(auto &pair : possible_pairs){
            QString idx = QString("%1_%2").arg(pair.UID1, pair.UID2);
            pairs[idx]++;
            if(pairs[idx] >= all_names.size()){
                right_pair = pair;
                break;
            }
        }

        if(right_pair.UID1 > 0){
            DbUser user1;
            DbUser user2;

            if(!user1.load(right_pair.UID1) ||
                    !user2.load(right_pair.UID2)){
                continue;
            }

            user1.chat_with = user2.user_id;
            user2.chat_with = user1.user_id;
            user1.now_search = user2.now_search = false;

            user1.update();
            user2.update();

            const QString msg_text = "Собеседник найден!)\n"
                                     "Пол: %1\n"
                                     "Факультет: %2";
            TgBot::Attachment::Message msg1, msg2;

            msg1.message_id = msg2.message_id = 777;

            msg1.chat.id = user1.user_id;
            msg1.text = msg_text.arg(user2.params["gender"].first, user2.params["department"].first).toStdString();


            msg2.chat.id = user2.user_id;
            msg2.text = msg_text.arg(user1.params["gender"].first, user1.params["department"].first).toStdString();


            outputMsgs->enqueue(msg1);
            outputMsgs->enqueue(msg2);



        }


    }

}
