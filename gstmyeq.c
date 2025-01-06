/* GStreamer
 * Copyright (C) 2025 FIXME <fixme@example.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Suite 500,
 * Boston, MA 02110-1335, USA.
 */
/**
 * SECTION:element-gstmyeq
 *
 * The myeq element does FIXME stuff.
 *
 * <refsect2>
 * <title>Example launch line</title>
 * |[
 * gst-launch-1.0 -v fakesrc ! myeq ! FIXME ! fakesink
 * ]|
 * FIXME Describe what the pipeline does.
 * </refsect2>
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <math.h>

#include <glib-object.h>
#include <gst/gst.h>
#include <gst/audio/gstaudiofilter.h>
#include "gstmyeq.h"

GST_DEBUG_CATEGORY_STATIC (gst_myeq_debug_category);
#define GST_CAT_DEFAULT gst_myeq_debug_category

/* prototypes */


static void gst_myeq_set_property (GObject * object,
    guint property_id, const GValue * value, GParamSpec * pspec);
static void gst_myeq_get_property (GObject * object,
    guint property_id, GValue * value, GParamSpec * pspec);
static void gst_myeq_dispose (GObject * object);
static void gst_myeq_finalize (GObject * object);

static gboolean gst_myeq_setup (GstAudioFilter * filter,
    const GstAudioInfo * info);
static GstFlowReturn gst_myeq_transform (GstBaseTransform * trans,
    GstBuffer * inbuf, GstBuffer * outbuf);
static GstFlowReturn gst_myeq_transform_ip (GstBaseTransform * trans,
    GstBuffer * buf);

enum
{
  PROP_0
};

/* pad templates */

/* FIXME add/remove the formats that you want to support */
static GstStaticPadTemplate gst_myeq_src_template =
GST_STATIC_PAD_TEMPLATE("src",
    GST_PAD_SRC,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS("audio/x-raw,format=(string){S16LE, S32LE, F32LE, F64LE},rate=[1,max],"
        "channels=[1,max],layout=interleaved")
);

/* FIXME add/remove the formats that you want to support */
static GstStaticPadTemplate gst_myeq_sink_template =
GST_STATIC_PAD_TEMPLATE("sink",
    GST_PAD_SINK,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS("audio/x-raw,format=(string){S16LE, S32LE, F32LE, F64LE},rate=[1,max],"
        "channels=[1,max],layout=interleaved")
);


/* class initialization */

G_DEFINE_TYPE_WITH_CODE (GstMyeq, gst_myeq, GST_TYPE_AUDIO_FILTER,
  GST_DEBUG_CATEGORY_INIT (gst_myeq_debug_category, "myeq", 0,
  "debug category for myeq element"));

static void
gst_myeq_class_init (GstMyeqClass * klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
  GstBaseTransformClass *base_transform_class = GST_BASE_TRANSFORM_CLASS (klass);
  GstAudioFilterClass *audio_filter_class = GST_AUDIO_FILTER_CLASS (klass);

  /* Setting up pads and setting metadata should be moved to
     base_class_init if you intend to subclass this class. */
  gst_element_class_add_static_pad_template (GST_ELEMENT_CLASS(klass),
      &gst_myeq_src_template);
  gst_element_class_add_static_pad_template (GST_ELEMENT_CLASS(klass),
      &gst_myeq_sink_template);

  gst_element_class_set_static_metadata (GST_ELEMENT_CLASS(klass),
      "FIXME Long name", "Generic", "FIXME Description",
      "FIXME <fixme@example.com>");

  gobject_class->set_property = gst_myeq_set_property;
  gobject_class->get_property = gst_myeq_get_property;
  gobject_class->dispose = gst_myeq_dispose;
  gobject_class->finalize = gst_myeq_finalize;
  audio_filter_class->setup = GST_DEBUG_FUNCPTR (gst_myeq_setup);
  base_transform_class->transform = GST_DEBUG_FUNCPTR (gst_myeq_transform);
  base_transform_class->transform_ip = GST_DEBUG_FUNCPTR (gst_myeq_transform_ip);

}

static void
gst_myeq_init (GstMyeq *myeq)
{
    myeq->gain = 0.3;
    myeq->frequency = 1000.0;
}

void gst_myeq_set_property (GObject * object, guint property_id, const GValue * value, GParamSpec * pspec)
{
  GstMyeq *myeq = GST_MYEQ (object);

  GST_DEBUG_OBJECT (myeq, "set_property");

  switch (property_id) {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}

void
gst_myeq_get_property (GObject * object, guint property_id,
    GValue * value, GParamSpec * pspec)
{
  GstMyeq *myeq = GST_MYEQ (object);

  GST_DEBUG_OBJECT (myeq, "get_property");

  switch (property_id) {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}

void
gst_myeq_dispose (GObject * object)
{
  GstMyeq *myeq = GST_MYEQ (object);

  GST_DEBUG_OBJECT (myeq, "dispose");

  /* clean up as possible.  may be called multiple times */

  G_OBJECT_CLASS (gst_myeq_parent_class)->dispose (object);
}

void
gst_myeq_finalize (GObject * object)
{
  GstMyeq *myeq = GST_MYEQ (object);

  GST_DEBUG_OBJECT (myeq, "finalize");

  /* clean up object here */

  G_OBJECT_CLASS (gst_myeq_parent_class)->finalize (object);
}

static gboolean
gst_myeq_setup (GstAudioFilter * filter, const GstAudioInfo * info)
{
  GstMyeq *myeq = GST_MYEQ (filter);

  GST_DEBUG_OBJECT (myeq, "setup");

  return TRUE;
}

/* transform */
static GstFlowReturn
gst_myeq_transform(GstBaseTransform* trans, GstBuffer* inbuf,
    GstBuffer* outbuf)
{
    GstMyeq* myeq = GST_MYEQ(trans);

    GST_DEBUG_OBJECT(myeq, "transform");

    GstMapInfo in_map, out_map;
    if (!gst_buffer_map(inbuf, &in_map, GST_MAP_READ)) {
        GST_ERROR_OBJECT(myeq, "Failed to map input buffer");
        return GST_FLOW_ERROR;
    }
    if (!gst_buffer_map(outbuf, &out_map, GST_MAP_WRITE)) {
        gst_buffer_unmap(inbuf, &in_map);
        GST_ERROR_OBJECT(myeq, "Failed to map output buffer");
        return GST_FLOW_ERROR;
    }
	GST_AUDIO_FORMAT_F32LE;

    const GstAudioInfo* info = GST_AUDIO_FILTER_INFO(myeq);
    gint sample_rate = GST_AUDIO_INFO_RATE(info);
    gint channels = GST_AUDIO_INFO_CHANNELS(info);
    gint bytes_per_frame = GST_AUDIO_INFO_BPF(info);
    gssize num_frames = in_map.size / bytes_per_frame; // 型を gssize に変更
    g_print("GST_AUDIO_FORMAT_F32LE Frequency: %f, Sample Rate: %d, Channels: %d, Num Frames: %zd\n",
        myeq->frequency, sample_rate, channels, num_frames);

    gfloat* in_data = (gfloat*)in_map.data;
    gfloat* out_data = (gfloat*)out_map.data;

    for (gint i = 0; i < num_frames; i++) {
        for (gint ch = 0; ch < channels; ch++) {
            gint sample_index = i * channels + ch;

            gdouble t = (gdouble)i / sample_rate;

            gdouble modulation = 1.0 + myeq->gain * sin(2.0 * G_PI * myeq->frequency * t);

            gfloat sample = in_data[sample_index];
            sample *= (gfloat)CLAMP(modulation, 0.0, 2.0);

            out_data[sample_index] = sample;
        }
    }

    gst_buffer_unmap(inbuf, &in_map);
    gst_buffer_unmap(outbuf, &out_map);

    return GST_FLOW_OK;
}

static GstFlowReturn
gst_myeq_transform_ip (GstBaseTransform * trans, GstBuffer * buf)
{
  GstMyeq *myeq = GST_MYEQ (trans);

  GST_DEBUG_OBJECT (myeq, "transform_ip");

  return GST_FLOW_OK;
}

static gboolean
plugin_init (GstPlugin * plugin)
{

  /* FIXME Remember to set the rank if it's an element that is meant
     to be autoplugged by decodebin. */
  return gst_element_register (plugin, "myeq", GST_RANK_NONE,
      GST_TYPE_MYEQ);
}

/* FIXME: these are normally defined by the GStreamer build system.
   If you are creating an element to be included in gst-plugins-*,
   remove these, as they're always defined.  Otherwise, edit as
   appropriate for your external plugin package. */
#ifndef VERSION
#define VERSION "0.0.FIXME"
#endif
#ifndef PACKAGE
#define PACKAGE "FIXME_package"
#endif
#ifndef PACKAGE_NAME
#define PACKAGE_NAME "FIXME_package_name"
#endif
#ifndef GST_PACKAGE_ORIGIN
#define GST_PACKAGE_ORIGIN "http://FIXME.org/"
#endif

GST_PLUGIN_DEFINE (GST_VERSION_MAJOR,
    GST_VERSION_MINOR,
    myeq,
    "FIXME plugin description",
    plugin_init, VERSION, "LGPL", PACKAGE_NAME, GST_PACKAGE_ORIGIN)

