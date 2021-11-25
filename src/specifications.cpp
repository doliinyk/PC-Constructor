#include "specifications.h"
#include "ui_specifications.h"

Specifications::Specifications(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Specifications)
{
    ui->setupUi(this);
}

Specifications::~Specifications()
{
    delete ui;
}
