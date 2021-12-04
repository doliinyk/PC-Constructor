#ifndef PC_CONSTRUCTOR_H
#define PC_CONSTRUCTOR_H

#include <QMainWindow>
#include <QTreeWidget>
#include "componentsmanager.h"
#include "createbuilddialog.h"
#include "sqlitedbmanager.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class PC_Constructor;
}
QT_END_NAMESPACE

//  Клас-головне вікно, яке містить у собі всі створені у проєкті
//  віджети та вікна, об'єднує їх у одну систему і з'єднує їх функціонал
class PC_Constructor : public QMainWindow
{
    Q_OBJECT

public:
    PC_Constructor(QWidget *parent = nullptr);
    ~PC_Constructor();

private slots:
    void createBuild(QString buildName);
    void on_action_NewBuild_triggered();
    void on_actionDeleteBuild_triggered();
    void on_tabWidget_tabBarClicked(int index);
    void on_tabWidget_tabCloseRequested(int index);
    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *build);
    void on_actionOpenComponentsManager_triggered();
    void on_actionAbout_triggered();

private:
    Ui::PC_Constructor *ui;
    IDBManager *db;

    QString activeBuildName;
    CreateBuildDialog createBuildDialog;
    ComponentsManager componentsManager;

    void setBuildMenuBar();
    void setTreeWidgetBuilds();
    void setTabWidgetBuilds(QString componentType = "", int componentId = 0);
    void setStatusBar();
};

#endif // PC_CONSTRUCTOR_H
