
#include <QMessageBox>
#include <QSharedMemory>
#include <QtGui/QGuiApplication>

#include <QtQuick/QQuickItem>
#include <QtQuick/QQuickView>
#include <QQmlEngine>
#include <QtQml>
#include <QtCore>
#include <QtQuick/QtQuick>
#include <QQmlComponent>
#include <QtQml/qqml.h>
#include "threadmanager.h"
#include "clientwindow.h"
int main(int argc, char *argv[])
{

    QGuiApplication app(argc, argv);

    QCoreApplication::setOrganizationName("Adrian Wasielewski");
    QCoreApplication::setApplicationName("Raportowanie przestojów");
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    QQuickWindow* window = qobject_cast<QQuickWindow*>(engine.rootObjects().at(0));
    window->show();
    ClientWindow cw1;
    cw1.createWindows(engine);
    cw1.object1->setY(240);
    ClientWindow cw2;
    cw2.createWindows(engine);
    cw2.object1->setY(0);

    QSharedMemory shared("62d60669-bb94-4a94-88bb-b964890a7e04");   //Creating shared memory to every single application

    if( !shared.create( 512, QSharedMemory::ReadWrite) )
    {
        //in case we have already one shared memory like above we should exit next app
        QMessageBox msgBox;
        msgBox.setText( QObject::tr( "Program został już uruchomiony. "
                                     "Sprawdź pasek zadań. Jeżeli problem występuje nadal sprawdź "
                                     "Menadżer zadań i zamknij proces RepoDest "));
        msgBox.setIcon( QMessageBox::Critical );
        msgBox.exec();
        exit(0);
    }



    app.setQuitOnLastWindowClosed(false); // option to check what window was closed



    return app.exec();
}
