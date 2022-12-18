#include "savedschedules.h"
#include "ui_savedschedules.h"

SavedSchedules::SavedSchedules(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SavedSchedules)
{
    ui->setupUi(this);

    ui -> savedSchedulesTable -> setColumnCount(2);
    ui -> savedSchedulesTable -> setColumnWidth(0, 130);
    ui -> savedSchedulesTable -> setColumnWidth(1, 200);
    QStringList headers;
    headers.append("Schedule");
    headers.append("Last update");
    ui -> savedSchedulesTable->setHorizontalHeaderLabels(headers);

    QSettings settings("Saved", "Schedules");
    settings.beginGroup("schedule");
    schedules = settings.childKeys();

    int i=0;
    for (QString schedule : schedules) {
        ui -> savedSchedulesTable -> insertRow (ui->savedSchedulesTable->rowCount());
        QTableWidgetItem *cellScheduleName = new QTableWidgetItem("");
        QTableWidgetItem *cellLastUpdate = new QTableWidgetItem("");
        cellScheduleName -> setText(schedule);
        cellLastUpdate -> setText(settings.value(schedule).toString());
        ui -> savedSchedulesTable -> setItem(i,0,cellScheduleName);
        ui -> savedSchedulesTable -> setItem(i,1,cellLastUpdate);
        i++;
    }

    disableButtonsOnEmptySchedules();
}

SavedSchedules::~SavedSchedules()
{
    delete ui;
}

QString SavedSchedules::getRequestedScheduleToModify()
{
    return requestedScheduleToModify;
}

QStringList SavedSchedules::getListOfSchedulesToDelete()
{
    return listOfSchedulesToDelete;
}

void SavedSchedules::disableButtonsOnEmptySchedules()
{
    if (schedules.isEmpty()) {
        ui -> deleteSelectedButton -> setEnabled(false);
        ui -> updateDataButton -> setEnabled(false);
    }
}

void SavedSchedules::on_updateDataButton_clicked()
{
    QModelIndex currentIndex = ui->savedSchedulesTable->currentIndex();
    requestedScheduleToModify = ui->savedSchedulesTable->item(currentIndex.row(), 0)->text();
    accept();
}


void SavedSchedules::on_deleteSelectedButton_clicked()
{
    QModelIndex currentIndex = ui->savedSchedulesTable->currentIndex();
    QString schedule = ui->savedSchedulesTable->item(currentIndex.row(), 0)->text();
    ui->savedSchedulesTable->removeRow(currentIndex.row());
    ui->savedSchedulesTable->setCurrentCell(0, 0);
    QSettings settings("Saved", "Schedules");
    settings.beginGroup("schedule");
    settings.remove(schedule);
    schedules.removeOne(schedule);
    listOfSchedulesToDelete.push_back(schedule);
    disableButtonsOnEmptySchedules();
}

