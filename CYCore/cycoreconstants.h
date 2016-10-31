#pragma once

#include <QtGlobal>

namespace CYCore {
namespace Constants {

typedef enum tagEColorSpace
{
    PIX_COLOR_GRAY  = 0x00000000,
    PIX_COLOR_BAYER = 0x00010000,
    PIX_COLOR_YUV   = 0x00020000,
    PIX_COLOR_RGB   = 0x00040000,
    PIX_COLOR_QIMAGE= 0x00080000
}EColorSpace;

typedef struct tagCYFRAME {
    uint        s_size;             // 结构体大小
    uint        s_version;          // 结构体版本号
    uint        s_seqno;            // 帧序列号
    uint        s_seqidx;           // 帧序列索引
    uint        s_id;               // 帧id
    uint        s_bw;               // 位宽,一个像素的一个颜色分量的位数
    EColorSpace s_color;            // 颜色空间
    uint        s_width;            // 宽
    uint        s_height;           // 高
    uint        s_stride;           // 内存图像跨度
    uint        s_pitch;            // 内存图像行与行之间的步进量(一般为s_bw * s_stride)
    uint        s_piclen;           // 图像占用的内存总字节数
    uint        s_length;           // 内存数据的总数据量
    void *      s_data;             // 指向内存数据的指针
    void *      s_context;          // 指向图像传递时的上下文数据库
}CYFRAME;
} // namespace Constants
} // namespace CYCore
