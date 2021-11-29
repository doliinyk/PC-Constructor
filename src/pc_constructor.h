#ifndef PC_CONSTRUCTOR_H
#define PC_CONSTRUCTOR_H

#include <QMainWindow>
#include <QTreeWidget>
#include <QVector>
#include "collection.h"
#include "componentsmanager.h"
#include "componentswidget.h"
#include "createbuilddialog.h"
#include "createcollectiondialog.h"
#include "specificationswidget.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class PC_Constructor;
}
QT_END_NAMESPACE

class PC_Constructor : public QMainWindow
{
    Q_OBJECT

public:
    PC_Constructor(QWidget *parent = nullptr);
    ~PC_Constructor();

private slots:
    void on_actionNewCollection_triggered();
    void on_actionNewBuild_triggered();
    void on_actionAbout_triggered();
    void createCollection(QString);
    void createBuild(QString);
    void on_actionDeleteBuild_triggered();
    void on_tabWidget_tabBarClicked(int);
    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *);
    void on_actionOpenComponentsManager_triggered();

private:
    Ui::PC_Constructor *ui;

    Collection collection;
    QString activeCollectionName;
    QString activeBuildName;
    unsigned int lastBuildTabIndex;
    CreateBuildDialog createBuildDialog;
    CreateCollectionDialog createCollectionDialog;
    ComponentsManager componentsManager;
};

#endif // PC_CONSTRUCTOR_H
