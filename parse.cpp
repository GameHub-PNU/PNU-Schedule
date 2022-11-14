#include "parse.h"
#include <QDebug>
#include <QRegularExpression>

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

    QString nameOfGroup = "<a title=\"Постійне посилання на тижневий розклад\" style=font-size:1\.4em>([\\S\\s]*?)</a><br>";

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

    QString globalSearchString = "<h4>[\\s\\S]*?</table></div>";
    QRegularExpression reg(globalSearchString);



    QStringList globalBlocksList;
    QRegularExpressionMatchIterator i = reg.globalMatch(html);

    while(i.hasNext()){
        QRegularExpressionMatch match = i.next();
        QString word = match.captured(0);
        globalBlocksList.append(word);
    }

    for(int i = 0; i<globalBlocksList.count(); i++){
        addScheduleList(globalBlocksList[i], finalList);
    }

    return finalList;
}

void Parse::addScheduleList(QString valueString, QList<ScheduleList> * list)
{
    QString scheduleSearchString = "<td>([\\S\\s]*?)<td>([\\s\\S]*?)<br>([\\s\\S]*?)<td style=max-width:340px;overflow:hidden>([\\S\\s]*?)<tr>";
    QString datePicker = "<h4>([\\S\\s]*?)<small>([\\S\\s]*?)</small>";
    QString addLinkString = "<img src=./../check.jpg>\\n<span class=remote_work>дист.</span>([\\S\\s]*?)<div class=link> <a href=\\\"([\\s\\S]*?)\\\">";


    QRegularExpression reg(scheduleSearchString);
    QRegularExpression regDate(datePicker);
    QRegularExpression regLink(addLinkString);

    QRegularExpressionMatchIterator i = reg.globalMatch(valueString);
    while(i.hasNext()){
        ScheduleList *newObj;
        QRegularExpressionMatch match = i.next();
        if(match.captured(4) != NULL){

            QString timeStap = match.captured(2) + "-" + match.captured(3);
            QRegularExpressionMatch matchLocal = regLink.match(match.captured(4).replace("<br>", " "));

            if(matchLocal.hasMatch()){
                QString result = matchLocal.captured(1) + " " + matchLocal.captured(2);
                newObj = new ScheduleList(regDate.match(valueString).captured(1).replace("\n"," "), regDate.match(valueString).captured(2),
                                                        match.captured(1).toInt(), timeStap, result);
            }
            else{
                newObj = new ScheduleList(regDate.match(valueString).captured(1).replace("\n"," "), regDate.match(valueString).captured(2),
                                                        match.captured(1).toInt(), timeStap, match.captured(4).replace("<br>", " "));
            }
            list->append(*newObj);
        }
    }
}
