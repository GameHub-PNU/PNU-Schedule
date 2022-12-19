#ifndef SCHEDULEUPDATER_H
#define SCHEDULEUPDATER_H

#include "parser.h"
#include "schedule.h"
#include "universitygroup.h"
#include "network/webfiledownloader.h"

#include <chrono>

#include <QObject>
#include <QTimer>

class ScheduleUpdater : public QObject
{
    Q_OBJECT
public:
    explicit ScheduleUpdater(UniversityGroup* group, int quantityOfAllGroups, QObject *parent = nullptr);
    virtual ~ScheduleUpdater();

private slots:
    void updateSchedule();
    Schedule readResponseFromScheduleServer();

private:
    QTimer* updateTimer;

    WebFileDownloader* webFileDownloader;
    Parser* parser;
    UniversityGroup* groupToUpdate;
    int quantityOfGroups;
    std::chrono::hours frequencyUpdate;

    QString generateGroupScheduleURL();

};

#endif // SCHEDULEUPDATER_H
