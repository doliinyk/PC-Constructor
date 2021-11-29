#include "componentswidget.h"
#include "singlecomponentwidget.h"
#include "ui_componentswidget.h"

ComponentsWidget::ComponentsWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ComponentsWidget)
    , componentNames({"Материнська плата",
                      "Процесор",
                      "Накопичувач",
                      "Оперативна пам'ять",
                      "Блок живлення",
                      "Відеокарта 1",
                      "Відеокарта 2"})
{
    ui->setupUi(this);
}

ComponentsWidget::~ComponentsWidget()
{
    delete ui;
}

void ComponentsWidget::on_pushButton_clicked()
{
    static int count = 0;
    if (count < componentNames.size())
        ui->gridLayout->addWidget(new SingleComponentWidget(componentNames.at(count++), this));
}
