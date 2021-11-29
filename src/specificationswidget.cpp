#include "specificationswidget.h"
#include "ui_specificationswidget.h"

SpecificationsWidget::SpecificationsWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SpecificationsWidget)
{
    ui->setupUi(this);
}

SpecificationsWidget::~SpecificationsWidget()
{
    delete ui;
}
