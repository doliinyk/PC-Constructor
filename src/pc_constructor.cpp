#include "pc_constructor.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QTableView>
#include "ui_pc_constructor.h"

PC_Constructor::PC_Constructor(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PC_Constructor)
    , db(SQLiteDBManager::getInstance())
{
    ui->setupUi(this);
    db->runScript("CREATE TABLE builds"
                  "("
                  "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                  "name VARCHAR, "
                  "motherboard INTEGER, "
                  "cpu INTEGER, "
                  "ram INTEGER, "
                  "rom INTEGER, "
                  "gpu INTEGER, "
                  "powerSupply INTEGER, "
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
        || !db->runScript(("INSERT INTO builds(name) VALUES('" + buildName + "')")))
        return;

    activeBuildName = buildName;

    QTreeWidgetItem *buildItem = new QTreeWidgetItem;
    buildItem->setText(0, activeBuildName);
    ui->treeWidget->addTopLevelItem(buildItem);

    setValidBuildMenuActions();
    setTabWidgetBuilds();
}

void PC_Constructor::on_actionDeleteBuild_triggered()
{
    if (QMessageBox::warning(this,
                             "Попередження",
                             "Ви точно хочете видалити збірку " + activeBuildName + "?",
                             QMessageBox::Ok | QMessageBox::Cancel)
            != QMessageBox::Ok
        || !db->runScript("DELETE FROM builds WHERE name LIKE '" + activeBuildName + "'"))
        return;

    activeBuildName.clear();
    ui->treeWidget->clear();
    ui->tabWidget->removeTab(ui->tabWidget->currentIndex());

    setValidBuildMenuActions();
    setTreeWidgetBuilds();
}

void PC_Constructor::on_tabWidget_tabBarClicked(int index)
{
    if (index == ui->tabWidget->currentIndex())
        return;

    activeBuildName = ui->tabWidget->tabText(index);
    setValidBuildMenuActions();
}

void PC_Constructor::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *build)
{
    if (activeBuildName == build->text(0))
        return;

    activeBuildName = build->text(0);
    setValidBuildMenuActions();

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

void PC_Constructor::setValidBuildMenuActions()
{
    ui->menuBuilds->setTitle("Збірка"
                             + (!activeBuildName.isEmpty() ? " [" + activeBuildName + "]" : ""));
    ui->actionDeleteBuild->setEnabled(!activeBuildName.isEmpty());
}

void PC_Constructor::setTreeWidgetBuilds()
{
    QSqlQuery tempQuery;

    tempQuery.exec("SELECT name FROM builds");
    while (tempQuery.next()) {
        QTreeWidgetItem *buildItem = new QTreeWidgetItem;
        buildItem->setText(0, tempQuery.value(0).toString());
        if (!buildItem->text(0).isEmpty())
            ui->treeWidget->addTopLevelItem(buildItem);
    }
}

void PC_Constructor::setTabWidgetBuilds()
{
    QWidget *tempTabWidget = new QWidget(this);
    QHBoxLayout *tabHBoxLayout = new QHBoxLayout(tempTabWidget);
    QSqlTableModel tempBuildModel(this, db->getDB());
    QSqlQuery tempQuery;

    tempQuery.exec("SELECT id, name FROM builds");
    while (tempQuery.next())
        if (tempQuery.value(1).toString() == activeBuildName)
            tabHBoxLayout->addWidget(new ComponentsWidget(tempQuery.value(0).toUInt()),
                                     QSizePolicy::Ignored);

    tabHBoxLayout->addWidget(new SpecificationsWidget, QSizePolicy::Expanding);
    ui->tabWidget->addTab(tempTabWidget, activeBuildName);
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count() - 1);
}
