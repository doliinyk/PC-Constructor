#include "singlecomponentwidget.h"
#include "ui_singlecomponentwidget.h"

SingleComponentWidget::SingleComponentWidget(QString componentName, int buildId, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SingleComponentWidget)
    , db(SQLiteDBManager::getInstance())
    , buildId(buildId)
    , componentId(0)
    , componentName(componentName)
    , query(db->getDB())
{
    ui->setupUi(this);

    if (componentName == "motherboard")
        ui->label->setText("Материнські плати");
    if (componentName == "cpu")
        ui->label->setText("Процесори");
    if (componentName == "ram")
        ui->label->setText("Оперативна пам'ять");
    if (componentName == "rom")
        ui->label->setText("Накопичувачі");
    if (componentName == "gpu")
        ui->label->setText("Відеокарти");
    if (componentName == "powerSupply")
        ui->label->setText("Блоки живлення");

    query.exec("SELECT name FROM " + componentName);
    while (query.next())
        ui->comboBox->addItem(query.value(0).toString());

    connect(ui->comboBox, &QComboBox::activated, this, [this](int index) {
        componentId = findIdByName(this->componentName, ui->comboBox->itemText(index));

        query.exec("UPDATE builds SET " + this->componentName + " = " + QString::number(componentId)
                   + " WHERE id = " + QString::number(this->buildId));

        if (componentCompatibility(this->componentName))
            ui->comboBox->setStyleSheet("color: black");
        else
            ui->comboBox->setStyleSheet("color: darkred");
    });
}

SingleComponentWidget::~SingleComponentWidget()
{
    delete ui;
}

void SingleComponentWidget::on_pushButton_clicked()
{
    emit componentDeleted(componentName);
}

bool SingleComponentWidget::componentCompatibility(QString componentName)
{
    if (componentName == "motherboard")
        return true;
    else if (componentName == "cpu")
        return compatibilityQuery(componentName, "socket");
    else if (componentName == "ram")
        return compatibilityQuery(componentName, "ramType");
    else if (componentName == "rom")
        return compatibilityQuery(componentName, "romInterface");
    else if (componentName == "gpu")
        return true;
    else if (componentName == "powerSupply") {
        int powerSum = 20;
        int ids[3] = {};

        query.exec("SELECT id, cpu, ram, gpu FROM builds");
        while (query.next())
            if (query.value(0).toInt() == buildId)
                for (int i = 0; i < 3; i++)
                    ids[i] = query.value(i + 1).toInt();

        query.exec("SELECT id, power FROM cpu");
        while (query.next())
            if (query.value(0).toInt() == ids[0])
                powerSum += query.value(1).toInt();

        query.exec("SELECT id, power FROM ram");
        while (query.next())
            if (query.value(0).toInt() == ids[1])
                powerSum += query.value(1).toInt();

        query.exec("SELECT id, power FROM gpu");
        while (query.next())
            if (query.value(0).toInt() == ids[2])
                powerSum += query.value(1).toInt();

        query.exec("SELECT id, power FROM powerSupply");
        while (query.next())
            if (query.value(0).toInt() == componentId && query.value(1).toInt() > powerSum)
                return true;
    }

    return false;
}

int SingleComponentWidget::findIdByName(QString tableName, QString componentName)
{
    query.exec(QString("SELECT id, name FROM %1").arg(tableName));
    while (query.next())
        if (query.value(1).toString() == componentName)
            return query.value(0).toUInt();

    return -1;
}

bool SingleComponentWidget::compatibilityQuery(QString componentName, QString compareName)
{
    int motherboardIndex = 0;
    QString compareResultName;

    query.exec("SELECT id, motherboard FROM builds");
    while (query.next())
        if (query.value(0).toInt() == buildId)
            motherboardIndex = query.value(1).toInt();

    query.exec(QString("SELECT id, %1 FROM motherboard").arg(compareName));
    while (query.next())
        if (query.value(0).toInt() == motherboardIndex)
            compareResultName = query.value(1).toString();

    query.exec(QString("SELECT id, %1 FROM %2").arg(compareName, componentName));
    while (query.next())
        if (query.value(0).toInt() == componentId && query.value(1).toString() == compareResultName)
            return true;

    return false;
}
