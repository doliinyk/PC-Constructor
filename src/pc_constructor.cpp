#include "pc_constructor.h"
#include <QMessageBox>
#include <QTableView>
#include "componentswidget.h"
#include "singlecomponentwidget.h"
#include "specificationswidget.h"
#include "ui_pc_constructor.h"

PC_Constructor::PC_Constructor(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PC_Constructor)
    , db(SQLiteDBManager::getInstance())
{
    ui->setupUi(this);

    query = QSqlQuery(db->getDB());

    db->runScript("CREATE TABLE builds"
                  "("
                  "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                  "name VARCHAR NOT NULL, "
                  "motherboard INTEGER, "
                  "cpu INTEGER, "
                  "ram INTEGER, "
                  "rom INTEGER, "
                  "rom2 INTEGER, "
                  "gpu INTEGER, "
                  "gpu2 INTEGER, "
                  "powerSupply INTEGER, "
                  "power INTEGER, "
                  "conflict VARCHAR, "
                  "price INTEGER, "
                  "UNIQUE(name)"
                  ")");

    connect(&createBuildDialog,
            &CreateBuildDialog::getBuildName,
            this,
            &PC_Constructor::createBuild);

    setTreeWidgetBuilds();
}

PC_Constructor::~PC_Constructor()
{
    delete db;
    delete ui;
}

void PC_Constructor::on_action_NewBuild_triggered()
{
    createBuildDialog.exec();
}

void PC_Constructor::createBuild(QString buildName)
{
    if (buildName.isEmpty()
        || !db->runScript(QString("INSERT INTO builds(name) VALUES('%1')").arg(buildName)))
        return;

    activeBuildName = buildName;

    QTreeWidgetItem *buildItem = new QTreeWidgetItem;
    buildItem->setText(0, activeBuildName);
    ui->treeWidget->addTopLevelItem(buildItem);

    setBuildMenuBar();
    setTabWidgetBuilds();
    setStatusBar();
}

void PC_Constructor::on_actionDeleteBuild_triggered()
{
    if (QMessageBox::warning(this,
                             "Попередження",
                             QString("Ви точно хочете видалити збірку %1?").arg(activeBuildName),
                             QMessageBox::Ok | QMessageBox::Cancel)
            != QMessageBox::Ok
        || !db->runScript(QString("DELETE FROM builds WHERE name LIKE '%1'").arg(activeBuildName)))
        return;

    activeBuildName.clear();
    ui->treeWidget->clear();
    ui->tabWidget->removeTab(ui->tabWidget->currentIndex());

    setBuildMenuBar();
    setTreeWidgetBuilds();
    setStatusBar();
}

void PC_Constructor::on_tabWidget_tabBarClicked(int index)
{
    if (index == ui->tabWidget->currentIndex())
        return;

    activeBuildName = ui->tabWidget->tabText(index);
    setStatusBar();
    setBuildMenuBar();
    setStatusBar();
}

void PC_Constructor::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *build)
{
    if (activeBuildName == build->text(0))
        return;

    activeBuildName = build->text(0);

    setBuildMenuBar();
    setStatusBar();

    for (int i = 0; i < ui->tabWidget->count(); i++)
        if (activeBuildName == ui->tabWidget->tabText(i)) {
            ui->tabWidget->setCurrentIndex(i);
            return;
        }

    setTabWidgetBuilds();
}

void PC_Constructor::on_actionOpenComponentsManager_triggered()
{
    componentsManager.show();
}

void PC_Constructor::on_actionAbout_triggered()
{
    QMessageBox::about(this,
                       "Інформація",
                       "PC Constructor - програма для створення збірок ПК\n"
                       "Олійник Денис, ВСП \"ТФК ТНТУ\" ім. І. Пулюя\n"
                       "2021 рік");
}

void PC_Constructor::setBuildMenuBar()
{
    ui->menuBuilds->setTitle(
        QString("Збірка%1")
            .arg(!activeBuildName.isEmpty() ? QString(" [%1]").arg(activeBuildName) : ""));
    ui->actionDeleteBuild->setEnabled(!activeBuildName.isEmpty());
}

void PC_Constructor::setTreeWidgetBuilds()
{
    query.exec("SELECT name FROM builds");
    while (query.next()) {
        QTreeWidgetItem *buildItem = new QTreeWidgetItem;
        buildItem->setText(0, query.value(0).toString());
        if (!buildItem->text(0).isEmpty())
            ui->treeWidget->addTopLevelItem(buildItem);
    }
}

void PC_Constructor::setTabWidgetBuilds()
{
    QWidget *tempTabWidget = new QWidget(this);
    QHBoxLayout *tabHBoxLayout = new QHBoxLayout(tempTabWidget);

    query.exec(QString("SELECT id FROM builds WHERE name LIKE '%1'").arg(activeBuildName));
    query.next();

    ComponentsWidget *componentsWidget = new ComponentsWidget(query.value(0).toInt());
    tabHBoxLayout->addWidget(componentsWidget, QSizePolicy::Ignored);
    connect(componentsWidget,
            &ComponentsWidget::componentConflict,
            this,
            [this](QStringList componentTypes) {
                static int conflictCount = 0;

                if (componentTypes.empty()) {
                    db->runScript(QString("UPDATE builds SET conflict = NULL WHERE name LIKE '%1'")
                                      .arg(activeBuildName));
                    conflictCount = 0;
                } else {
                    QString tempMessage = ui->statusbar->currentMessage();

                    if (!conflictCount)
                        tempMessage = "Конфлікт: ";

                    for (int i = 0; i < componentTypes.size(); i++)
                        tempMessage.append(
                            SingleComponentWidget::translateComponentToText(componentTypes[i])
                            + (i + 1 != componentTypes.size() ? " - " : ""));

                    tempMessage.append("; ");

                    db->runScript(QString("UPDATE builds SET conflict = '%1' WHERE name LIKE '%2'")
                                      .arg(tempMessage, activeBuildName));

                    conflictCount++;
                }
                setStatusBar();
            });
    componentsWidget->emitSignalAfterRestore();

    tabHBoxLayout->addWidget(new SpecificationsWidget, QSizePolicy::Expanding);
    ui->tabWidget->addTab(tempTabWidget, activeBuildName);
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count() - 1);
}

void PC_Constructor::setStatusBar()
{
    query.exec(QString("SELECT conflict FROM builds WHERE name LIKE '%1'").arg(activeBuildName));

    QString tempMessage;
    if (query.next())
        tempMessage = query.value(0).toString();

    if (tempMessage.isEmpty() || activeBuildName.isEmpty()) {
        ui->statusbar->setStyleSheet("background-color: white;"
                                     "color: black");
        ui->statusbar->clearMessage();
    } else {
        ui->statusbar->setStyleSheet("background-color: rgb(75, 0, 0);"
                                     "color: white");
        ui->statusbar->showMessage(tempMessage);
    }
}

void PC_Constructor::on_tabWidget_tabCloseRequested(int index)
{
    activeBuildName.clear();

    ui->tabWidget->removeTab(index);

    activeBuildName = ui->tabWidget->tabText(ui->tabWidget->currentIndex());
    setBuildMenuBar();
    setStatusBar();
}
