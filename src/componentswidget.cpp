#include "componentswidget.h"
#include <QSqlQuery>
#include "ui_componentswidget.h"

ComponentsWidget::ComponentsWidget(int buildId, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ComponentsWidget)
    , db(SQLiteDBManager::getInstance())
    , componentTypeList({"motherboard", "cpu", "ram", "rom", "rom2", "gpu", "gpu2", "powerSupply"})
    , buildId(buildId)
    , isRestored(false)
{
    ui->setupUi(this);
}

ComponentsWidget::~ComponentsWidget()
{
    delete ui;
}

void ComponentsWidget::createWidget()
{
    QSqlQuery query(db->getDB());
    query.exec(QString("SELECT * FROM builds WHERE id = %1").arg(buildId));
    query.next();

    for (int i = 2, deletedCount = 0; i <= 9; i++) {
        if (!query.value(i).toInt())
            continue;

        createComponent(i - 2 - deletedCount, true);

        deletedCount++;
    }
}

void ComponentsWidget::on_addComponentButton_clicked()
{
    createComponent();
}

void ComponentsWidget::createComponent(int index, bool isRestored)
{
    this->isRestored = isRestored;

    if (componentTypeList.empty())
        return;

    SingleComponentWidget *singleComponentWidget
        = new SingleComponentWidget(componentTypeList[index], buildId, isRestored);
    connect(singleComponentWidget,
            &SingleComponentWidget::componentChoosed,
            this,
            &ComponentsWidget::checkCompatibility);
    connect(singleComponentWidget,
            &SingleComponentWidget::componentDeleted,
            this,
            [this, singleComponentWidget](QString componentName) {
                if (ui->frame->isHidden())
                    ui->frame->show();

                db->runScript(QString("UPDATE builds SET %1 = NULL WHERE id = %2")
                                  .arg(componentName)
                                  .arg(buildId));

                componentTypeList.push_back(componentName);

                for (int i = 0; i < conflictList.size(); i++)
                    if (componentName == conflictList[i])
                        conflictList.erase(conflictList.constBegin() + i);

                emit conflictResult(conflictList);

                delete singleComponentWidget;
            });

    singleComponentWidget->createWidget();

    ui->gridLayout->addWidget(singleComponentWidget);

    componentTypeList.erase(componentTypeList.constBegin() + index);
    if (componentTypeList.empty())
        ui->frame->hide();
}

void ComponentsWidget::checkCompatibility(QString componentType,
                                          int componentId,
                                          SingleComponentWidget *singleComponentWidget)
{
    bool result = false;
    int idList[4] = {};

    if (componentType == "motherboard")
        result = true;
    else if (componentType == "cpu")
        result = compatibilityQuery(componentType, "socket", componentId);
    else if (componentType == "ram")
        result = compatibilityQuery(componentType, "ramType", componentId);
    else if (componentType.startsWith("rom"))
        result = compatibilityQuery(componentType, "romInterface", componentId);
    else if (componentType.startsWith("gpu"))
        result = true;
    else if (componentType == "powerSupply") {
        int generalPower = 20;
        QSqlQuery query(db->getDB());

        query.exec(QString("SELECT cpu, ram, gpu, gpu2 FROM builds WHERE id = %1").arg(buildId));
        query.next();
        for (int i = 0; i < 4; i++)
            idList[i] = query.value(i).toInt();

        query.exec(QString("SELECT power FROM cpu WHERE id = %1").arg(idList[0]));
        query.next();
        generalPower += query.value(0).toInt();

        query.exec(QString("SELECT power FROM ram WHERE id = %1").arg(idList[1]));
        query.next();
        generalPower += query.value(0).toInt();

        for (int i = 2; i <= 3; i++) {
            query.exec(QString("SELECT power FROM gpu WHERE id = %1").arg(idList[i]));
            query.next();
            generalPower += query.value(0).toInt();
        }

        query.exec(QString("SELECT power FROM powerSupply WHERE id = %1").arg(componentId));
        query.next();
        result = (query.value(0).toInt() > generalPower);
    }

    singleComponentWidget->setComboBoxColor(result);

    conflictList.clear();
    if (!result) {
        if (componentType == "cpu" || componentType == "ram" || componentType == "rom") {
            conflictList.push_back("motherboard");
            conflictList.push_back(componentType);
        } else if (componentType == "powerSupply") {
            conflictList.push_back(componentType);
            for (int i = 0; i < 4; i++) {
                if (!idList[i])
                    conflictList.push_back((
                        !i ? "cpu" : (i == 1 ? "ram" : (i == 2 ? "gpu" : (i == 3 ? "gpu2" : "")))));
            }
        }
    }

    emit conflictResult(conflictList);
}

bool ComponentsWidget::compatibilityQuery(QString componentType,
                                          QString compareField,
                                          int componentId)
{
    QSqlQuery query(db->getDB());
    query.exec(QString("SELECT motherboard FROM builds WHERE id = %1").arg(buildId));
    query.next();
    int motherboardIndex = query.value(0).toInt();

    query.exec(
        QString("SELECT %1 FROM motherboard WHERE id = %2").arg(compareField).arg(motherboardIndex));
    query.next();
    QString compareResult = query.value(0).toString();

    query.exec(
        QString("SELECT %1 FROM %2 WHERE id = %3").arg(compareField, componentType).arg(componentId));
    query.next();

    return (query.value(0).toString() == compareResult);
}
