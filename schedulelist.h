#ifndef SCHEDULELIST_H
#define SCHEDULELIST_H

#include <QString>

class ScheduleList
{
public:
    QString date;
    QString nameOfDay;

    int numOfCouple;
    QString timeStapOfCouple;

    QString coupleDesc;

    ScheduleList();
    ScheduleList(QString date, QString nameOfDay, int numOfCouple, QString timeStapOfCouple, QString coupleDesc);


};

#endif // SCHEDULELIST_H
