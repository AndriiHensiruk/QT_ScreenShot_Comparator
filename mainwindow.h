#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlTableModel>
#include <QModelIndex>
#include <QBuffer>
#include <QTimer>
#include <QPushButton>
#include <QPixmap>

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

    void on_startButton_clicked();

    void on_stopButton_clicked();

    // Slot for getting image from database
    void slotCurrentPic(QModelIndex index);

    QString calcMD5(const QByteArray inByteArray);

    int simile_ScreenShot(QString tableHAS1, QString tableHAS2);

    QString getHAS();

    // Slot for writing a screenshot to the database
    void screenShot();

private:
    Ui::MainWindow *ui;

    DataBase        *db;
    QSqlTableModel  *model;

private:

    void setupModel(const QString &tableName, const QStringList &headers);
    void createUI();


private:

    QTimer *timer;

};

#endif // MAINWINDOW_H
