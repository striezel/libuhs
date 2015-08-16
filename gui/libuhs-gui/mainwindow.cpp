#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include <QtGui>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  createActions();
  createMenus();
  this->setWindowTitle("libuhs-gui");
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::createActions()
{
  openAct = new QAction(tr("&Open..."), this);
  openAct->setShortcuts(QKeySequence::Open);
  openAct->setStatusTip(tr("Open an UHS file"));
  connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

  exitAct = new QAction(tr("E&xit"), this);
  exitAct->setShortcuts(QKeySequence::Quit);
  exitAct->setStatusTip(tr("Exit the application"));
  connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    /*
    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
    */
}

void MainWindow::open()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty())
       loadFile(fileName);
    else
    {
      QMessageBox::information(this, "libuhs-gui", tr("No file selected!"));
    } //else
}

void MainWindow::loadFile(const QString &fileName)
{
    #warning TODO!
    QMessageBox::information(this, "libuhs-gui",
        tr("The feature to load a file is not implemented yet!"));
    statusBar()->showMessage(tr("Not implemented yet"), 2000);
}
