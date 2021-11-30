#ifndef COMPONENTSWIDGET_H
#define COMPONENTSWIDGET_H

#include <QWidget>

namespace Ui {
class ComponentsWidget;
}

class ComponentsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ComponentsWidget(int buildId, QWidget *parent = nullptr);
    ~ComponentsWidget();

private slots:
    void on_addComponentButton_clicked();

private:
    Ui::ComponentsWidget *ui;

    QStringList componentList;
    int buildId;
};

#endif // COMPONENTSWIDGET_H
