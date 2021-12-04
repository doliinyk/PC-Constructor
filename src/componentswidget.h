#ifndef COMPONENTSWIDGET_H
#define COMPONENTSWIDGET_H

#include <QWidget>
#include "singlecomponentwidget.h"
#include "sqlitedbmanager.h"

namespace Ui {
class ComponentsWidget;
}

//  Клас-віджет, який містить в собі віджети SingleComponentWidget
//  візуалізовує компоненти збірки і дозволяє їх змінювати
//  перевіряти їх сумісність
class ComponentsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ComponentsWidget(int buildId, QWidget *parent = nullptr);
    ~ComponentsWidget();

    void createWidget();

signals:
    void conflictResult(QStringList componentTypes);
    void clearWidget(QString componentType, int componentId);
    void lastComponentCreated();

private slots:
    void createComponent(int index = 0, bool isRestored = false);
    void on_singleComponentChanged(QString componentType, int componentId, bool isChoosed);
    void on_addComponentButton_clicked();

private:
    Ui::ComponentsWidget *ui;
    IDBManager *db;

    QStringList componentTypeList;
    QString componentConflictType;
    int buildId;
    bool isConflict;
    bool isRestored;

    void checkCompatibility(QString componentType,
                            int componentId,
                            SingleComponentWidget *singleComponentWidget);
    bool motherboardCompatibilityQuery(QString componentType, QString compareField, int componentId);
    bool printError();
};

#endif // COMPONENTSWIDGET_H
