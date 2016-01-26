#include "threadmanager.h"



ThreadManager::ThreadManager(QObject *parent) :
    QObject     (parent),
    mainWindow  (nullptr),
    sqlH        (nullptr),
    settings    (nullptr)
{
    if (mainWindow != nullptr){
        delete mainWindow;
        mainWindow = nullptr;
    }
    mainWindow = new MainWindow();

    loadSettings();
    initSqlConnection();
    createClientDeclaration();

}
ThreadManager::~ThreadManager()
{
    delete mainWindow;
    mainWindow = nullptr;

    delete sqlH;
    sqlH = nullptr;

    delete settings;
    settings = nullptr;

    clientList.clear();
}
//------------------------------------------------------------------------------
// Init SQL Connection
//------------------------------------------------------------------------------
void ThreadManager::initSqlConnection()
{
    if ( sqlH != nullptr ){
        delete sqlH;
        sqlH = nullptr;
    }
    sqlH = new SqlHandler;
    sqlH->setApuDatabasePath( _apuDbPath, _apuDbName );
    sqlH->setPcsDatabasePath( _pcsDbPath, _pcsDbName );

    connect(sqlH,SIGNAL(messageText(QString,QString)),mainWindow,SLOT(onTextUpdate(QString, QString)));
    if ( !_apuDbName.isEmpty() ){
        mainWindow->apuUpdate( _apuDbPath.path() + _apuDbName);
    }
    if ( !_pcsDbName.isEmpty() ){
        mainWindow->pcsUpdate( _pcsDbPath.path() + _pcsDbName);
    }

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

        clientList[i].clientWindow->createWindows(mainWindow->engine, mainWindow->window);
        clientList[i].clientWindow->onStationNameUpdate( settingsList[i].stationName.data() );
        clientList[i].clientWindow->onIpUpdate( settingsList[i].ipAddress.data() );
        clientList[i].clientWindow->onDbUpdate( settingsList[i].dbNumber );

        connect(clientList[i].client,SIGNAL(loopTime( const QString & )),clientList[i].clientWindow,SLOT(onLoopTimeUpdate( const QString & )));
        connect(clientList[i].client,SIGNAL(connectionStatus( bool )),clientList[i].clientWindow,SLOT(onConnectionStatusUpdate( bool )));
        connect(clientList[i].client,SIGNAL(messageOk( int )),clientList[i].clientWindow,SLOT(onOkUpdate( int )));
        connect(clientList[i].client,SIGNAL(messageKo( int )),clientList[i].clientWindow,SLOT(onNokUpdate( int )));
        connect(clientList[i].client,SIGNAL(messageText( const QString & )),clientList[i].clientWindow,SLOT(onTextUpdate( const QString & )));
    }
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
    mainWindow->window->hide();
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
    qDebug() << "Settings status:" << settings->status();


    _apuDbPath = settings->value( "APU_database/path" ).toString();
    _apuDbName = settings->value( "APU_database/name" ).toString();
    _pcsDbPath = settings->value( "PCS_database/path" ).toString();
    _pcsDbName = settings->value( "PCS_database/name" ).toString();

    int settingsSize = settings->beginReadArray("Logical_stations_settings");
    if ( settingsSize == 0 ){
        QMessageBox msgBox;
        msgBox.setText( QObject::tr( " Wystąpił problem podczas otwarcia pliku 'settings.ini'. " ));
        msgBox.setIcon( QMessageBox::Critical );
        msgBox.exec();
        exit(0);
    }
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


