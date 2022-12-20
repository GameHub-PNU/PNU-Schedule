#include "scheduleurlgenerator.h"

QString ScheduleURLGenerator::generateGroupScheduleURL(int groupUnitCode, int groupSequenceNumber)
{
    QString unitCode = QString::number(groupUnitCode);
    int amountOfDigitsInMaxGroupNumber = 4;
    QString groupSchedulelink = "https://asu.pnu.edu.ua/static/groups/" + unitCode + '/'
            + unitCode + '-'
            + QStringLiteral("%1").arg(groupSequenceNumber, amountOfDigitsInMaxGroupNumber,
                                       10, QLatin1Char('0')) + ".html";
    return groupSchedulelink;
}
