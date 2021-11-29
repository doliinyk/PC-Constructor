#include "componentsmanager.h"
#include <QDir>
#include "QFile"
#include "ui_componentsmanager.h"

ComponentsManager::ComponentsManager(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ComponentsManager)
    , db(new SQLiteDBManager)
    , activeRow(0)
{
    ui->setupUi(this);

    if (!QDir("..\\db\\components").exists())
        QDir().mkdir("..\\db\\components");

    if (!QFile::exists("..\\db\\components\\components.sqlite")) {
        db->createDB("components\\components");
        db->connectToDB("components\\components");
        db->createTable("CREATE TABLE motherboard("
                        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                        "name VARCHAR NOT NULL, "
                        "chipset VARCHAR NOT NULL, "
                        "socket VARCHAR NOT NULL, "
                        "ramType VARCHAR NOT NULL, "
                        "romInterface VARCHAR NOT NULL, "
                        "year INTEGER NOT NULL, "
                        "price INTEGER NOT NULL"
                        ")");
        db->createTable("CREATE TABLE cpu("
                        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                        "name VARCHAR NOT NULL, "
                        "socket VARCHAR NOT NULL, "
                        "core INTEGER NOT NULL, "
                        "thread INTEGER NOT NULL, "
                        "graphics INTEGER NOT NULL, "
                        "cache INTEGER NOT NULL, "
                        "price INTEGER NOT NULL"
                        ")");
        db->createTable("CREATE TABLE rom("
                        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                        "name VARCHAR NOT NULL, "
                        "romInterface VARCHAR NOT NULL, "
                        "type VARCHAR NOT NULL, "
                        "space INTEGER NOT NULL, "
                        "readSpeed INTEGER NOT NULL, "
                        "writeSpeed INTEGER NOT NULL, "
                        "price INTEGER NOT NULL"
                        ")");
        db->createTable("CREATE TABLE ram("
                        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                        "name VARCHAR NOT NULL, "
                        "ramType VARCHAR NOT NULL, "
                        "frequency INTEGER NOT NULL, "
                        "space INTEGER NOT NULL, "
                        "price INTEGER NOT NULL"
                        ")");
        db->createTable("CREATE TABLE supply("
                        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                        "name VARCHAR NOT NULL, "
                        "power INTEGER NOT NULL, "
                        "slots INTEGER NOT NULL, "
                        "price INTEGER NOT NULL"
                        ")");
        db->createTable("CREATE TABLE gpu("
                        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                        "name VARCHAR NOT NULL, "
                        "frequency INTEGER NOT NULL, "
                        "space INTEGER NOT NULL, "
                        "price INTEGER NOT NULL"
                        ")");
    }
}

ComponentsManager::~ComponentsManager()
{
    delete db;
    delete ui;
}

void ComponentsManager::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *component)
{
    ui->label->setText(component->text(0));
    model = new QSqlTableModel(this, db->getDB("components\\components"));
    model->setTable(translateText(component->text(0)));
    model->select();
    ui->tableView->setModel(model);
    ui->tableView->setColumnHidden(0, true);
}

QString ComponentsManager::translateText(QString text)
{
    if (text == "Материнські плати")
        return "motherboard";
    if (text == "Процесори")
        return "cpu";
    if (text == "Накопичувачі")
        return "rom";
    if (text == "Оперативна пам'ять")
        return "ram";
    if (text == "Блоки живлення")
        return "supply";
    if (text == "Відеокарти")
        return "gpu";
    return text;
}

void ComponentsManager::on_tableView_clicked(const QModelIndex &index)
{
    activeRow = index.row();
}

void ComponentsManager::on_addComponentButton_clicked()
{
    model->insertRow(activeRow);
}

void ComponentsManager::on_deleteComponentButton_clicked()
{
    model->removeRow(activeRow);
}
