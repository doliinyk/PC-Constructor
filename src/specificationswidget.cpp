#include "specificationswidget.h"
#include "componentsmanager.h"
#include "sqlitedbmanager.h"
#include "ui_specificationswidget.h"

SpecificationsWidget::SpecificationsWidget(QString componentType, int componentId, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SpecificationsWidget)
    , model(new QSqlTableModel(this, SQLiteDBManager::getInstance()->getDB()))
    , componentType(componentType)
    , componentId(componentId)
{
    ui->setupUi(this);
    ui->label->hide();

    if (!componentId)
        return;

    model->clear();
    model->setTable(componentType);
    model->select();

    for (int i = 0; i < model->columnCount(); i++)
        model->setHeaderData(i,
                             Qt::Horizontal,
                             ComponentsManager::translateTableHeader(
                                 model->headerData(i, Qt::Horizontal).toString()));

    ui->tableView->setModel(model);
    ui->tableView->hideColumn(0);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    int tempPhotoIndex = 0;
    for (int i = model->columnCount() - 1; i >= 0; i--)
        if (model->headerData(i, Qt::Horizontal).toString() == "Фотографія") {
            ui->tableView->hideColumn(i);
            tempPhotoIndex = i;
            break;
        }

    QString tempPhotoPath;
    for (int i = 0; i < model->rowCount(); i++) {
        if (model->index(i, 0).data().toInt() == componentId) {
            tempPhotoPath = model->index(i, tempPhotoIndex).data().toString();
            continue;
        }

        ui->tableView->hideRow(i);
    }

    for (int i = 0; i < model->rowCount(); i++)
        if (model->index(i, 1).data().toString().contains("Відсутн")
            && !model->index(i, 3).data().toInt() && !ui->tableView->isRowHidden(i)) {
            ui->tableView->hide();
            break;
        }

    if (componentType.startsWith("gpu"))
        model->setHeaderData(2, Qt::Horizontal, "Кількість пам'яті, Мб");

    ui->label->setPixmap(QPixmap(tempPhotoPath).scaled(600, 600, Qt::KeepAspectRatio));
    ui->label->show();
}

SpecificationsWidget::~SpecificationsWidget()
{
    delete model;
    delete ui;
}
