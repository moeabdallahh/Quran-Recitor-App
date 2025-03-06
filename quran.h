#ifndef QURAN_H
#define QURAN_H

#include <QWidget>
#include <QFont>
#include <QSlider>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QDebug>
#include <QPaintEvent>
#include <QPainter>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QString>
#include <QFile>
#include <QTextEdit>
#include <QLabel>
#include <QScrollArea>
#include <QTime>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QStringList>
#include <QPdfWriter>
#include <QPainter>
#include <QTextDocument>


using namespace std;

class quran
{
public:

    QString surahVerse;
    QString AyahVerse;
    QString surahNameEnglish;
    QString surahName;
    int ayahNum;
    int n;
    QString num;

    //methods

    quran();
    void set_from_Json(QJsonObject myJson);
};

#endif // QURAN_H





