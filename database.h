#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSql>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QFile>
#include <QDate>
#include <QDebug>
#include <QSqlDriver>
#include <QCryptographicHash>

/* Directives for table names, table fields, and database fields */
#define DATABASE_HOSTNAME   "ScreenDataBase"
#define DATABASE_NAME       "Screen.db"

#define TABLE                   "ScreenTable"
#define TABLE_NAME              "Name"
#define TABLE_PIC               "Pic"
#define TABLE_SIMILE            "Simile"
#define TABLE_HAS               "HashValue"



class DataBase : public QObject
{
    Q_OBJECT
public:
    explicit DataBase(QObject *parent = 0);
    ~DataBase();

    /* Methods for direct work with the class
     * Connect to a database and insert records into a table
     * */

    void connectToDataBase();

private:

    QSqlDatabase    db;

private:

    /* Internal methods for working with the database
     * */

    bool openDataBase();
    bool restoreDataBase();
    void closeDataBase();
    bool createTable();

public slots:
    bool insertIntoTable(const QVariantList &data);
    bool insertIntoTable(const QString &name, const QByteArray &pic, const int &simile, const QString &has);
};

#endif // DATABASE_H
