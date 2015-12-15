#include "sqlhandler.h"

SqlHandler::SqlHandler(QObject *parent) :
    QObject     (parent),
    _database ( nullptr ),
    _query    ( nullptr )
{

}
SqlHandler::~SqlHandler()
{
    _database->close();
    delete _database;
    _database = nullptr;

    delete _query;
    _query = nullptr;
}
//------------------------------------------------------------------------------
// Set APU Database path
//------------------------------------------------------------------------------
void SqlHandler::setApuDatabasePath( QDir path, QString databaseName )
{
    _apuDbPath = path;
    _apuDbName = databaseName;
}
//------------------------------------------------------------------------------
// Set PCS Database path
//------------------------------------------------------------------------------
void SqlHandler::setPcsDatabasePath( QDir path, QString databaseName )
{
    _pcsDbPath = path;
    _pcsDbName = databaseName;
}
//------------------------------------------------------------------------------
// Open APU database
//------------------------------------------------------------------------------
bool SqlHandler::_openApuDatabase()
{
    if ( !_apuDbPath.exists() ){
        if ( _apuDbPath.mkpath(".") ){
            emit messageText( "LogMsg",
                              "Program create dir path for DB file: " +
                              _apuDbPath.path()
                              );
        } else {
            emit messageText( "LogMsg",
                              "Program has problem with creating path: " +
                              _apuDbPath.path()
                              );
        }
    }
    if ( _database != nullptr ){
        delete _database;
        _database = nullptr;
    }
    _database = new QSqlDatabase( QSqlDatabase::addDatabase( "QODBC3", "APUDatabase" ) );
    _database->setDatabaseName("DRIVER={Microsoft Access Driver (*.mdb)};"
                              "FIL={MS Access};"
                              "DBQ="+_apuDbPath.path()+_apuDbName );
    _database->setConnectOptions("SQL_ATTR_ODBC_VERSION=SQL_OV_ODBC3");
    _database->setHostName ( "PLC" );


    if ( !_database->open() ){
        emit messageText( "LogMsg",
                          "Cannot open Database " +
                          _apuDbPath.path()+_apuDbName+
                          "| Error: " + _database->lastError().text()
                          );
    }
    return _database->isOpen ();
}
//------------------------------------------------------------------------------
// Open PCS database
//------------------------------------------------------------------------------
bool SqlHandler::_openPcsDatabase()
{

    if ( _database != nullptr ){
        delete _database;
        _database = nullptr;
    }
    _database = new QSqlDatabase( QSqlDatabase::addDatabase("QODBC3", "PCSDatabase") );
    _database->setDatabaseName("DRIVER={Microsoft Access Driver (*.mdb)};"
                               "FIL={MS Access};"
                               "DBQ="+ _pcsDbPath.path() + _pcsDbName);
    _database->setConnectOptions("SQL_ATTR_ODBC_VERSION=SQL_OV_ODBC3");
    _database->setHostName ( "PLC" );


    if ( !_database->open() ){
        emit messageText( "LogMsg",
                          "Cannot open Database " +
                          _pcsDbPath.path() + _pcsDbName +
                          "| Error: " + _database->lastError().text() );
    }
    return _database->isOpen ();
}
//------------------------------------------------------------------------------
// Close APU database
//------------------------------------------------------------------------------
void SqlHandler::_closeDatabase()
{
    _database->close();
    QString connectionName = _database->connectionName();
    delete _database;
    _database = nullptr;
    _database = new QSqlDatabase();
    _database->removeDatabase( connectionName );
}
//------------------------------------------------------------------------------
// Create table in APU DB breakes
//------------------------------------------------------------------------------
bool SqlHandler::_createApuTable_Breakes(SqlApuDataStruct* data)
{
    bool tableExist = false;
    if ( _database->isOpen() ){
        QString command;
        tableExist = _database->tables().contains( QString( data->stationName + "_PRZESTOJE" ) );
        if ( !tableExist ){
            command = QString("CREATE TABLE %1_PRZESTOJE "
                              "(ID AUTOINCREMENT, Data DATE, Godzina TIME, Referencja TEXT, [Czas przestoju] DOUBLE, [Kod bledu] INTEGER, [Opis bledu] TEXT)"
                              ).arg( data->stationName );

            if ( _query != nullptr ){
                delete _query;
                _query = nullptr;
            }
            _query = new QSqlQuery( *_database );
            _query->prepare( command );

            if ( !_query->exec() ){
                emit messageText( "LogMsg",
                                  "SQL error when Table was created for " +
                                  data->stationName + "_PRZESTOJE" ": " +
                                  _query->lastError().text()
                                  );
            } else {
                emit messageText( "LogMsg",
                                  "SQL query: Correctly created table for " +
                                  data->stationName + "_PRZESTOJE"
                                  );
                tableExist = true;
            }
            _query->clear();
        }
    }
    return tableExist;
}
//------------------------------------------------------------------------------
// Create table in APU DB parts in last minute
//------------------------------------------------------------------------------
bool SqlHandler::_createApuTable_PartsInLastMinute(SqlApuDataStruct* data)
{
    bool tableExist = false;
    if ( _database->isOpen() ){
        QString command;
        tableExist = _database->tables().contains( QString( data->stationName + "_LICZNIK_SZTUK" ) );
        if ( !tableExist ){
            command = QString("CREATE TABLE %1_LICZNIK_SZTUK "
                              "(ID AUTOINCREMENT, Data DATE, Godzina TIME, Referencja TEXT, [Ilosc sztuk] INTEGER)"
                              ).arg( data->stationName );

            if ( _query != nullptr ){
                delete _query;
                _query = nullptr;
            }
            _query = new QSqlQuery( *_database );
            _query->prepare( command );

            if ( !_query->exec() ){
                emit messageText( "LogMsg",
                                  "SQL error when Table was created for " +
                                  data->stationName + "_LICZNIK_SZTUK" +
                                  ": " + _query->lastError().text()
                                  );
            } else {
                emit messageText( "LogMsg",
                                  "SQL query: Correctly created table for " +
                                  data->stationName + "_LICZNIK_SZTUK"
                                  );
                tableExist = true;
            }
            _query->clear();
        }
    }
    return tableExist;
}
//------------------------------------------------------------------------------
// Insert value in APU DB into breakes
//------------------------------------------------------------------------------
bool SqlHandler::insertValueIntoApu_Breakes(SqlApuDataStruct* data)
{   
    bool done = false;

    if ( _openApuDatabase() ){
        if ( _createApuTable_Breakes( data ) ){
            QString command;
            command = QString( "INSERT INTO %1_PRZESTOJE (Data, Godzina, Referencja, [Czas przestoju], [Kod bledu]) "
                               "VALUES ( %2, %3, %4, %5, %6)" )
                    .arg( data->stationName )
                    .arg( "'" + data->date + "'" )
                    .arg( "'" + data->hour + "'" )
                    .arg( "'" + data->reference + "'")
                    .arg( data->timeElapsed )
                    .arg( data->faultNumber );
            if ( _query != nullptr ){
                delete _query;
                _query = nullptr;
            }
            _query = new QSqlQuery( *_database );
            _query->prepare( command );

            if ( !_query->exec( ) ){
                emit messageText( "LogMsg",
                                  "SQL error when Insert value for " +
                                  data->stationName + "_PRZESTOJE" +
                                  ": " + _query->lastError().text()
                                  );
            } else {
                emit messageText( "LogMsg",
                                  "SQL query: Correctly inserted value for " +
                                  data->stationName + "_PRZESTOJE"
                                  );
                done = true;
            }
            _query->finish();
            _query->clear();
            _closeDatabase();
        }
    }
    return done != 0;
}
//------------------------------------------------------------------------------
// Insert value in APU DB into parts in last minute
//------------------------------------------------------------------------------
bool SqlHandler::insertValueIntoApu_PartsInLastMinute(SqlApuDataStruct* data)
{
    bool done = false;

    if ( _openApuDatabase() ){
        if ( _createApuTable_PartsInLastMinute( data ) ){
            QString command;
            command = QString( "INSERT INTO %1_LICZNIK_SZTUK (Data, Godzina, Referencja, [Ilosc sztuk]) "
                               "VALUES ( %2, %3, %4, %5)" )
                    .arg( data->stationName )
                    .arg( "'" + data->date + "'" )
                    .arg( "'" + data->hour + "'" )
                    .arg( "'" + data->reference + "'")
                    .arg( data->partsInLastMinute );

            if ( _query != nullptr ){
                delete _query;
                _query = nullptr;
            }
            _query = new QSqlQuery( *_database );
            _query->prepare( command );

            if ( !_query->exec( ) ){
                emit messageText( "LogMsg",
                                  "SQL error when Insert value for " +
                                  data->stationName + "_LICZNIK_SZTUK" +
                                  ": " + _query->lastError().text()
                                  );
            } else {
                emit messageText( "LogMsg",
                                  "SQL query: Correctly inserted value for " +
                                  data->stationName + "_LICZNIK_SZTUK"
                                  );
                done = true;
            }
            _query->finish();
            _query->clear();
            _closeDatabase();
        }
    }
    return done != 0;
}
//------------------------------------------------------------------------------
// Insert value in PCS DB into GSTOP_DATA
//------------------------------------------------------------------------------
bool SqlHandler::insertValueIntoPcs_GSTOP_DATA(SqlPcsDataStruct* data)
{
    /* Data need to be insert into GSTOP_DATA table
    QString     plant;            //fix value "SK01"
    QString     cellid;           //station name
    int         modeid;           //fix value 99
    QString     stopid;           //name of station + 0 + fault number
    QString     stoptype;         //fix value "M"
    QString     partnb;           //reference
    QString     kimpact;          //fix value "I"
    QString     refend;           //reference
    QString     empid;            //fix value "PLC"
    QString     shiftid;          //actual shift like "ZM1" || "ZM2" || "ZM3"
    QDateTime   stdate;           //begining of fault
    QDateTime   endate;           //end of fault
    int         stoptime;         //fault time in seconds
    QString     level1;           //name of station + 0 + fault number
    QDateTime   crdate;           //time of insert to database operation
    QString     cruser;           //fix value "PLC"
    QString     recstat;          //fix value "A"
    QDateTime   usertim1;         //time of insert to database operation
    QDateTime   usertim2;         //time of insert to database operation
    QDateTime   usertim3;         //time of insert to database operation
    QDateTime   usertim4;         //time of insert to database operation
    int         upload;           //fix value 0
    */

    bool done = false;
    if ( !_pcsDbPath.path().isEmpty() && !_pcsDbName.isEmpty() ){ //if path is declarated in settings.ini
        if ( data->stoptime <= 300){ // if fault time is less than 5min(300s)
            if ( _openPcsDatabase() ){

                if ( _query != nullptr ){
                    delete _query;
                    _query = nullptr;
                }
                _query = new QSqlQuery( *_database );
                _query->prepare( QString( "INSERT INTO GSTOP_DATA (PLANT, RECID, CELLID, MODEID, STOPID, STOPTYPE, PARTNB, "
                                          "KIMPACT, REFEND, EMPID, SHIFTID, STDATE, ENDATE, STOPTIME, LEVEL1, CRDATE, "
                                          "CRUSER, RECSTAT, USERTIM1, USERTIM2, USERTIM3, USERTIM4, UPLOAD) "
                                          "VALUES ( :plant, :recid, :cellid, :modeid, :stopid, :stoptype, :partnb, "
                                          ":kimpact, :refend, :empid, :shiftid, :stdate, :endate, :stoptime, :level1, :crdate, "
                                          ":cruser, :recstat, :usertim1, :usertim2, :usertim3, :usertim4, :upload)" )
                                 );
                _query->bindValue( ":plant",     data->plant     );
                _query->bindValue( ":recid",     data->recid     );
                _query->bindValue( ":cellid",    data->cellid    );
                _query->bindValue( ":modeid",    data->modeid    );
                _query->bindValue( ":stopid",    data->stopid    );
                _query->bindValue( ":stoptype",  data->stoptype  );
                _query->bindValue( ":partnb",    data->partnb    );
                _query->bindValue( ":kimpact",   data->kimpact   );
                _query->bindValue( ":refend",    data->refend    );
                _query->bindValue( ":empid",     data->empid     );
                _query->bindValue( ":shiftid",   data->shiftid   );
                _query->bindValue( ":stdate",    data->stdate    );
                _query->bindValue( ":endate",    data->endate    );
                _query->bindValue( ":stoptime",  data->stoptime  );
                _query->bindValue( ":level1",    data->level1    );
                _query->bindValue( ":crdate",    data->crdate    );
                _query->bindValue( ":cruser",    data->cruser    );
                _query->bindValue( ":recstat",   data->recstat   );
                _query->bindValue( ":usertim1",  data->usertim1  );
                _query->bindValue( ":usertim2",  data->usertim2  );
                _query->bindValue( ":usertim3",  data->usertim3  );
                _query->bindValue( ":usertim4",  data->usertim4  );
                _query->bindValue( ":upload",    data->upload    );

                if ( !_query->exec( ) ){
                    emit messageText( "LogMsg",
                                      "SQL error when Insert value for "
                                      "GSTOP_DATA"
                                      ": " + _query->lastError().text()
                                      );
                } else {
                    emit messageText( "LogMsg",
                                      "SQL query: Correctly inserted value for "
                                      "GSTOP_DATA"
                                      );
                    done = true;
                }
                _query->finish();
                _query->clear();
                _closeDatabase();
            }
        }
    }
    return done != 0;
}




