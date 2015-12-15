/*|============================================================================|
|                                                                              |
|  plcClient Communication class                                                |
|                                                                              |
|=============================================================================*/

#include <QDebug>

#include "core/snap7.h"
#include "plchandler.h"


#ifdef OS_WINDOWS
# define WIN32_LEAN_AND_MEAN
# include <windows.h>
#endif



PlcHandler::PlcHandler(QObject *parent) :
    QObject             (parent),
    plcClient           (nullptr),
    _address            (nullptr)
{
    isConnected = false;
    initRun = true;
    _dbNumber = 0;
    _connectionTryWasFault = 0;
    ok = 0;
    ko = 0;
}

PlcHandler::~PlcHandler()
{
    _makeDisconnect();

    delete plcClient;
    plcClient = nullptr;

    delete _address;
    _address = nullptr;
}
void PlcHandler::setIpAddress(const char* arg1)
{
    if ( _address != nullptr){
        delete _address;
        _address = nullptr;
    }
    _address = arg1;
}
void PlcHandler::setDbNumber(const int &arg1)
{
    _dbNumber = arg1;
}
//------------------------------------------------------------------------------
// Unit Connection
//------------------------------------------------------------------------------
bool PlcHandler::_makeConnect()
{
    _checkOkKo();                       // check OK and KO value - reset to 0 when OK + KO >= 1000

    int result = 0;
    if ( _address != nullptr ){         // checking if Ip address is correctly set
        if ( plcClient != nullptr ){    // destroy plcClient object if exist
            plcClient->Disconnect();
            delete plcClient;
            plcClient = nullptr;
        }
        int32_t timeout = 500;          // create int32_t object to handle SetParam value for plcClient object
        plcClient = new TS7Client();
        plcClient->SetParam( p_i32_PingTimeout, &timeout );
        plcClient->SetParam( p_i32_SendTimeout, &timeout );
        plcClient->SetParam( p_i32_RecvTimeout, &timeout );

        result = plcClient->ConnectTo(_address,PLCRACK,PLCSLOT);
        _pduNegotation = plcClient->PDULength();

        // checking of connection in time
        if ( ( result ==0 && isConnected == false ) ){

            if ( initRun == true || _connectionTryWasFault >= 5 ){
                initRun = false;
                qDebug() <<       "Connected to " +
                                  QString::fromStdString(_address) +
                                  " (Rack=" + QString::number(PLCRACK) +
                                  ", Slot=" + QString::number(PLCSLOT) +
                                  ")"
                                  ;

                qDebug() <<        "Execution time : " +
                                   QString::number(plcClient->ExecTime()) +
                                   "ms"
                                   ;
                qDebug() <<        "PDU : " +
                                   QString::number(plcClient->PDULength());
            }
            /* A S7 protocol job consists of:
                 Data preparation.
                 Data transmission.
                 Waiting for the response.
                 Decoding of the reply telegram.

                Each block transmitted is called PDU (protocol data unit) which is the greatest block that can be handled per transmission.
                The “max pdu size” concept belongs to the IsoTCP protocol and it’s negotiated during the S7 connection
                We need value at connection negotitation to calculate future multi read/write  PDU*/


            _connectionTryWasFault = 0;

            isConnected = plcClient->Connected();

            ok++;
            emit messageOk( ok );

        } else if ( result != 0 ){
            _currentError = QString::fromStdString(CliErrorText(result).c_str());
            int compareResoult = QString::compare( _currentError, _lastErrorMemory, Qt::CaseSensitive );
            if ( compareResoult != 0 && _connectionTryWasFault >=3 ) {
                _lastErrorMemory = QString::fromStdString(CliErrorText(result).c_str());
                qWarning() << "Connection NOK" ;
                qWarning() << "Problem is :" + QString::fromStdString(CliErrorText(result).c_str());
            }
            // if we lost connection for a long time (like a 5 time try), we set initRun connection to make an init write to PLC DB
            _connectionTryWasFault++;
            if ( _connectionTryWasFault >=5 ){
                initRun = true;
            }

            isConnected = plcClient->Connected();

            ko++;
            emit messageKo( ko );
        }
        emit connectionStatus( isConnected );
    }else {
        qCritical() << "Ip address of client is not set";
        emit messageText( "Ip address of client is not set" );
        ko++;
        emit messageKo(ko);
    }
    return result==0;
}
//------------------------------------------------------------------------------
// Unit Disconnection
//------------------------------------------------------------------------------
void PlcHandler::_makeDisconnect()
{
    plcClient->Disconnect();
}
//------------------------------------------------------------------------------
// Multi Read
//------------------------------------------------------------------------------
bool PlcHandler::makeMultiRead(RepoDestDbStruct* dbStruct)
{
    int result = 99;
    if ( _makeConnect() ){
        if ( _dbNumber != 0 ){

            /* PDU max payload is:
            PDULength-(14+4*Amunt*ItemsLenght), for NumItems=20 of byte it is 146. */
            _pduNegotation = _pduNegotation - ( 14 + 4 * ( 20 + 1 + 2 + 2 ) );

            qDebug() << "PDU negotiation on multiread is: " + QString::number(_pduNegotation);
            plcClient->SetParam( p_i32_PDURequest, &_pduNegotation );

            // Prepare struct
            TS7DataItem Items[4];

            // NOTE : *AMOUNT IS NOT SIZE* , it's the number of items

            byte referenceBuffer [20];
            Items[0].Area     = S7AreaDB;
            Items[0].WordLen  = S7WLByte;
            Items[0].DBNumber = _dbNumber;
            Items[0].Start    = 0;
            Items[0].Amount   = 20;
            Items[0].pdata    = &referenceBuffer;

            byte bitBuffer [1];
            Items[1].Area     = S7AreaDB;
            Items[1].WordLen  = S7WLByte;
            Items[1].DBNumber = _dbNumber;
            Items[1].Start    = 20;
            Items[1].Amount   = 1;
            Items[1].pdata    = &bitBuffer;

            byte faultBuffer[2];
            Items[2].Area     = S7AreaDB;
            Items[2].WordLen  = S7WLByte;
            Items[2].DBNumber = _dbNumber;
            Items[2].Start    = 30;
            Items[2].Amount   = 2;
            Items[2].pdata    = &faultBuffer;

            byte partsBuffer[2];
            Items[3].Area     = S7AreaDB;
            Items[3].WordLen  = S7WLByte;
            Items[3].DBNumber = _dbNumber;
            Items[3].Start    = 32;
            Items[3].Amount   = 2;
            Items[3].pdata    = &partsBuffer;

            result = plcClient->ReadMultiVars( &Items[0], 4 );

            if ( result == 0 )
            {
                // Result of plcClient->ReadMultivars is the "global result" of
                // the function, it's OK if something was exchanged.

                // But we need to check single Var results.
                // Let shall suppose that we ask for 5 vars, 4 of them are ok but
                // the 5th is inexistent, we will have 4 results ok and 1 not ok.

                if (Items[0].Result != 0){
                    emit messageText( "Problem with reading data from DB" + QString::number( _dbNumber ) + ".DBX0.0" );
                    result = 95;
                } else {
                    dbStruct->reference = QByteArray((const char*)&referenceBuffer);
                    dbStruct->reference = dbStruct->reference.left(20);
                    dbStruct->reference = dbStruct->reference.trimmed();
                }
                if (Items[1].Result != 0){
                    emit messageText( "Problem with reading data from DB" + QString::number( _dbNumber ) + ".DBB1" );
                    result = 95;
                } else {
                    dbStruct->fault         = GetBitAt( &bitBuffer, 0, 0 );
                    dbStruct->fault_ack     = GetBitAt( &bitBuffer, 0, 1 );
                    dbStruct->part_ok       = GetBitAt( &bitBuffer, 0, 2 );
                    dbStruct->part_ok_ack   = GetBitAt( &bitBuffer, 0, 3 );
                }
                if (Items[2].Result != 0){
                    emit messageText( "Problem with reading data from DB" + QString::number( _dbNumber ) + " current fault" );
                    result = 95;
                } else {
                    dbStruct->fault_number = GetIntAt( &faultBuffer, 0);
                }
                if (Items[3].Result != 0){
                    emit messageText( "Problem with reading data from DB" + QString::number( _dbNumber ) + "- partsInLastMinute" );
                    result = 95;
                } else {
                    dbStruct->partsInLastMinute     = GetIntAt( &partsBuffer, 0);
                }

                _check(result,"Multiread Vars"); // check result of reading
            };
        } else {
            result = 99;
            emit messageText( "Number of client DB is not set" );
            ko++;
            emit messageKo(ko);
        }
        _makeDisconnect();
    }
    return result != 0;
}
//------------------------------------------------------------------------------
// Multi Write
//------------------------------------------------------------------------------
bool PlcHandler::makeMultiWrite(RepoDestDbStruct* dbStruct)
{
    int result = 99;
    if ( _makeConnect() ){
        if ( _dbNumber != 0 ){
            /* PDU max payload is:
            PDULength-(14+4*Amunt*ItemsLenght), for NumItems=20 of byte it is 146. */
            _pduNegotation = _pduNegotation - ( 14 + 4 * ( 1 + 2 + 2 ) );
            qDebug() << "PDU negotiation on multiwrite is: " + QString::number(_pduNegotation);
            plcClient->SetParam( 10, &_pduNegotation );

            // Prepare struct
            TS7DataItem Items[3];

            // NOTE : *AMOUNT IS NOT SIZE* , it's the number of items
            byte bitBuffer [1];
            SetBitAt( &bitBuffer, 0, 0, dbStruct->fault );
            SetBitAt( &bitBuffer, 0, 1, dbStruct->fault_ack );
            SetBitAt( &bitBuffer, 0, 2, dbStruct->part_ok );
            SetBitAt( &bitBuffer, 0, 3, dbStruct->part_ok_ack );

            Items[0].Area     = S7AreaDB;
            Items[0].WordLen  = S7WLByte;
            Items[0].DBNumber = _dbNumber;
            Items[0].Start    = 20;
            Items[0].Amount   = 1;
            Items[0].pdata    = &bitBuffer;

            byte faultBuffer[2];
            SetIntAt( &faultBuffer, 0, dbStruct->fault_number);

            Items[1].Area     = S7AreaDB;
            Items[1].WordLen  = S7WLByte;
            Items[1].DBNumber = _dbNumber;
            Items[1].Start    = 30;
            Items[1].Amount   = 2;
            Items[1].pdata    = &faultBuffer;

            byte partsBuffer[2];
            SetIntAt( &partsBuffer, 0, dbStruct->fault_number);

            Items[2].Area     = S7AreaDB;
            Items[2].WordLen  = S7WLByte;
            Items[2].DBNumber = _dbNumber;
            Items[2].Start    = 32;
            Items[2].Amount   = 2;
            Items[2].pdata    = &partsBuffer;

            result=plcClient->WriteMultiVars(&Items[0],3);
            _check(result,"Multiwrite Vars");

        } else {
            result = 99;
            qCritical() << "Number of client DB is not set";
            emit messageText( "Number of client DB is not set" );
        }
        _makeDisconnect();
    }
    return result != 0;
}
//------------------------------------------------------------------------------
// Check error
//------------------------------------------------------------------------------
bool PlcHandler::_check(int &result, const char * function)
{
    if (result==0) {
        ok++;
        emit messageOk(ok);
    } else {
        qWarning() << "Something goes wrong with " + QString::fromStdString( function );
        ko++;
        emit messageKo(ko);
    }
    return result==0;
}
//------------------------------------------------------------------------------
// CPU Info : catalog
//------------------------------------------------------------------------------
void PlcHandler::_orderCode()
{
    TS7OrderCode Info;
    int res=plcClient->GetOrderCode(&Info);
    if (_check(res,"Catalog"))
    {
        qDebug("  Order Code : %s",Info.Code);
        qDebug("  Version    : %d.%d.%d",Info.V1,Info.V2,Info.V3);
    };
}
//------------------------------------------------------------------------------
// CPU Info : unit info
//------------------------------------------------------------------------------
void PlcHandler::_cpuInfo()
{
    TS7CpuInfo Info;
    int res=plcClient->GetCpuInfo(&Info);
    if (_check(res,"Unit Info"))
    {
        qDebug() <<  "  Module Type Name  : "+QString::fromStdString(Info.ModuleTypeName);
        qDebug() <<  "  Serial Number     : "+QString::fromStdString(Info.SerialNumber);
        qDebug() <<  "  AS Name           : "+QString::fromStdString(Info.ASName);
        qDebug() <<  "  Module Name       : "+QString::fromStdString(Info.ModuleName);
    };
}
//------------------------------------------------------------------------------
// PLC Status
//------------------------------------------------------------------------------
void PlcHandler::_unitStatus()
{
    int res=0;
    int Status=plcClient->PlcStatus();
    if (_check(res,"CPU Status"))
    {
        switch (Status)
        {
        case S7CpuStatusRun : qDebug("  RUN"); break;
        case S7CpuStatusStop: qDebug("  STOP"); break;
        default             : qDebug("  UNKNOWN"); break;
        }
    };
}
//------------------------------------------------------------------------------
// Tests Summary
//------------------------------------------------------------------------------
void PlcHandler::_summary()
{
    qDebug() << "+-----------------------------------------------------";
    qDebug() << "| Test Summary ";
    qDebug() << "+-----------------------------------------------------";
    qDebug() << "| Performed  : "+QString::number( ok + ko );
    qDebug() << "| Passed     : "+QString::number( ok );
    qDebug() << "| Failed     : "+QString::number( ko );
    qDebug() << "+-----------------------------------------------------";
}
//------------------------------------------------------------------------------
// Check OK and KO value
//------------------------------------------------------------------------------
void PlcHandler::_checkOkKo()
{
    int okAndKo = ok + ko;
    if ( okAndKo >= 1000 ){
        _summary();
        ok = 0;
        ko = 0;
        emit messageOk( ok );
        emit messageKo( ko );
    }
}

