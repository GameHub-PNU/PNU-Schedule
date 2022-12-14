#include "schedulelist.h"

ScheduleList::ScheduleList()
{

}

ScheduleList::ScheduleList(QDate date, QString nameOfDay, int numOfCouple, QString timeStapOfCouple, QString coupleDesc)
{
    this->date = date;
    this->nameOfDay = nameOfDay;
    this->numOfCouple = numOfCouple;
    this->timeStapOfCouple = timeStapOfCouple;
    this->coupleDesc = coupleDesc;
}
