#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "network/filedownloader.h"
#include "parse.h"

#include <QDate>
#include <QMainWindow>
#include <QtSql>
#include <QSqlDatabase>


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

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    FileDownloader *fileDownloader;
    Parse* parser;

    QVector<UniversityGroup> groups;
    QDate startFilterDate;
    QDate endFilterDate;
};
#endif // MAINWINDOW_H
