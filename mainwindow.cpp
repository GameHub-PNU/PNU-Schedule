#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QString>
#include "parse.h"
#include "schedule.h"





#include <QFile>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Establishing connection to databasee
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setDatabaseName("world");
    db.setUserName("root");

    // Your password
    db.setPassword("your-password-goes-here");

    if (db.open()) {
        QSqlQuery query;
        query.exec("SELECT * FROM city;");
        while (query.next()) {
            int id = query.value(0).toInt();
            QString name = query.value(1).toString();
            QString countryCode = query.value(2).toString();
            QString district = query.value(3).toString();
            int population = query.value(4).toInt();
            qDebug() << "Id:" << id << "City:" << name << "Country code:" << countryCode
                     << "District:" << district << "Population:" << population;
        }
    }


    Parse *parse = new Parse();
    Schedule newList = parse->parseSchedule("https://asu.pnu.edu.ua/static/groups/1002/1002-0732.html");



    qDebug() << "Ok";
}

MainWindow::~MainWindow()
{
    delete ui;
}

