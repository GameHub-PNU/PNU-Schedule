#include "schedule.h"

Schedule::Schedule()
{

}

Schedule::Schedule(QString name, QList<UniversityClass> *groupSchedule)
{
    Schedule::groupName = name;
    Schedule::groupSchedule = groupSchedule;
}
