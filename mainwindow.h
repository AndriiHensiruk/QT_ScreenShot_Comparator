#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlTableModel>
#include <QModelIndex>
#include <QBuffer>

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

    void on_screenButton_clicked();

    void slotCurrentPic(QModelIndex index);

    QString calcMD5(const QByteArray inByteArray);

private:
    Ui::MainWindow *ui;

    DataBase        *db;
    QSqlTableModel  *model;

private:

    void setupModel(const QString &tableName, const QStringList &headers);
    void createUI();

};

#endif // MAINWINDOW_H
