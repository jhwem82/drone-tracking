
#include <iostream>

#include "BebopUAVControl.h"

cv::Mat *getCVMatFromAVVideoFrame(AVFrame *raw_av_frame, int width, int height) {
  // allocated new AVFrame
  int buf_size = avpicture_get_size(AV_PIX_FMT_BGR24, width, height);
  uint8_t *buf = (uint8_t*) malloc(buf_size);
  struct SwsContext *sws_ctx;

  AVFrame *av_frame = av_frame_alloc();
  if (av_frame != NULL) {
    // setup frame
    av_frame->width = width;
    av_frame->height = height;
    av_frame->format = AV_PIX_FMT_BGR24;
    // fill frame
    avpicture_fill((AVPicture*)av_frame, buf, AV_PIX_FMT_BGR24, width, height);
    // from raw_av_frame to av_frame
    // setup context
    sws_ctx = 
      sws_getContext(
          raw_av_frame->width,
          raw_av_frame->height,
          (enum AVPixelFormat) raw_av_frame->format,
          av_frame->width,
          av_frame->height,
          (enum AVPixelFormat) av_frame->format,
          SWS_BILINEAR, NULL, NULL, NULL);
    // check NULL
    if (sws_ctx == NULL) {
      std::cerr << "sws_ctx should not be NULL" << std::endl;
      exit(1);
    }
    // scale 
    sws_scale(sws_ctx, (uint8_t const * const *)raw_av_frame->data, 
        raw_av_frame->linesize, 0, 
        av_frame->height, av_frame->data, av_frame->linesize);
    sws_freeContext(sws_ctx);
    // create cv::Mat
    cv::Mat *img = new cv::Mat(av_frame->height, av_frame->width, CV_8UC3, av_frame->data[0]);
    return img;
  } else {
    // cannot allocated new frame
    std::cerr << "Cannot allocated new frame" << std::endl;
    exit(1);
  }
}

void refreshDeviceManager(BD_MANAGER_t *manager, cv::Mat *img) {
  int op;
  if (manager == NULL || img == NULL) {
    // just return
    return ;
  }
  cv::Size img_size = img->size();
  cv::imwrite("frame.png", *img); 
  op = fromFaceFrameToOp(*img, img_size.width, img_size.height, 20);
  switch (op) {
    case OP_STAY:
      IHM_PrintInfo(manager->ihm, (char *) "Next command : STAY");
      manager->dataPCMD.flag = 0;
      manager->dataPCMD.roll = 0;
      manager->dataPCMD.pitch = 0;
      manager->dataPCMD.yaw = 0;
      manager->dataPCMD.gaz = 0;
      break;
    case OP_FORWARD:
      IHM_PrintInfo(manager->ihm, (char *) "Next command : FORWARD");
      manager->dataPCMD.flag = 1;
      manager->dataPCMD.pitch = 10;
      break;
    case OP_BACKWARD:
      IHM_PrintInfo(manager->ihm, (char *) "Next command : BACKWARD");
      manager->dataPCMD.flag = 1;
      manager->dataPCMD.pitch = -10;
      break;
    case OP_TURNLEFT:
      IHM_PrintInfo(manager->ihm, (char *) "Next command : TURNLEFT");
      manager->dataPCMD.flag = 1;
      manager->dataPCMD.roll = -5;
      break;
    case OP_TURNRIGHT:
      IHM_PrintInfo(manager->ihm, (char *) "Next command : TURNRIGHT");
      manager->dataPCMD.flag = 1;
      manager->dataPCMD.roll = 5;
      break;
    case OP_UPWARD:
      IHM_PrintInfo(manager->ihm, (char *) "Next command : UPWARD");
      manager->dataPCMD.flag = 1;
      manager->dataPCMD.gaz = 10;
      break;
    case OP_DOWNWARD:
      IHM_PrintInfo(manager->ihm, (char *) "Next command : DOWNWARD");
      manager->dataPCMD.flag = 1;
      manager->dataPCMD.gaz = -10;
      break;
    default:
      fprintf(stderr, "Cannot recognise op code %d\n", op);
      exit(1);
  }
}
