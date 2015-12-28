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
    QQuickItem* root = window->findChild<QQuickItem*>("gridLayout");
    QQmlContext* context = new QQmlContext(engine.rootContext());
    QQmlComponent component(&engine, QUrl("qrc:/ClientUiForm.qml"));

    context->setContextProperty("clientWindow", this);

    clientObject = qobject_cast<QQuickItem*>(component.create(context));
    if (component.isError()){
        qWarning() << component.errorString();
    }

    QQmlEngine::setObjectOwnership(clientObject, QQmlEngine::CppOwnership);

    clientObject->setParentItem(root);

}
void ClientWindow::onNokUpdate(int text)
{
    nokUpdate(QString::number(text));
}
void ClientWindow::onOkUpdate(int text)
{
    okUpdate(QString::number(text));
}
void ClientWindow::onLoopTimeUpdate(const QString &text)
{
    loopTimeUpdate(text);
}
void ClientWindow::onTextUpdate(const QString &text)
{
    textUpdate(text);
}
void ClientWindow::onStationNameUpdate(QString text)
{
    stationNameUpdate(text);
}
