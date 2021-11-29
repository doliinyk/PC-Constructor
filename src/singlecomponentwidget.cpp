#include "singlecomponentwidget.h"
#include "ui_singlecomponentwidget.h"

SingleComponentWidget::SingleComponentWidget(QString componentName, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SingleComponentWidget)
{
    ui->setupUi(this);
    ui->label->setText(componentName);
}

SingleComponentWidget::~SingleComponentWidget()
{
    delete ui;
}

void SingleComponentWidget::on_pushButton_clicked()
{
    delete this;
}
