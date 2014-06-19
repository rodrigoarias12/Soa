#include <QtGui/QApplication>
#include "TP4.h"


int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    TP4 foo;
    foo.show();
    return app.exec();
}
