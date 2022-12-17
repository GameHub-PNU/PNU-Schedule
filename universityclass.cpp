#include "universityclass.h"

UniversityClass::UniversityClass()
{

}

UniversityClass::UniversityClass(QDate date, QString nameOfDay, int numOfCouple, QString timeStapOfCouple, QString coupleDesc)
{
    this->date = date;
    this->nameOfDay = nameOfDay;
    this->numberOfClass = numOfCouple;
    this->timeStampOfClass = timeStapOfCouple;
    this->classDescription = coupleDesc;
}
