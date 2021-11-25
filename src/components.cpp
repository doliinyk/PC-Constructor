#include "components.h"
#include "ui_components.h"

Components::Components(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Components)
{
    ui->setupUi(this);
}

Components::~Components()
{
    delete ui;
}
