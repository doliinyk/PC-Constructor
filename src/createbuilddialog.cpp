#include "createbuilddialog.h"
#include "ui_createbuilddialog.h"

CreateBuildDialog::CreateBuildDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateBuildDialog)
{
    ui->setupUi(this);
    setModal(true);
}

CreateBuildDialog::~CreateBuildDialog()
{
    delete ui;
}

void CreateBuildDialog::on_buttonBox_accepted()
{
    emit getBuildName(ui->lineEdit->text());
    ui->lineEdit->clear();
}

void CreateBuildDialog::on_buttonBox_rejected()
{
    ui->lineEdit->clear();
}

void CreateBuildDialog::on_CreateBuildDialog_finished(int result)
{
    if (!result)
        ui->lineEdit->clear();
}
