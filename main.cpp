/*
 * This is a simple task manager that allows user to start
 * new tasks choosing from available types of them.
 * User can choose type, interval and add some type-related
 * informations.
 *
 * The code includes some lists and map population to easily
 * allow the development of future functions such as listing,
 * start and stop or other management of tasks.
 *
 * Simple GUI and SystemTrayMenu are included in this version.
 *
 * Qt version: 5.9.9
 *
 * System requirement: Windows XP, 7, 8.1, 10
 *
 */

#include <QApplication>

#ifndef QT_NO_SYSTEMTRAYICON

#include <QMessageBox>
#include "window.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        QMessageBox::critical(0, QObject::tr("Ti-Care Task Manager"),
                              QObject::tr("Ti-Care Task Manager couldn't "
                                          "detect any system tray "
                                          "on this system, app usage is limited"));
        return 1;
    }
    QApplication::setQuitOnLastWindowClosed(false);

    Window window;
    window.show();
    return app.exec();
}

#endif
