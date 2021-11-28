#include "pc_constructor.h"
#include "ui_pc_constructor.h"

PC_Constructor::PC_Constructor(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PC_Constructor)
    , collection(new Collection)
{
    ui->setupUi(this);
    ui->menuBuilds->setEnabled(false);
    ui->actionNewBuild->setEnabled(false);

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
    });
}

PC_Constructor::~PC_Constructor()
{
    delete collection;
    delete ui;
}

void PC_Constructor::createCollection(QString collectionName)
{
    if (!collection->createCollection(collectionName))
        return;
    //collections.push_back(*collection);

    QMenu *collectionMenu = new QMenu(collection->getName());
    QAction *openCollection = new QAction("Відкрити...");
    QAction *deleteCollection = new QAction("Видалити...");

    activeCollectionName = collection->getName();
    collectionMenu->addAction(openCollection);
    collectionMenu->addAction(deleteCollection);
    ui->menuCollections->insertMenu(ui->actionNewCollection, collectionMenu);
    ui->menuCollections->setTitle("Колекції збірок [" + activeCollectionName + "]");
    ui->treeWidget->setHeaderLabel(activeCollectionName);
    ui->actionNewBuild->setEnabled(true);

    connect(openCollection, &QAction::triggered, [this]() {
        if (activeCollectionName != collection->getName())
            if (collection->connectToDB()) {
                activeCollectionName = collection->getName();
                ui->menuCollections->setTitle("Колекції збірок [" + activeCollectionName + "]");
                ui->treeWidget->setHeaderLabel(activeCollectionName);
            }
    });
    connect(deleteCollection,
            &QAction::triggered,
            [this, collectionMenu, openCollection, deleteCollection]() {
                if (collection->deleteDB()) {
                    delete openCollection;
                    delete deleteCollection;
                    delete collectionMenu;
                    //                    for (QVector<Collection>::iterator it = collections.begin();
                    //                         it != collections.end();
                    //                         it++)
                    //                        if (it->getName() == collection->getName())
                    //                            collections.erase(it);
                    if (activeCollectionName == ui->treeWidget->headerItem()->text(0)) {
                        ui->menuCollections->setTitle("Колекції збірок");
                        ui->menuBuilds->setTitle("Збірка");
                        ui->treeWidget->clear();
                        ui->treeWidget->setHeaderLabel("PC Constructor");
                        ui->menuBuilds->setEnabled(false);
                        ui->actionNewBuild->setEnabled(false);
                        activeCollectionName.clear();
                    }
                }
            });
}

void PC_Constructor::createBuild(QString buildName)
{
    if (!collection->createBuild(buildName))
        return;

    activeBuildName = buildName;
    ui->menuBuilds->setTitle("Збірка [" + activeBuildName + "]");
    QTreeWidgetItem *buildItem = new QTreeWidgetItem;
    buildItem->setText(0, activeBuildName);
    ui->treeWidget->addTopLevelItem(buildItem);
    ui->menuBuilds->setEnabled(true);
    //    connect(ui->treeWidget, &QTreeWidget::itemDoubleClicked, [](QTreeWidgetItem *build) {
    //        qDebug() << build->text(0);
    //    });
}

void PC_Constructor::on_actionNewCollection_triggered()
{
    createCollectionDialog.show();
}

void PC_Constructor::on_actionNewBuild_triggered()
{
    createBuildDialog.show();
}

void PC_Constructor::on_actionAbout_triggered() {}

void PC_Constructor::on_actionDeleteBuild_triggered()
{
    if (!collection->deleteBuild(activeBuildName))
        return;

    ui->treeWidget->clear();
    ui->treeWidget->setHeaderLabel(activeCollectionName);
    for (auto &it : collection->getBuildNames()) {
        QTreeWidgetItem *tempBuildItem = new QTreeWidgetItem;
        tempBuildItem->setText(0, it);
        ui->treeWidget->addTopLevelItem(tempBuildItem);
    }
    ui->menuBuilds->setTitle("Збірка");
    ui->menuBuilds->setEnabled(false);
    activeBuildName.clear();
}
