#include "dbmanager.h"
#include "ui_dbmanager.h"

DBManager::DBManager(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DBManager)
{
    ui->setupUi(this);
}

DBManager::~DBManager()
{
    delete ui;
}
