#ifndef __MESSAGE_H_
#define __MESSAGE_H_
/*
��Ϣӳ���
������Ϣ
SP_MSG_INITIAL һ��

for()
{
    SP_MSG_UPDATE 
    linker
    SP_MSG_DERIVE
}

*/
#define SP_MSG_BEG 1024                 //��Ϣӳ���ʼ
#define SP_MSG_INITIAL (SP_MSG_BEG + 1) //��ʼ��

#define SP_MSG_DERIVE (SP_MSG_BEG + 2) //΢�ּ���
#define SP_MSG_UPDATE (SP_MSG_BEG + 3) //���
#define SP_MSG_STOP (SP_MSG_BEG + 4)   //ֹͣ
#define SP_MSG_CLEAR (SP_MSG_BEG + 5)  //��������

#endif