#ifndef GLOBAL_H
#define GLOBAL_H


#define NTSC "720x480"
#define PAL "720x576"
#define QNTSC "352x240"
#define QPAL "352x288"
#define SNTSC "640x480"
#define SPAL "768x576"
#define FILM "352x240"
#define NTSC-film "352x240"
#define SQCIF "128x96"
#define QCIF "176x144"
#define CIF "352x288"
//#define 4CIF "704x576"
//#define 16CIF 1408x1152
#define QQVGA "160x120"
#define QVGA "320x240"
#define VGA "640x480"
#define SVGA "800x600"
#define XGA "1024x768"
#define UXGA "1600x1200"
//#define QXGA 2048x1536
#define SXGA "1280x1024"
//#define QSXGA 2560x2048
//#define HSXGA 5120x4096
#define WVGA "852x480"
#define WXGA "1366x768"
//#define WSXGA 1600x1024
//#define WUXGA 1920x1200
//#define WOXGA 2560x1600
//#define WQSXGA 3200x2048
//#define WQUXGA 3840x2400
//#define WHSXGA 6400x4096
//#define WHUXGA 7680x4800
#define CGA "320x200"
#define EGA "640x350"
#define HD480 "852x480"
#define HD720 "1280x720"
#define HD1080 "1920x1080"
//#define 2K 2048x1080
//#define 2KFLAT 1998x1080
//#define 2KSCOPE 2048x858
//#define 4K 4096x2160
//#define 4KFLAT 3996x2160
//#define 4KSCOPE 4096x1716
#define NHD "640x360"
#define HQVGA "240x160"
#define WQVGA "400x240"
#define FWQVGA "432x240"
#define HVGA "480x320"
#define QHD "960x540"


typedef struct{
    char *name;
    char *size;
} VideoSize;

typedef struct {
    QString  filename;
    QString  video_info;
    QString  audio_info;
    QString  file_time;
    QString  image;
    QString  fullpath;
    QString  outputFullPath;
    bool isConverted;
    bool isStandby;
    bool isSingleConvert;
    bool isCancelConvert;
} itemstruct;


typedef struct {
    QString OutputDir;
    bool AutoOpen;
    QString StartTime;
    QString Endpos;
   // QString Height;
   // QString Width;
    QString VScale;
    QString VEncoder;
    QString HWRatio;
    QString VBitRate;
    QString EncoderCount;
    QString Frame;
    QString AEncoder;
    QString ABitRate;
    QString Channel;
    QString SampleRate;

} ConvertCfg;


static const VideoSize  support_scale[]={{"NTSC", "720x480"},
                           {"PAL", "720x576"},
                           {"QNTSC", "352x240"},
                           {"QPAL", "352x288"},
                           {"SNTSC", "640x480"},
                           {"SPAL", "768x576"},
                           {"FILM", "352x240"},
//                           {"NTSC-film","352x240"},
                           {"SQCIF", "128x96"},
                           {"QCIF", "176x144"},
                           {"CIF", "352x288"},
                           {"4CIF", "704x576"},
                           {"16CIF", "1408x1152"},
                           {"QQVGA", "160x120"},
                           {"QVGA", "320x240"},
                           {"VGA", "640x480"},
                           {"SVGA", "800x600"},
                           {"XGA", "1024x768"},
                           {"UXGA", "1600x1200"},
//                           {"QXGA", "2048x1536"},
//                           {"SXGA", "1280x1024"},
//                           {"QSXGA", "2560x2048"},
//                           {"HSXGA", "5120x4096"},
                           {"WVGA", "852x480"},
                           {"WXGA", "1366x768"},
                           {"WSXGA", "1600x1024"},
                           {"WUXGA", "1920x1200"},
//                           {"WOXGA", "2560x1600"},
//                           {"WQSXGA", "3200x2048"},
//                           {"WQUXGA", "3840x2400"},
//                           {"WHSXGA", "6400x4096"},
//                           {"WHUXGA", "7680x4800"},
                           {"CGA", "320x200"},
                           {"EGA", "640x350"},
                           {"HD480", "852x480"},
                           {"HD720", "1280x720"},
                           {"HD1080", "1920x1080"},
//                           {"2K", "2048x1080"},
//                           {"2KFLAT", "1998x1080"},
//                           {"2KSCOPE", "2048x858"},
//                           {"4K", "4096x2160"},
//                           {"4KFLAT", "3996x2160"},
//                           {"4KSCOPE", "4096x1716"},
                           {"NHD", "640x360"},
                           {"HQVGA", "240x160"},
                           {"WQVGA", "400x240"},
                           {"FWQVGA", "432x240"},
                           {"HVGA", "480x320"},
                           {"QHD", "960x540"},
                                    };


static const char *support_format[] ={"avi","mp4","rm","mkv","wmv","mov","3gp","mpeg","dat","end"};



;
#endif // GLOBAL_H
