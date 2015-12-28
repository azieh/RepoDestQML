#include "threadmanager.h"
#include <QtQuick/QQuickItem>
#include <QtQuick/QQuickView>
#include <QQmlEngine>
#include <QtQml>
#include <QtCore>
#include <QtQuick/QtQuick>
#include <QQmlComponent>
#include <QtQml/qqml.h>

ThreadManager::ThreadManager(QObject *parent) :
    QObject (parent),
    sqlH    (nullptr),
    settings(nullptr)
{
    loadSettings();
    createThreads();
    createViewEngine();
    createClientDeclaration();
}
ThreadManager::~ThreadManager()
{
    delete sqlH;
    sqlH = nullptr;

    //Quit and clear every thread


    //Clear memory about every station instance


}
//------------------------------------------------------------------------------
// Create threads
//------------------------------------------------------------------------------
void ThreadManager::createThreads()
{
    if ( sqlH != nullptr ){
        delete sqlH;
        sqlH = nullptr;
    }
    sqlH = new SqlHandler;
    sqlH->setApuDatabasePath( _apuDbPath, _apuDbName );
    sqlH->setPcsDatabasePath( _pcsDbPath, _pcsDbName );

    //first generete thread instance


}
//------------------------------------------------------------------------------
// Create client objects and give them settings
//------------------------------------------------------------------------------
void ThreadManager::createClientDeclaration()
{
    //genereting WorkArea class instance for each station on production line

    for (int i = 0; i < settingsList.size(); ++i){
        clientStruct singleClientStruct;
        singleClientStruct.client = new WorkArea(sqlH);
        singleClientStruct.thread = new QThread;
        singleClientStruct.clientWindow = new ClientWindow();
        clientList.append( singleClientStruct );
    }
    for (int i = 0; i < clientList.size(); ++i){
        clientList[i].client -> doSetup( clientList[i].thread );
        clientList[i].client -> moveToThread( clientList[i].thread );
        clientList[i].client -> setIpAddress( settingsList[i].ipAddress.data() );
        clientList[i].client -> setDbNumber( settingsList[i].dbNumber );
        clientList[i].client -> setName ( settingsList[i].stationName.data() );

        clientList[i].clientWindow->createWindows(engine);
        clientList[i].clientWindow->onStationNameUpdate( settingsList[i].stationName.data() );

        connect(clientList[i].client,SIGNAL(loopTime(const QString &)),clientList[i].clientWindow,SLOT(onLoopTimeUpdate(const QString &)));
        connect(clientList[i].client,SIGNAL(messageText(const QString &)),clientList[i].clientWindow,SLOT(onTextUpdate(const QString &)));
        connect(clientList[i].client,SIGNAL(messageOk(int)),clientList[i].clientWindow,SLOT(onOkUpdate(int)));
        connect(clientList[i].client,SIGNAL(messageKo(int)),clientList[i].clientWindow,SLOT(onNokUpdate(int)));
    }


//    cw1 = new ClientWindow();
//    cw1->createWindows(engine);

//    connect(st10,SIGNAL(messageKo(int)),cw1,SLOT(onNokUpdate(int)));


}
//------------------------------------------------------------------------------
// Start threads
//------------------------------------------------------------------------------
void ThreadManager::start()
{
    //start threads
    for (int i = 0; i < clientList.size(); ++i){
    clientList[i].thread->start();
    }

}
//------------------------------------------------------------------------------
// Load settings
//------------------------------------------------------------------------------
void ThreadManager::loadSettings()
{
    if ( settings != nullptr ){
        delete settings;
        settings = nullptr;
    }
    settings = new QSettings( QApplication::applicationDirPath() + "/settings.ini", QSettings::IniFormat );
    qDebug() << settings->status();


    _apuDbPath = settings->value( "APU_database/path" ).toString();
    _apuDbName = settings->value( "APU_database/name" ).toString();
    _pcsDbPath = settings->value( "PCS_database/path" ).toString();
    _pcsDbName = settings->value( "PCS_database/name" ).toString();

    int settingsSize = settings->beginReadArray("Logical_stations_settings");
    for ( int i = 0; i < settingsSize; ++i ){

      settings->setArrayIndex( i );
      settingsStruct singleSettings;

      singleSettings.ipAddress = settings->value( "Ip_address" ).toByteArray();
      singleSettings.dbNumber = settings->value( "Db_number" ).toInt();
      singleSettings.stationName = settings->value( "Station_name" ).toByteArray();

      settingsList.append( singleSettings );
    }
    settings->endArray();
    delete settings;
    settings = nullptr;
}
//------------------------------------------------------------------------------
// Create view engine
//------------------------------------------------------------------------------
void ThreadManager::createViewEngine()
{
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    QQuickWindow* window = qobject_cast<QQuickWindow*>(engine.rootObjects().at(0));

    window->show();
}
