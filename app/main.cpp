#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
//#include <QQuickStyle>
#include <QIcon>
#include <QCommandLineParser>
#include <QFileInfo>
#include <QDebug>
#include <index.h>
#include "inx.h"

#ifdef Q_OS_ANDROID
#include <QGuiApplication>
#include <QIcon>
#else
#include <QApplication>
#endif

#ifdef STATIC_KIRIGAMI
#include "../3rdparty/kirigami/src/kirigamiplugin.h"
#include "../mauikit/src/mauikit.h"
#endif

#include "MauiKit/tagging.h"
#include "MauiKit/fm.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

#ifdef Q_OS_ANDROID
    QGuiApplication app(argc, argv);
#else
    QApplication app(argc, argv);
#endif

    app.setApplicationName(INX::app);
    app.setApplicationVersion(INX::version);
    app.setApplicationDisplayName(INX::app);
    app.setWindowIcon(QIcon(":/index.png"));

    QCommandLineParser parser;
    parser.setApplicationDescription(INX::description);
    const QCommandLineOption versionOption = parser.addVersionOption();
    parser.addOption(versionOption);
    parser.process(app);

    Index index;
    auto tag = Tagging::getInstance(INX::app, INX::version, "org.kde.index");

    QQmlApplicationEngine engine;

    auto context = engine.rootContext();

    context->setContextProperty("inx", &index);
    context->setContextProperty("tag", tag);

#ifdef STATIC_KIRIGAMI
    KirigamiPlugin::getInstance().registerTypes();
    MauiKit::getInstance().registerTypes();
#endif

#ifdef Q_OS_ANDROID
    QIcon::setThemeName("Luv");
#else
    QStringList importPathList = engine.importPathList();
    importPathList.prepend(QCoreApplication::applicationDirPath() + "/kde/qmltermwidget");
    engine.setImportPathList(importPathList);
//    QQuickStyle::setStyle("material");
#endif

    engine.load(QUrl(QLatin1String("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
