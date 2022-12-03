#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QSqlDatabase>
#include "network/filedownloader.h"
#include <parse.h>

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

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    FileDownloader *fileDownloader;

};
#endif // MAINWINDOW_H
