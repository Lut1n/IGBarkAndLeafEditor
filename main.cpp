#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlProperty>
#include <QQmlContext>

#include <QSurfaceFormat>

#include "CustomView/impgearsitem.h"

#include "GuiContext/loadinginfo.h"
#include "GuiContext/displayoptions.h"
#include "GuiContext/treemodel.h"
#include "GuiContext/barkmodel.h"
#include "GuiContext/leafmodel.h"
#include "GuiContext/impgearsconfig.h"

int main(int argc, char *argv[])
{

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QSurfaceFormat fmt;
    fmt.setDepthBufferSize(24);

    // Request OpenGL 3.3 core or OpenGL ES 3.0.
    if (QOpenGLContext::openGLModuleType() == QOpenGLContext::LibGL) {
        qDebug("Requesting 3.3 core context");
        fmt.setVersion(3, 3);
        fmt.setProfile(QSurfaceFormat::CoreProfile);
    } else {
        qDebug("Requesting 3.0 context");
        fmt.setVersion(3, 0);
    }

    QSurfaceFormat::setDefaultFormat(fmt);


    QGuiApplication app(argc, argv);

    LoadingInfo loadinginfo;
    DisplayOptions displayoptions;
    TreeModel treeModel;
    BarkModel barkmodel;
    LeafModel leafmodel;
    ImpGearsConfig igconfig;

    qmlRegisterType<ImpGearsItem>("ImpGears", 1, 0, "ImpGearsItem");

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    engine.rootContext()->setContextProperty("loadinginfo", &loadinginfo);
    engine.rootContext()->setContextProperty("displayoptions", &displayoptions);
    engine.rootContext()->setContextProperty("treemodel", &treeModel);
    engine.rootContext()->setContextProperty("barkmodel", &barkmodel);
    engine.rootContext()->setContextProperty("leafmodel", &leafmodel);
    engine.rootContext()->setContextProperty("igconfig", &igconfig);

    loadinginfo.signalProgress(1.0, "starting");
    engine.load(url);

    return app.exec();
}
