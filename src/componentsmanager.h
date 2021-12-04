#ifndef COMPONENTSMANAGER_H
#define COMPONENTSMANAGER_H

#include <QSqlTableModel>
#include <QTreeWidgetItem>
#include <QWidget>
#include "sqlitedbmanager.h"

namespace Ui {
class ComponentsManager;
}

//  Клас-вікно, який реалізовує графічний менеджер бази даних і
//  дозволяє вносити/редагувати/видаляти компоненти з таблиць БД
class ComponentsManager : public QWidget
{
    Q_OBJECT

public:
    explicit ComponentsManager(QWidget *parent = nullptr);
    ~ComponentsManager();

    static QString translateTextToComponent(QString text);
    static QString translateTableHeader(QString text);

private slots:
    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *component);
    void on_tableView_clicked(const QModelIndex &index);
    void on_addComponentButton_clicked();
    void on_deleteComponentButton_clicked();
    void on_photoComponentButton_clicked();

private:
    Ui::ComponentsManager *ui;
    QSqlTableModel *model;
    IDBManager *db;

    QString activeComponentType;
    int activeRow;
};

#endif // COMPONENTSMANAGER_H
