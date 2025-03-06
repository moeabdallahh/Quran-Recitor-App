#ifndef WIDGET_H
#define WIDGET_H
#include "quran.h"
#include <QWidget>
#include <vector>


QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:

    int n;
    QString nums;
    QLabel *myLabel;
    QTextEdit *myTextEdit;
    QSlider *mySlider;
    QLabel *text;
    QFont font;
    QLabel *textLabel;
    QLabel *textLabel1;
    QLabel *textLabel3;
    QLabel *myLabels;
    QVBoxLayout *myVBoxLayout;
    QGridLayout *myGrid;
    QGridLayout *myGrid1;
    QLineEdit *myEdit;
    QString recitation;
    int R;
    QString temp;
    QLabel *TOC;
    QTextEdit *opening;
    QPushButton *showTOC;
    QString temp2;

    vector<string> SNE;
    vector<string> Name;
    vector<string> NA;
    vector<string> S;
    vector<string> time;

    FILE *fp;
    QTime now;

    QPushButton *myButton;
    QPushButton *Favorite;
    QPushButton *View;
    QPushButton *Recite;
    QPushButton *Pause;
    QPushButton *Resume;

    QScrollArea *myScroll;
    QScrollArea *myScroll1;
    quran quran;
    Widget(QWidget *parent = nullptr);
    ~Widget();

    void displaytext();
    void request_json(QString urlStrTerms);
private:
    Ui::Widget *ui;
    QMediaPlayer *play;
    QAudioOutput *audio;

private slots:
    void onReciteButton();
    void onSubmitButton();
    void onFavoriteButton();
    void onViewButton();
    void response_json(QNetworkReply *reply);
    void onPlayButtonClicked();
    void onStopButtonClicked();
    void printTOC();

};
#endif // WIDGET_H





