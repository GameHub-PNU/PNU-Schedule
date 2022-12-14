#include "parser.h"

#include <QDebug>
#include <QRegularExpression>

Schedule Parser::parseSchedule(QString scheduleContent)
{
    QString nameOfGroup = "<a title=\"Постійне посилання на тижневий розклад\" style=font-size:1\\.4em>([\\S\\s]*?)</a><br>";

    Schedule *finalResult = new Schedule(getName(scheduleContent, nameOfGroup),getSchedule(scheduleContent));

    return *finalResult;
}

QVector<UniversityGroup> Parser::parseJSFileWithAllGroups(QString input)
{
    const int quantityOfGroups = 1200;
    QVector<UniversityGroup> allGroups(quantityOfGroups);
    QRegularExpression getGroupsDataRegex(R"(f:[\s\S]*?(\d{4,15}),[\s\S]*?i:[\s\S]*?(\d{1,15}),[\s\S]*?l:[\s\S]*?\'(.*)\')",
                                          QRegularExpression::MultilineOption | QRegularExpression::UseUnicodePropertiesOption);
    QRegularExpressionMatchIterator iterator = getGroupsDataRegex.globalMatch(input);
    int counterValue = 0;
    while (iterator.hasNext()) {
        QRegularExpressionMatch match = iterator.next();
        UniversityGroup group;
        group.unitCode = match.captured(1).toInt();
        group.sequenceNumber = match.captured(2).toInt();
        group.name = match.captured(3);
        group.name.replace(" ", "");
        allGroups[counterValue++] = group;
    }
    allGroups.resize(counterValue);
    return allGroups;
}

QString Parser::getName(QString insertedHtml, QString inputedRegex)
{
    QRegularExpression reg(inputedRegex);

    QStringList list;
    QRegularExpressionMatch match = reg.match(insertedHtml);
    if(match.hasMatch()){
        return match.captured(1);
    }
    return "UNKNOWN";
}

QList<UniversityClass> * Parser::getSchedule(QString html)
{
    QList<UniversityClass> *finalList = new QList<UniversityClass>();
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

void Parser::addScheduleList(QString valueString, QList<UniversityClass> * list)
{
    QString scheduleSearchString = "<td>([\\S\\s]*?)<td>([\\s\\S]*?)<br>([\\s\\S]*?)<td style=max-width:340px;overflow:hidden>([\\S\\s]*?)(<tr>|</table>)";
    QString datePicker = "<h4>([\\S\\s]*?)<small>([\\S\\s]*?)</small>";
    QString addLinkString = "<img src=./../check.jpg>\\n<span class=remote_work>дист.</span>([\\S\\s]*?)<div class=link> <a href=\\\"([\\s\\S]*?)\\\">";


    QRegularExpression reg(scheduleSearchString);
    QRegularExpression regDate(datePicker);
    QRegularExpression regLink(addLinkString);

    QRegularExpressionMatchIterator i = reg.globalMatch(valueString);
    while(i.hasNext()){
        UniversityClass *newObj;
        QRegularExpressionMatch match = i.next();
        if(match.captured(4) != NULL){

            QString timeStap = match.captured(2) + "-" + match.captured(3);
            QRegularExpressionMatch matchLocal = regLink.match(match.captured(4).replace("<br>", " "));

            if(matchLocal.hasMatch()){
                QString result = matchLocal.captured(1) + " " + matchLocal.captured(2);
                newObj = new UniversityClass(QDate::fromString(regDate.match(valueString).captured(1).replace("\n",""),"dd.MM.yyyy"), regDate.match(valueString).captured(2),
                                                        match.captured(1).toInt(), timeStap, result);
            }
            else{
                newObj = new UniversityClass(QDate::fromString(regDate.match(valueString).captured(1).replace("\n",""),"dd.MM.yyyy"), regDate.match(valueString).captured(2),
                                                        match.captured(1).toInt(), timeStap, match.captured(4).replace("<br>", " "));
            }
            list->append(*newObj);
        }
    }
}
