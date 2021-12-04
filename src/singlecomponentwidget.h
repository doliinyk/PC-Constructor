#ifndef SINGLECOMPONENTWIDGET_H
#define SINGLECOMPONENTWIDGET_H

#include <QVector>
#include <QWidget>
#include "sqlitedbmanager.h"

namespace Ui {
class SingleComponentWidget;
}

//  Клас-віджет, який графічно представляє один взятий компонент збірки
//  дозволяє його змінювати, реагує на несумісність компонентів
class SingleComponentWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SingleComponentWidget(QString componentType,
                                   int buildId,
                                   bool isRestored,
                                   QWidget *parent = nullptr);
    ~SingleComponentWidget();

    void createWidget();
    void setComboBoxColor(bool value);
    static QString isComponentTypeSecond(QString componentType);
    static QString translateComponentToText(QString componentType);

signals:
    void componentCreated(QString componentType,
                          int componentId,
                          SingleComponentWidget *singleComponentWidget);
    void componentChanged(QString componentType, int componentId = 0, bool isDeleted = true);
    void specificationsRequest(QString componentType, int componentId);

private slots:
    void on_componentBox_activated(int index);
    void on_deleteButton_clicked();

private:
    Ui::SingleComponentWidget *ui;
    IDBManager *db;

    int buildId;
    bool isRestored;
    int componentId;
    QString componentType;

    int findIdByName(QString componentType, QString componentName);
};

#endif // SINGLECOMPONENTWIDGET_H
