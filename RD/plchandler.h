#ifndef CLIENT_H
#define CLIENT_H
#include <QObject>
#include <QApplication>

#include "core/snap7.h"

struct RepoDestDbStruct{    // Struct of RepoDest DB
    QString reference;
    bool    fault;
    bool    fault_ack;
    bool    part_ok;
    bool    part_ok_ack;
    int     fault_number;
    int     partsInLastMinute;

};

class PlcHandler : public QObject, private TS7Client
{
    Q_OBJECT
public:
    explicit PlcHandler( QObject *parent = 0 );
    ~PlcHandler();

    TS7Client* plcClient;

    bool isConnected;

    int ok;// = 0; // Number of test pass
    int ko;// = 0; // Number of test failure

    bool initRun;
    void setPlcParameters(const QString &typeOfPlc, const char* ip, const int rackOrLocalTsap, const int slotOrRemoteTsap);
    void setDbNumber(const int &dbNumber);
    bool makeMultiRead(RepoDestDbStruct* dbStruct);
    bool makeMultiWrite(RepoDestDbStruct* dbStruct);

private:

    QString         _currentError;
    QString         _lastErrorMemory;

    QString         _typeOfPlc;            // Type of PLC device like "S7-200", "S7-300", "S1200"
    const char*     _address;              // PLC IP Address
    int             _rack;                 // PLC Rack  - only in case of use Siemens S7-300, S1200
    int             _slot;                 // PLC Slot /
    int             _localTsap;            // PC-local TSAP   - only in case of use Siemens S7-200
    int             _remoteTsap;           // PLC-remote TSAP /
    int             _dbNumber;             // Number of RepoDest DB
    int             _connectionTryWasFault;// Number of fault connection
    int32_t         _pduNegotation;        // Size of S7 PDU ethernet frame
    bool            _check( int &result, const char* function );
    void            _orderCode();
    void            _cpuInfo();
    void            _unitStatus();
    void            _summary();
    void            _checkOkKo();
    bool            _makeConnect();
    void            _makeDisconnect();

signals:

    void messageText( const QString & );
    void messageOk( int );
    void messageKo( int );
    void connectionStatus( bool );

};
#endif // CLIENT_H
