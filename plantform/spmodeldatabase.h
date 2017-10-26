#ifndef __SPMODELDATABASE_H
#define __SPMODELDATABASE_H
#include <vector>
#include <string>
#include <tuple>
#include <map>
#include "recorder.h"
#include <boost/dll.hpp>
#include <boost/random.hpp>
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

enum eveltype
{
    rk1 = 1,
    rk2 = 2,
    rk4 = 4,
    rk8 = 8
};
struct modelinfo
{
    std::string _modelname;
    std::vector<string> _x_name;
    std::vector<string> _in_name;
    std::vector<string> _out_name;
    std::vector<string> _param_name;
    std::vector<tuple<string, string, string>> _linker;
    std::string _database; //���ݾ�����ô��������˵Ӧ���Ǳ������Լ�������
    std::string _dllpath;  //�����·��
    std::string _funcname; //���غ�������
};
struct projectinfo
{
    std::string _projectname;
    std::string _recorderpath;
    std::vector<modelinfo> _models;
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
    std::vector<std::tuple<spindex, spindex, spindex>> _linker;
    void *_dllmodel;
    sysinfo _sys;
};
struct distibuter
{
    boost::variate_generator<boost::mt19937,
                             boost::normal_distribution<double>>
        _sampler0;
    boost::variate_generator<boost::mt19937,
                             boost::normal_distribution<double>>
        _sampler1;
};
struct spprojection
{
    unsigned int _projectid;    //��Ŀ���
    std::vector<model> _models; //ģ��
    spindex _xdim;              //״̬����ά��
    spindex _indim;             //����ά��
    spindex _outdim;            //���ά��
    spindex _paramdim;          //����ά��
    spfloat *_x;                //״̬����ά��
    spfloat *_in;               //����ά��
    spfloat *_out;              //���ά��
    spfloat *_param;            //����ά��
    eveltype _evetype;
    recorderinfo *_recorderinfo;
    spfloat _time; //ʱ��
    spfloat _step; //ʱ��
    spfloat _endtime;
    spfloat _begtime;
    std::vector<std::tuple<distibuter, uint32_t>> _distribute;
};

#endif