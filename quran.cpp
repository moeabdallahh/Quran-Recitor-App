#include "quran.h"

quran::quran()
{

}

void quran::set_from_Json(QJsonObject myJson)
{

    QJsonValue DD = myJson.value("data");
    QJsonArray data = DD.toArray();

    QJsonObject obj = DD.toObject();
    QJsonValue value = obj.value("numberOfAyahs");
    QString num = value.toString();

    ayahNum =  value.toInt();
    //qDebug() << "\nNumber of Ayahs in Surah: " << ayahNum;

    n = ayahNum;
    //qDebug() << "n: " << n;

    QJsonObject myObj = DD.toObject();
    QJsonValue myValue = myObj.value("englishName");
    surahNameEnglish = myValue.toString();
    //qDebug() << "English name: " << surahNameEnglish;

    QJsonObject myobj = DD.toObject();
    QJsonValue myvalue = myobj.value("englishNameTranslation");
    surahName = myvalue.toString();
    //qDebug() << "name: " << surahName;

    QJsonValue val = obj.value("ayahs");
    QJsonArray myArray = val.toArray();

    surahVerse = 0;
    for(int i=0;i<n;i++) //resultCount;i++)
    {

        QJsonObject myObj = myArray[i].toObject();
        QJsonValue myValue = myObj.value("text");

        surahVerse = surahVerse + "\n\n" + myValue.toString();
    }

    //qDebug() << "Text: " << surahVerse;
}






