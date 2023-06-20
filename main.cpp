#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "kbmlang.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setApplicationName("kbLanguage");
    QCoreApplication::setOrganizationName("LandWell");
    QCoreApplication::setOrganizationDomain("land-well.com");

    qmlRegisterType<KbmLang>("KbmLang", 1, 0, "KbmLang");

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
