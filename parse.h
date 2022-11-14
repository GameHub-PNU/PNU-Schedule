#ifndef PARSE_H
#define PARSE_H

#include <schedule.h>
#include <schedulelist.h>
#include <QFile>

class Parse
{
public:
    Parse();

    Schedule parseSchedule(QString html);



private:

    QString getName(QString insertedHtml, QString inputedRegex);

    QList<ScheduleList> * getSchedule(QString html);

    void addScheduleList(QString valueString, QList<ScheduleList> *list);

};

#endif // PARSE_H
