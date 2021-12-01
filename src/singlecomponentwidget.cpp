#include "singlecomponentwidget.h"
#include "ui_singlecomponentwidget.h"

SingleComponentWidget::SingleComponentWidget(QString componentName,
                                             int buildId,
                                             bool isRestored,
                                             QWidget *parent)
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
        ui->label->setText("Материнська плата");
    if (componentName == "cpu")
        ui->label->setText("Процесор");
    if (componentName == "ram")
        ui->label->setText("Оперативна пам'ять");
    if (componentName.startsWith("rom"))
        ui->label->setText("Накопичувач");
    if (componentName.startsWith("gpu"))
        ui->label->setText("Відеокарта");
    if (componentName == "powerSupply")
        ui->label->setText("Блок живлення");

    query.exec("SELECT name FROM " + checkSecondComponent(componentName));
    while (query.next())
        ui->comboBox->addItem(query.value(0).toString());

    if (isRestored) {
        int tempIndex;
        query.exec(QString("SELECT id, %1 FROM builds").arg(componentName));
        while (query.next())
            if (query.value(0) == buildId)
                tempIndex = query.value(1).toInt();

        ui->comboBox->setCurrentIndex(tempIndex - 1);
        qDebug() << QString("%1: %2").arg(componentName).arg(tempIndex);

        componentCompatibility(componentName);
    }

    connect(ui->comboBox, &QComboBox::activated, this, [this](int index) {
        componentId = findIdByName(this->componentName, ui->comboBox->itemText(index));

        query.exec("UPDATE builds SET " + this->componentName + " = " + QString::number(componentId)
                   + " WHERE id = " + QString::number(this->buildId));

        componentCompatibility(this->componentName);
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

void SingleComponentWidget::componentCompatibility(QString componentName)
{
    bool result = false;

    if (componentName == "motherboard")
        result = true;
    else if (componentName == "cpu")
        result = compatibilityQuery(componentName, "socket");
    else if (componentName == "ram")
        result = compatibilityQuery(componentName, "ramType");
    else if (componentName.startsWith("rom"))
        result = compatibilityQuery(checkSecondComponent(componentName), "romInterface");
    else if (componentName.startsWith("gpu"))
        result = true;
    else if (componentName == "powerSupply") {
        int powerSum = 20;
        int ids[4] = {};

        query.exec("SELECT id, cpu, ram, gpu, gpu2 FROM builds");
        while (query.next())
            if (query.value(0).toInt() == buildId)
                for (int i = 0; i < 4; i++)
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

        query.exec("SELECT id, power FROM gpu");
        while (query.next())
            if (query.value(0).toInt() == ids[3])
                powerSum += query.value(1).toInt();

        query.exec("SELECT id, power FROM powerSupply");
        while (query.next())
            if (query.value(0).toInt() == componentId && query.value(1).toInt() > powerSum)
                result = true;
    }

    ui->comboBox->setStyleSheet(QString("color: %1").arg(result ? "black" : "darkred"));
}

int SingleComponentWidget::findIdByName(QString tableName, QString componentName)
{
    query.exec(QString("SELECT id, name FROM %1").arg(checkSecondComponent(tableName)));
    while (query.next())
        if (query.value(1).toString() == componentName)
            return query.value(0).toUInt();

    return 0;
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

    query.exec(QString("SELECT id, %1 FROM %2").arg(compareName).arg(componentName));
    while (query.next())
        if (query.value(0).toInt() == componentId && query.value(1).toString() == compareResultName)
            return true;

    return false;
}

QString SingleComponentWidget::checkSecondComponent(QString componentName)
{
    return (componentName.endsWith("2") ? componentName.left(componentName.length() - 1)
                                        : componentName);
}
