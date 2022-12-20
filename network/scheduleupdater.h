#ifndef SCHEDULEUPDATER_H
#define SCHEDULEUPDATER_H

#include "parser.h"
#include "schedule.h"
#include "universitygroup.h"
#include "network/webfiledownloader.h"

#include <chrono>

#include <QObject>
#include <QTimer>
#include <QVector>

class ScheduleUpdater : public QObject
{
    Q_OBJECT
public:
    explicit ScheduleUpdater(const QVector<UniversityGroup>&, QObject *parent = nullptr);
    virtual ~ScheduleUpdater();
    void immediateGroupScheduleUpdate(UniversityGroup, bool);
    Schedule getUpdatedSchedule();
signals:
    void updated(bool);

public slots:
    void updateGroupsSchedule();

private slots:
    void readResponseFromScheduleServer();

private:
    QTimer* updateTimer;

    WebFileDownloader* webFileDownloader;
    Parser* parser;
    QVector<UniversityGroup> groupsToUpdate;
    std::chrono::hours frequencyUpdate;

    Schedule updatedSchedule;
    bool isImmediateUpdateFromDialog = false;
};

#endif // SCHEDULEUPDATER_H
