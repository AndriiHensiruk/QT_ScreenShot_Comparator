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


#define DATABASE_HOSTNAME   "ScreenDataBase"
#define DATABASE_NAME       "Screen.db"

#define TABLE                   "ScreenTable"
#define TABLE_NAME              "Name"
#define TABLE_PIC               "Pic"



class DataBase : public QObject
{
    Q_OBJECT
public:
    explicit DataBase(QObject *parent = 0);
    ~DataBase();

    void connectToDataBase();

private:

    QSqlDatabase    db;

private:

    bool openDataBase();
    bool restoreDataBase();
    void closeDataBase();
    bool createTable();

public slots:
    bool insertIntoTable(const QVariantList &data);
    bool insertIntoTable(const QString &name, const QByteArray &pic);
};

#endif // DATABASE_H
