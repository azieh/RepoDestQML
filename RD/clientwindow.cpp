#include "clientwindow.h"



ClientWindow::ClientWindow(QObject *parent) :
    QObject     (parent),
    clientObject(nullptr),
    window      (nullptr),
    root        (nullptr),
    context     (nullptr),
    component   (nullptr)
{

}
void ClientWindow::createWindows(QQmlApplicationEngine& engine)
{
    if (window != nullptr){
        delete window;
        window = nullptr;
    }
    window = new QQuickWindow;
    window = qobject_cast<QQuickWindow*>(engine.rootObjects().at(0));
    if (!window) {
        qFatal("Error: Your root item has to be a window.");
    }

    if (root != nullptr){
        delete root;
        root = nullptr;
    }
    root = new QQuickItem;
    root = window->findChild<QQuickItem*>("gridLayout");

    if (component != nullptr){
        delete component;
        component = nullptr;
    }
    component = new QQmlComponent(&engine, QUrl("qrc:/ClientUiForm.qml"));

    if (context != nullptr){
        delete context;
        context = nullptr;
    }
    QQmlContext* context = new QQmlContext(engine.rootContext());
    context->setContextProperty("clientWindow", this);

    if (clientObject != nullptr ){
        clientObject->deleteLater();
        clientObject = nullptr;
    }
    clientObject = new QQuickItem;
    clientObject = qobject_cast<QQuickItem*>(component->create(context));
    if (component->isError()){
        qWarning() << component->errorString();
    }

    QQmlEngine::setObjectOwnership(clientObject, QQmlEngine::CppOwnership);

    clientObject->setParentItem(root);

}
void ClientWindow::onNokUpdate(int number)
{
    nokUpdate(QString::number(number));
}
void ClientWindow::onOkUpdate(int number)
{
    okUpdate(QString::number(number));
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
