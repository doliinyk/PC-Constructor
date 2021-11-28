#include "createbuilddialog.h"
#include "ui_createbuilddialog.h"

CreateBuildDialog::CreateBuildDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateBuildDialog)
{
    ui->setupUi(this);
    setModal(true);
    setFixedSize(QSize(450, 80));
}

CreateBuildDialog::~CreateBuildDialog()
{
    delete ui;
}

void CreateBuildDialog::on_buttonBox_accepted()
{
    emit getBuildName(ui->lineEdit->text());
    ui->lineEdit->setText("");
}
