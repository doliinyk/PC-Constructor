#include "createcollectiondialog.h"
#include "ui_createcollectiondialog.h"

CreateCollectionDialog::CreateCollectionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateCollectionDialog)
{
    ui->setupUi(this);
    setModal(true);
    setFixedSize(QSize(450, 80));
}

CreateCollectionDialog::~CreateCollectionDialog()
{
    delete ui;
}

void CreateCollectionDialog::on_buttonBox_accepted()
{
    emit getCollectionName(ui->lineEdit->text());
    ui->lineEdit->setText("");
}
