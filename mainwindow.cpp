#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QApplication>
#include <QScreen>

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
                                   << trUtf8("HAS")
                     );

    this->createUI();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::setupModel(const QString &tableName, const QStringList &headers)
{

    model = new QSqlTableModel(this);
    model->setTable(tableName);


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

void MainWindow::on_screenButton_clicked()
{
    QScreen *screen = QApplication::primaryScreen();
    QPixmap inPixmap = screen->grabWindow( 0 );
    QByteArray inByteArray;
    QBuffer inBuffer( &inByteArray );
    inBuffer.open( QIODevice::WriteOnly );
    inPixmap.save( &inBuffer, "PNG" );

    db->insertIntoTable(QDateTime::currentDateTime().toString("dd.MM.yyyy_hh:mm:ss.png"), inByteArray,
                       calcMD5(inByteArray));

        simile_ScreenShot(calcMD5(inByteArray), calcMD5(inByteArray));
    model->select();
}

void MainWindow::slotCurrentPic(QModelIndex index)
{
    QPixmap outPixmap = QPixmap();

    outPixmap.loadFromData(model->data(model->index(index.row(), 2)).toByteArray());

    ui->picLabel->setPixmap(outPixmap.scaled(600,300));
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

    for (int i = 0; i < tableHAS1.length(); i++ )
    {
       if (tableHAS1.at(i) != tableHAS2.at(i))
        {
            dist_counter += 1;
       }
   }
   qDebug() << dist_counter;
    return dist_counter;
}
