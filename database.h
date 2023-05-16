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

/* Директивы имен таблицы, полей таблицы и базы данных */
#define DATABASE_HOSTNAME   "ScreenDataBase"
#define DATABASE_NAME       "Screen.db"

#define TABLE                   "ScreenTable"       // Название таблицы
#define TABLE_NAME              "Name"              // Вторая колонка
#define TABLE_PIC               "Pic"               // Третья колонка

// Первая колонка содержит Autoincrement ID

class DataBase : public QObject
{
    Q_OBJECT
public:
    explicit DataBase(QObject *parent = 0);
    ~DataBase();
    /* Методы для непосредственной работы с классом
         * Подключение к базе данных и вставка записей в таблицу
         * */
    void connectToDataBase();

private:
    // Сам объект базы данных, с которым будет производиться работа
    QSqlDatabase    db;

private:
    /* Внутренние методы для работы с базой данных
         * */
    bool openDataBase();        // Открытие базы данных
    bool restoreDataBase();     // Восстановление базы данных
    void closeDataBase();       // Закрытие базы данных
    bool createTable();         // Создание таблицы в базе данных

public slots:
    bool insertIntoTable(const QVariantList &data);      // Добавление записей в таблицу
    bool insertIntoTable(const QString &name, const QByteArray &pic);
};

#endif // DATABASE_H
