#include <iostream>
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

        for( child; child; child=child->NextSiblingElement() )
        {

            lat = atof(child->Attribute("lat"));
            lon = atof(child->Attribute("lon"));

            TiXmlElement* tmp = child->FirstChild()->ToElement();
            string time(tmp->GetText());
            struct tm tm;
            strptime(time.c_str(), "%FT%TZ", &tm);
            time_t tmt = mktime(&tm);

            tmp = tmp->NextSiblingElement();
            ele = atof(tmp->GetText());
            scanner.add_track(tmt, lat, lon, ele);
        }

        cout << scanner.all_distance() << endl;

        cout << scanner.duration() << endl;

        cout << scanner.run_time() << endl;
        cout << scanner.stop_time() << endl;


    }
    catch(...){

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
