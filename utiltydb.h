#ifndef UTILTYDB_H
#define UTILTYDB_H

#include <QtSql>
#include <QSqlDatabase>
#include <qmessagebox.h>
#include "schedule.h"

class UtilityDB
{
public:
    UtilityDB();
    bool isConnected();
    bool isTableExists(QString tableName);
    void clearTable(QString tableName);
    void dropTable(QString tableName);
    void createScheduleTable(QString tableName);
    Schedule getScheduleByTableName(QString tableName);
    void insertSchedultToTable(QString tableName, Schedule schedule);

private:
    QSqlDatabase db;
    void establishConnection();
    void initialSetup();
    QString DB_DRIVER_NAME = "QSQLITE";
    QString DB_FILE_PATH = "./../../../../PNU-Schedule/database/schedule.db";

};

#endif // UTILTYDB_H
