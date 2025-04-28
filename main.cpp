#include <QApplication>
#include <QFile>
#include <QDir>
#include "loginwindow.h"
#include "userdatabase.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Set application style
    QFile styleFile(":/styles/style.qss");
    if (styleFile.open(QFile::ReadOnly)) {
        a.setStyleSheet(styleFile.readAll());
        styleFile.close();
    }

    // Create database directory if it doesn't exist
    QDir dir;
    if (!dir.exists("data")) {
        dir.mkdir("data");
    }

    // Initialize user database
    UserDatabase::instance();

    // Show login window
    LoginWindow w;
    w.show();

    return a.exec();
}
