#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QLocale>
#include <QTranslator>
#include <QQuickView>


#include <utility>
#include <memory>
#include "InterMessageBroker.h"
#include "JsonReader.h"
#include "Common.h"
#include <QProcess>
#include <unistd.h>
#include <iostream>

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

    auto configurationJson=readJsonFromFile(NETSHIELD_DATA_CONFIGURATION);

    bool isModuleLoaded=configurationJson[MODULE_LOADED]=="yes";
    DriverCommunication driver(isModuleLoaded);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "NetShield_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            app.installTranslator(&translator);
            break;
        }
    }

    QQmlApplicationEngine engine;

    InterMessageBroker broker(&app, &driver);
    engine.rootContext()->setContextProperty("interMessageBroker", &broker);

    const QUrl url(QStringLiteral("qrc:/main.qml"));

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {

        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
