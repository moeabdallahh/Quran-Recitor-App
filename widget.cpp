#include "widget.h"
#include "ui_widget.h"
#include "quran.h"

using namespace std;
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    n = 0;

    // Set up UI

    myVBoxLayout = new QVBoxLayout(this);
    myScroll  = nullptr;

    this->setLayout(myVBoxLayout);

    myEdit = new QLineEdit(this);
    myButton = new QPushButton("Submit");
    myGrid = new QGridLayout(this);
    myGrid1 = new QGridLayout(this);
    Favorite = new QPushButton("Favorite");
    View = new QPushButton("View Your Favorites");
    Recite = new QPushButton("Recite This For Me");
    play = new QMediaPlayer(this);
    audio = new QAudioOutput(this);
    Resume = new QPushButton("Resume Reciation");
    Pause = new QPushButton("Pause Recitation");
    //TOC = nullptr; //
    //opening = nullptr; //
    showTOC = new QPushButton("Show Table of Contents"); //

    play->setAudioOutput(audio);
    audio->setVolume(50);

    font.setPixelSize(14);
    textLabel3 = new QLabel("Please Enter The Number of The Surah You Would Like To Read - Use The Table Of Contents To Find The Surah You Want");


    // temp2 = "Please Enter The Number of The Surah You Would Like To Read\nUse The Table Of Contents To Find The Surah You Want";
    // textLabel3 = new QLabel(temp2, this);
    // textLabel3->setFont(font);
    // textLabel3->setWordWrap(1);
    // myGrid1->addWidget(textLabel3);

    myVBoxLayout->addWidget(textLabel3);
    myVBoxLayout->addWidget(myEdit);
    myVBoxLayout->addWidget(myButton);
    myVBoxLayout->addWidget(showTOC); //
    // myVBoxLayout->addWidget(TOC); //
    myVBoxLayout->addLayout(myGrid);


    connect(myButton, &QPushButton::clicked, this, &Widget::onSubmitButton); //connects to submit
    connect(Favorite, &QPushButton::clicked, this, &Widget::onFavoriteButton); //connects to favorite
    connect(View, &QPushButton::clicked, this, &Widget::onViewButton); //conects to view favorites
    connect(Recite, &QPushButton::clicked, this, &Widget::onReciteButton); //connects to the recitation
    connect(Pause, &QPushButton::clicked, this, &Widget::onStopButtonClicked); //connects to pause
    connect(Resume, &QPushButton::clicked, this, &Widget::onPlayButtonClicked); //connects to play
    connect(showTOC, &QPushButton::clicked, this, &Widget::printTOC); //connects table of contents

    myLabels = nullptr;
    textLabel = nullptr;
    ui->setupUi(this);


}

Widget::~Widget()
{
    delete ui;
}

void Widget::onSubmitButton()
{

    if (SNE.size() >= 1)
    {
        View->show();
    }
    else View->hide();
    Resume->hide();
    Pause->hide();
    play->stop();
    //delete TOC;
    //delete showTOC; //uncomment this one to try the table of contents
    //delete myScroll1;

    QString urlStr = myEdit->text();

    request_json(urlStr); //requests JSON
}

void Widget::request_json(QString urlStrTerms)
{
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);

    // Built get request from terms in myEdit

    // For now: hard-coded:

    QString baseUrlStr = "https://api.alquran.cloud/v1/surah/";
    recitation = urlStrTerms;
    R = urlStrTerms.toInt();

    // To do: need to parse urlStrTerms
    // For now, assume just 1 term

    QString urlStr = baseUrlStr + urlStrTerms + "/en.asad";

    QUrl myUrl(urlStr);

    QNetworkRequest myRequest(myUrl);

    connect(manager,&QNetworkAccessManager::finished,this, &Widget::response_json);

    manager->get(myRequest);    // Make the get request
}

void Widget::response_json(QNetworkReply *reply)
{
    QByteArray *myByteArray = new QByteArray();
    myByteArray->append(reply->readAll());

    QJsonDocument myJson = QJsonDocument::fromJson(*myByteArray);
    //qDebug() << "\nmyJson: \n\n" << myJson;

    QJsonObject D = myJson.object();
    quran.set_from_Json(D);
    displaytext();
}

void Widget::displaytext()
{
    delete textLabel;
    delete myScroll;

    Favorite->show();
    Recite->show();

    nums = QString::number(quran.ayahNum);
    QString temp = "\nEnglish Name: " + quran.surahNameEnglish + "\n\nTranslated Name: " + quran.surahName + "\n\nNumber of Ayahs in the Surah: " + nums + "\n\nTranslated Surah: " + quran.surahVerse;
    //qDebug() << quran.surahVerse;
    font.setPixelSize(16);

    textLabel = new QLabel(temp, this);
    textLabel->setFont(font);
    textLabel->setWordWrap(1);

    myGrid->addWidget(textLabel);
    myScroll = new QScrollArea(this);

    myScroll->setWidgetResizable(1);
    myScroll->setWidget(textLabel);

    myGrid->addWidget(myScroll);

    myVBoxLayout->addWidget(Favorite);
    myVBoxLayout->addWidget(View);
    myVBoxLayout->addWidget(Recite);
    myVBoxLayout->addWidget(Resume);
    myVBoxLayout->addWidget(Pause);
    //myVBoxLayout->addWidget(myScroll1);
    repaint();

    // if(showTOC->isChecked()) //->isEnabled())
    // {
    //     connect(showTOC, &QPushButton::clicked, this, &Widget::printTOC);
    // }
}



void Widget::onFavoriteButton()
{
    View->show();

    //qDebug() <<"button worked";
    SNE.push_back(quran.surahNameEnglish.toStdString());
    Name.push_back(quran.surahName.toStdString());
    NA.push_back(nums.toStdString());
    S.push_back(quran.surahVerse.toStdString());
    time.push_back(now.currentTime().toString().toStdString());
}

void Widget::onViewButton()
{
    Resume->hide();
    Pause->hide();
    Favorite->hide();
    View->hide();
    Recite->hide();

    qDebug() << "view works";
    delete myScroll;


    if (QFile::remove("C:\\QTCoding\\C++Program\\build-EL_Abdallah-Mohamed-370FINAL-Desktop_Qt_6_6_1_MinGW_64_bit-Debug\\YourFavoriteSurahVerses.txt"))
    {
        qDebug() << "File deleted successfully.";
    }

    QString content;
    QString filename = "YourFavoriteSurahVerses.txt";
    QFile file(filename);

    if (file.open(QIODevice::ReadWrite))
    {
        for (size_t i = 0; i < SNE.size(); i++)
        {
            QTextStream stream(&file);
            stream << "Time Added:" + QString::fromStdString(time[i]) + "\nEnglish Name: " + QString::fromStdString(SNE[i]) + "\n\nTranslated Name: " + QString::fromStdString(Name[i])+ "\n\nNumber of Ayahs in the Surah: " + QString::fromStdString(NA[i]) + "\n\nTranslated Surah: " + QString::fromStdString(S[i]) + "\n\n";
        }
    }

    QFile readFile("C:\\QTCoding\\C++Program\\build-EL_Abdallah-Mohamed-370FINAL-Desktop_Qt_6_6_1_MinGW_64_bit-Debug\\YourFavoriteSurahVerses.txt");
    if (readFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&readFile);

        // Read the file into a QString
        content = in.readAll();
        //qDebug() << "File content:\n" << content;

        readFile.close();
    }
    else
    {
        qDebug() << "Error opening file for reading:" << readFile.errorString();
    }

    textLabel = new QLabel(content, this);
    textLabel->setFont(font);
    textLabel->setWordWrap(1);

    myGrid->addWidget(textLabel);
    myScroll = new QScrollArea(this);

    myScroll->setWidgetResizable(1);
    myScroll->setWidget(textLabel);

    myGrid->addWidget(myScroll);
}

void Widget::onReciteButton()
{
    qDebug() << "recite works";

    if(R <= 9)
    {
        play->setSource(QUrl::fromLocalFile("C:\\QTCoding\\C++Program\\EL_Abdallah-Mohamed-370FINAL\\Recitation\\00" + recitation + ".mp3"));
    }
    if(R >= 10 && R <= 99)
    {
        play->setSource(QUrl::fromLocalFile("C:\\QTCoding\\C++Program\\EL_Abdallah-Mohamed-370FINAL\\Recitation\\0" + recitation + ".mp3"));
    }
    if(R >= 100)
    {
        play->setSource(QUrl::fromLocalFile("C:\\QTCoding\\C++Program\\EL_Abdallah-Mohamed-370FINAL\\Recitation\\" + recitation + ".mp3"));
    }

    play->play();
    Resume->show();
    Pause->show();
}

void Widget::onPlayButtonClicked()
{
    play->play();
}
void Widget::onStopButtonClicked()
{
    play->pause();
}

void Widget::printTOC()
{
    delete myScroll;
    //delete textLabel1;
    //delete textLabel;

    Favorite->hide();
    View->hide();
    Recite->hide();
    Resume->hide();
    Pause->hide();

    QString quranChapters[] = {"Al-Fatiha", "Al-Baqarah", "Al Imran", "An-Nisa", "Al-Ma'idah",
                               "Al-An'am", "Al-A'raf", "Al-Anfal", "At-Tawbah", "Yunus",
                               "Hud", "Yusuf", "Ar-Ra'd", "Ibrahim", "Al-Hijr", "An-Nahl",
                               "Al-Isra", "Al-Kahf", "Maryam", "Ta-Ha", "Al-Anbiya", "Al-Hajj",
                               "Al-Mu'minun", "An-Nur", "Al-Furqan", "Ash-Shu'ara", "An-Naml",
                               "Al-Qasas", "Al-Ankabut", "Ar-Rum", "Luqmaan", "As-Sajda",
                               "Al-Ahzaab", "Saba", "Faatir", "Yaseen", "As-Saaffaat",
                               "Saad", "Az-Zumar", "Ghafir", "Fussilat", "Ash_Shooraa",
                               "Az-Zukhruf", "Ad-Dukhaan", "Al-Jaathiyah", "Al-Ahqaaf",
                               "Muhammad", "Al-Fath", "Al-Hujuraat", "Qaaf", "Adh-Dhaariyaat",
                               "At-Toor", "An-Najm", "Al-Qamar", "Ar-Rahman", "Al-Waqi'a",
                               "Al-Hadeed", "Al-Mujadila", "Al-Hashr", "Al-Mumtahanah",
                               "As-Saff", "Al-Jumu'ah", "Al-Munafiqoon", "At-Taghabun",
                               "At-Talaq", "At-Tahreem", "Al-Mulk", "Al-Qalam", "Al-Haaqqa",
                               "Al-Ma'aarij", "Nooh", "Al-Jinn", "Al-Muzzammil", "Al-Muddaththir",
                               "Al-Qiyamah", "Al-Insaan", "Al-Mursalaat", "An-Naba'", "An-Naazi'aat",
                               "Abasa", "At-Takweer", "Al-Infitar", "Al-Mutaffifeen",
                               "Al-Inshiqaaq", "Al-Burooj", "At-Taariq", "Al-A'laa", "Al-Ghaashiyah",
                               "Al-Fajr", "Al-Balad", "Ash-Shams", "Al-Layl", "Ad-Dhuha",
                               "Ash-Sharh (Al-Inshirah)", "At-Teen", "Al-Alaq", "Al-Qadr",
                               "Al-Bayyinahh", "Az-Zalzalah", "Al-'Aadiyaat", "Al-Qaari'ah",
                               "At-Takaathur", "Al-'Asr", "Al-Humazah", "Al-Feel", "Quraysh",
                               "Al-Maa'oon", "Al-Kawthar", "Al-Kaafiroon", "An-Nasr", "Al-Masad",
                               "Al-Ikhlaas", "Al-Falaq", "Al-Naas"};

    QString temp1;

    for (int i = 1; i <115; i++)
    {
        temp1 = temp1 + QString::number(i) + ". " + quranChapters[i -1] +  "\n";
    }

    font.setPixelSize(16);
    textLabel = new QLabel(temp1, this);
    textLabel->setFont(font);
    textLabel->setWordWrap(1);

    myGrid->addWidget(textLabel);
    myScroll = new QScrollArea(this);

    myScroll->setWidgetResizable(1);
    myScroll->setWidget(textLabel);

    myGrid->addWidget(myScroll);

    repaint();
}





