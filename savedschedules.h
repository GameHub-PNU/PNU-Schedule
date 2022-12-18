#ifndef SAVEDSCHEDULES_H
#define SAVEDSCHEDULES_H

#include <QDialog>
#include <QSettings>

namespace Ui {
class SavedSchedules;
}

class SavedSchedules : public QDialog
{
    Q_OBJECT

public:
    explicit SavedSchedules(QWidget *parent = nullptr);
    ~SavedSchedules();
    QString getRequestedScheduleToModify();
    QStringList getListOfSchedulesToDelete();

private:
    Ui::SavedSchedules *ui;
    QString requestedScheduleToModify = NULL;
    QStringList listOfSchedulesToDelete;
    QStringList schedules;
    void disableButtonsOnEmptySchedules();

private slots:
    void on_updateDataButton_clicked();
    void on_deleteSelectedButton_clicked();
};

#endif // SAVEDSCHEDULES_H
