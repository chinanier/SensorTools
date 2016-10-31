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
    uint        s_size;             // �ṹ���С
    uint        s_version;          // �ṹ��汾��
    uint        s_seqno;            // ֡���к�
    uint        s_seqidx;           // ֡��������
    uint        s_id;               // ֡id
    uint        s_bw;               // λ��,һ�����ص�һ����ɫ������λ��
    EColorSpace s_color;            // ��ɫ�ռ�
    uint        s_width;            // ��
    uint        s_height;           // ��
    uint        s_stride;           // �ڴ�ͼ����
    uint        s_pitch;            // �ڴ�ͼ��������֮��Ĳ�����(һ��Ϊs_bw * s_stride)
    uint        s_piclen;           // ͼ��ռ�õ��ڴ����ֽ���
    uint        s_length;           // �ڴ����ݵ���������
    void *      s_data;             // ָ���ڴ����ݵ�ָ��
    void *      s_context;          // ָ��ͼ�񴫵�ʱ�����������ݿ�
}CYFRAME;
} // namespace Constants
} // namespace CYCore
