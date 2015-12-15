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
     QQuickItem *object1;

signals:

public slots:
};

#endif // CLIENTWINDOW_H
