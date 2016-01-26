#ifndef THREADMANAGER_H
#define THREADMANAGER_H

#include "clientwindow.h"
#include "workarea.h"
#include "sqlhandler.h"
#include "mainwindow.h"

#include <QMessageBox>
#include <QObject>
#include <QtCore>


struct settingsStruct {
    QByteArray ipAddress;
    int dbNumber;
    QByteArray stationName;
};

struct clientStruct {
    WorkArea* client;
    QThread* thread;
    ClientWindow* clientWindow;
};


class ThreadManager : public QObject
{
    Q_OBJECT
public:
    explicit ThreadManager(QObject *parent = 0);
    ~ThreadManager();

    MainWindow*             mainWindow;
    SqlHandler*             sqlH;
    QSettings*              settings;

    QList<settingsStruct>   settingsList;
    QList<clientStruct>     clientList;

    void initSqlConnection();
    void createClientDeclaration();
    void loadSettings();
    void createViewEngine();



private:

    QDir    _apuDbPath;
    QString _apuDbName;
    QDir    _pcsDbPath;
    QString _pcsDbName;


public slots:
    void start();


};

#endif // THREADMANAGER_H
