#ifndef PARSER_H
#define PARSER_H

#include "universitygroup.h"
#include <schedule.h>
#include <universityclass.h>
#include <QFile>

class Parser
{
public:
    Parser();

    Schedule parseSchedule(QString scheduleContent);
    QVector<UniversityGroup> parseJSFileWithAllGroups(QString input);

   QString toLat(QString word);
private:

    QString getName(QString insertedHtml, QString inputedRegex);

    QList<UniversityClass> * getSchedule(QString html);

    void addScheduleList(QString valueString, QList<UniversityClass> *list);


};

#endif // PARSER_H
