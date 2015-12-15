#include <QMessageBox>

#include "mainwindow.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    tm          ( nullptr ),
    ui          ( new Ui::MainWindow ),
    sysTray     ( new QSystemTrayIcon(this) )

{
    ui->setupUi(this);
    createThreadManager();
    setGuiSetup(); //initial settings for GUI
    setGuiConnection(); //generete and manage connection between GUI and WorkArea stations


    QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon();
    sysTray->showMessage(tr("Raportowanie przestojów"),
                         "Program uruchomiony pomyślnie. Jego działanie prowadzone jest cały czas w tle.",
                         icon);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete sysTray;
    delete tm;
}
//------------------------------------------------------------------------------
// Create thread manager
//------------------------------------------------------------------------------
void MainWindow::createThreadManager()
{
    if ( tm != nullptr ){
        delete tm;
        tm = nullptr;
    }
    tm = new ThreadManager;
    tm->createThreads();
    tm->createClientDeclaration();
}
//------------------------------------------------------------------------------
// Create GUI with station object conenction
//------------------------------------------------------------------------------
void MainWindow::setGuiConnection()
{
    connect(tm->sqlH, SIGNAL( messageText( const QString &, const QString & )), this, SLOT( plainTextEdit_textChanged( const QString &, const QString & )));
    connect(tm->sqlH, SIGNAL( messageOk( const QString &, int )),               this, SLOT( lineEditOk_Changed( const QString &, int )));
    connect(tm->sqlH, SIGNAL( messageKo( const QString &, int )),               this, SLOT( lineEditNok_Changed( const QString &, int )));

    connect(tm->st10, SIGNAL( messageText( const QString &, const QString & )), this, SLOT( plainTextEdit_textChanged( const QString &, const QString & )));
    connect(tm->st10, SIGNAL( messageOk( const QString &, int )),               this, SLOT( lineEditOk_Changed( const QString &, int )));
    connect(tm->st10, SIGNAL( messageKo( const QString &, int )),               this, SLOT( lineEditNok_Changed( const QString &, int )));
    connect(tm->st10, SIGNAL( connectionStatus( const QString &, bool )),       this, SLOT( labelStatus_Changed( const QString &, bool )));
    connect(tm->st10, SIGNAL( loopTime( const QString &, const QString &)),     this, SLOT( lineEditTime_Changed( const QString &, const QString &)));

    connect(sysTray, SIGNAL( activated( QSystemTrayIcon::ActivationReason )),   this, SLOT( iconActivated( QSystemTrayIcon::ActivationReason )));

}
//------------------------------------------------------------------------------
// GUI setup
//------------------------------------------------------------------------------
void MainWindow::setGuiSetup()
{
    setWindowFlags( Qt::WindowCloseButtonHint );
    sysTray->setToolTip("Raportowanie przestojów");
    sysTray->setIcon(QIcon(":/ico/logo.png"));
    QMenu* menu = new QMenu(this);
    QAction* viewWindow = new QAction("Open", this);
    viewWindow->setIcon(QIcon(":/ico/logo.png"));
    QAction* quitAction = new QAction("Close", this);
    quitAction->setIcon(QIcon(":/ico/close.png"));

    connect(viewWindow, SIGNAL(triggered()), this, SLOT(show()));
    connect(quitAction, SIGNAL(triggered()), this, SLOT(closeWindow()));

    menu->addAction(viewWindow);
    menu->addSeparator();
    menu->addAction(quitAction);

    sysTray->setContextMenu(menu);
    sysTray->show();

    int blockCount = 50; // number of lines in plainText window
    // implement here every settings for GUI

    ui->plainTextSystemLog->setMaximumBlockCount(blockCount);
    ui->plainTextEditSt10->setMaximumBlockCount(blockCount);
    ui->lineEditSt10Ok->setText(QString::number(0));
    ui->lineEditSt10Nok->setText(QString::number(0));

    _connectedIcon = QPixmap(":/ico/connected.png");
    _connectedIcon = _connectedIcon.scaled(QSize(32, 32), Qt::KeepAspectRatio);
    _warningIcon = QPixmap(":/ico/disconnected.png");
    _warningIcon = _warningIcon.scaled(QSize(32, 32), Qt::KeepAspectRatio);
}
//------------------------------------------------------------------------------
// Text changed slot
//------------------------------------------------------------------------------
void MainWindow::plainTextEdit_textChanged(const QString &stName, const QString &arg1)
{
    if ( stName == "RB1" ){
        ui->plainTextEditSt10->appendPlainText(arg1);

    } else if ( stName == "LogMsg" ){
        ui->plainTextSystemLog->appendPlainText(arg1);
    }
}
//------------------------------------------------------------------------------
// Connection status slot
//------------------------------------------------------------------------------
void MainWindow::labelStatus_Changed(const QString &stName, bool arg1)
{
    if ( stName == "RB1" ){
        if ( arg1 == true ){
            ui->labelSt10Status->setText("Connected");
            ui->labelSt10Ico->setPixmap(_connectedIcon);
        } else {
            ui->labelSt10Status->setText("Dissconected");
            ui->labelSt10Ico->setPixmap(_warningIcon);
        }

    }
}
//------------------------------------------------------------------------------
// Operations OK slot
//------------------------------------------------------------------------------
void MainWindow::lineEditOk_Changed(const QString &stName, int arg1)
{
    if ( stName == "RB1" ){
        ui->lineEditSt10Ok->setText(QString::number(arg1));
    }
}
//------------------------------------------------------------------------------
// Operations NOK slot
//------------------------------------------------------------------------------
void MainWindow::lineEditNok_Changed(const QString &stName, int arg1)
{
    if ( stName == "RB1" ){
        ui->lineEditSt10Nok->setText(QString::number(arg1));
    }
}
//------------------------------------------------------------------------------
// Loop time slot
//------------------------------------------------------------------------------
void MainWindow::lineEditTime_Changed(const QString &stName, const QString &arg1)
{
    if ( stName == "RB1" ){
        ui->lineEditSt10Time->setText(arg1);
    }

}
//------------------------------------------------------------------------------
// Ignore close event
//------------------------------------------------------------------------------
void MainWindow::closeEvent(QCloseEvent * event)
{ 
    hide();
    event->ignore();
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
        this->show();
        break;
    default:
        break;
    }
}
//------------------------------------------------------------------------------
// Close window with password
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
          exit(0);
          break;
      case QMessageBox::Cancel:
          hide();
          break;
      default:
          // should never be reached
          break;
    }
}
