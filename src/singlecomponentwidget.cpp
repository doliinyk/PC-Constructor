#include "singlecomponentwidget.h"
#include "ui_singlecomponentwidget.h"

SingleComponentWidget::SingleComponentWidget(QString componentName, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SingleComponentWidget)
    , db(SQLiteDBManager::getInstance())
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

    model = new QSqlTableModel(this, db->getDB());
    model->setTable("__" + componentName);
    model->select();
    for (int i = 0; i < model->rowCount(); i++)
        ui->comboBox->addItem(model->index(i, 1).data().toString());
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
