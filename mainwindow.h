#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlTableModel>
#include <QModelIndex>

#include "database.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    // Слот для записи скриншота в базу данных
    void on_screenButton_clicked();
    // Слот для получения изображения из базы данных
    void slotCurrentPic(QModelIndex index);

private:
    Ui::MainWindow *ui;
    /* В проекте используются объекты для взаимодействия с информацией в базе данных
         * и моделью представления таблицы базы данных
         * */
    DataBase        *db;
    QSqlTableModel  *model;

private:
    /* Также присутствуют два метода, которые формируют модель
         * и внешний вид TableView
         * */
    void setupModel(const QString &tableName, const QStringList &headers);
    void createUI();
};

#endif // MAINWINDOW_H
