#include "schedule.h"






Schedule::Schedule()
{

}

Schedule::Schedule(QString name, QList<ScheduleList> *groupSchedule)
{
    Schedule::groupName = name;
    Schedule::groupSchedule = groupSchedule;
}


