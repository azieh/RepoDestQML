#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include "threadmanager.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    ThreadManager* tm;

private:
    Ui::MainWindow* ui;
    QSystemTrayIcon* sysTray;

    QPixmap _connectedIcon;
    QPixmap _warningIcon;

    void createThreadManager();
    void setGuiConnection();
    void setGuiSetup();
    void createTrayIcon();



private slots:
    void plainTextEdit_textChanged(const QString &stName, const QString &arg1);
    void labelStatus_Changed(const QString &stName, bool arg1);
    void lineEditOk_Changed(const QString &stName, int arg1);
    void lineEditNok_Changed(const QString &stName, int arg1);
    void lineEditTime_Changed(const QString &stName, const QString &arg1);
    void closeEvent(QCloseEvent * event);
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void closeWindow();

signals:
    void startThread();

};

#endif // MAINWINDOW_H
