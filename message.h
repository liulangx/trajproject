#ifndef __MESSAGE_H_
#define __MESSAGE_H_
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