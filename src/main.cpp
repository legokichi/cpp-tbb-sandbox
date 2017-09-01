#include <iostream>
extern "C" {
    #include <libavdevice/avdevice.h>
    #include <libavcodec/avcodec.h>
    #include <libavfilter/avfilter.h>
    #include <libavformat/avformat.h>
    #include <libswscale/swscale.h>
};
// https://github.com/abdullahfarwees/screen-recorder-ffmpeg-cpp/blob/master/ScreenRecorder.cpp
auto main(int argc, char* argv[])-> int {
    using std::cout;
    using std::endl;
    av_register_all();
	avcodec_register_all();
    avdevice_register_all();
    auto pAVFormatContext = avformat_alloc_context();
    auto pAVInputFormat = av_find_input_format("x11grab");
    AVDictionary *options { nullptr };
    cout << avformat_open_input(&pAVFormatContext, ":0.0+10,250", pAVInputFormat, nullptr) << endl;
    cout << av_dict_set(&options,"framerate","30", 0) << endl;
    cout << av_dict_set(&options, "preset", "medium", 0) << endl;
    cout << avformat_find_stream_info(pAVFormatContext,NULL) << endl;
    int VideoStreamIndx = -1;
    for(int i = 0; i < pAVFormatContext->nb_streams; i++){ // find video stream posistion/index.
	  if(pAVFormatContext->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO){
	     VideoStreamIndx = i;
	     break;
	  }
    }
    cout << VideoStreamIndx << endl;
    auto pAVCodecContext = pAVFormatContext->streams[VideoStreamIndx]->codec;
    auto pAVCodec = avcodec_find_decoder(pAVCodecContext->codec_id);
    cout << avcodec_open2(pAVCodecContext , pAVCodec , NULL) << endl;

    auto pAVPacket = (AVPacket*)av_malloc(sizeof(AVPacket));
    av_init_packet(pAVPacket);
    auto pAVFrame = av_frame_alloc();
    auto outFrame = av_frame_alloc();
    AVCodecContext *outAVCodecContext { nullptr };
	
    
	if( video_outbuf == NULL )
	{
	cout<<"\n\nError : av_malloc()";
	}

    cout << "ok" << endl;
    avformat_close_input(&pAVFormatContext);
    avformat_free_context(pAVFormatContext);
    return EXIT_SUCCESS;
}
