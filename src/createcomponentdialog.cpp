#include "createcomponentdialog.h"
#include "ui_createcomponentdialog.h"

CreateComponentDialog::CreateComponentDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateComponentDialog)
{
    ui->setupUi(this);
    setModal(true);
    setFixedSize(QSize(450, 80));
}

CreateComponentDialog::~CreateComponentDialog()
{
    delete ui;
}
