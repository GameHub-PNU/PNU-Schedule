#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utilitydb.h"
#include "parser.h"
#include "schedule.h"
#include "savedschedules.h"

#include <algorithm>

#include <QCompleter>
#include <QDebug>
#include <QFile>
#include <QString>
#include <QTableWidgetItem>
#include <QTextCodec>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    applicationSetup();
    fileDownloader = new FileDownloader(QUrl("https://asu.pnu.edu.ua/data/groups-list.js"), this);
    connect(fileDownloader, SIGNAL(downloaded()), this, SLOT(loadAllGroups()));

    // Some little example
    /*
     *
    UtilityDB* utilityDb = new UtilityDB();
    utilityDb->dropTable("КН-41");

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
    */
    //Parse *parse = new Parse();
    //Schedule newList = parse->parseSchedule("https://asu.pnu.edu.ua/static/groups/1002/1002-0732.html");
}

MainWindow::~MainWindow()
{
    delete ui;
    delete fileDownloader;
    delete db;
    delete parser;
}

void MainWindow::applicationSetup()
{
    db = new UtilityDB();
    parser = new Parser();
    ui->scheduleTableWidget->setVisible(false);
    ui->startDateCalendarWidget->setSelectedDate(QDate::currentDate());
    ui->endDateCalendarWidget->setSelectedDate(QDate::currentDate().addDays(1));
}

void MainWindow::fillScheduleTable(const Schedule &schedule)
{
    QStringList header;
    header << "Дата" << "День" << "Номер пари" << "Час" << "Опис пари";
    ui->scheduleTableWidget->setColumnCount(header.size());
    ui->scheduleTableWidget->setHorizontalHeaderLabels(header);
    ui->scheduleTableWidget->setRowCount(schedule.groupSchedule->size());
    for (int i = 0; i < schedule.groupSchedule->size(); ++i) {
        ui->scheduleTableWidget->setItem(i, 0, new QTableWidgetItem(schedule.groupSchedule->at(i).date.toString("dd-MM-yyyy")));
        ui->scheduleTableWidget->setItem(i, 1, new QTableWidgetItem(QString(schedule.groupSchedule->at(i).nameOfDay).replace("&#39;", "'")));
        ui->scheduleTableWidget->setItem(i, 2, new QTableWidgetItem(QString::number(schedule.groupSchedule->at(i).numberOfClass)));
        ui->scheduleTableWidget->setItem(i, 3, new QTableWidgetItem(schedule.groupSchedule->at(i).timeStampOfClass));
        ui->scheduleTableWidget->setItem(i, 4, new QTableWidgetItem(schedule.groupSchedule->at(i).classDescription));
    }
    ui->scheduleTableWidget->setVisible(true);
}


void MainWindow::loadAllGroups()
{
    QByteArray response = fileDownloader->getDownloadedData();
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QString jsFileContent = codec->toUnicode(response);
    groups = parser->parseJSFileWithAllGroups(jsFileContent);
    QStringList groupNames;
    std::for_each(groups.begin(), groups.end(), [this, &groupNames](UniversityGroup group) {
        ui->allGroupsComboBox->addItem(group.name);
        groupNames << group.name;
    });
    QCompleter *comboBoxCompleter = new QCompleter(groupNames, this);
    comboBoxCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    ui->allGroupsComboBox->setCompleter(comboBoxCompleter);
}

void MainWindow::on_getScheduleButton_clicked()
{
    auto chosenGroup = std::find_if(groups.begin(), groups.end(),
                                   [this](UniversityGroup group){ return group.name == ui->allGroupsComboBox->currentText().trimmed(); });

    if (chosenGroup != groups.end()) {
        Schedule schedule;
        if (db->doesTableExist(chosenGroup->name)) {
            qDebug() << "EXIST";
            schedule = db->getScheduleByTableNameInRange(chosenGroup->name, startFilterDate, endFilterDate);
        }
        else {
            schedule = getSchedule(chosenGroup);

            QMessageBox msgBox;
            msgBox.setText("The schedule has been dowloaded");
            msgBox.setInformativeText("Do you want to save it?");
            msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard);
            msgBox.setDefaultButton(QMessageBox::Save);
            QSettings settings("Saved", "Schedules");
            settings.beginGroup("schedule");

            switch (msgBox.exec()) {
                case QMessageBox::Save:
                    db->createScheduleTable(schedule.groupName);
                    // TODO: the database saving process should be done asynchronously
                    //std::future<void> insertDBFuture = std::async(std::launch::async, &UtilityDB::insertScheduleToTable,
                    //                                            testScheduleList.groupName, testScheduleList);
                    db->insertScheduleToTable(schedule.groupName, schedule);
                    settings.setValue(schedule.groupName, QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm"));
                    break;
                case QMessageBox::Discard:
                    break;
                default:
                    break;
            }
        }

        fillScheduleTable(schedule);
    }
    else {
        QMessageBox::critical(this, "Error", "There is no such group at the university!");
    }
}


void MainWindow::on_startDateCalendarWidget_clicked(const QDate &date) {
    startFilterDate = date;
    ui->endDateCalendarWidget->setMinimumDate(startFilterDate.addDays(1));
}

void MainWindow::on_endDateCalendarWidget_clicked(const QDate &date) {
    endFilterDate = date;
    ui->startDateCalendarWidget->setMaximumDate(endFilterDate.addDays(-1));
}

void MainWindow::on_savedSchedulesButton_clicked()
{
    QScopedPointer<SavedSchedules> dlg(new SavedSchedules());
    dlg->exec();

    QStringList schedulesToDelete = dlg->getListOfSchedulesToDelete();
    for (QString schedule : schedulesToDelete) {
        db->dropTable(schedule);
    }

    QString scheduleToUpdate = dlg -> getRequestedScheduleToModify();
    if (scheduleToUpdate != NULL) {
        QSettings settings("Saved", "Schedules");
        settings.beginGroup("schedule");
        settings.setValue(scheduleToUpdate, QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm"));
        db->clearTable(scheduleToUpdate);
        auto groupToUpdate = std::find_if(groups.begin(), groups.end(),
                                          [&scheduleToUpdate](UniversityGroup group){ return group.name == scheduleToUpdate;});
        db->insertScheduleToTable(scheduleToUpdate, getSchedule(groupToUpdate));

        QMessageBox::information(this, "Update schedule information", "Schedule " + scheduleToUpdate + " was successfully updated!");
    }

}

Schedule MainWindow::getSchedule(UniversityGroup* group)
{
    QString groupUnitCode = QString::number(group->unitCode);
    int amountOfDigitsInMaxGroupNumber = QString::number(groups.length()).length();
    QString groupSchedulelink = "https://asu.pnu.edu.ua/static/groups/" + groupUnitCode + '/' + groupUnitCode + '-'
            + QStringLiteral("%1").arg(group->sequenceNumber, amountOfDigitsInMaxGroupNumber, 10, QLatin1Char('0')) + ".html";

    return parser->parseSchedule(groupSchedulelink);
}


void MainWindow::on_startDateCalendarWidget_currentPageChanged(int year, int month)
{
    startFilterDate = QDate(year, month, 1);
    ui->startDateCalendarWidget->setSelectedDate(startFilterDate);
    emit ui->startDateCalendarWidget->clicked(startFilterDate);
}


void MainWindow::on_endDateCalendarWidget_currentPageChanged(int year, int month)
{
    endFilterDate = QDate(year, month, 1);
    ui->endDateCalendarWidget->setSelectedDate(endFilterDate);
    emit ui->endDateCalendarWidget->clicked(endFilterDate);
}

