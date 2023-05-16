#include "database.h"

DataBase::DataBase(QObject *parent) : QObject(parent)
{

}

DataBase::~DataBase()
{

}


void DataBase::connectToDataBase()
{

    if(!QFile("C:/Qt/project/ScreenShotProject/database" DATABASE_NAME).exists()){
        this->restoreDataBase();
    } else {
        this->openDataBase();
    }
}


bool DataBase::restoreDataBase()
{

    if(this->openDataBase()){

        return (this->createTable()) ? true : false;
    } else {
        qDebug() << "Не удалось восстановить базу данных";
        return false;
    }
    return false;
}


bool DataBase::openDataBase()
{

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName(DATABASE_HOSTNAME);
    db.setDatabaseName("C:/Qt/project/ScreenShotProject/database" DATABASE_NAME);
    if(db.open()){
        return true;
    } else {
        return false;
    }
}


void DataBase::closeDataBase()
{
    db.close();
}


bool DataBase::createTable()
{

    QSqlQuery query;
    if(!query.exec( "CREATE TABLE " TABLE " ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                    TABLE_NAME     " VARCHAR(255)    NOT NULL,"
                    TABLE_PIC      " BLOB            NOT NULL"
                    " )"
                    )){
        qDebug() << "DataBase: error of create " << TABLE;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}


bool DataBase::insertIntoTable(const QVariantList &data)
{

    QSqlQuery query;

    query.prepare("INSERT INTO " TABLE " ( " TABLE_NAME ", "
                  TABLE_PIC " ) "
                  "VALUES (:Name, :Pic)");
    query.bindValue(":Name",        data[0].toString());
    query.bindValue(":Pic",         data[1].toByteArray());


    if(!query.exec()){
        qDebug() << "error insert into " << TABLE;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}


bool DataBase::insertIntoTable(const QString &name, const QByteArray &pic)
{
    QVariantList data;
    data.append(name);
    data.append(pic);

    if(insertIntoTable(data))
        return true;
    else
        return false;
}
