#ifndef SPECIFICATIONSWIDGET_H
#define SPECIFICATIONSWIDGET_H

#include <QSqlTableModel>
#include <QWidget>

namespace Ui {
class SpecificationsWidget;
}

//  Клас-віджет, який надає і дозволяє переглядати інформацію
//  про взятий компонент, також показує його зображення
class SpecificationsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SpecificationsWidget(QString componentType, int componentId, QWidget *parent = nullptr);
    ~SpecificationsWidget();

private:
    Ui::SpecificationsWidget *ui;
    QSqlTableModel *model;

    QString componentType;
    int componentId;
};

#endif // SPECIFICATIONSWIDGET_H
