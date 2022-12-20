#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "network/webfiledownloader.h"
#include "network/scheduleupdater.h"
#include "parser.h"
#include "utilitydb.h"
#include "schedule.h"
#include "universitygroup.h"

#include <QByteArray>
#include <QDate>
#include <QMainWindow>
#include <QStringList>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void loadAllGroups();
    void getUpdatedSchedule(bool);
    void on_getScheduleButton_clicked();


    void on_startDateCalendarWidget_clicked(const QDate&);

    void on_endDateCalendarWidget_clicked(const QDate&);

    void on_savedSchedulesButton_clicked();

    void on_startDateCalendarWidget_currentPageChanged(int year, int month);

    void on_endDateCalendarWidget_currentPageChanged(int year, int month);

private:
    Ui::MainWindow *ui;

    ScheduleUpdater *scheduleUpdater;
    WebFileDownloader *webFileDownloader = nullptr;
    UtilityDB *db;
    Parser *parser;
    Schedule schedule;
    QVector<UniversityGroup> universityGroups;
    QDate startFilterDate = QDate::currentDate();
    QDate endFilterDate = QDate::currentDate();

    //const QString GROUPS_FILE_PATH = "./../../../../PNU-Schedule/database/university-groups.txt";

    // My (YuraRov) GROUPS_FILE_PATH, others should comment the line below and uncomment above
    const QString GROUPS_FILE_PATH = "./../PNU-Schedule/database/university-groups.txt";

    void applicationSetup();
    void saveGroupsToFile();
    void fillScheduleTable();
    void congratulateUser();
    int showDownloadedScheduleDialogToUser();
    QVector<UniversityClass> filterSchedule();
    Schedule getSchedule(UniversityGroup *group);
    void saveUpdatedSchedule();
};
#endif // MAINWINDOW_H
