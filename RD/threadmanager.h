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
    SqlHandler*             sqlH;
    QSettings*              settings;
    QQuickWindow*           window;
    QQmlApplicationEngine   engine;


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



signals:
    void textUpdate(QString stName, QString text);
    void apuUpdate(QString text);
    void pcsUpdate(QString text);

public slots:
    void start();
    void onTextUpdate(QString stName, QString text);

};

#endif // THREADMANAGER_H
