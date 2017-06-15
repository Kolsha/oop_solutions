#include <iostream>
#include <math.h>
#include "gpsscanner.h"
#include "utils/tinyxml/tinyxml.h"


using namespace std;

int main(int argc, char *argv[])
{
    if(argc != 2){
        cout << "Give me file" << endl;
        return 1;
    }
    try{
        GPSScanner scanner;
        TiXmlDocument docHandle(argv[1]);
        if(!docHandle.LoadFile()){
            cout << "Bad file" << endl;
            return 2;
        }
        TiXmlElement* child = docHandle.FirstChild("gpx")->FirstChild("trk")
                ->FirstChild( "trkseg" )->FirstChild( "trkpt" )->ToElement();

        double lat, lon, ele;

        for( child; child; child = child->NextSiblingElement())
        {

            string tmp_str(child->Attribute("lat"));
            size_t last_pos;
            lat = stof(tmp_str, &last_pos);
            if(last_pos < 1){
                continue;
            }

            tmp_str.assign(child->Attribute("lon"));

            lon = stof(tmp_str, &last_pos);
            if(last_pos < 1){
                continue;
            }

            TiXmlElement* tmp = child->FirstChild()->ToElement();
            struct tm tm;
            strptime(tmp->GetText(), "%FT%TZ", &tm);
            time_t tmt = mktime(&tm);


            tmp = tmp->NextSiblingElement();
            tmp_str.assign(tmp->GetText());

            ele = stof(tmp_str, &last_pos);
            if(last_pos < 1){
                continue;
            }

            if(isnan(ele) || isnan(lon) || isnan(lat)){
                continue;
            }

            scanner.add_track(tmt, lat, lon, ele);
        }

        cout << scanner.average_speed(false) << endl;

        cout << scanner.duration() << endl;

        cout << scanner.run_time() << endl;
        cout << scanner.stop_time() << endl;


    }
    catch(...){

        return 1;
    }

    //doc.Parse()
    // http://www.answerium.com/article17/
    cout << "Hello World!" << endl;
    return 0;
}

/*
 *
-------Общая продолжительность по времени
-------Расстояние
-------Средняя скорость
Время движения и время стоянок
Средняя скорость движения (без учёта стоянок)
-------Максимальная скорость
-------Минимальная высота
-------Максимальная высота
-------Общий набор высоты
-------Общий спуск
Распределение скорости по времени (набор пар: диапазон скоростей, длительность). Например, пара 8-14, 15:37 будет означать, что движение со скоростью от 8 до 14 км/ч шло в течение 15 минут 37 сек. Предусмотреть задание параметров для вычисления гистограммы.
*/
