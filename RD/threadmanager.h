#ifndef THREADMANAGER_H
#define THREADMANAGER_H

#include <QObject>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QQmlContext>
#include <QQuickView>
#include <QQmlEngine>

#include "workarea.h"
#include "sqlhandler.h"

struct settingsStruct {
    QByteArray ipAddress;
    int dbNumber;
    QByteArray stationName;
};


class ThreadManager : public QObject
{
    Q_OBJECT
public:
    explicit ThreadManager(QObject *parent = 0);
    ~ThreadManager();
    SqlHandler* sqlH;
    QThread* thread1;
    WorkArea* st10;
    WorkArea* st20;
    QSettings* settings;

    QList<settingsStruct> settingsList;

    void createThreads();
    void createClientDeclaration();
    void loadSettings();
 QQuickItem *object1;


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
