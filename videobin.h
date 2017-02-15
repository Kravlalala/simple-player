#ifndef VIDEOBIN_H
#define VIDEOBIN_H
#include <QMessageBox>

#include <gst/gst.h>

class VideoBin
{
private:
  GstElement *pipeline;
  GstElement *file_source;
  GstElement *demuxer;
  GstElement *conv_to_vout;
  GstElement *conv_to_aout;
  GstElement *video_out;
  GstElement *audio_out;
public:
  VideoBin();
  GstElement *get_file_source();
  GstElement *get_pipeline();
  void video_pipeline_add_elements ();
  gboolean video_pipeline_link_elements ();
  static void link_demux (GstElement *, GstPad *, gpointer);
};

#endif // VIDEOBIN_H
