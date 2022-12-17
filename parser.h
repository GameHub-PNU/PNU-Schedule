#ifndef PARSER_H
#define PARSER_H

#include "universitygroup.h"
#include <schedule.h>
#include <schedulelist.h>
#include <QFile>

class Parser
{
public:
    Parser();

    Schedule parseSchedule(QString htmlLink);
    QVector<UniversityGroup> parseJSFileWithAllGroups(QString input);

   QString toLat(QString word);
private:

    QString getName(QString insertedHtml, QString inputedRegex);

    QList<ScheduleList> * getSchedule(QString html);

    void addScheduleList(QString valueString, QList<ScheduleList> *list);


};

#endif // PARSER_H
