#ifndef CLIENTWINDOW_H
#define CLIENTWINDOW_H

#include <QObject>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QQmlContext>
#include <QQuickItem>
#include <QQmlEngine>
#include <QQuickWindow>

class ClientWindow : public QObject
{
    Q_OBJECT
public:
    explicit ClientWindow(QObject *parent = 0);
    ~ClientWindow();

    void createWindows(QQmlApplicationEngine& engine, QQuickWindow* window);
    QQuickItem*    clientObject;

private:
    QQuickItem*    root;
    QQmlContext*   context;
    QQmlComponent* component;
    QQuickWindow*  window_memory;

signals:
    void hideClientWindow(bool b);
    void stationNameUpdate(QString text);
    void connectionStatusUpdate(bool b);
    void loopTimeUpdate(QString text);
    void okUpdate(QString text);
    void nokUpdate(QString text);
    void textUpdate(QString text);
    void ipUpdate(QString text);
    void dbUpdate(QString text);

public slots:
    void onHideClientWindow(bool b);
    void onShowClientWindow(bool b);
    void onStationNameUpdate(QString text);
    void onConnectionStatusUpdate(bool b);
    void onLoopTimeUpdate(const QString &text);
    void onOkUpdate(int number);
    void onNokUpdate(int number);
    void onTextUpdate(const QString &text);
    void onIpUpdate(QString text);
    void onDbUpdate(int number);

};

#endif // CLIENTWINDOW_H
