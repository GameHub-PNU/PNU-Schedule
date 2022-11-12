#include "parse.h"
#include <QDebug>
#include <QRegularExpression>



/*Date and day:
 * <h4>([\S\s]{0,10})
<small>([\S\s]{0,10})<\/small>

*/
//allSchedules

/*
(<h4>[\S\s]{0,10}
<small>[\S\s]{0,10}<\/small><\/h4>[\S\s]{0,10000}<\/table><\/div><div)|(<h4>[\S\s]{0,10}
<small>[\S\s]{0,10}<\/small><\/h4>[\S\s]{0,10000}<script)

*/



Parse::Parse()
{

}

Schedule Parse::parseSchedule(QString filePath)
{
    QFile html(filePath);

    if(!html.open(QIODevice::ReadOnly)){
        qDebug() << "Something wrong!";
    }

    QTextStream stream(&html);
    stream.setCodec("UTF-8");
    QString mainHtml = stream.readAll();
    html.close();


    QString nameOfGroup = "<a title=\"Постійне посилання на тижневий розклад\" style=\"font-size:1.4em\">([\\S\\s]{0,10})</a>";
    //finalResult->groupName = getName(mainHtml, nameOfGroup);

    Schedule *finalResult = new Schedule(getName(mainHtml, nameOfGroup),getSchedule(mainHtml));


    return *finalResult;
}


QString Parse::getName(QString insertedHtml, QString inputedRegex)
{
    QRegularExpression reg(inputedRegex);

    QStringList list;
    QRegularExpressionMatch match = reg.match(insertedHtml);
    if(match.hasMatch()){
        return match.captured(1);
    }
    return "UNKNOWN";
}

QList<ScheduleList> * Parse::getSchedule(QString html)
{
    QList<ScheduleList> *finalList = new QList<ScheduleList>();

    QString searchString = "(<h4>[\\S\\s]{0,10000}</table></div><div)|(<h4>[\\S\\s]{0,10000}<script)";

    QRegularExpression reg(searchString);

    QRegularExpressionMatch match = reg.match(html);

    qDebug() << match.hasMatch();
    qDebug() << match.capturedLength();


    for(int i = 0; i < match.capturedEnd(); i++){
        if(match.captured(i) != ""){
            qDebug() << match.captured(i);
        }
    }

    //Воно не робить по-людськи я в рот їбав

    return finalList;
}










