#ifndef BEBOP_UAV_CONTROL_H__
#define BEBOP_UAV_CONTROL_H__

#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>

// FFmpeg and Bebop related headers
extern "C" {
  #include <libavformat/avformat.h>
  #include <libavcodec/avcodec.h>
  #include <libavutil/imgutils.h>
  #include <libswscale/swscale.h>  
  
  #include <libARSAL/ARSAL.h>
  #include <libARSAL/ARSAL_Print.h>
  #include <libARNetwork/ARNetwork.h>
  #include <libARNetworkAL/ARNetworkAL.h>
  #include <libARDiscovery/ARDiscovery.h>
  #include <libARStream/ARStream.h>
}
#include "BebopDroneDecodeStream.h"
#include "UAVControl.h"

cv::Mat *getCVMatFromAVVideoFrame(AVFrame *raw_av_frame, int width, int height);
void refreshDeviceManager(BD_MANAGER_t *manager, cv::Mat *img);

#endif
