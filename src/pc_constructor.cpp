#include "pc_constructor.h"
#include <QMessageBox>
#include <QSqlTableModel>
#include <QTableView>
#include "ui_pc_constructor.h"

PC_Constructor::PC_Constructor(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PC_Constructor)
{
    ui->setupUi(this);

    connect(&createCollectionDialog,
            &CreateCollectionDialog::getCollectionName,
            this,
            &PC_Constructor::createCollection);
    connect(&createBuildDialog,
            &CreateBuildDialog::getBuildName,
            this,
            &PC_Constructor::createBuild);
    connect(ui->treeWidget, &QTreeWidget::itemDoubleClicked, [this](QTreeWidgetItem *build) {
        if (activeBuildName == build->text(0))
            return;
        activeBuildName = build->text(0);
        ui->menuBuilds->setTitle("Збірка [" + activeBuildName + "]");
        ui->menuBuilds->setEnabled(true);
        ui->actionDeleteBuild->setShortcut(QKeySequence("Ctrl+Del"));
    });
}

PC_Constructor::~PC_Constructor()
{
    delete ui;
}

void PC_Constructor::on_actionNewCollection_triggered()
{
    createCollectionDialog.show();
}

void PC_Constructor::on_actionNewBuild_triggered()
{
    createBuildDialog.show();
}

void PC_Constructor::on_actionAbout_triggered()
{
    QMessageBox::information(this,
                             "Інформація",
                             "PC Constructor - програма для створення збірок ПК\nОлійник Денис, "
                             "ВСП \"ТФК ТНТУ\" ім. І. Пулюя, 2021 рік");
}

void PC_Constructor::on_actionDeleteBuild_triggered()
{
    if (QMessageBox::warning(this,
                             "Попередження",
                             "Ви точно хочете видалити збірку " + activeBuildName + "?",
                             QMessageBox::Ok | QMessageBox::Cancel)
            != QMessageBox::Ok
        || !collection.deleteBuild(activeBuildName))
        return;

    ui->treeWidget->clear();
    ui->treeWidget->setHeaderLabel(activeCollectionName);
    ui->menuBuilds->setTitle("Збірка");
    ui->menuBuilds->setEnabled(false);
    ui->actionDeleteBuild->setShortcut(QKeySequence());
    activeBuildName.clear();

    QStringList tempBuildNames = collection.getBuildNames();
    for (int i = 0; i < tempBuildNames.size(); i++) {
        QTreeWidgetItem *tempBuildItem = new QTreeWidgetItem;
        tempBuildItem->setText(0, tempBuildNames[i]);
        ui->treeWidget->addTopLevelItem(tempBuildItem);
    }
}

void PC_Constructor::createCollection(QString collectionName)
{
    if (collectionName.isEmpty() || !collection.createCollection(collectionName))
        return;

    QMenu *collectionMenu = new QMenu(collection.getName());
    QAction *openCollection = new QAction("Відкрити...");
    QAction *deleteCollection = new QAction("Видалити...");

    activeCollectionName = collection.getName();
    ui->treeWidget->setHeaderLabel(activeCollectionName);
    collectionMenu->addAction(openCollection);
    collectionMenu->addAction(deleteCollection);
    ui->menuCollections->insertMenu(ui->actionNewCollection, collectionMenu);
    ui->menuCollections->setTitle("Колекції збірок [" + activeCollectionName + "]");
    ui->actionNewBuild->setEnabled(true);

    connect(openCollection, &QAction::triggered, [this, collectionName]() {
        if (activeCollectionName == collectionName
            || !collection.connectToCollection(collectionName))
            return;

        activeCollectionName = collection.getName();
        ui->menuCollections->setTitle("Колекції збірок [" + activeCollectionName + "]");
        ui->menuBuilds->setTitle("Збірка");
        ui->treeWidget->setHeaderLabel(activeCollectionName);
        ui->treeWidget->clear();
        ui->actionDeleteBuild->setShortcut(QKeySequence());

        QStringList tempBuildNames = collection.getBuildNames();
        for (int i = 0; i < tempBuildNames.size(); i++) {
            QTreeWidgetItem *tempBuildItem = new QTreeWidgetItem;
            tempBuildItem->setText(0, tempBuildNames[i]);
            ui->treeWidget->addTopLevelItem(tempBuildItem);
        }
    });
    connect(deleteCollection,
            &QAction::triggered,
            [this, collectionMenu, openCollection, deleteCollection, collectionName]() {
                if (QMessageBox::warning(this,
                                         "Попередження",
                                         "Ви точно хочете видалити колекцію " + collectionName + "?",
                                         QMessageBox::Ok | QMessageBox::Cancel)
                        != QMessageBox::Ok
                    || !collection.deleteCollection(collectionName))
                    return;

                if (collectionName == activeCollectionName) {
                    ui->menuCollections->setTitle("Колекції збірок");
                    ui->menuBuilds->setTitle("Збірка");
                    ui->treeWidget->clear();
                    ui->treeWidget->setHeaderLabel("PC Constructor");
                    ui->menuBuilds->setEnabled(false);
                    ui->actionNewBuild->setEnabled(false);
                    ui->actionDeleteBuild->setShortcut(QKeySequence());
                    activeCollectionName.clear();
                }
                delete openCollection;
                delete deleteCollection;
                delete collectionMenu;
            });
}

void PC_Constructor::createBuild(QString buildName)
{
    if (buildName.isEmpty() || !collection.createBuild(buildName))
        return;

    activeBuildName = buildName;
    QTreeWidgetItem *buildItem = new QTreeWidgetItem;
    buildItem->setText(0, activeBuildName);
    ui->treeWidget->insertTopLevelItem(0, buildItem);
    ui->menuBuilds->setTitle("Збірка [" + activeBuildName + "]");
    ui->menuBuilds->setEnabled(true);
    ui->actionDeleteBuild->setShortcut(QKeySequence("Ctrl+Del"));

    QWidget *tempTabWidget = new QWidget(this);
    QHBoxLayout *tabHBoxLayout = new QHBoxLayout(tempTabWidget);
    tabHBoxLayout->addWidget(new ComponentsWidget, QSizePolicy::Maximum);
    tabHBoxLayout->addWidget(new SpecificationsWidget, QSizePolicy::Maximum);
    ui->tabWidget->addTab(tempTabWidget, buildName);
}
