/*
recorder:���ݼ�¼����
*/
#ifndef __RECORDER_H
#define __RECORDER_H
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <stdint.h>
#include <malloc.h>
/*
  size_t   //ֻ�洢һ�ε���������
  size_t   //��γ�����������
  size_t   //��γ������ݼ���
*/
enum datatype
{
  _size1 = 1,
  _size2 = 2,
  _size4 = 4,
  _size8 = 8,
  _size16 = 16,
};
struct datainfo
{
  uint64_t _datatype;
  uint64_t _signum = 0;     //ֻ�洢һ�ε���������
  uint64_t _mutinum = 0;    //��γ�����������
  uint64_t _mutilength = 0; //��γ������ݳ���(�ֽڳ���)
};
struct recorder
{
  datainfo _info; //�洢������Ϣ
  char *_sigbuff; //һ��д������
  FILE *_file;
  uint32_t _step;  //�洢ʱ��
  uint32_t _count; //д�����
};
struct recorderinfo
{
  uint32_t _step;    //�洢ʱ��
  std::string _path; //�洢·��
};
//(���𲻴� ������Ӳ�����ܻ���ɶԭ�򲻹� linux �¾Ͳ���˵�����Ի��Ǳ�����)
int setRecorderBuffer(recorder *_recorder, uint64_t _size);
int recorderWriteSig(recorder *_recorder,
                     char *_data, uint64_t _length);

int recorderWriteMulti(recorder *_recorder,
                       char *_data, uint64_t _length);
//ֻ�洢һ�ε���������
//��γ�����������
recorder *createrRecorder(uint64_t _signum,
                          uint64_t _mutinum,
                          datatype _datatype,
                          const char *_path,
                          uint32_t _step = 0);
int closeRecorder(recorder *_recorder);

#endif