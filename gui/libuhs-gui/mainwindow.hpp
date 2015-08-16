#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

private:
  void createActions();
  void createMenus();

  Ui::MainWindow *ui;

  QMenu *fileMenu;
  QAction *exitAct;
};

#endif // MAINWINDOW_HPP
