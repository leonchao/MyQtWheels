#include <QApplication>
#include "colorwidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CColorWidget wid;
    wid.resize(270,440);
    wid.show();
    return a.exec();
}
