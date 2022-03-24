#include "componentswidget.h"
#include <QMessageBox>
#include <QSqlQuery>
#include "ui_componentswidget.h"

ComponentsWidget::ComponentsWidget(int buildId, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ComponentsWidget)
    , db(SQLiteDBManager::getInstance())
    , componentTypeList({"motherboard", "cpu", "ram", "rom", "rom2", "gpu", "gpu2", "powerSupply"})
    , buildId(buildId)
    , isConflict(false)
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

    for (int i = 2, deletedCount = 0; i <= 9; i++)
        if (query.value(i).toInt()) {
            createComponent(i - 2 - deletedCount, true);

            deletedCount++;
        }
}

void ComponentsWidget::createComponent(int index, bool isRestored)
{
    if (printError())
        return;

    this->isRestored = isRestored;

    SingleComponentWidget *singleComponentWidget
        = new SingleComponentWidget(componentTypeList[index], buildId, isRestored, this);
    connect(singleComponentWidget,
            &SingleComponentWidget::componentCreated,
            this,
            [this](QString componentType,
                   int componentId,
                   SingleComponentWidget *singleComponentWidget) {
                checkCompatibility(componentType, componentId, singleComponentWidget);

                if (componentType == "powerSupply")
                    emit lastComponentCreated();
            });
    connect(singleComponentWidget,
            &SingleComponentWidget::componentChanged,
            this,
            &ComponentsWidget::on_singleComponentChanged);
    connect(singleComponentWidget,
            &SingleComponentWidget::specificationsRequest,
            this,
            [this](QString componentType, int componentId) {
                emit clearWidget(SingleComponentWidget::isComponentTypeSecond(componentType),
                                 componentId);
            });

    singleComponentWidget->createWidget();

    ui->gridLayout->addWidget(singleComponentWidget);

    componentTypeList.removeFirst();
    if (componentTypeList.empty())
        ui->frame->hide();
}

void ComponentsWidget::on_singleComponentChanged(QString componentType,
                                                 int componentId,
                                                 bool isDeleted)
{
    const QStringList tempComponentList(
        {"motherboard", "cpu", "ram", "rom", "rom2", "gpu", "gpu2", "powerSupply"});

    for (int i = 0; i < tempComponentList.size() - tempComponentList.indexOf(componentType)
                            - (isDeleted ? 0 : 1);
         i++)
        db->runScript(QString("UPDATE builds SET %1 = NULL WHERE id = %2")
                          .arg(*(tempComponentList.crbegin() + i))
                          .arg(buildId));

    QSqlQuery query(db->getDB());
    query.exec(QString("SELECT name FROM %1 WHERE id = %2")
                   .arg(SingleComponentWidget::isComponentTypeSecond(componentType))
                   .arg(componentId));
    query.next();

    emit clearWidget(SingleComponentWidget::isComponentTypeSecond(componentType), componentId);
}

void ComponentsWidget::on_addComponentButton_clicked()
{
    createComponent();
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
        result = motherboardCompatibilityQuery(componentType, "socket", componentId);
    else if (componentType == "ram")
        result = motherboardCompatibilityQuery(componentType, "ramType", componentId);
    else if (componentType == "rom")
        result = motherboardCompatibilityQuery(componentType, "romInterface", componentId);
    else if (componentType == "rom2") {
        QSqlQuery query(db->getDB());

        query.exec(QString("SELECT rom2 FROM builds WHERE id = %1").arg(buildId));
        query.next();

        query.exec(QString("SELECT storage FROM rom WHERE id = %1").arg(query.value(0).toInt()));
        query.next();

        if (!query.value(0).toBool())
            result = true;
        else
            result = motherboardCompatibilityQuery(componentType, "romInterface", componentId);
    } else if (componentType.startsWith("gpu")) {
        QSqlQuery query(db->getDB());

        query.exec(QString("SELECT cpu FROM builds WHERE id = %1").arg(buildId));
        query.next();

        query.exec(QString("SELECT graphics FROM cpu WHERE id = %1").arg(query.value(0).toInt()));
        query.next();

        if (!query.value(0).toInt()) {
            if (componentType == "gpu") {
                query.exec(QString("SELECT storage FROM gpu WHERE id = %1").arg(componentId));
                query.next();

                result = query.value(0).toBool();
            } else if (componentType == "gpu2") {
                query.exec(QString("SELECT gpu FROM builds WHERE id = %1").arg(buildId));
                query.next();

                query.exec(
                    QString("SELECT storage FROM gpu WHERE id = %1").arg(query.value(0).toInt()));
                query.next();
                bool tempGPU2Exists = query.value(0).toBool();

                query.exec(QString("SELECT storage FROM gpu WHERE id = %1").arg(componentId));
                query.next();

                result = (tempGPU2Exists || query.value(0).toBool());
            }
        } else
            result = true;
    } else if (componentType == "powerSupply") {
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

    isConflict = !result;

    db->runScript(
        QString("UPDATE builds SET conflict = %1 WHERE id = %2").arg(isConflict).arg(buildId));

    if (isConflict) {
        componentConflictType = componentType;

        if (componentConflictType == "powerSupply")
            printError();
    }
}

bool ComponentsWidget::motherboardCompatibilityQuery(QString componentType,
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

    query.exec(QString("SELECT %1 FROM %2 WHERE id = %3")
                   .arg(compareField, SingleComponentWidget::isComponentTypeSecond(componentType))
                   .arg(componentId));
    query.next();

    if (componentType.startsWith("rom"))
        return compareResult.contains(query.value(0).toString());

    return query.value(0).toString() == compareResult;
}

bool ComponentsWidget::printError()
{
    if (!isConflict)
        return false;

    QMessageBox *a = new QMessageBox(
        QMessageBox::Warning,
        "Конфлікт",
        QString("%1 не є сумісним з даною збіркою ПК за параметром: %2")
            .arg(SingleComponentWidget::translateComponentToText(componentConflictType),
                 componentConflictType == "cpu"
                     ? "Сокет"
                     : (componentConflictType == "ram"
                            ? "Тип оперативної пам'яті"
                            : (componentConflictType.startsWith("rom")
                                   ? "Інтерфейс/наявність накопичувача"
                                   : (componentConflictType.startsWith("gpu")
                                          ? "Наявність графічного ядра"
                                          : (componentConflictType == "powerSupply" ? "Потужність"
                                                                                    : ""))))),
        QMessageBox::Ok,
        this);
    a->show();

    return true;
}
