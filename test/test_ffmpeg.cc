
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
}
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

int main(int argc, char *argv[]) {
  int c;

  const char *file_name = NULL;

  while ((c = getopt(argc, argv, "f:")) != -1) 
    switch (c) {
      case 'f':
        file_name = optarg;
        break;
      default:
        fprintf(stderr, "unrecognised option: %c\n", c);
        exit(1);
    }

  if (file_name == NULL) {
    fprintf(stderr, "Error: Please specify a video file\n");
    exit(1);
  }

  AVFormatContext *av_format_ctx = NULL;

  av_register_all();
  if (avformat_open_input(&av_format_ctx, file_name, NULL, NULL) != 0) {
    fprintf(stderr, "Error: Cannot open file %s\n", file_name);
    exit(1);
  }
  // Retrieve stream information
  if (avformat_find_stream_info(av_format_ctx, NULL) < 0) {
    fprintf(stderr, "Error: Cannot find stream info of video file %s\n", file_name);
    exit(1);
  }
  // av_dump_format(av_format_ctx, 0, file_name, 0);
  // set the video stream idx
  int i;
  int video_stream_idx = -1;
  for (i = 0; i < av_format_ctx->nb_streams; i ++) {
    if (av_format_ctx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
      video_stream_idx = i;
      break;
    }
  }
  if (video_stream_idx == -1) {
    fprintf(stderr, "Error: Cannot find video stream\n");
    exit(1);
  }

  // Dealing with codec
  AVCodecContext *av_codec_ctx = NULL;
  AVCodec *av_codec = NULL;
  AVFrame *av_frame = NULL;
  AVFrame *av_frame_RGB = NULL;

  av_codec_ctx = av_format_ctx->streams[video_stream_idx]->codec;
  av_codec = avcodec_find_decoder(av_codec_ctx->codec_id);
  if (av_codec == NULL) {
    fprintf(stderr, "Error: Cannot find decoder for codec\n");
    exit(1);
  }

  AVDictionary *options = NULL;
  if (avcodec_open2(av_codec_ctx, av_codec, &options) < 0) {
    fprintf(stderr, "Error: Cannot open codec\n");
    exit(1);
  }
  av_frame = av_frame_alloc();
  av_frame_RGB = av_frame_alloc();
  if (av_frame == NULL || av_frame_RGB == NULL) {
    fprintf(stderr, "Error: Cannot allocate AVFrame anymore!\n");
    exit(1);
  }

  int num_bytes = avpicture_get_size(PIX_FMT_BGR24, av_codec_ctx->width, av_codec_ctx->height);
  uint8_t *buffer = NULL;

  printf("Width:    %d\n", av_codec_ctx->width);
  printf("Height:   %d\n", av_codec_ctx->height);
  printf("NumBytes: %d\n", num_bytes);

  buffer = (uint8_t*) malloc(sizeof(uint8_t) * num_bytes);
  if (buffer == NULL) {
    fprintf(stderr, "Error: Cannot allocate more buffer\n");
    exit(1);
  }

  struct SwsContext *sws_ctx = 
    sws_getContext(
      av_codec_ctx->width,
      av_codec_ctx->height,
      av_codec_ctx->pix_fmt,
      av_codec_ctx->width,
      av_codec_ctx->height,
      PIX_FMT_BGR24,
      SWS_BILINEAR,
      NULL, NULL, NULL
    );
  if (sws_ctx == NULL) {
    fprintf(stderr, "Error: SwsContext failed\n");
    exit(1);
  }

  avpicture_fill((AVPicture *)av_frame_RGB, buffer, PIX_FMT_BGR24,
      av_codec_ctx->width, av_codec_ctx->height);

  // save frames
  AVPacket packet;
  int frame_finished;
  while (av_read_frame(av_format_ctx, &packet) >= 0) {
    if (packet.stream_index == video_stream_idx) {
      avcodec_decode_video2(av_codec_ctx, av_frame, &frame_finished, &packet);
      if (frame_finished) {
        sws_scale(
            sws_ctx,
            (uint8_t const * const *) av_frame->data,
            av_frame->linesize,
            0,
            av_codec_ctx->height,
            av_frame_RGB->data,
            av_frame_RGB->linesize);

        cv::Mat img(av_frame->height, av_frame->width,CV_8UC3, av_frame_RGB->data[0]); //dst->data[0]);
        cv::imshow("display",img);
        cvWaitKey(1);
      }
    }
  }

  return 0;
}
