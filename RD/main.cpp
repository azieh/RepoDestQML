
#include <QMessageBox>
#include <QSharedMemory>
#include <QtGui/QGuiApplication>


#include "threadmanager.h"

int main(int argc, char *argv[])
{

    QGuiApplication app(argc, argv);

    QCoreApplication::setOrganizationName("Adrian Wasielewski");
    QCoreApplication::setApplicationName("Raportowanie przestojów");


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

    ThreadManager tm;
    tm.start();

    app.setQuitOnLastWindowClosed(false); // option to check what window was closed



    return app.exec();
}
