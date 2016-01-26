#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QSystemTrayIcon>
#include <QQmlApplicationEngine>
#include <QtQuick/QQuickView>
#include <QtQuick/QtQuick>
#include <QMessageBox>
#include <QMenu>

class MainWindow : public QObject
{
        Q_OBJECT
public:
    explicit MainWindow();
    ~MainWindow();

    QQuickWindow*           window;
    QSystemTrayIcon*        sysTray;

    QPixmap                 _connectedIcon;
    QPixmap                 _warningIcon;
    QQmlApplicationEngine   engine;

private:
    QMenu*      menu;
    QAction*    viewWindow;
    QAction*    quitAction;

    void        createViewEngine();
    void        createSysTray();


signals:
    void textUpdate(QString stName, QString text);
    void apuUpdate(QString text);
    void pcsUpdate(QString text);

public slots:
    void closeEvent(QQuickCloseEvent* event);
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void closeWindow();
    void onTextUpdate(QString stName, QString text);
};

#endif // MAINWINDOW_H
