#include "log.h"

extern logger log;

int fftrans(const char *filename){
    AVFormatContext *fmtCtx = NULL;
    AVStream *vstream = NULL;
    int ret = 0;

    log.infoLog("fftrans start");

    ret = avformat_open_input(&fmtCtx,filename,NULL,NULL);

    if(ret < 0){
        log.errorLog(&log,"avformat open input error");
        return -1;
    }

    ret = avformat_find_stream_info(fmtCtx,NULL);

    if(ret < 0){
        log.errorLog(&log,"avformat find stream info");
        return -1;
    }


    for(int i=0;i<fmtCtx->nb_streams;i++){
        if(fmtCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO){
         vstream = fmtCtx->streams[i];
        }
    }

    if(vstream == NULL){
     log.errorLog(&log,video stream not found);
     return -1;
    }

    AVCodec *vcodec = avcodec_find_decoder(vstream->codecpar->codecid);

    if(vcodec == NULL){

    }

    AVCodecContect *vcodecCtx = avcodec_alloc_context3(vcodec);
    if(vcodecCtx == NULL){

    }

    ret = avcodec_open2(vcodecCtx,vcodec,NULL):
    if(ret < 0){
        
    }

    
}