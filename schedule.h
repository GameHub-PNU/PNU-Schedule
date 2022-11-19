#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <QString>
#include <schedulelist.h>

class Schedule
{
public:
    QString groupName;
    QList<ScheduleList> *groupSchedule;
    
    Schedule();
    Schedule(QString name, QList<ScheduleList> *groupSchedule);
};

#endif // SCHEDULE_H
