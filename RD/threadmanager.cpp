#include "threadmanager.h"


ThreadManager::ThreadManager(QObject *parent) :
    QObject (parent),
    sqlH    (nullptr),
    thread1 (nullptr),
    st10    (nullptr),
    settings(nullptr)
{
    loadSettings();
    createThreads();
    createClientDeclaration();

}
ThreadManager::~ThreadManager()
{
    delete sqlH;
    sqlH = nullptr;

    //Quit and clear every thread
    thread1->quit();
    delete thread1;
    thread1 = nullptr;

    //Clear memory about every station instance
    delete st10;
    st10 = nullptr;

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
    if ( thread1 != nullptr){
        thread1->quit();
        delete thread1;
        thread1 = nullptr;
    }
    thread1 = new QThread;

}
//------------------------------------------------------------------------------
// Create client objects and give them settings
//------------------------------------------------------------------------------
void ThreadManager::createClientDeclaration()
{
    //genereting WorkArea class instance for each station on production line

    if ( st10 != nullptr){
        delete st10;
        st10 = nullptr;
    }
    st10 = new WorkArea(sqlH);
    st10->doSetup       ( thread1 );  // declarate thread for program
    st10->moveToThread  ( thread1 ); // declarate thread for program
    st10->setIpAddress  ( settingsList[0].ipAddress.data() );
    st10->setDbNumber   ( settingsList[0].dbNumber );
    st10->setName       ( settingsList[0].stationName.data() );


    st20 = new WorkArea(sqlH);
    st20->doSetup       ( thread1 );  // declarate thread for program
    st20->moveToThread  ( thread1 ); // declarate thread for program
    st20->setIpAddress  ( settingsList[1].ipAddress.data() );
    st20->setDbNumber   ( settingsList[1].dbNumber );
    st20->setName       ( settingsList[1].stationName.data() );

}
//------------------------------------------------------------------------------
// Start threads
//------------------------------------------------------------------------------
void ThreadManager::start()
{
    //start threads
   thread1->start();
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

