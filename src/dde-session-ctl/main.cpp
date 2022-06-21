#include <QCoreApplication>
#include <QCommandLineParser>
#include <QDebug>
#include <QDBusInterface>

#include "Session.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QCommandLineParser parser;
    parser.setApplicationDescription("dde-session-ctl");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption shutdown(QStringList{ "s", "shutdown", "shutdown dde"});
    parser.addOption(shutdown);

    QCommandLineOption restartDBus(QStringList{"r", "restart-dbus", "restart dbus.service"});
    parser.addOption(restartDBus);

    QCommandLineOption logout(QStringList{"l", "logout", "logout session"});
    parser.addOption(logout);

    parser.process(app);

    const bool isShutdown = parser.isSet(shutdown);
    const bool isRestartDBus = parser.isSet(restartDBus);

    if (parser.isSet(logout)) {
        org::deepin::Session session("org.deepin.Session", "/org/deepin/Session", QDBusConnection::sessionBus());
        session.Logout();

        return 0;
    }

    if (isShutdown) {
        // kill startdde-session or call login1
        QDBusInterface systemd("org.freedesktop.systemd1", "/org/freedesktop/systemd1", "org.freedesktop.systemd1.Manager");
        qInfo() << systemd.call("StartUnit", "dde-session-shutdown.target", "replace-irreversibly");
        return 0;
    }

    if (isRestartDBus) {
        QDBusInterface systemd("org.freedesktop.systemd1", "/org/freedesktop/systemd1", "org.freedesktop.systemd1.Manager");
        qInfo() << systemd.call("StopUnit", "dbus.service", "replace-irreversibly");
        return 0;
    }

    return 0;
}