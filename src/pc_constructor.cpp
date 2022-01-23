#include "pc_constructor.h"
#include <QKeyEvent>
#include <QMessageBox>
#include <QSqlQuery>
#include <QTableView>
#include "componentswidget.h"
#include "discordrpc.h"
#include "singlecomponentwidget.h"
#include "specificationswidget.h"
#include "ui_pc_constructor.h"

PC_Constructor::PC_Constructor(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PC_Constructor)
    , db(SQLiteDBManager::getInstance())
{
    ui->setupUi(this);
    g_DiscordRPC.Init();

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
                  "conflict INTEGER DEFAULT 0, "
                  "UNIQUE(name)"
                  ")");

    connect(&createBuildDialog,
            &CreateBuildDialog::getBuildName,
            this,
            &PC_Constructor::createBuild);
    connect(ui->treeWidget,
            &QTreeWidget::itemActivated,
            this,
            &PC_Constructor::on_treeWidget_itemDoubleClicked);

    setTreeWidgetBuilds();
}

PC_Constructor::~PC_Constructor()
{
    g_DiscordRPC.Deinit();

    delete db;
    delete ui;
}

void PC_Constructor::createBuild(QString buildName)
{
    if (buildName.isEmpty()
        || !db->runScript(QString("INSERT INTO builds(name) VALUES('%1')").arg(buildName))) {
        QMessageBox::warning(this,
                             "Помилка",
                             QString("Збірку %1 не створено або збірка вже існує.").arg(buildName),
                             QMessageBox::Ok);
        return;
    }

    activeBuildName = buildName;

    QTreeWidgetItem *buildItem = new QTreeWidgetItem;
    buildItem->setText(0, activeBuildName);
    ui->treeWidget->addTopLevelItem(buildItem);

    setBuildMenuBar();
    setTabWidgetBuilds();
    setStatusBar();
}

void PC_Constructor::on_action_NewBuild_triggered()
{
    createBuildDialog.exec();
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
    on_tabWidget_tabCloseRequested(ui->tabWidget->currentIndex());

    setBuildMenuBar();
    setTreeWidgetBuilds();
    setStatusBar();
}

void PC_Constructor::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Tab && event->modifiers() == Qt::ControlModifier) {
        if (ui->tabWidget->currentIndex() + 1 == ui->tabWidget->count())
            ui->tabWidget->setCurrentIndex(0);
        else
            ui->tabWidget->setCurrentIndex(ui->tabWidget->currentIndex() + 1);
    } else if (event->key() == Qt::Key_Backtab) {
        if (!ui->tabWidget->currentIndex())
            ui->tabWidget->setCurrentIndex(ui->tabWidget->count() - 1);
        else
            ui->tabWidget->setCurrentIndex(ui->tabWidget->currentIndex() - 1);
    } else
        return;

    activeBuildName = ui->tabWidget->tabText(ui->tabWidget->currentIndex());

    setBuildMenuBar();
    setStatusBar();
}

void PC_Constructor::on_tabWidget_tabBarClicked(int index)
{
    if (index == ui->tabWidget->currentIndex())
        return;

    activeBuildName = ui->tabWidget->tabText(index);

    setBuildMenuBar();
    setStatusBar();
}

void PC_Constructor::on_tabWidget_tabCloseRequested(int index)
{
    ui->tabWidget->removeTab(index);

    activeBuildName.clear();
    activeBuildName = ui->tabWidget->tabText(ui->tabWidget->currentIndex());

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

    g_DiscordRPC.Update(activeBuildName);
    g_DiscordRPC.Update("", "");
}

void PC_Constructor::setTreeWidgetBuilds()
{
    QSqlQuery query(db->getDB());

    query.exec("SELECT name FROM builds ORDER BY id ASC");
    while (query.next()) {
        QTreeWidgetItem *buildItem = new QTreeWidgetItem;
        buildItem->setText(0, query.value(0).toString());
        if (!buildItem->text(0).isEmpty())
            ui->treeWidget->addTopLevelItem(buildItem);
    }
}

void PC_Constructor::setTabWidgetBuilds(QString componentType, int componentId)
{
    QSqlQuery query(db->getDB());

    query.exec(QString("SELECT id FROM builds WHERE name LIKE '%1'").arg(activeBuildName));
    query.next();

    QWidget *tempTabWidget = new QWidget(this);
    QHBoxLayout *tabHBoxLayout = new QHBoxLayout(tempTabWidget);
    ComponentsWidget *componentsWidget = new ComponentsWidget(query.value(0).toInt());

    connect(componentsWidget,
            &ComponentsWidget::clearWidget,
            this,
            [this, tempTabWidget](QString componentType, int componentId) {
                ui->tabWidget->removeTab(ui->tabWidget->currentIndex());
                tempTabWidget->deleteLater();

                setTabWidgetBuilds(componentType, componentId);
                setStatusBar();
            });
    connect(componentsWidget,
            &ComponentsWidget::lastComponentCreated,
            this,
            &PC_Constructor::setStatusBar);

    componentsWidget->createWidget();

    tabHBoxLayout->addWidget(componentsWidget, QSizePolicy::Ignored);
    tabHBoxLayout->addWidget(new SpecificationsWidget(componentType, componentId),
                             QSizePolicy::Expanding);
    ui->tabWidget->addTab(tempTabWidget, activeBuildName);
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count() - 1);
}

void PC_Constructor::setStatusBar()
{
    if (activeBuildName.isEmpty()) {
        ui->statusbar->clearMessage();
        return;
    }

    QSqlQuery query(db->getDB());

    int tempPrice = 0;
    int tempPower = 0;

    QStringList tempComponentsList({"cpu", "ram", "gpu", "gpu"});
    QVector<int> ids(4);

    query.exec(
        QString("SELECT cpu, ram, gpu, gpu2 FROM builds WHERE name LIKE '%1'").arg(activeBuildName));
    query.next();
    for (int i = 0; i < 4; i++)
        ids[i] = query.value(i).toInt();
    for (int i = 0; i < 4; i++) {
        query.exec(
            QString("SELECT power FROM %1 WHERE id = %2").arg(tempComponentsList[i]).arg(ids[i]));
        if (query.next())
            tempPower += query.value(0).toInt();
    }

    tempComponentsList = {"motherboard", "cpu", "ram", "rom", "rom", "gpu", "gpu", "powerSupply"};
    ids.clear();
    ids.resize(8);

    query.exec(QString("SELECT conflict FROM builds WHERE name LIKE '%1'").arg(activeBuildName));
    query.next();
    bool tempConflict = query.value(0).toBool();

    query.exec(QString("SELECT * FROM builds WHERE name LIKE '%1'").arg(activeBuildName));
    query.next();
    for (int i = 2; i < 10; i++)
        ids[i - 2] = query.value(i).toInt();
    if (!ids.contains(0))
        tempPower += 20;
    else
        tempConflict = true;
    for (int i = 0; i < 8; i++) {
        query.exec(
            QString("SELECT price FROM %1 WHERE id = %2").arg(tempComponentsList[i]).arg(ids[i]));
        if (query.next())
            tempPrice += query.value(0).toInt();
    }

    ui->statusbar->showMessage(QString("Характеристики збірки %1:"
                                       "        Ціна: %2 грн"
                                       "        Споживання: %3 Вт"
                                       "        Стан збірки: %4")
                                   .arg(activeBuildName)
                                   .arg(tempPrice)
                                   .arg(tempPower)
                                   .arg(tempConflict ? "Наявні конфлікти/Не вистачає компонентів"
                                                     : "Конфліктів немає. ПК зібрано"));
}
