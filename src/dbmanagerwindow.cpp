#include "dbmanagerwindow.h"
#include "ui_dbmanagerwindow.h"

DBManagerWindow::DBManagerWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::DBManagerWindow)
{
    ui->setupUi(this);
}

DBManagerWindow::~DBManagerWindow()
{
    delete ui;
}
