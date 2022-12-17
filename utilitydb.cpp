#include "utilitydb.h"

UtilityDB::UtilityDB() { establishConnection(); }

bool UtilityDB::isConnected() { return db.isOpen(); }

bool UtilityDB::doesTableExist(QString tableName) {
    tableName = decorateTableName(tableName);
    return db.tables().contains(tableName);
}

void UtilityDB::clearTable(QString tableName) {
    tableName = decorateTableName(tableName);
    if (doesTableExist(tableName)) {
        QSqlQuery query;
        query.exec("DELETE FROM " + tableName);
    }
}

void UtilityDB::dropTable(QString tableName) {
    tableName = decorateTableName(tableName);
    if (doesTableExist(tableName)) {
        QSqlQuery query;
        query.exec("DROP TABLE " + tableName);
    }
}

void UtilityDB::createScheduleTable(QString tableName) {
    tableName = decorateTableName(tableName);
    QString statement = "CREATE TABLE IF NOT EXISTS " + tableName +
            "("
            "date_ TEXT, "
            "name_of_day TEXT, "
            "num_of_pair INTEGER,"
            "time_stamp_of_pair TEXT, "
            "pair_description TEXT)";
    QSqlQuery query;
    query.exec(statement);
}

Schedule UtilityDB::getScheduleByTableName(QString tableName) {
    tableName = decorateTableName(tableName);
    Schedule schedule;
    schedule.groupName = QString(tableName).replace("_", "-");
    QList<UniversityClass> *groupSchedule = new QList<UniversityClass>();
    QSqlQuery query;
    query.exec("SELECT * FROM " + tableName);
    while (query.next()) {
        UniversityClass scheduleList;
        scheduleList.date = QDate::fromString(query.value(0).toString(), "yyyy-MM-dd");
        scheduleList.nameOfDay = query.value(1).toString();
        scheduleList.numberOfClass = query.value(2).toInt();
        scheduleList.timeStampOfClass = query.value(3).toString();
        scheduleList.classDescription = query.value(4).toString();
        groupSchedule->push_back(scheduleList);
    }
    schedule.groupSchedule = groupSchedule;
    return schedule;
}

void UtilityDB::insertScheduleToTable(QString tableName, Schedule schedule) {
    tableName = decorateTableName(tableName);
    if (doesTableExist(tableName) && db.transaction()) {
        QSqlQuery query;
        query.prepare(QString("INSERT INTO %1 (date_, name_of_day, num_of_pair, time_stamp_of_pair, pair_description) VALUES (?, ?, ?, ?, ?)").arg(tableName));
        QVariantList dates, namesOfDates, numsOfPairs, timeStampsOfPairs, pairsDescriptions;
        QListIterator<UniversityClass> *iterator = new QListIterator(*schedule.groupSchedule);
        UniversityClass scheduleList;
        while (iterator->hasNext()) {
            scheduleList = iterator->next();
            dates << scheduleList.date.toString("yyyy-MM-dd");
            namesOfDates << scheduleList.nameOfDay;
            numsOfPairs << QString::number(scheduleList.numberOfClass);
            timeStampsOfPairs << scheduleList.timeStampOfClass;
            pairsDescriptions << scheduleList.classDescription;
        }

        query.addBindValue(dates);
        query.addBindValue(namesOfDates);
        query.addBindValue(numsOfPairs);
        query.addBindValue(timeStampsOfPairs);
        query.addBindValue(pairsDescriptions);

        if (!query.execBatch() || !db.commit()){
            qDebug() << query.lastError();
        }
    }
}

Schedule UtilityDB::getScheduleByTableNameInRange(QString tableName, QDate startDate, QDate endDate)
{
    tableName = decorateTableName(tableName);
    QString dateFormat = "yyyy-MM-dd";
    QString startDateStr = startDate.toString(dateFormat);
    QString endDateStr = endDate.toString(dateFormat);
    Schedule schedule;
    schedule.groupName = QString(tableName).replace("_", "-");
    QList<UniversityClass> *groupSchedule = new QList<UniversityClass>();
    QSqlQuery query;
    QString statement = QString("SELECT * FROM %1 WHERE date_ BETWEEN '%2' AND '%3'")
            .arg(tableName, startDateStr, endDateStr);
    query.exec(statement);
    while (query.next()) {
        UniversityClass scheduleList;
        scheduleList.date = QDate::fromString(query.value(0).toString(),"yyyy-MM-dd");
        scheduleList.nameOfDay = query.value(1).toString();
        scheduleList.numberOfClass = query.value(2).toInt();
        scheduleList.timeStampOfClass = query.value(3).toString();
        scheduleList.classDescription = query.value(4).toString();
        groupSchedule->push_back(scheduleList);
    }
    schedule.groupSchedule = groupSchedule;
    return schedule;
}

void UtilityDB::establishConnection() {
    db = QSqlDatabase::addDatabase(DB_DRIVER_NAME);
    db.setDatabaseName(DB_FILE_PATH);
    db.open();
}

QString UtilityDB::decorateTableName(QString tableName)
{
    return QString(tableName).replace("-", "_");
}
