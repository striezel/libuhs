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

private slots:
  void open();

private:
  void createActions();
  void createMenus();

  void loadFile(const QString &fileName);

  Ui::MainWindow *ui;

  QMenu *fileMenu;
  QAction *openAct;
  QAction *exitAct;
};

#endif // MAINWINDOW_HPP
