#include "mainwindow.h"

MainWindow::MainWindow() :
    window      (nullptr),
    sysTray     (new QSystemTrayIcon(this)),
    menu        (nullptr),
    viewWindow  (nullptr),
    quitAction  (nullptr)
{
    createViewEngine();
    createSysTray();

    connect(window, SIGNAL(closing(QQuickCloseEvent*)), this, SLOT(closeEvent(QQuickCloseEvent*))); //connection close window event with messages on system try
}
MainWindow::~MainWindow()
{
    delete window;
    window = nullptr;

    delete sysTray;
    sysTray = nullptr;

    delete viewWindow;
    viewWindow = nullptr;

    delete quitAction;
    quitAction = nullptr;
}
//------------------------------------------------------------------------------
// Create view engine
//------------------------------------------------------------------------------
void MainWindow::createViewEngine()
{
    engine.rootContext()->setContextProperty("mainWindow", this);//create signals/slots connection between C++ and QML
    engine.load(QUrl::fromLocalFile(QStringLiteral("qml/main.qml")));

    if (window != nullptr){
        delete window;
        window = nullptr;
    }
    window = qobject_cast<QQuickWindow*>(engine.rootObjects().at(0));
}
//------------------------------------------------------------------------------
// Ignore close event
//------------------------------------------------------------------------------
void MainWindow::closeEvent(QQuickCloseEvent* event)
{
    window->hide();
    QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon();
    sysTray->showMessage(tr("Raportowanie przestojów"),
                         "Program działa cały czas w tle.",
                         icon);

}
//------------------------------------------------------------------------------
// Icon activated slot
//------------------------------------------------------------------------------
void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::DoubleClick:
        window->show();
        break;
    default:
        break;
    }
}
//------------------------------------------------------------------------------
// Close window
//------------------------------------------------------------------------------
void MainWindow::closeWindow()
{
    QMessageBox closeMessageBox;
    closeMessageBox.setText("Raportowanie przestojów - zamknięcie programu");
    closeMessageBox.setInformativeText("Czy na pewno chcesz zamknąć program?");
    closeMessageBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    closeMessageBox.setDefaultButton(QMessageBox::Ok);
    int ret = closeMessageBox.exec();

    switch (ret) {
      case QMessageBox::Ok:
          sysTray->setVisible(false);
          exit(0);
          break;
      case QMessageBox::Cancel:
          window->hide();
          break;
      default:
          // should never be reached
          break;
    }
}
//------------------------------------------------------------------------------
// Create system tray with menu
//------------------------------------------------------------------------------
void MainWindow::createSysTray()
{
    sysTray->setToolTip("Raportowanie przestojów");
    sysTray->setIcon(QIcon(":/ico/logo.png"));

    if (menu != nullptr){
        delete menu;
        menu = nullptr;
    }
    menu = new QMenu("Menu");

    if (viewWindow != nullptr){
        delete viewWindow;
        viewWindow = nullptr;
    }
    viewWindow = new QAction("Open", this);
    viewWindow->setIcon(QIcon(":/ico/logo.png"));

    if (quitAction != nullptr){
        delete quitAction;
        quitAction = nullptr;
    }
    quitAction = new QAction("Close", this);
    quitAction->setIcon(QIcon(":/ico/close.png"));

    connect(viewWindow, SIGNAL(triggered()), window, SLOT(show()));
    connect(quitAction, SIGNAL(triggered()), this, SLOT(closeWindow()));

    menu->addAction(viewWindow);
    menu->addSeparator();
    menu->addAction(quitAction);

    sysTray->setContextMenu(menu);
    sysTray->show();

    QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon();
    sysTray->showMessage(tr("Raportowanie przestojów"),
                         "Program uruchomiony pomyślnie. Jego działanie prowadzone jest cały czas w tle.",
                         icon);
}
//------------------------------------------------------------------------------
// Main log window text update
//------------------------------------------------------------------------------
void MainWindow::onTextUpdate(QString stName, QString text)
{
    textUpdate(stName, text);
}
