#include "pc_constructor.h"
#include "ui_pc_constructor.h"

PC_Constructor::PC_Constructor(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PC_Constructor)
{
    ui->setupUi(this);
    lbl = new QLabel("Test", this);
    lbl->show();
}

PC_Constructor::~PC_Constructor()
{
    delete lbl;
    delete ui;
}

