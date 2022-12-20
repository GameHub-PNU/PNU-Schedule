#include "scheduleupdater.h"
#include "scheduleurlgenerator.h"

#include <QByteArray>
#include <QString>
#include <QTextCodec>

ScheduleUpdater::ScheduleUpdater(const QVector<UniversityGroup>& groups, QObject *parent)
    : QObject(parent)
{
    updateTimer = new QTimer();
    webFileDownloader = new WebFileDownloader();
    parser = new Parser();
    groupsToUpdate = groups;
    connect(webFileDownloader, SIGNAL(downloaded()), this, SLOT(readResponseFromScheduleServer()));
    connect(updateTimer, SIGNAL(timeout()), this, SLOT(updateGroupsSchedule()));
    frequencyUpdate = std::chrono::hours(2);
    updateTimer->start(frequencyUpdate);
}

ScheduleUpdater::~ScheduleUpdater() {
    delete updateTimer;
    delete webFileDownloader;
    delete parser;
}

void ScheduleUpdater::immediateGroupScheduleUpdate(UniversityGroup group, bool isFromDialog)
{
    QString groupSchedulelink = ScheduleURLGenerator::generateGroupScheduleURL(
                group.unitCode, group.sequenceNumber);

    webFileDownloader->sendGetHttpRequest(groupSchedulelink);
    isImmediateUpdateFromDialog = isFromDialog;
}

Schedule ScheduleUpdater::getUpdatedSchedule()
{
    return updatedSchedule;
}

void ScheduleUpdater::updateGroupsSchedule()
{
    foreach (auto group, groupsToUpdate) {
        QString groupSchedulelink = ScheduleURLGenerator::generateGroupScheduleURL(
                    group.unitCode, group.sequenceNumber);

        webFileDownloader->sendGetHttpRequest(groupSchedulelink);
    }
    isImmediateUpdateFromDialog = false;
}

void ScheduleUpdater::readResponseFromScheduleServer()
{
    QByteArray response = webFileDownloader->getDownloadedData();
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QString fileContent = codec->toUnicode(response);
    updatedSchedule = parser->parseSchedule(fileContent);
    emit updated(isImmediateUpdateFromDialog);
}
