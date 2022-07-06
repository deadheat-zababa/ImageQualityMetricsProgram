#include<libavcodec/avcodec.h>
#include<libavformt/avformat.h>
#include "log.h"

extern logger log;

int fftrans(const char *filename,unsigned char* yuvdata){
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
     log.errorLog(&log,"video stream not found");
     return -1;
    }

    AVCodec *vcodec = avcodec_find_decoder(vstream->codecpar->codecid);

    if(vcodec == NULL){
        log.errorLog(&log,"decoder not found");
    }

    AVCodecContect *vcodecCtx = avcodec_alloc_context3(vcodec);
    if(vcodecCtx == NULL){
        log.errorLog(&log,"not allocate memory");
    }
    ret = avcodec_parameters_to_context(vcodecCtx,fmtCtx->streams[vstream]->codecpar);

    ret = avcodec_open2(vcodecCtx,vcodec,NULL);
    if(ret < 0){
        log.errorLog(&log,"not open codec");
    }

    AVFrame *frame;
    frame = av_frame_alloc();
    if(pkt==NULL){
        log.errorLog(&log,"Error:av_frame_alloc");
    }

    AVPacket *pkt;
    pkt = av_packet_alloc();
    if(pkt==NULL){
        log.errorLog(&log,"Error:av_packet_alloc");
    }

    int framenum;
    framenum = vstream->nb_frames;

    if(framenum < 1){
        log.errorLog(&log,"frame is 0");
        return -1;
    }

    yuvdata = (unsigned char*)malloc(sizeof(unsigned char) * vcodecCtx->width * vcodecCtx->height * 3 * framenum);

    while(1){
        ret = av_read_frame(AVFormatContext,pkt);
        
        if(ret < 0){
            log.debugLog(&log,"pkt empty");
            break;
        }

        if(pkt->stream_index == vstream){
            
            ret = avcodec_send_packet(vcodecCtx,pkt);
            if(ret < 0 ){
                log.errorLog(&log,"Error:avcodec_send_packet");
                return -1;
            }

            while(ret >= 0){

                ret =avcodec_receive_frame(vcodecCtx,frame);

                if(ret == AVERROR(EAGAIN)||ret == AVERROR_EOF){
                    log.debugLog(&log,"frame empty");
                    break;
                }
                else if(ret < 0){
                    log.errorLog(&log,"Error:avcodec_receive_frame");
                    return -1;
                }
            }

            //1Frame
            memcpy(yuvdata,frame->data,vcodecCtx->width * vcodecCtx->height * 3);
            framenum--;
            yuvdata = yuvdata + (vcodecCtx->width * vcodecCtx->height * 3);

        }

    }

    if(framenum != 0){
        log.errorLog(&log,"frame number unmatch");
        return -1;
    }

    log.infoLog("fftrans end");

    return 0;    
}