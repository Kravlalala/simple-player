#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileSystemModel>
#include <QTreeView>
#include <QMessageBox>

#include <gst/gst.h>
#include <glib.h>

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);

  void add_message_handler (GMainLoop *, GstElement *, guint *);
  static gboolean bus_call (GstBus *, GstMessage *, gpointer);
  void set_up_elements (GstElement *, char *);
  void start (GMainLoop *, GstElement *);

  ~MainWindow();

private slots:
  void on_treeView_clicked(const QModelIndex &index);

  void on_listView_doubleClicked(const QModelIndex &index);

private:
  Ui::MainWindow *ui;
  QFileSystemModel *dir_model;
  QFileSystemModel *file_model;
  QString root_path;
};

#endif // MAINWINDOW_H
