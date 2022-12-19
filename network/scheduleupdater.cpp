#include "scheduleupdater.h"

#include <QByteArray>
#include <QString>
#include <QTextCodec>

ScheduleUpdater::ScheduleUpdater(UniversityGroup* group, int quantityOfAllGroups, QObject *parent)
    : QObject(parent)
{
    updateTimer = new QTimer();
    webFileDownloader = new WebFileDownloader();
    parser = new Parser();
    groupToUpdate = group;
    quantityOfGroups = quantityOfAllGroups;
    connect(webFileDownloader, SIGNAL(downloaded()), this, SLOT(readResponseFromScheduleServer()));
    connect(updateTimer, SIGNAL(timeout()), this, SLOT(updateSchedule()));
    updateTimer->start(frequencyUpdate);
}

ScheduleUpdater::~ScheduleUpdater() {
    delete updateTimer;
    delete webFileDownloader;
    delete parser;
    delete groupToUpdate;
}

void ScheduleUpdater::updateSchedule()
{
    QString groupSchedulelink = generateGroupScheduleURL();
    webFileDownloader->sendGetHttpRequest(groupSchedulelink);
}

QString ScheduleUpdater::generateGroupScheduleURL()
{
    QString groupUnitCode = QString::number(groupToUpdate->unitCode);
    int amountOfDigitsInMaxGroupNumber = QString::number(quantityOfGroups).length();
    QString groupSchedulelink = "https://asu.pnu.edu.ua/static/groups/" + groupUnitCode + '/'
            + groupUnitCode + '-'
            + QStringLiteral("%1").arg(groupToUpdate->sequenceNumber, amountOfDigitsInMaxGroupNumber,
                                       10, QLatin1Char('0')) + ".html";
    return groupSchedulelink;
}

Schedule ScheduleUpdater::readResponseFromScheduleServer()
{
    QByteArray response = webFileDownloader->getDownloadedData();
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QString fileContent = codec->toUnicode(response);
    return parser->parseSchedule(fileContent);
}
