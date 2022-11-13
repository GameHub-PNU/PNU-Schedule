#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utiltydb.h"

#include <QDebug>
#include <QString>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Some little example
    UtilityDB* utilityDb = new UtilityDB();
    utilityDb->dropTable("KN_31");

    utilityDb->createScheduleTable("KN_31");

    Schedule schedule;
    schedule.groupName = "KN-31";

    QVector<ScheduleList> *groupSchedule = new QVector<ScheduleList>();
    ScheduleList scheduleList;
    scheduleList.date = "22.03.2001";
    scheduleList.nameOfDay = "monday";
    scheduleList.numOfCouple = 2;
    scheduleList.timeStapOfCouple = "12:00 - 13:20";
    scheduleList.coupleDesc =  "C++";
    groupSchedule->push_back(scheduleList);
    schedule.groupSchedule = groupSchedule;
    utilityDb -> insertSchedultToTable("KN_31", schedule);
    Schedule schedule1 = utilityDb ->getScheduleByTableName("KN_31");
    qDebug() << utilityDb -> isTableExists("KN_31");
}

MainWindow::~MainWindow()
{
    delete ui;
}

