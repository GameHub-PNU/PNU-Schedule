#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utilitydb.h"
#include "parser.h"
#include "schedule.h"
#include "savedschedules.h"

#include <algorithm>
#include <iterator>

#include <QCompleter>
#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include <QString>
#include <QTableWidgetItem>
#include <QTextCodec>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    applicationSetup();
    if (!QFile::exists(GROUPS_FILE_PATH)) {
        webFileDownloader = new WebFileDownloader(this);
        connect(webFileDownloader, SIGNAL(downloaded()), this, SLOT(loadAllGroups()));
        webFileDownloader->sendGetHttpRequest(QUrl("https://asu.pnu.edu.ua/data/groups-list.js"));
    }
    else {
        QFile fileWithUniversityGroups(GROUPS_FILE_PATH);
        if (fileWithUniversityGroups.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream stream(&fileWithUniversityGroups);
            while (!fileWithUniversityGroups.atEnd()) {
                UniversityGroup group;
                stream >> group.unitCode >> group.sequenceNumber >> group.name;
                ui->allGroupsComboBox->addItem(group.name);
                universityGroups.push_back(group);
            }

            fileWithUniversityGroups.close();
        }
    }
    scheduleUpdater = new ScheduleUpdater({});
    connect(scheduleUpdater, SIGNAL(updated(bool)), this, SLOT(getUpdatedSchedule(bool)));
}

MainWindow::~MainWindow()
{
    delete ui;
    if (webFileDownloader != nullptr) {
        delete webFileDownloader;
    }
    delete db;
    delete parser;
    delete scheduleUpdater;
}

void MainWindow::applicationSetup()
{
    db = new UtilityDB();
    parser = new Parser();

    ui->startDateCalendarWidget->setSelectedDate(startFilterDate);
    ui->startDateCalendarWidget->setMaximumDate(endFilterDate.addDays(-1));
    ui->endDateCalendarWidget->setSelectedDate(endFilterDate);
    ui->endDateCalendarWidget->setMinimumDate(startFilterDate.addDays(1));

    QStringList header;
    header << "Дата" << "День" << "Номер пари" << "Час" << "Опис пари";
    ui->scheduleTableWidget->setColumnCount(header.size());
    ui->scheduleTableWidget->setHorizontalHeaderLabels(header);
    QHeaderView* headerView = new QHeaderView(Qt::Horizontal);
    headerView->sectionResizeMode(QHeaderView::Stretch);
    headerView->setStretchLastSection(true);
    ui->scheduleTableWidget->setHorizontalHeader(headerView);
}

void MainWindow::saveGroupsToFile()
{
    QFile fileWithUniversityGroups(GROUPS_FILE_PATH);
    if (fileWithUniversityGroups.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream stream(&fileWithUniversityGroups);
        stream.setCodec("UTF-8");
        foreach (auto group, universityGroups) {
            stream << group.unitCode << ' ' << group.sequenceNumber << ' ' << group.name << '\n';
        }

        fileWithUniversityGroups.close();
    }
}

void MainWindow::fillScheduleTable()
{
    QVector<UniversityClass> filteredUniversityClasses = filterSchedule();
    if (filteredUniversityClasses.empty()) {
        congratulateUser();
        return;
    }

    ui->scheduleTableWidget->setRowCount(filteredUniversityClasses.size());
    for (int i = 0; i < filteredUniversityClasses.size(); ++i) {
        ui->scheduleTableWidget->setItem(i, 0, new QTableWidgetItem(filteredUniversityClasses[i].date.toString("dd-MM-yyyy")));
        ui->scheduleTableWidget->setItem(i, 1, new QTableWidgetItem(QString(filteredUniversityClasses[i].nameOfDay).replace("&#39;", "'")));
        ui->scheduleTableWidget->setItem(i, 2, new QTableWidgetItem(QString::number(filteredUniversityClasses[i].numberOfClass)));
        ui->scheduleTableWidget->setItem(i, 3, new QTableWidgetItem(filteredUniversityClasses[i].timeStampOfClass));
        ui->scheduleTableWidget->setItem(i, 4, new QTableWidgetItem(filteredUniversityClasses[i].classDescription));
    }
}

void MainWindow::congratulateUser()
{
    QMessageBox gratulationBox;
    gratulationBox.setText("Вітаємо, у вас пар нема :)");
    gratulationBox.setStandardButtons(QMessageBox::Ok);
    gratulationBox.exec();
}


QVector<UniversityClass> MainWindow::filterSchedule()
{
    QVector<UniversityClass> filteredUniversityClasses;
    std::copy_if(schedule.groupSchedule->begin(), schedule.groupSchedule->end(), std::back_inserter(filteredUniversityClasses),
                [this](UniversityClass universityClass) {
                    return universityClass.date >= startFilterDate && universityClass.date <= endFilterDate;
                });
    return filteredUniversityClasses;
}

void MainWindow::loadAllGroups()
{
    QByteArray response = webFileDownloader->getDownloadedData();
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QString jsFileContent = codec->toUnicode(response);
    universityGroups = parser->parseJSFileWithAllGroups(jsFileContent);
    QStringList groupNames;
    std::for_each(universityGroups.begin(), universityGroups.end(), [this, &groupNames](UniversityGroup group) {
        ui->allGroupsComboBox->addItem(group.name);
        groupNames << group.name;
    });
    QCompleter *comboBoxCompleter = new QCompleter(groupNames, this);
    comboBoxCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    ui->allGroupsComboBox->setCompleter(comboBoxCompleter);
    saveGroupsToFile();
}

void MainWindow::saveUpdatedSchedule()
{
    QSettings settings("Saved", "Schedules");
    settings.beginGroup("schedule");
    db->insertScheduleToTable(schedule.groupName, schedule);
    QString currentTime = QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm");
    settings.setValue(schedule.groupName, currentTime);
    ui->statusbar->showMessage("Дані для групи " + schedule.groupName
                               + " були успішно збережні (" + currentTime + ")", 5000);
}

void MainWindow::getUpdatedSchedule(bool wasUpdateFromDialog)
{
    schedule = scheduleUpdater->getUpdatedSchedule();
    if (wasUpdateFromDialog) {
        db->clearTable(schedule.groupName);
        saveUpdatedSchedule();
        QMessageBox::information(this, "Інформація про оновлення",
                                 "Розклад групи " + schedule.groupName + " був успішно оновлений!");
    }
    else {
        auto dialogCode = showDownloadedScheduleDialogToUser();
        switch (dialogCode) {
            case QMessageBox::Save:
            {
                db->createScheduleTable(schedule.groupName);
                saveUpdatedSchedule();
                break;
            }
            default:
                break;
        }
    }
    fillScheduleTable();
}

int MainWindow::showDownloadedScheduleDialogToUser()
{
    QMessageBox msgBox;
    msgBox.setText("Розклад було отримано");
    msgBox.setInformativeText("Чи хочете ви зберегти його?");
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard);
    msgBox.setDefaultButton(QMessageBox::Save);
    return msgBox.exec();
}

void MainWindow::on_getScheduleButton_clicked()
{
    auto chosenGroup = std::find_if(universityGroups.begin(), universityGroups.end(),
                                   [this](UniversityGroup group){ return group.name == ui->allGroupsComboBox->currentText().trimmed(); });

    if (chosenGroup != universityGroups.end()) {
        if (db->doesTableExist(chosenGroup->name)) {
            schedule = db->getScheduleByTableNameInRange(chosenGroup->name, startFilterDate, endFilterDate);
            fillScheduleTable();
        }
        else {
            scheduleUpdater->immediateGroupScheduleUpdate(*chosenGroup, false);
        }
    }
    else {
        QMessageBox::critical(this, "Помилка", "Обраної групи в університеті немає!");
    }
}

void MainWindow::on_startDateCalendarWidget_clicked(const QDate &date) {
    startFilterDate = date;
    ui->startDateCalendarWidget->setSelectedDate(startFilterDate);
    ui->endDateCalendarWidget->setMinimumDate(startFilterDate.addDays(1));
    ui->endDateCalendarWidget->setSelectedDate(endFilterDate);
}

void MainWindow::on_endDateCalendarWidget_clicked(const QDate &date) {
    endFilterDate = date;
    ui->endDateCalendarWidget->setSelectedDate(endFilterDate);
    ui->startDateCalendarWidget->setMaximumDate(endFilterDate.addDays(-1));
    ui->startDateCalendarWidget->setSelectedDate(startFilterDate);
}

void MainWindow::on_startDateCalendarWidget_currentPageChanged(int year, int month)
{
    startFilterDate = QDate(year, month, startFilterDate.day());
    ui->startDateCalendarWidget->setSelectedDate(startFilterDate);
    ui->endDateCalendarWidget->setMinimumDate(startFilterDate.addDays(1));
}

void MainWindow::on_endDateCalendarWidget_currentPageChanged(int year, int month)
{
    endFilterDate = QDate(year, month, endFilterDate.day());
    ui->endDateCalendarWidget->setSelectedDate(endFilterDate);
    ui->startDateCalendarWidget->setMaximumDate(endFilterDate.addDays(-1));
}

void MainWindow::on_savedSchedulesButton_clicked()
{
    QScopedPointer<SavedSchedules> dlg(new SavedSchedules());
    dlg->exec();

    QStringList schedulesToDelete = dlg->getListOfSchedulesToDelete();
    if (!schedulesToDelete.isEmpty()) {
        QString schedulesMessage = "";
        for (int i = 0; i < schedulesToDelete.size(); ++i) {
            db->dropTable(schedulesToDelete[i]);
            schedulesMessage.append(schedulesToDelete[i]);
            if (i < schedulesToDelete.size()-1) {
                schedulesMessage.append(", ");
            }
        }
        QMessageBox::information(this, "Інформація про видалення", "Розклади " + schedulesMessage + " - успішно видалено!");
    }

    QString scheduleToUpdate = dlg -> getRequestedScheduleToModify();
    if (scheduleToUpdate != NULL) {
        auto groupToUpdate = std::find_if(universityGroups.begin(), universityGroups.end(),
                                          [&scheduleToUpdate](UniversityGroup group){ return group.name == scheduleToUpdate;});
        scheduleUpdater->immediateGroupScheduleUpdate(*groupToUpdate, true);
    }
}
