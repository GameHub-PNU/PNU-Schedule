#ifndef PARSER_H
#define PARSER_H

#include "universitygroup.h"
#include "schedule.h"
#include "universityclass.h"

#include <QFile>

class Parser
{
public:
    Schedule parseSchedule(QString scheduleContent);
    QVector<UniversityGroup> parseJSFileWithAllGroups(QString input);
private:

    QString getName(QString insertedHtml, QString inputedRegex);

    QList<UniversityClass> * getSchedule(QString html);

    void addScheduleList(QString valueString, QList<UniversityClass> *list);


};

#endif // PARSER_H
