#include "pc_constructor.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PC_Constructor w;
    w.show();
    return a.exec();
}
