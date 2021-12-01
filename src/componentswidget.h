#ifndef COMPONENTSWIDGET_H
#define COMPONENTSWIDGET_H

#include <QSqlQuery>
#include <QWidget>
#include "sqlitedbmanager.h"

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
    IDBManager *db;

    QSqlQuery query;
    QStringList componentList;
    int buildId;

    void restoreComponents();
    void createComponent(int step = 0, bool isRestored = false);
};

#endif // COMPONENTSWIDGET_H
