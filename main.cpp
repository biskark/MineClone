#include <QApplication> 
#include <QFile>
#include <stdlib.h>
#include <time.h>

#include "mainwindow.h"

int main(int argc, char *argv[]) {
    srand(time(NULL));
    QApplication app(argc, argv);
    QFile style_file(":qss/mine.qss");
    if (style_file.open(QFile::ReadOnly)) {
        QString style_sheet = style_file.readAll();
        app.setStyleSheet(style_sheet);
    }
    MainWindow mainwin;
    mainwin.show();
    return app.exec();
}
