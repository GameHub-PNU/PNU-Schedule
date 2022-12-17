#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <QString>
#include <universityclass.h>

class Schedule
{
public:
    QString groupName;
    QList<UniversityClass> *groupSchedule;
    
    Schedule();
    Schedule(QString name, QList<UniversityClass> *groupSchedule);
};

#endif // SCHEDULE_H
