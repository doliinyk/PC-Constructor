#include "componentswidget.h"
#include "singlecomponentwidget.h"
#include "ui_componentswidget.h"

ComponentsWidget::ComponentsWidget(int buildId, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ComponentsWidget)
    , db(SQLiteDBManager::getInstance())
    , query(db->getDB())
    , componentList({"motherboard", "cpu", "ram", "rom", "rom2", "gpu", "gpu2", "powerSupply"})
    , buildId(buildId)
{
    ui->setupUi(this);

    restoreComponents();
}

ComponentsWidget::~ComponentsWidget()
{
    delete ui;
}

void ComponentsWidget::on_addComponentButton_clicked()
{
    createComponent();
}

void ComponentsWidget::restoreComponents()
{
    query.exec(QString("SELECT * FROM builds WHERE id = %1").arg(buildId));
    while (query.next()) {
        int deletedCount = 0;
        for (int i = 2; i <= 9; i++) {
            if (!query.value(i).toUInt())
                continue;

            createComponent(i - 2 - deletedCount, true);

            deletedCount++;
        }
    }
}

void ComponentsWidget::createComponent(int step, bool isRestored)
{
    SingleComponentWidget *singleComponentWidget;

    if (!componentList.empty()) {
        ui->gridLayout->addWidget(
            singleComponentWidget = new SingleComponentWidget(componentList[step],
                                                              buildId,
                                                              isRestored));

        componentList.erase(componentList.constBegin() + step);
        if (componentList.empty())
            ui->frame->hide();

        connect(singleComponentWidget,
                &SingleComponentWidget::componentDeleted,
                this,
                [this, singleComponentWidget](QString componentName) {
                    if (ui->frame->isHidden())
                        ui->frame->show();

                    db->runScript(QString("UPDATE builds SET %1 = NULL WHERE id = %2")
                                      .arg(componentName)
                                      .arg(buildId));

                    componentList.push_back(componentName);
                    delete singleComponentWidget;
                });
    }
}
