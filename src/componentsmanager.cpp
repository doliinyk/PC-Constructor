#include "componentsmanager.h"
#include <QDir>
#include "QFile"
#include "ui_componentsmanager.h"

ComponentsManager::ComponentsManager(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ComponentsManager)
    , db(SQLiteDBManager::getInstance())
    , activeRow(0)
{
    ui->setupUi(this);

    db->connectToDB();
    db->runScript("CREATE TABLE motherboard"
                  "("
                  "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                  "name VARCHAR NOT NULL, "
                  "chipset VARCHAR NOT NULL, "
                  "socket VARCHAR NOT NULL, "
                  "ramType VARCHAR NOT NULL, "
                  "romInterface VARCHAR NOT NULL, "
                  "year INTEGER NOT NULL, "
                  "price INTEGER NOT NULL"
                  ")");
    db->runScript("CREATE TABLE cpu"
                  "("
                  "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                  "name VARCHAR NOT NULL, "
                  "socket VARCHAR NOT NULL, "
                  "core INTEGER NOT NULL, "
                  "thread INTEGER NOT NULL, "
                  "graphics INTEGER NOT NULL, "
                  "cache INTEGER NOT NULL, "
                  "price INTEGER NOT NULL"
                  ")");
    db->runScript("CREATE TABLE rom"
                  "("
                  "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                  "name VARCHAR NOT NULL, "
                  "romInterface VARCHAR NOT NULL, "
                  "type VARCHAR NOT NULL, "
                  "space INTEGER NOT NULL, "
                  "readSpeed INTEGER NOT NULL, "
                  "writeSpeed INTEGER NOT NULL, "
                  "price INTEGER NOT NULL"
                  ")");
    db->runScript("CREATE TABLE ram"
                  "("
                  "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                  "name VARCHAR NOT NULL, "
                  "ramType VARCHAR NOT NULL, "
                  "frequency INTEGER NOT NULL, "
                  "space INTEGER NOT NULL, "
                  "price INTEGER NOT NULL"
                  ")");
    db->runScript("CREATE TABLE supply"
                  "("
                  "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                  "name VARCHAR NOT NULL, "
                  "power INTEGER NOT NULL, "
                  "slots INTEGER NOT NULL, "
                  "price INTEGER NOT NULL"
                  ")");
    db->runScript("CREATE TABLE gpu"
                  "("
                  "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                  "name VARCHAR NOT NULL, "
                  "frequency INTEGER NOT NULL, "
                  "space INTEGER NOT NULL, "
                  "price INTEGER NOT NULL"
                  ")");
}

ComponentsManager::~ComponentsManager()
{
    delete ui;
}

void ComponentsManager::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *component)
{
    ui->labelComponent->setText(component->text(0));
    model = new QSqlTableModel(this, db->getDB());
    model->setTable(translateText(component->text(0)));
    model->select();
    ui->tableView->setModel(model);
    //ui->tableView->setColumnHidden(0, true);
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
        return "builds";
    return text;
}

void ComponentsManager::on_tableView_clicked(const QModelIndex &index)
{
    if (!ui->deleteComponentButton->isEnabled())
        ui->deleteComponentButton->setEnabled(true);

    activeRow = index.row();
}

void ComponentsManager::on_addComponentButton_clicked()
{
    model->insertRow(model->rowCount());
}

void ComponentsManager::on_deleteComponentButton_clicked()
{
    model->removeRow(activeRow);
}
