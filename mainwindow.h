#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "network/webfiledownloader.h"
#include "parser.h"
#include "utilitydb.h"
#include "schedule.h"
#include "universitygroup.h"

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
    void on_getScheduleButton_clicked();
    void loadAllGroups();

    void on_startDateCalendarWidget_clicked(const QDate &date);

    void on_endDateCalendarWidget_clicked(const QDate &date);

    void on_savedSchedulesButton_clicked();

    void on_startDateCalendarWidget_currentPageChanged(int year, int month);

    void on_endDateCalendarWidget_currentPageChanged(int year, int month);

private:
    Ui::MainWindow *ui;
    WebFileDownloader *webFileDownloader;
    UtilityDB *db;
    Parser *parser;
    Schedule schedule;
    QStringList savedShedulesNames;
    QVector<UniversityGroup> groups;
    QDate startFilterDate = QDate::currentDate();
    QDate endFilterDate = QDate::currentDate();

    void applicationSetup();
    void fillScheduleTable();
    void congratulateUser();
    QVector<UniversityClass> filterSchedule();
    Schedule getSchedule(UniversityGroup *group);

};
#endif // MAINWINDOW_H
