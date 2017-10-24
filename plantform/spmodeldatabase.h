#ifndef __SPMODELDATABASE_H
#define __SPMODELDATABASE_H
#include <vector>
#include <string>
#include <tuple>
#include <map>
#include <boost/dll.hpp>
using namespace std;
typedef double spfloat; //64λ���ȸ���
typedef int spindex;    //32λ����
struct modelimpl;
struct modeldataimpl;
struct spprojection;
struct model;
struct projectinfo;
struct modelinfo;
struct scop;
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

enum eveltype
{
    rk1 = 1,
    rk2 = 2,
    rk4 = 4,
    rk8 = 8
};
struct modelinfo
{
    string _modelname;
    vector<string> _x_name;
    vector<string> _in_name;
    vector<string> _out_name;
    vector<string> _param_name;
    vector<tuple<string, string, string>> _linker;
    string _database; //���ݾ�����ô��������˵Ӧ���Ǳ������Լ�������
    string _dllpath;  //�����·��
    string _funcname; //���غ�������
};
struct projectinfo
{
    string _projectname;
    vector<modelinfo> _models;
};
struct scop
{
};

struct modelimpl
{
    spindex _xdim;     //״̬����ά��
    spindex _indim;    //����ά��
    spindex _outdim;   //���ά��
    spindex _paramdim; //����ά��
};

struct modeldataimpl
{
    spfloat *_x;     //״̬����ά��
    spfloat *_in;    //����ά��
    spfloat *_out;   //���ά��
    spfloat *_param; //����ά��
    spfloat *_f;
};
struct model
{
    modelimpl _impl;
    modeldataimpl _data;
    spfunc _func;
    boost::dll::shared_library lib;
    model *_models;
    modelinfo *_modelinfo;
    //������������(ǰ��)(����˿ڱ�ţ�����ģ�ͱ�ţ�����ģ������˿ڱ��)
    vector<tuple<spindex, spindex, spindex>> _linker;
    void *_dllmodel;
    sysinfo _sys;
};
struct spprojection
{
    unsigned int _projectid; //��Ŀ���
    vector<model> _models;   //ģ��
    spindex _xdim;           //״̬����ά��
    spindex _indim;          //����ά��
    spindex _outdim;         //���ά��
    spindex _paramdim;       //����ά��
    spfloat *_x;             //״̬����ά��
    spfloat *_in;            //����ά��
    spfloat *_out;           //���ά��
    spfloat *_param;         //����ά��
    eveltype _evetype;
    spfloat _time; //ʱ��
    spfloat _step; //ʱ��
    spfloat _endtime;
    spfloat _begtime;
};
#endif