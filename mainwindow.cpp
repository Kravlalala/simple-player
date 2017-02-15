#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "videobin.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  /* Create models for directories and for files  */
  dir_model = new QFileSystemModel(this);
  file_model = new QFileSystemModel(this);

  /* Set root paths  */
  root_path = "/home/root/video";
  dir_model->setRootPath(root_path);
  file_model->setRootPath(root_path);

  /* Set filters  */
  dir_model->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
  file_model->setFilter(QDir::NoDotAndDotDot | QDir::Files);

  /* Attach the model to the view  */
  ui->treeView->setModel(dir_model);
  ui->treeView->setRootIndex(dir_model->index(root_path));
  ui->listView->setModel(file_model);

}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::on_treeView_clicked(const QModelIndex &index)
{
  QString chosen_path = dir_model->fileInfo(index).absoluteFilePath();
  ui->listView->setRootIndex(file_model->setRootPath(chosen_path));
}

void MainWindow::on_listView_doubleClicked(const QModelIndex &index)
{
  QMessageBox msgBox;
  guint *bus_watch_id;
  GMainLoop *loop;

  gst_init (NULL, NULL);
  VideoBin video_bin;
  GstElement *video_pipeline = video_bin.get_pipeline() ;

  QString file_path = file_model->fileInfo(index).absoluteFilePath();
  QByteArray byte_path = file_path.toLatin1();
  char *result_path = byte_path.data();

  set_up_elements(video_bin.get_file_source(),result_path);
  loop = g_main_loop_new (NULL, FALSE);
  bus_watch_id = (guint*) g_malloc (sizeof(guint*));

  add_message_handler (loop, video_pipeline, bus_watch_id);
  start (loop, video_pipeline);

  gst_element_set_state (video_pipeline, GST_STATE_NULL);
  gst_object_unref (GST_OBJECT(video_pipeline));
  g_free (bus_watch_id);
  g_main_loop_unref (loop);

}

/* Add a message handler to pipeline bus in main loop function */
void MainWindow::add_message_handler (GMainLoop *loop, GstElement *pipeline, guint *bus_watch_id) {
  GstBus *bus;
  bus = gst_pipeline_get_bus (GST_PIPELINE(pipeline));
  *bus_watch_id = gst_bus_add_watch (bus, bus_call, loop);
  gst_object_unref (bus);
}

/*
 * Callback function for bus message processing.
 * returns TRUE, because regular handling needed.
 */
gboolean MainWindow::bus_call (GstBus *bus, GstMessage *msg, gpointer data) {
  GMainLoop *loop = (GMainLoop *) data;

  switch (GST_MESSAGE_TYPE(msg)) {
    case GST_MESSAGE_EOS: {
        g_print ("End of stream\n");
        g_main_loop_quit (loop);
        break;
      }
    case GST_MESSAGE_ERROR: {
        gchar *debug;
        GError *error;
        gst_message_parse_error (msg, &error, &debug);
        g_print ("%s\n", debug);
        g_free (debug);
        g_printerr ("Error: %s\n", error->message);
        g_error_free (error);
        g_main_loop_quit (loop);
        break;
      }
    default:
      break;
    }

  return TRUE;
}


/* Setting up properties for the video source  */
void MainWindow::set_up_elements (GstElement *file_src, char *path) {

  /* Set properties for v4l2src */
  g_object_set (G_OBJECT(file_src), "location", path, NULL);

}

/* Starting loopback and set media pipeline to the PLAYING state */
void MainWindow::start (GMainLoop *loop, GstElement *pipeline) {

  gst_element_set_state (pipeline, GST_STATE_PLAYING);
  g_main_loop_run (loop);
}
