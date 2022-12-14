#ifndef SCHEDULELIST_H
#define SCHEDULELIST_H

#include <QString>
#include <QDate>

class ScheduleList
{
public:
    QDate date;
    QString nameOfDay;

    int numOfCouple;
    QString timeStapOfCouple;

    QString coupleDesc;

    ScheduleList();
    ScheduleList(QDate date, QString nameOfDay, int numOfCouple, QString timeStapOfCouple, QString coupleDesc);
};

#endif // SCHEDULELIST_H
