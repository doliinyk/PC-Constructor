#include "singlecomponentwidget.h"
#include <QSqlQuery>
#include "ui_singlecomponentwidget.h"

SingleComponentWidget::SingleComponentWidget(QString componentType,
                                             int buildId,
                                             bool isRestored,
                                             QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SingleComponentWidget)
    , db(SQLiteDBManager::getInstance())
    , buildId(buildId)
    , isRestored(isRestored)
    , componentId(0)
    , componentType(componentType)
{
    ui->setupUi(this);
}

SingleComponentWidget::~SingleComponentWidget()
{
    delete ui;
}

void SingleComponentWidget::createWidget()
{
    QSqlQuery query(db->getDB());
    ui->label->setText(translateComponentToText(componentType));

    query.exec(QString("SELECT name FROM %1").arg(isComponentTypeSecond(componentType)));
    while (query.next())
        ui->componentBox->addItem(query.value(0).toString());

    if (isRestored) {
        query.exec(QString("SELECT %1 FROM builds WHERE id = %2").arg(componentType).arg(buildId));
        query.next();

        componentId = query.value(0).toInt();
        ui->componentBox->setCurrentIndex(query.value(0).toInt() - 1);
    }

    emit componentChoosed(componentType, componentId, this);
}

void SingleComponentWidget::setComboBoxColor(bool value)
{
    ui->componentBox->setStyleSheet(QString("color: %1").arg(value ? "black" : "darkred"));
}

QString SingleComponentWidget::isComponentTypeSecond(QString componentType)
{
    return (componentType.endsWith("2") ? componentType.left(componentType.length() - 1)
                                        : componentType);
}

QString SingleComponentWidget::translateComponentToText(QString componentType)
{
    if (componentType == "motherboard")
        return "Материнська плата";
    if (componentType == "cpu")
        return "Процесор";
    if (componentType == "ram")
        return "Оперативна пам`ять";
    if (componentType.startsWith("rom"))
        return "Накопичувач";
    if (componentType.startsWith("gpu"))
        return "Відеокарта";
    if (componentType == "powerSupply")
        return "Блок живлення";
    return componentType;
}

void SingleComponentWidget::on_componentBox_activated(int index)
{
    QSqlQuery query(db->getDB());
    componentId = findIdByName(componentType, ui->componentBox->itemText(index));

    query.exec(QString("UPDATE builds SET %1 = %2 WHERE id = %3")
                   .arg(componentType)
                   .arg(componentId)
                   .arg(buildId));

    emit componentChoosed(componentType, componentId, this);
}

void SingleComponentWidget::on_deleteButton_clicked()
{
    emit componentDeleted(componentType);
}

int SingleComponentWidget::findIdByName(QString componentType, QString componentName)
{
    QSqlQuery query(db->getDB());
    query.exec(QString("SELECT id FROM %1 WHERE name LIKE '%2'")
                   .arg(isComponentTypeSecond(componentType), componentName));
    query.next();

    return query.value(0).toInt();
}
