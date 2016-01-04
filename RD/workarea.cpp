#include <QDebug>
#include <QTimer>
#include <QMutex>
#include "workarea.h"

QMutex mutex;

WorkArea::WorkArea(SqlHandler* sqlHandler, QObject *parent) :
    QObject     (parent),
    dbStruct    (nullptr),
    _thread     (nullptr),
    _plcHandler  (nullptr),
    _sqlHandler (sqlHandler)
{
    _faultIsDetected        = false;
    _makeWritingPlc         = true;
    _makeWritingSql         = false;
    _prepareDataForPartsInLastMinute = false;
    _repeatThreadTime       = 500;

    dbStruct = new RepoDestDbStruct;
    if ( dbStruct != nullptr ){
        dbStruct->fault         = false;
        dbStruct->fault_ack     = false;
        dbStruct->part_ok       = false;
        dbStruct->part_ok_ack   = false;
        dbStruct->fault_number  = 0;
        dbStruct->partsInLastMinute = 0;
    }

    sqlApuDataStruct = new SqlApuDataStruct;
    if (sqlApuDataStruct != nullptr ){
        // only intiger value have to be init with 0
        sqlApuDataStruct->faultNumber = 0;
        sqlApuDataStruct->timeElapsed = 0.0;
        sqlApuDataStruct->partsInLastMinute = 0;
    }

    sqlPcsDataStruct = new SqlPcsDataStruct;
    if (sqlPcsDataStruct != nullptr ){
        // only intiger value have to be init with 0
        sqlPcsDataStruct->modeid    = 0;
        sqlPcsDataStruct->stoptime  = 0;
        // QDateTime value have initial value set to start program DateTime
        sqlPcsDataStruct->crdate    = QDateTime::currentDateTime();
        sqlPcsDataStruct->endate    = QDateTime::currentDateTime();
        sqlPcsDataStruct->stdate    = QDateTime::currentDateTime();
        sqlPcsDataStruct->usertim1  = QDateTime::currentDateTime();
        sqlPcsDataStruct->usertim2  = QDateTime::currentDateTime();
        sqlPcsDataStruct->usertim3  = QDateTime::currentDateTime();
        sqlPcsDataStruct->usertim4  = QDateTime::currentDateTime();
        // Set fixed initial value
        sqlPcsDataStruct->plant     = "SK01";
        sqlPcsDataStruct->modeid    = 99;
        sqlPcsDataStruct->stoptype  = "M";
        sqlPcsDataStruct->kimpact   = "I";
        sqlPcsDataStruct->empid     = "PLC";
        sqlPcsDataStruct->cruser    = "PLC";
        sqlPcsDataStruct->recstat   = "A";
        sqlPcsDataStruct->upload    = 0;
    }
    _plcHandler = new PlcHandler;







}
WorkArea::~WorkArea()
{
    delete _thread;
    _thread = nullptr;

    delete _plcHandler;
    _plcHandler = nullptr;
}
//------------------------------------------------------------------------------
// Main work area
//------------------------------------------------------------------------------
void WorkArea::mainOperation()
{
    checkLoopTime();
    _date = _date.currentDate();
    _time = _time.currentTime();
    _datetime = _datetime.currentDateTime();

    if ( _makeWritingPlc ) _makeWritingPlc = _plcHandler->makeMultiWrite( dbStruct );

    else {
        _plcHandler->makeMultiRead( dbStruct );
        checkDbStruct( dbStruct );
    }
    repeatThread();                                                         //method to make loop thread
}
//------------------------------------------------------------------------------
// Do setup for object
//------------------------------------------------------------------------------
void WorkArea::doSetup(QThread* cThread)
{
    if ( _thread != nullptr ){
        delete _thread;
        _thread = nullptr;
    }
    _thread = cThread;

    connect(_thread, SIGNAL( started() ), this, SLOT(mainOperation()));
    connect(_thread, SIGNAL( finished() ), _thread, SLOT(start()));

    connect(_plcHandler, SIGNAL( messageText( const QString & )),   this, SLOT( messageTextChanged( const QString & ) ));
    connect(_plcHandler, SIGNAL( connectionStatus( bool )),         this, SLOT( labelStatus_Changed( bool ) ));
    connect(_plcHandler, SIGNAL( messageOk( int )),                 this, SLOT( lineEditOk_Changed( int ) ));
    connect(_plcHandler, SIGNAL( messageKo( int )),                 this, SLOT( lineEditNok_Changed( int ) ));



}
//------------------------------------------------------------------------------
// Repeat thread
//------------------------------------------------------------------------------
void WorkArea::repeatThread()
{
    QTimer::singleShot(_repeatThreadTime, _thread, SLOT(quit()));
}
//------------------------------------------------------------------------------
// Check loop time
//------------------------------------------------------------------------------
void WorkArea::checkLoopTime()
{
    if ( _loopTimer.isValid() ){
        emit loopTime ( QString::number( _loopTimer.elapsed() ) + "ms"); // read elapsed time of one loop timer
    }
    _loopTimer.start(); // start one loop timer
}
//------------------------------------------------------------------------------
// SQL handler call for insert data into Parts in last minute
//------------------------------------------------------------------------------
void WorkArea::sqlHInsertDataPartsInLastMinute()
{
    mutex.lock();                              // need to lock this part of code | program can have only one connection to database in one time
    _sqlHandler->insertValueIntoApu_PartsInLastMinute( sqlApuDataStruct );
    mutex.unlock();
}
//------------------------------------------------------------------------------
// SQL handler call for insert data into Last fault information
//------------------------------------------------------------------------------
void WorkArea::sqlHInsertDataLastFaultInfo()
{
    mutex.lock();                              // need to lock this part of code | program can have only one connection to database in one time
    _sqlHandler->insertValueIntoApu_Breakes( sqlApuDataStruct );
    _sqlHandler->insertValueIntoPcs_GSTOP_DATA( sqlPcsDataStruct );
    mutex.unlock();
}
//------------------------------------------------------------------------------
// Check DB structure
//------------------------------------------------------------------------------
void WorkArea::checkDbStruct(RepoDestDbStruct* dbStruct)
{
    checkDbForFault             ( dbStruct );
    checkDbForPartOk            ( dbStruct );
    checkDbForPartsInLastMinute ( dbStruct );
}
//------------------------------------------------------------------------------
// Signals and slot area
//------------------------------------------------------------------------------
void WorkArea::checkDbForFault( RepoDestDbStruct* dbStruct )
{
    if ( dbStruct->fault == true  && _faultIsDetected == false ){
        _faultTimer.start(); // if we have fault on mstation -> start timer


        /*-----------------------------------------------------------------------------------------
         *  Preapare SQL APU Datastruct
         * ---------------------------------------------------------------------------------------*/

        sqlApuDataStruct->stationName  = _name;
        sqlApuDataStruct->date         = _date.toString("yy-MM-dd"); // get current date of fault
        sqlApuDataStruct->hour         = _time.toString("hh:mm"); // get current time of fault
        sqlApuDataStruct->faultNumber  = dbStruct->fault_number; // coppy fault number
        sqlApuDataStruct->reference    = dbStruct->reference;    // coppy reference number

        /*-----------------------------------------------------------------------------------------
         *  Preapare SQL PCS Datastruct
         * ---------------------------------------------------------------------------------------*/

        sqlPcsDataStruct->cellid       = _name;
        sqlPcsDataStruct->recid        = pcsRecIdGenerator();
        sqlPcsDataStruct->stopid       = _name + "0" + QString::number( dbStruct->fault_number );
        sqlPcsDataStruct->partnb       = dbStruct->reference;
        sqlPcsDataStruct->refend       = dbStruct->reference;
        sqlPcsDataStruct->shiftid      = shiftCheck();
        sqlPcsDataStruct->stdate       = _datetime;
        sqlPcsDataStruct->level1       = _name + "0" + QString::number( dbStruct->fault_number );

        emit messageText(  _date.toString("yy-MM-dd") + "       " + _time.toString("hh:mm") );
        emit messageText(  "Fault on machine number " + QString::number( sqlApuDataStruct->faultNumber ) );
        emit messageText(  "On reference: " + sqlApuDataStruct->reference );

        // after read problem data, set ack bit for PLC
        dbStruct->fault         = false;
        dbStruct->fault_ack     = true;
        dbStruct->part_ok       = false;
        dbStruct->part_ok_ack   = false;
        dbStruct->fault_number  = 0;

        _faultIsDetected        = true;
        _makeWritingPlc         = true;

    }
}
//------------------------------------------------------------------------------
// Signals and slot area
//------------------------------------------------------------------------------
void WorkArea::checkDbForPartOk( RepoDestDbStruct* dbStruct )
{
    if ( _faultIsDetected == true && dbStruct->part_ok == true ){

        int _faultTimeElapsedMemory         =  _faultTimer.elapsed(); // if we have first good part after fault, read time since start timer

        /* IF WE WANT RESULT IN mm:ss we need to start this part of code */
        /*
        int _faultTimeElapsedMemoryMinutes  =   _faultTimeElapsedMemory / 60000; // coppy time from ms to minute
        int _faultTimeElapsedMemorySeconds  = ( _faultTimeElapsedMemory % 60000 ) / 1000; // coppy time from ms to seconds
        // change prepared time value to double in format [min.ss]
        sqlDataStruct->timeElapsed          = (double)_faultTimeElapsedMemoryMinutes + ( (double)_faultTimeElapsedMemorySeconds / 100.0 );
        emit messageText( _name, "Fault time = " + ( QString::number( _faultTimeElapsedMemoryMinutes) + " min ") + ( QString::number ( _faultTimeElapsedMemorySeconds ) + " sec") );
        */
        /* IF WE WANT RESULT IN seconds we need to start this part of code */
        int _faultTimeElapsedMemorySeconds  = ( _faultTimeElapsedMemory ) / 1000; // coppy time from ms to seconds
        // change prepared time value to double in format [ss.ms] but we use just seconds format in that moment

        /*-----------------------------------------------------------------------------------------
         *  Preapare SQL APU Datastruct
         * ---------------------------------------------------------------------------------------*/
        sqlApuDataStruct->timeElapsed          =  (double)_faultTimeElapsedMemorySeconds;
        /*-----------------------------------------------------------------------------------------
         *  Preapare PCS APU Datastruct
         * ---------------------------------------------------------------------------------------*/
        sqlPcsDataStruct->endate               = _datetime;
        sqlPcsDataStruct->stoptime             = _faultTimeElapsedMemorySeconds;
        sqlPcsDataStruct->crdate               = _datetime;
        sqlPcsDataStruct->usertim1             = _datetime;
        sqlPcsDataStruct->usertim2             = _datetime;
        sqlPcsDataStruct->usertim3             = _datetime;
        sqlPcsDataStruct->usertim4             = _datetime;

        emit messageText(  "Fault time = " + ( QString::number ( _faultTimeElapsedMemorySeconds ) + " sec") );
        emit messageText(  "-----------------------");

        // after read ok product data, set ack bit for PLC
        dbStruct->fault         = false;
        dbStruct->fault_ack     = false;
        dbStruct->part_ok       = false;
        dbStruct->part_ok_ack   = true;
        dbStruct->fault_number  = 0;


        _makeWritingPlc  = true;

        sqlHInsertDataLastFaultInfo();

        _faultIsDetected = false;
    }
}
//------------------------------------------------------------------------------
// Signals and slot area
//------------------------------------------------------------------------------
void WorkArea::checkDbForPartsInLastMinute(RepoDestDbStruct* dbStruct)
{
    if ( _prepareDataForPartsInLastMinute == false ){
        if ( _time.second() == 0) _prepareDataForPartsInLastMinute = true;
    }

    if ( _prepareDataForPartsInLastMinute && _time.second() != 0 ){
        sqlApuDataStruct->stationName          = _name;
        sqlApuDataStruct->date                 = _date.toString("yy-MM-dd"); // get current date of fault
        sqlApuDataStruct->hour                 = _time.toString("hh:mm"); // get current time of fault
        sqlApuDataStruct->reference            = dbStruct->reference;    // coppy reference number
        sqlApuDataStruct->partsInLastMinute    = dbStruct->partsInLastMinute;

        dbStruct->partsInLastMinute = 0;

        _makeWritingPlc = true;

        sqlHInsertDataPartsInLastMinute();

        _prepareDataForPartsInLastMinute = false;
    }
}
//------------------------------------------------------------------------------
// Shift check
//------------------------------------------------------------------------------
QString WorkArea::shiftCheck()
{
    QString shiftName;

    if ( _time.hour() >= 6 && _time.hour() < 14 ){
        shiftName = "ZM1";
    }
    else if ( _time.hour() >= 14 && _time.hour() < 22 ){
        shiftName = "ZM2";
    }
    else if ( _time.hour() >= 22 && _time.hour() < 6 ){
        shiftName = "ZM3";
    }

    return shiftName;
}
//------------------------------------------------------------------------------
// PCS RECID generator
//------------------------------------------------------------------------------
QString WorkArea::pcsRecIdGenerator()
{
    QString recId;

    recId = "PLC" + QString::number( _date.dayOfYear() ) + _time.toString("HH") + _time.toString("mm");

    return recId;
}
//------------------------------------------------------------------------------
// Signals and slot area
//------------------------------------------------------------------------------
void WorkArea::setIpAddress(const char *arg1)
{
    _plcHandler->setIpAddress( arg1 );
}
void WorkArea::setDbNumber(const int &arg1)
{
    _plcHandler->setDbNumber( arg1 );
}
void WorkArea::setName(const char *arg1)
{
    _name = arg1;
}
void WorkArea::messageTextChanged(const QString &arg1)
{
    emit messageText( arg1);
}
void WorkArea::labelStatus_Changed(bool arg1)
{
    emit connectionStatus( arg1);
}
void WorkArea::lineEditOk_Changed(int arg1)
{
    emit messageOk( arg1);
}
void WorkArea::lineEditNok_Changed(int arg1)
{
    emit messageKo( arg1);
}
