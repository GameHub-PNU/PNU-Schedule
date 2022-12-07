#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utilitydb.h"
#include "parse.h"
#include "schedule.h"

#include <algorithm>
#include <QDebug>
#include <QFile>
#include <QString>
#include <QTextCodec>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    parser = new Parse();
    fileDownloader = new FileDownloader(QUrl("https://asu.pnu.edu.ua/data/groups-list.js"), this);
    connect(fileDownloader, SIGNAL(downloaded()), this, SLOT(loadAllGroups()));

    // Some little example
    UtilityDB* utilityDb = new UtilityDB();
    //utilityDb->dropTable("KN_31");

    //utilityDb->createScheduleTable("KN_31");

    Schedule schedule;
    schedule.groupName = "KN-31";

    QList<ScheduleList> *groupSchedule = new QList<ScheduleList>();
    ScheduleList scheduleList;
    scheduleList.date = "22.05.2001";
    scheduleList.nameOfDay = "monday";
    scheduleList.numOfCouple = 2;
    scheduleList.timeStapOfCouple = "12:00 - 13:20";
    scheduleList.coupleDesc =  "C++";
    groupSchedule->push_back(scheduleList);
    schedule.groupSchedule = groupSchedule;
    utilityDb -> insertScheduleToTable("KN_31", schedule);
    Schedule schedule1 = utilityDb ->getScheduleByTableName("KN_31");
    Schedule schedule2 = utilityDb ->getScheduleByTableNameInRange("KN_31", QDate(2001, 3, 22), QDate(2002, 3, 22));
    qDebug() << utilityDb -> doesTableExist("KN_31");
    
    //Parse *parse = new Parse();
    //Schedule newList = parse->parseSchedule("https://asu.pnu.edu.ua/static/groups/1002/1002-0732.html");
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::loadAllGroups()
{
    QByteArray response = fileDownloader->getDownloadedData();
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QString jsFileContent = codec->toUnicode(response);
    groups = parser->parseJSFileWithAllGroups(jsFileContent);
}

void MainWindow::on_getScheduleButton_clicked()
{
   auto iterObject = std::find_if(groups.begin(), groups.end(),
                                [this](UniversityGroup group){ return group.name == ui->groupNameLineEdit->text().trimmed();});

   if (iterObject != groups.end()) {
       QString groupUnitCode = QString::number(iterObject->unitCode);
       int amountOfDigitsInMaxGroupNumber = QString::number(groups.length()).length();
       QString groupSchedulelink = "https://asu.pnu.edu.ua/static/groups/" + groupUnitCode + '/' + groupUnitCode + '-'
               + QStringLiteral("%1").arg(iterObject->sequenceNumber, amountOfDigitsInMaxGroupNumber, 10, QLatin1Char('0')) + ".html";
       Schedule testScheduleList = parser->parseSchedule(groupSchedulelink);
   }
   else {
       QMessageBox::critical(this, "Error", "There is no such group at the university!");
   }
}

