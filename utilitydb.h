#ifndef UTILITYDB_H
#define UTILITYDB_H

#include <QtSql>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <qmessagebox.h>
#include "schedule.h"

class UtilityDB
{
public:
    UtilityDB();
    bool isConnected();
    bool doesTableExist(QString tableName);
    void clearTable(QString tableName);
    void dropTable(QString tableName);
    void createScheduleTable(QString tableName);
    Schedule getScheduleByTableName(QString tableName);
    void insertScheduleToTable(QString tableName, Schedule schedule);
    Schedule getScheduleByTableNameInRange(QString tableName, QDate startDate, QDate endDate);
private:
    QSqlDatabase db;
    void establishConnection();
    QString decorateTableName(QString tableName);
    QString DB_DRIVER_NAME = "QSQLITE";

    QString DB_FILE_PATH = "./../../../../PNU-Schedule/database/schedule.db";

    // My (YuraRov) DB_FILE_PATH, others should comment the line below and uncomment above
    //QString DB_FILE_PATH = "./../PNU-Schedule/database/schedule.db";
};

#endif // UTILITYDB_H
