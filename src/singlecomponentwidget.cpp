#include "singlecomponentwidget.h"
#include "ui_singlecomponentwidget.h"

SingleComponentWidget::SingleComponentWidget(QString componentName,
                                             unsigned int buildId,
                                             QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SingleComponentWidget)
    , db(SQLiteDBManager::getInstance())
    , buildId(buildId)
{
    ui->setupUi(this);

    if (componentName == "motherboard")
        ui->label->setText("Материнські плати");
    if (componentName == "cpu")
        ui->label->setText("Процесори");
    if (componentName == "rom")
        ui->label->setText("Накопичувачі");
    if (componentName == "ram")
        ui->label->setText("Оперативна пам'ять");
    if (componentName == "supply")
        ui->label->setText("Блоки живлення");
    if (componentName == "gpu")
        ui->label->setText("Відеокарти");

    buildModel = new QSqlTableModel(this, db->getDB());
    buildModel->setTable("builds");
    buildModel->select();
    activeComponentModel = new QSqlTableModel(this, db->getDB());
    activeComponentModel->setTable(componentName);
    activeComponentModel->select();
    for (int i = 0; i < activeComponentModel->rowCount(); i++)
        ui->comboBox->addItem(activeComponentModel->index(i, 1).data().toString());

    connect(ui->comboBox, &QComboBox::activated, this, [this, componentName, buildId](int index) {
        if (componentCompatibility(componentName))
            db->runScript("UPDATE builds SET " + componentName + " = "
                          + QString::number(activeComponentModel->index(index, 0).data().toUInt())
                          + " WHERE id = " + QString::number(buildId));
        else
            ui->comboBox->setStyleSheet("background-color: rgb(122, 50, 50);"
                                        "color: white");
    });
}

SingleComponentWidget::~SingleComponentWidget()
{
    delete db;
    delete ui;
}

void SingleComponentWidget::on_pushButton_clicked()
{
    delete this;
}

bool SingleComponentWidget::componentCompatibility(QString componentName)
{
    if (componentName == "cpu") {
        compareComponentModel = new QSqlTableModel(this, db->getDB());
        compareComponentModel->setTable("motherboard");
        compareComponentModel->select();
        unsigned int motherBoardId = buildModel->index(buildId, 2).data().toUInt();
        qDebug() << "motherboard: "
                        + compareComponentModel->index(motherBoardId, 1).data().toString();
        return true;
    }
    return true;
}
