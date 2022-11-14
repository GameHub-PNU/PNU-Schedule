#include "utilitydb.h"

UtilityDB::UtilityDB() { establishConnection(); }

bool UtilityDB::isConnected() { return db.isOpen(); }

bool UtilityDB::doesTableExist(QString tableName) {
  return db.tables().contains(tableName);
}

void UtilityDB::clearTable(QString tableName) {
  if (doesTableExist(tableName)) {
    QSqlQuery query;
    query.exec("DELETE FROM " + tableName);
  }
}

void UtilityDB::dropTable(QString tableName) {
  if (doesTableExist(tableName)) {
    QSqlQuery query;
    query.exec("DROP TABLE " + tableName);
  }
}

void UtilityDB::createScheduleTable(QString tableName) {
  QString statement = "CREATE TABLE IF NOT EXISTS " + tableName +
                      "("
                      "date_ TEXT, "
                      "name_of_day TEXT, "
                      "num_of_pair INTEGER,"
                      "time_stamp_of_pair TEXT, "
                      "pair_description TEXT);";
  QSqlQuery query;
  query.exec(statement);
}

Schedule UtilityDB::getScheduleByTableName(QString tableName) {
  Schedule schedule;
  schedule.groupName = QString(tableName).replace("_", "-");
  QList<ScheduleList> *groupSchedule = new QList<ScheduleList>();
  QSqlQuery query;
  query.exec("SELECT * FROM " + tableName);
  while (query.next()) {
    ScheduleList scheduleList;
    scheduleList.date = query.value(0).toString();
    scheduleList.nameOfDay = query.value(1).toString();
    scheduleList.numOfCouple = query.value(2).toInt();
    scheduleList.timeStapOfCouple = query.value(3).toString();
    scheduleList.coupleDesc = query.value(4).toString();
    groupSchedule->push_back(scheduleList);
  }
  schedule.groupSchedule = groupSchedule;
  return schedule;
}

void UtilityDB::insertScheduleToTable(QString tableName, Schedule schedule) {
  if (doesTableExist(tableName)) {
    QSqlQuery query;
    QListIterator<ScheduleList> *iterator =
        new QListIterator(*schedule.groupSchedule);
    while (iterator->hasNext()) {
      ScheduleList scheduleList = iterator->next();
      QString statement =
          QString("INSERT INTO %1 (date_, name_of_day, num_of_pair, "
                  "time_stamp_of_pair, pair_description) VALUES (\"%2\", "
                  "\"%3\", %4, \"%5\", \"%6\")")
              .arg(tableName)
              .arg(scheduleList.date)
              .arg(scheduleList.nameOfDay)
              .arg(scheduleList.numOfCouple)
              .arg(scheduleList.timeStapOfCouple)
              .arg(scheduleList.timeStapOfCouple);
      query.exec(statement);
    }
  }
}

void UtilityDB::establishConnection() {
  db = QSqlDatabase::addDatabase(DB_DRIVER_NAME);
  db.setDatabaseName(DB_FILE_PATH);
  db.open();
}
