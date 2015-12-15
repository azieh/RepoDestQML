#ifndef SQLHANDLER_H
#define SQLHANDLER_H
#include <QObject>
#include <QApplication>
#include <QDebug>
#include <QtSql>

//const QDir    APUDATABASEPATH  ("P:/APU1/Baza_mikrozatrzyman");
//const QString APUDBFILENAME    ("/RB1.mdb");

const QDir    APUDATABASEPATH  ("C:/Baza_przestojow");
const QString APUDBFILENAME    ("/RB1.mdb");

const QDir    PCSDATABASEPATH  ("C:/PCS/pcsrec");
const QString PCSDBFILENAME    ("/pcsrec.mdb");

struct SqlApuDataStruct{    // Struct defined by Apu engineers
    QString     stationName;
    QString     date;
    QString     hour;
    QString     reference;
    double      timeElapsed;
    int         faultNumber;
    int         partsInLastMinute;
};

struct SqlPcsDataStruct{   // Struct defined by IT for PCS software
    QString     plant;            //fix value "SK01"
    QString     recid;            // ??
    QString     cellid;           //station name
    int         modeid;           //fix value 99
    QString     stopid;           //name of station + 0 + fault number
    QString     stoptype;         //fix value "M"
    QString     kimpact;          //fix value "I"
    QString     partnb;           //reference
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
};

class SqlHandler : public QObject
{
    Q_OBJECT
public:
    explicit SqlHandler( QObject *parent = 0 );
    ~SqlHandler();

    bool insertValueIntoApu_Breakes(SqlApuDataStruct* data);
    bool insertValueIntoApu_PartsInLastMinute(SqlApuDataStruct* data);
    bool insertValueIntoPcs_GSTOP_DATA(SqlPcsDataStruct* data);
    void setApuDatabasePath( QDir path, QString databaseName );
    void setPcsDatabasePath( QDir path, QString databaseName );

private:
    QSqlDatabase*   _database;
    QSqlQuery*      _query;
    bool            _openApuDatabase();
    bool            _openPcsDatabase();
    void            _closeDatabase();
    bool            _createApuTable_Breakes(SqlApuDataStruct* data);
    bool            _createApuTable_PartsInLastMinute(SqlApuDataStruct* data);
    QDir            _apuDbPath;
    QDir            _pcsDbPath;
    QString         _apuDbName;
    QString         _pcsDbName;

signals:
    void messageText(const QString &, const QString &);
    void messageOk(const QString &, int );
    void messageKo(const QString &, int );

public slots:
};

#endif // SQLHANDLER_H
