#ifndef CLIENT_H
#define CLIENT_H
#include <QObject>
#include <QApplication>

#include "core/snap7.h"


// Default value for
const int PLCRACK  = 0; // Rack and
const int PLCSLOT  = 2; // Slot

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
    void setIpAddress(const char* arg1);
    void setDbNumber(const int &arg1);

    bool makeMultiRead(RepoDestDbStruct* dbStruct);
    bool makeMultiWrite(RepoDestDbStruct* dbStruct);

private:

    QString         _currentError;
    QString         _lastErrorMemory;

    const char*     _address;              // PLC IP Address
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
