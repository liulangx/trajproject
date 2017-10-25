#pragma once
#ifndef __MESSAGE_H_
#define __MESSAGE_H_

struct sysinfo;
typedef int (*spfunc)(int _msg, void *_model, sysinfo *_sys);
struct sysinfo
{
    unsigned int _stepcount; //���沽��
    unsigned int _xlen;
    unsigned int _outlen;
    unsigned int _inlen;
    unsigned int _parmlen;
    double _time; //ʱ��
    double _setp; //���沽��
    const char *_database;
    double *_x;
    double *_f;
    double *_param;
    double *_in;
    double *_out;
};
/*
��Ϣӳ���
������Ϣ
only:
SP_MSG_LOAD
loop:
    SP_MSG_INITIAL һ��
    for()
    {
        SP_MSG_UPDATE 
        SP_MSG_DERIVE
    }
SP_MSG_STOP

*/

#define SP_MSG_BEG 1024                 //��Ϣӳ���ʼ
#define SP_MSG_INITIAL (SP_MSG_BEG + 1) //��ʼ��

#define SP_MSG_DERIVE (SP_MSG_BEG + 2) //΢�ּ���
#define SP_MSG_UPDATE (SP_MSG_BEG + 3) //���
#define SP_MSG_STOP (SP_MSG_BEG + 4)   //ֹͣ
#define SP_MSG_CLEAR (SP_MSG_BEG + 5)  //��������
#define SP_MSG_LOAD (SP_MSG_BEG + 6)   //��������
#endif