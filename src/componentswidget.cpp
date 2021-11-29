#include "componentswidget.h"
#include "singlecomponentwidget.h"
#include "ui_componentswidget.h"

ComponentsWidget::ComponentsWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ComponentsWidget)
{
    ui->setupUi(this);
}

ComponentsWidget::~ComponentsWidget()
{
    delete ui;
}

void ComponentsWidget::on_pushButton_clicked() {}
