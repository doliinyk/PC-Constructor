#include <QApplication>
#include "pc_constructor.h"

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    PC_Constructor mainWindow;

    mainWindow.show();

    return application.exec();
}
