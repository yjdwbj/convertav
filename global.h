#ifndef GLOBAL_H
#define GLOBAL_H

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
} itemstruct;


typedef struct {
    QString OutputDir;
    bool AutoOpen;
    QString Height;
    QString Width;
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



#endif // GLOBAL_H
