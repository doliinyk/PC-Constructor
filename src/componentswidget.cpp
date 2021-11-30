#include "componentswidget.h"
#include "singlecomponentwidget.h"
#include "ui_componentswidget.h"

ComponentsWidget::ComponentsWidget(int buildId, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ComponentsWidget)
    , componentList({"motherboard", "cpu", "ram", "rom", "gpu", "powerSupply"})
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
    SingleComponentWidget *singleComponentWidget;

    if (!componentList.empty()) {
        ui->gridLayout->addWidget(
            singleComponentWidget = new SingleComponentWidget(componentList[0], buildId));

        componentList.erase(componentList.constBegin());
        if (componentList.empty())
            ui->frame->hide();

        connect(singleComponentWidget,
                &SingleComponentWidget::componentDeleted,
                this,
                [this, singleComponentWidget](QString componentName) {
                    if (ui->frame->isHidden())
                        ui->frame->show();

                    componentList.push_back(componentName);
                    delete singleComponentWidget;
                });
    }
}
