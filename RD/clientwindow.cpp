#include "clientwindow.h"



ClientWindow::ClientWindow(QObject *parent) : QObject(parent)
{



}

void ClientWindow::createWindows(QQmlApplicationEngine& engine)
{
    QQuickWindow* window = qobject_cast<QQuickWindow*>(engine.rootObjects().at(0));
    if (!window) {
        qFatal("Error: Your root item has to be a window.");
    }
    QQuickItem *root = window->contentItem();

    QQmlComponent component1(&engine, QUrl("qrc:/ClientUiForm.qml"));
    engine.rootContext()->setContextProperty("asdasd", "_name");

    object1 = qobject_cast<QQuickItem*>(component1.create());
    if (component1.isError()){
        qWarning() << component1.errorString();
    }

    QQmlEngine::setObjectOwnership(object1, QQmlEngine::CppOwnership);
    object1->setParentItem(root);
    object1->setParent(&engine);
}
