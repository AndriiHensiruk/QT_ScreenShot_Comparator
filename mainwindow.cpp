#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QApplication>
#include <QScreen>
#include <QList>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    db = new DataBase();
    db->connectToDataBase();

    this->setupModel(TABLE,
                     QStringList() << trUtf8("id")
                                   << trUtf8("Name image")
                                   << trUtf8("Image")
                                   << trUtf8("Simile")
                                   << trUtf8("HAS")
                     );

    this->createUI();

    timer = new QTimer();
}

/* Method for initializing the data view mode
 * */
MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::setupModel(const QString &tableName, const QStringList &headers)
{

    model = new QSqlTableModel(this);
    model->setTable(tableName);
    model->sort(1, Qt::DescendingOrder);

    for(int i = 0, j = 0; i < model->columnCount(); i++, j++){
        model->setHeaderData(i,Qt::Horizontal,headers[j]);
    }
}

void MainWindow::createUI()
{
    ui->tableView->setModel(model);
    ui->tableView->setColumnHidden(0, true);
    ui->tableView->setColumnHidden(2, true);

    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->tableView->resizeColumnsToContents();
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);


    connect(ui->tableView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            this, SLOT(slotCurrentPic(QModelIndex)));

    model->select();
}

/* Take a screenshot of the screen and save it to a QByteArray object,
     * for this ...
     * */
void MainWindow::screenShot()
{
    QScreen *screen = QApplication::primaryScreen();
    QPixmap inPixmap = screen->grabWindow( 0 );
    QByteArray inByteArray;
    QBuffer inBuffer( &inByteArray );
    inBuffer.open( QIODevice::WriteOnly );
    inPixmap.save( &inBuffer, "PNG" );

    int simile = simile_ScreenShot(getHAS(), calcMD5(inByteArray));

    db->insertIntoTable(QDateTime::currentDateTime().toString("dd.MM.yyyy_hh:mm:ss.png"), inByteArray,
                        simile, calcMD5(inByteArray));

    model->select();
}

void MainWindow::on_stopButton_clicked()
{
    this->timer->stop();
}

void MainWindow::on_startButton_clicked()
{
    connect(timer, SIGNAL(timeout()), this, SLOT(screenShot()));
    timer->start(1000);
}

void MainWindow::slotCurrentPic(QModelIndex index)
{
    QPixmap outPixmap = QPixmap();

    outPixmap.loadFromData(model->data(model->index(index.row(), 2)).toByteArray());

    ui->picLabel->setPixmap(outPixmap.scaled(400, 300, Qt::KeepAspectRatio));

}

QString MainWindow::getHAS()
{

    QList <QString> Hash;
    QSqlQuery query;
     query.exec("SELECT id, Name , Pic, Simile, HashValue FROM ScreenTable ");
     int i = 0;
     while (query.next())
     {
        Hash.append(query.value(4).toString());
        //qDebug() << Hash[i];
        ++i;
     }

     return  Hash[i-2];
}


QString MainWindow::calcMD5(QByteArray inByteArray)
{
    QString result;
    QByteArray data;
    QCryptographicHash cryp(QCryptographicHash::Md5);

    QBuffer file( &inByteArray );

    if (file.open(QIODevice::ReadOnly) )
    {
        data = file.readAll();
        cryp.addData(data);
        result = cryp.result().toHex().data();
        file.close();
    }
    return result;
}


int MainWindow::simile_ScreenShot(QString tableHAS1, QString tableHAS2)
{
   int dist_counter = 0;

    if (tableHAS1.length() != tableHAS2.length())
    {
        qDebug() << "Не удалось восстановить данных";
   }
    else
    for (int i = 0; i < tableHAS1.length(); i++ )
    {
       if (tableHAS1.at(i) != tableHAS2.at(i))
        {
            dist_counter += 1;
       }
   }
  // qDebug() <<100 - dist_counter;
    return 100 - dist_counter;
}
