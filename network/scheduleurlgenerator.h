#ifndef SCHEDULEURLGENERATOR_H
#define SCHEDULEURLGENERATOR_H

#include "universitygroup.h"

#include <QString>

class ScheduleURLGenerator
{
public:
    static QString generateGroupScheduleURL(int groupUnitCode, int groupSequenceNumber);
};

#endif // SCHEDULEURLGENERATOR_H
