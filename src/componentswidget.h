#ifndef COMPONENTSWIDGET_H
#define COMPONENTSWIDGET_H

#include <QWidget>
#include "singlecomponentwidget.h"
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

    void createWidget();

signals:
    void conflictResult(QStringList componentTypes);

private slots:
    void on_addComponentButton_clicked();

private:
    Ui::ComponentsWidget *ui;
    IDBManager *db;

    QStringList componentTypeList;
    QStringList conflictList;
    int buildId;
    bool isRestored;

    void createComponent(int index = 0, bool isRestored = false);
    void checkCompatibility(QString componentType,
                            int componentId,
                            SingleComponentWidget *singleComponentWidget);
    bool compatibilityQuery(QString componentType, QString compareField, int componentId);
};

#endif // COMPONENTSWIDGET_H
