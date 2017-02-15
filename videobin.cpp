#include "videobin.h"

VideoBin::VideoBin()
{
  gboolean ret;
  QMessageBox msgBox;

  /* Create elements */
  pipeline = gst_pipeline_new ("video_pipeline");
  file_source = gst_element_factory_make ("filesrc", "source_file");
  demuxer = gst_element_factory_make("avidemux","demuxer" );
  conv_to_vout = gst_element_factory_make ("videoconvert", "to_vout_conv");
  video_out = gst_element_factory_make ("waylandsink", "video_out");

  if (!pipeline) {
      msgBox.setText("Pipeline couldn't be created\n");
      msgBox.exec();
    }
  if (!file_source) {
      msgBox.setText("File source couldn't be created\n");
      msgBox.exec();
    }

  if (!demuxer) {
      msgBox.setText("Demuxer couldn't be created\n");
      msgBox.exec();
    }
  if (!conv_to_vout) {
      msgBox.setText("Video converter couldn't be created\n");
      msgBox.exec();
    }
  if (!video_out) {
      msgBox.setText("Video out couldn't be created\n");
      msgBox.exec();
    }

  /* Add elements into the pipeline */
  video_pipeline_add_elements();
  ret = video_pipeline_link_elements();
  if (ret == FALSE){
      msgBox.setText("Video Bin init error");
      msgBox.exec();
    }
}

/* Return pointer on file_src element  */
GstElement *VideoBin:: get_file_source(){
  return file_source;
}

/* Return pointer on pipeline  */
GstElement *VideoBin:: get_pipeline(){
  return pipeline;
}

/* Add elements into the video pipeline  */
void VideoBin::video_pipeline_add_elements () {

  /* Edit elements into the pipeline */
  gst_bin_add_many (GST_BIN(pipeline), file_source, demuxer, conv_to_vout,
                    video_out, NULL);
}

/* Link elements in the pipeline  */
gboolean VideoBin :: video_pipeline_link_elements () {

  QMessageBox msgBox;
  gboolean ret;

  /* Link file source with demuxer */
  ret = gst_element_link (file_source, demuxer);
  if (ret == FALSE) {
      msgBox.setText("File source and demuxer couldn't be linked");
      msgBox.exec();
      return FALSE;
    }

  /* Link video converter and video out */
  ret = gst_element_link (conv_to_vout, video_out);
  if (ret == FALSE) {
      msgBox.setText("Video converter and video out couldn't be linked\n");
      msgBox.exec();
      return FALSE;
    }

  g_signal_connect (demuxer, "pad-added", G_CALLBACK ( link_demux), conv_to_vout);

  return TRUE;
}

/* Link autmatically created pad to any element  */
void VideoBin::link_demux(GstElement *element, GstPad *new_pad, gpointer data){

  QMessageBox msgBox;
  QString name(gst_pad_get_name (new_pad));
  GstPad *sink_pad;
  GstElement *connected_element = (GstElement *) data;
  gboolean ret;

  /* Get pad compatible with created pad */
  sink_pad = gst_element_get_compatible_pad (connected_element, new_pad, NULL);
  //sink_pad = gst_element_get_static_pad (connected_element, "sink");
  if (sink_pad == NULL) {
      msgBox.setText("there is no compatible pads");
      msgBox.exec();
    }

  ret = gst_pad_link (new_pad,sink_pad);
  if (ret != GST_PAD_LINK_OK) {
      msgBox.setText("Linking pad error");
      msgBox.exec();
    }
}
