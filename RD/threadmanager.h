#ifndef THREADMANAGER_H
#define THREADMANAGER_H

#include <QObject>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QQmlContext>
#include <QQuickView>
#include <QQmlEngine>
#include "clientwindow.h"
#include "workarea.h"
#include "sqlhandler.h"

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
    SqlHandler* sqlH;
    QSettings* settings;
    QQmlApplicationEngine engine;
    QList<settingsStruct> settingsList;
    QList<clientStruct> clientList;


    void createThreads();
    void createClientDeclaration();
    void loadSettings();
    void createViewEngine();



private:

    QDir    _apuDbPath;
    QString _apuDbName;
    QDir    _pcsDbPath;
    QString _pcsDbName;



signals:

public slots:
    void start();
};

#endif // THREADMANAGER_H
