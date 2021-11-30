#include "componentswidget.h"
#include "singlecomponentwidget.h"
#include "ui_componentswidget.h"

ComponentsWidget::ComponentsWidget(unsigned int buildId, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ComponentsWidget)
    , componentList({"motherboard", "cpu", "rom", "ram", "supply", "gpu"})
    , buildId(buildId)
{
    ui->setupUi(this);
}

ComponentsWidget::~ComponentsWidget()
{
    delete ui;
}

void ComponentsWidget::on_addComponentButton_clicked()
{
    if (!componentList.empty()) {
        ui->gridLayout->addWidget(new SingleComponentWidget(componentList[0], buildId));
        componentList.erase(componentList.constBegin());
    } else
        ui->frame->hide();
}
