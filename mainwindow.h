#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "network/filedownloader.h"
#include "parser.h"
#include "utilitydb.h"

#include <QDate>
#include <QMainWindow>

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

private:
    Ui::MainWindow *ui;
    FileDownloader *fileDownloader;
    UtilityDB *db;
    Parser *parser;
    QStringList savedShedulesNames;
    QVector<UniversityGroup> groups;
    QDate startFilterDate = QDate::currentDate();
    QDate endFilterDate = QDate::currentDate();

    Schedule getSchedule(UniversityGroup *group);

};
#endif // MAINWINDOW_H
