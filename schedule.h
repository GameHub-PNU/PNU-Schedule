#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <QString>
#include "schedulelist.h"


class Schedule
{
public:
    QString groupName;
    QVector<ScheduleList> *groupSchedule;

    Schedule();
};

#endif // SCHEDULE_H
