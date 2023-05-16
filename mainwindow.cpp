#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QApplication>
#include <QBuffer>
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
                     );

    this->createUI();
}

MainWindow::~MainWindow()
{
    delete ui;
}

/* Метод для инициализации модеи представления данных
     * */
void MainWindow::setupModel(const QString &tableName, const QStringList &headers)
{
    /* Производим инициализацию модели представления данных
         * с установкой имени таблицы в базе данных, по которому
         * будет производится обращение в таблице
         * */
    model = new QSqlTableModel(this);
    model->setTable(tableName);

    /* Устанавливаем названия колонок в таблице с сортировкой данных
         * */
    for(int i = 0, j = 0; i < model->columnCount(); i++, j++){
        model->setHeaderData(i,Qt::Horizontal,headers[j]);
    }
}

void MainWindow::createUI()
{
    ui->tableView->setModel(model);     // Устанавливаем модель на TableView
    ui->tableView->setColumnHidden(0, true);    // Скрываем колонку с id записей
    ui->tableView->setColumnHidden(2, true);    // Скрываем колонку с изображением
    // Разрешаем выделение строк
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    // Устанавливаем режим выделения лишь одной строки в таблице
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    // Устанавливаем размер колонок по содержимому
    ui->tableView->resizeColumnsToContents();
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);  // Запрещаем редактирование
    ui->tableView->horizontalHeader()->setStretchLastSection(true);     // Растягиваем последнюю колонку по всему tableView

    /* Подключаем сигнал об изменении выбора текущей строки в таблицу
         * к СЛОТу для установки изображения в picLabel
         * */
    connect(ui->tableView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            this, SLOT(slotCurrentPic(QModelIndex)));

    model->select(); // Делаем выборку данных из таблицы
}

void MainWindow::on_screenButton_clicked()
{
    /* Делаем скриншот экрана и сохраняем его в объект QByteArray,
         * для этого ...
         * */
    QScreen *screen = QApplication::primaryScreen();    // Берём объект экрана
    QPixmap inPixmap = screen->grabWindow( 0 );         // Сохраняем его в изображение объекта QPixmap
    QByteArray inByteArray;                             // Создаём объект QByteArray для сохранения изображения
    QBuffer inBuffer( &inByteArray );                   // Сохранение изображения производим через буффер
    inBuffer.open( QIODevice::WriteOnly );              // Открываем буффер
    inPixmap.save( &inBuffer, "PNG" );                  // Записываем inPixmap в inByteArray

    // Записываем скриншот в базу данных
    db->insertIntoTable(QDateTime::currentDateTime().toString("dd.MM.yyyy_hh:mm:ss.png"), inByteArray);

    // Делаем выборку таблицы из Базы Данных
    model->select();
}

void MainWindow::slotCurrentPic(QModelIndex index)
{
    QPixmap outPixmap = QPixmap(); // Создаём QPixmap, который будет помещаться в picLabel
    /* Забираем данные об изображении из таблицы в качестве QByteArray
         * и помещаем их в QPixmap
         * */
    outPixmap.loadFromData(model->data(model->index(index.row(), 2)).toByteArray());
    // Устанавливаем изображение в picLabel
    ui->picLabel->setPixmap(outPixmap.scaled(600,300));
}
