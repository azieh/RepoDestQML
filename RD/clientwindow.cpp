#include "clientwindow.h"



ClientWindow::ClientWindow(QObject *parent) :
    QObject     (parent),
    clientObject(nullptr),
    root        (nullptr),
    context     (nullptr),
    component   (nullptr)
{

}
ClientWindow::~ClientWindow()
{
    delete clientObject;
    clientObject = nullptr;

    delete root;
    root = nullptr;

    delete context;
    context = nullptr;

    delete component;
    component = nullptr;
}
//------------------------------------------------------------------------------
// Create single client window
//------------------------------------------------------------------------------
void ClientWindow::createWindows(QQmlApplicationEngine& engine, QQuickWindow* window)
{
    if (root != nullptr){
        delete root;
        root = nullptr;
    }
    root = new QQuickItem;
    root = window->findChild<QQuickItem*>("gridLayoutRest"); // find QML object where the window will be add

    if (component != nullptr){
        delete component;
        component = nullptr;
    }
#ifdef QT_DEBUG
    component = new QQmlComponent(&engine, QUrl("qrc:/ClientUiForm.qml")); // load new QML window to engine
#else
    component = new QQmlComponent(&engine, QUrl::fromLocalFile(QStringLiteral("qml/ClientUiForm.qml"))); // load new QML window to engine
#endif
    if ( component->isError() ){
        qDebug() << component->errors();
    }
    if (context != nullptr){
        delete context;
        context = nullptr;
    }
    context = new QQmlContext(engine.rootContext());
    context->setContextProperty("clientWindow", this); //create signals/slots connection between C++ and QML

    if (clientObject != nullptr ){
        clientObject->deleteLater();
        clientObject = nullptr;
    }
    clientObject = new QQuickItem;
    clientObject = qobject_cast<QQuickItem*>(component->create(context)); //create complete QML object
    clientObject->setParentItem(root); //and set it a parent item


    QQmlEngine::setObjectOwnership(clientObject, QQmlEngine::CppOwnership);

    connect(this->clientObject, SIGNAL(clientWindowWasClicked( bool )), this, SLOT(onShowClientWindow(bool)));
    window_memory = window;
}
//------------------------------------------------------------------------------
// Signals and slots
//------------------------------------------------------------------------------
void ClientWindow::onShowClientWindow(bool b)
{
    root = window_memory->findChild<QQuickItem*>("gridLayoutFirst"); // find QML object where the window will be add
    clientObject->setParentItem(root);
}
void ClientWindow::onHideClientWindow(bool b)
{
    hideClientWindow(b);
    root = window_memory->findChild<QQuickItem*>("gridLayoutRest"); // find QML object where the window will be add
    clientObject->setParentItem(root);
}
void ClientWindow::onStationNameUpdate(QString text)
{
    stationNameUpdate(text);
}
void ClientWindow::onConnectionStatusUpdate(bool b)
{
    connectionStatusUpdate(b);
}
void ClientWindow::onLoopTimeUpdate(const QString &text)
{
    loopTimeUpdate(text);
}
void ClientWindow::onOkUpdate(int number)
{
    okUpdate(QString::number(number));
}
void ClientWindow::onNokUpdate(int number)
{
    nokUpdate(QString::number(number));
}
void ClientWindow::onTextUpdate(const QString &text)
{
    textUpdate(text);
}
void ClientWindow::onIpUpdate(QString text)
{
    ipUpdate(text);
}
void ClientWindow::onDbUpdate(int number)
{
    dbUpdate(QString::number(number));
}
