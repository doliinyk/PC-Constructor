#include "componentsmanager.h"
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include "ui_componentsmanager.h"

ComponentsManager::ComponentsManager(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ComponentsManager)
    , model(nullptr)
    , db(SQLiteDBManager::getInstance())
    , activeRow(0)
{
    ui->setupUi(this);
    db->connectToDB("pc_constructor_db");
    db->runScript("CREATE TABLE motherboard"
                  "("
                  "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                  "name VARCHAR NOT NULL, "
                  "chipset VARCHAR NOT NULL, "
                  "socket VARCHAR NOT NULL, "
                  "ramType VARCHAR NOT NULL, "
                  "romInterface VARCHAR NOT NULL, "
                  "price INTEGER NOT NULL, "
                  "photo VARCHAR, "
                  "UNIQUE(name)"
                  ")");
    db->runScript("CREATE TABLE cpu"
                  "("
                  "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                  "name VARCHAR NOT NULL, "
                  "core INTEGER NOT NULL, "
                  "thread INTEGER NOT NULL, "
                  "frequency INTEGER NOT NULL, "
                  "graphics INTEGER NOT NULL, "
                  "cache INTEGER NOT NULL, "
                  "socket VARCHAR NOT NULL, "
                  "power INTEGER NOT NULL, "
                  "price INTEGER NOT NULL, "
                  "photo VARCHAR, "
                  "UNIQUE(name)"
                  ")");
    db->runScript("CREATE TABLE ram"
                  "("
                  "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                  "name VARCHAR NOT NULL, "
                  "storage INTEGER NOT NULL, "
                  "frequency INTEGER NOT NULL, "
                  "ramType VARCHAR NOT NULL, "
                  "power INTEGER NOT NULL, "
                  "price INTEGER NOT NULL, "
                  "photo VARCHAR, "
                  "UNIQUE(name)"
                  ")");
    db->runScript("CREATE TABLE rom"
                  "("
                  "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                  "name VARCHAR NOT NULL, "
                  "type VARCHAR NOT NULL, "
                  "storage INTEGER NOT NULL, "
                  "readSpeed INTEGER NOT NULL, "
                  "writeSpeed INTEGER NOT NULL, "
                  "romInterface VARCHAR NOT NULL, "
                  "price INTEGER NOT NULL, "
                  "photo VARCHAR, "
                  "UNIQUE(name)"
                  ")");
    db->runScript("CREATE TABLE gpu"
                  "("
                  "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                  "name VARCHAR NOT NULL, "
                  "storage INTEGER NOT NULL, "
                  "frequency INTEGER NOT NULL, "
                  "power INTEGER NOT NULL, "
                  "price INTEGER NOT NULL, "
                  "photo VARCHAR, "
                  "UNIQUE(name)"
                  ")");
    db->runScript("CREATE TABLE powerSupply"
                  "("
                  "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                  "name VARCHAR NOT NULL, "
                  "power INTEGER NOT NULL, "
                  "price INTEGER NOT NULL, "
                  "photo VARCHAR, "
                  "UNIQUE(name)"
                  ")");

    model = new QSqlTableModel(this, db->getDB());
}

ComponentsManager::~ComponentsManager()
{
    delete model;
    delete ui;
}

QString ComponentsManager::translateTextToComponent(QString text)
{
    if (text == "Материнські плати")
        return "motherboard";
    if (text == "Процесори")
        return "cpu";
    if (text == "Оперативна пам'ять")
        return "ram";
    if (text == "Накопичувачі")
        return "rom";
    if (text == "Відеокарти")
        return "gpu";
    if (text == "Блоки живлення")
        return "powerSupply";
    return text;
}

QString ComponentsManager::translateTableHeader(QString text)
{
    if (text == "name")
        return "Найменування";
    if (text == "chipset")
        return "Чіпсет";
    if (text == "socket")
        return "Сокет";
    if (text == "ramType")
        return "Тип оперативної пам'яті";
    if (text == "romInterface")
        return "Інтерфейс накопичувача";
    if (text == "price")
        return "Ціна, грн";
    if (text == "photo")
        return "Фотографія";
    if (text == "core")
        return "Кількість ядер";
    if (text == "thread")
        return "Кількість потоків";
    if (text == "frequency")
        return "Частота, МГц";
    if (text == "graphics")
        return "Графічне ядро, є/нема";
    if (text == "cache")
        return "Кеш пам'яті, Кб";
    if (text == "power")
        return "Потужність, Вт";
    if (text == "storage")
        return "Кількість пам'яті, Гб";
    if (text == "type")
        return "Тип";
    if (text == "readSpeed")
        return "Швидкість читання, Мб/с";
    if (text == "writeSpeed")
        return "Швидкість записування, Мб/с";
    return "";
}

void ComponentsManager::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *component)
{
    activeComponentType = translateTextToComponent(component->text(0));

    ui->labelComponent->setText(component->text(0));

    model->clear();
    model->setTable(activeComponentType);
    model->select();

    for (int i = 0; i < model->columnCount(); i++)
        model->setHeaderData(i,
                             Qt::Horizontal,
                             translateTableHeader(model->headerData(i, Qt::Horizontal).toString()));

    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    for (int i = 0; i < model->rowCount(); i++)
        if (model->index(i, 1).data().toString().contains("Відсутн")
            && !model->index(i, 3).data().toInt()) {
            ui->tableView->hideRow(i);
            break;
        }

    ui->tableView->hideColumn(0);
    ui->tableView->hideColumn(model->columnCount() - 1);

    if (ui->labelComponent->text() == "Відеокарти")
        model->setHeaderData(2, Qt::Horizontal, "Кількість пам'яті, Мб");
}

void ComponentsManager::on_tableView_clicked(const QModelIndex &index)
{
    if (!ui->deleteComponentButton->isEnabled())
        ui->deleteComponentButton->setEnabled(true);
    if (!ui->photoComponentButton->isEnabled())
        ui->photoComponentButton->setEnabled(true);

    activeRow = index.row();
}

void ComponentsManager::on_addComponentButton_clicked()
{
    model->insertRow(model->rowCount());
}

void ComponentsManager::on_deleteComponentButton_clicked()
{
    if (QMessageBox::warning(this,
                             "Попередження",
                             QString("Ви точно хочете видалити компонент %1?")
                                 .arg(model->index(activeRow, 1).data().toString()),
                             QMessageBox::Ok | QMessageBox::Cancel)
            != QMessageBox::Ok
        || !model->removeRow(activeRow))
        return;

    ui->deleteComponentButton->setEnabled(false);
    ui->photoComponentButton->setEnabled(false);
}

void ComponentsManager::on_photoComponentButton_clicked()
{
    QString tempPhotoPath = QFileDialog::getOpenFileName(
        this,
        QString("Оберіть фотографію для %1").arg(model->index(activeRow, 1).data().toString()));

    db->runScript(QString("UPDATE %1 SET photo = '%2' WHERE id = %3")
                      .arg(activeComponentType, QDir().relativeFilePath(tempPhotoPath))
                      .arg(model->index(activeRow, 0).data().toInt()));
}
