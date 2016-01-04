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

     void createWindows(QQmlApplicationEngine& engine);
     QQuickItem*    clientObject;

private:
     QQuickWindow*  window;
     QQuickItem*    root;
     QQmlContext*   context;
     QQmlComponent* component;
signals:
     void nokUpdate(QString text);
     void okUpdate(QString text);
     void loopTimeUpdate(QString text);
     void textUpdate(QString text);
     void stationNameUpdate(QString text);

public slots:

     void onNokUpdate(int number);
     void onOkUpdate(int number);
     void onLoopTimeUpdate(const QString &text);
     void onTextUpdate(const QString &text);
     void onStationNameUpdate(QString text);
};

#endif // CLIENTWINDOW_H
