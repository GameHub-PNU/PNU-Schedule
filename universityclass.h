#ifndef UNIVERSITYCLASS_H
#define UNIVERSITYCLASS_H

#include <QString>
#include <QDate>

class UniversityClass
{
public:
    QDate date;
    QString nameOfDay;

    int numberOfClass;
    QString timeStampOfClass;

    QString classDescription;

    UniversityClass();
    UniversityClass(QDate date, QString nameOfDay, int numOfCouple, QString timeStapOfCouple, QString coupleDesc);
};

#endif // UNIVERSITYCLASS_H
