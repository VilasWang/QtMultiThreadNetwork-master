#include <QtWidgets/QApplication>
#include <QDir>
#include <QDebug>
#include "networktool.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon("resources/networktool.ico"));
    NetworkTool w;
    w.show();
    return a.exec();
}
