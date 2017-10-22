#include "modelbase.h"
#include <iostream>
#include <codecvt>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/typeof/typeof.hpp>
int _backfunc(int _msg, double _time, double *_x, double *_f,
              double *_in, double *_out, double *_params)
{
    printf("Ĭ��\n");
    return 0;
}
using namespace boost::property_tree;
void eraseStringHeadAndEnd(string &_string)
{
    uint32_t index = 0;
    for (auto word : _string)
    {
        if (word != '\t' && word != '\n' && word != ' ')
            break;
        index++;
    }
    _string.erase(_string.begin(), _string.begin() + index);
    index = 0;
    for (auto word = _string.rbegin(); word != _string.rend(); ++word)
    {
        if (*word != '\t' && *word != '\n' && *word != ' ')
            break;
        index++;
    }
    _string.erase(_string.end() - index, _string.end());
}
spindex findmodelindex(const string &_name, projectinfo *_themodel)
{
    spindex _index = 0;
    for (auto model_ : _themodel->_models)
    {
        if (_name == model_._modelname)
            return _index;
        ++_index;
    }
    return -1;
}
spindex findoutindex(const string &_name, modelinfo *_themodel)
{
    spindex _index = 0;
    for (auto name_ : _themodel->_out_name)
    {
        if (_name == name_)
            return _index;
        ++_index;
    }
    return -1;
}
spindex findinindex(const string &_name, modelinfo *_themodel)
{
    spindex _index = 0;
    for (auto name_ : _themodel->_in_name)
    {
        if (_name == name_)
            return _index;
        ++_index;
    }
    return -1;
}
spprojection *make_project(projectinfo *_projectinfo)
{
    spprojection *theproject_ = new spprojection();
    theproject_->_models.resize(_projectinfo->_models.size());
    //��һ����дmodel _impl
    size_t index_ = 0, sizex_ = 0, sizein_ = 0, sizeout_ = 0, sizeparam_ = 0;
    double *begx_, *begin_, *begout_, *begparam_;
    for (auto &projectmodel : theproject_->_models)
    {
        projectmodel._impl._indim = _projectinfo->_models[index_]._in_name.size();
        projectmodel._impl._outdim = _projectinfo->_models[index_]._out_name.size();
        projectmodel._impl._xdim = _projectinfo->_models[index_]._x_name.size();
        projectmodel._impl._paramdim = _projectinfo->_models[index_]._param_name.size();
        projectmodel._models = theproject_->_models.data();
        index_++;
    }
    //�ڶ�������model _implͳ��������
    for (auto &projectmodel : theproject_->_models)
    {
        sizex_ += projectmodel._impl._xdim;
        sizein_ += projectmodel._impl._indim;
        sizeout_ += projectmodel._impl._outdim;
        sizeparam_ += projectmodel._impl._paramdim;
    }
    //ͳ�ƽ��д��
    theproject_->_indim = sizein_;
    theproject_->_xdim = sizex_;
    theproject_->_outdim = sizeout_;
    theproject_->_paramdim = sizeparam_;
    //ͳһ��ʼ���ڴ�
    theproject_->_x = new spfloat[sizex_];
    theproject_->_in = new spfloat[sizein_];
    theproject_->_out = new spfloat[sizeout_];
    theproject_->_param = new spfloat[sizeparam_];
    //�����ÿ��ģ��
    begx_ = theproject_->_x;
    begin_ = theproject_->_in;
    begout_ = theproject_->_out;
    begparam_ = theproject_->_param;
    for (auto &projectmodel : theproject_->_models)
    {
        projectmodel._data._x = begx_;
        projectmodel._data._in = begin_;
        projectmodel._data._out = begout_;
        projectmodel._data._param = begparam_;
        begx_ += projectmodel._impl._xdim;
        begin_ += projectmodel._impl._indim;
        begout_ += projectmodel._impl._outdim;
        begparam_ += projectmodel._impl._paramdim;
    }

    //����������
    //������������(ǰ��)(����˿����ƣ�����ģ����������ģ������˿�����)
    //vector<tuple<string, string, string>> _linker;;
    //������������(ǰ��)(����˿ڱ�ţ�����ģ�ͱ�ţ�����ģ������˿ڱ��)
    //vector<tuple<spindex, spindex, spindex>> _linker;
    vector<modelinfo> &themodels = _projectinfo->_models;
    index_ = 0;
    for (auto &projectmodel : theproject_->_models)
    {
        for (auto &linker_ : themodels[index_]._linker)
        {
            const string &inname_ = std::get<0>(linker_);
            spindex inindex_ = findinindex(inname_, &themodels[index_]);
            if (inindex_ >= 0)
            {
                const string &modelname_ = std::get<1>(linker_);
                spindex modeloutindex_ = findmodelindex(modelname_, _projectinfo);
                if (modeloutindex_ >= 0 && modeloutindex_ < themodels.size())
                {
                    const string &outname_ = std::get<2>(linker_);
                    spindex outindex_ = findoutindex(outname_, &themodels[modeloutindex_]);
                    if (outindex_ >= 0)
                        projectmodel._linker.push_back(make_tuple(inindex_, modeloutindex_, outindex_));
                }
            }
        }
        index_++;
    }
    //��ȡ����dll
    index_ = 0;
    for (auto &projectmodel : theproject_->_models)
    {
        if (themodels[index_]._dllpath.empty() || themodels[index_]._funcname.empty())
            projectmodel._func = _backfunc;
        try
        {
            projectmodel.lib.load(themodels[index_]._dllpath);
        }
        catch (exception &_e)
        {
            projectmodel._func = _backfunc;
            index_++;
            continue;
        }
        if (projectmodel.lib.is_loaded())
        {
            try
            {
                if (projectmodel.lib.has(themodels[index_]._funcname))
                    projectmodel._func = projectmodel.lib.get<decltype(_backfunc)>(themodels[index_]._funcname);
                else
                    projectmodel._func = _backfunc;
            }
            catch (exception &_e)
            {
                std::cout << _e.what() << endl
                          << themodels[index_]._funcname << themodels[index_]._dllpath;
                projectmodel._func = _backfunc;
            }
        }
        index_++;
    }
    //Ĭ��
    theproject_->_evetype = eveltype::rk1;
    theproject_->_step = 0.01;
    theproject_->_begtime = 0;
    theproject_->_endtime = 1000;
    return theproject_;
}
//��ȡparam
int readparam(const ptree &_ptree, vector<string> &_param)
{
    auto params_ = _ptree.get_child("");
    //_param.push_back(make_pair(submodel.second.get<string>("<xmlattr>.name", "����δ����"), vector<pair<char *, string>>()));
    for (auto param_ : params_)
    {
        if (string("subparam") == param_.first.data())
        {
            string str = param_.second.get<string>("<xmlattr>.name", "����δ����");
            _param.push_back(str);
        }
    }
    return _param.size();
};
//��ȡx
int readx(const ptree &_ptree, vector<string> &_x)
{
    auto params_ = _ptree.get_child("");
    //_param.push_back(make_pair(submodel.second.get<string>("<xmlattr>.name", "����δ����"), vector<pair<char *, string>>()));
    for (auto param_ : params_)
    {
        if (string("substata") == param_.first.data())
        {
            string str = param_.second.get<string>("<xmlattr>.name", "����δ����");
            _x.push_back(str);
        }
    }
    return _x.size();
};
//��ȡin
int readin(const ptree &_ptree, modelinfo &_model)
{
    auto params_ = _ptree.get_child("");
    auto &_in = _model._in_name;
    auto &_linkers = _model._linker;
    //_param.push_back(make_pair(submodel.second.get<string>("<xmlattr>.name", "����δ����"), vector<pair<char *, string>>()));
    for (auto param_ : params_)
    {
        if (string("subin") == param_.first.data())
        {
            string instr_ = param_.second.get<string>("<xmlattr>.name", "����δ����");
            _in.push_back(instr_);
            auto linkers = param_.second.get_child("");
            for (auto link : linkers)
            {
                if (string("link") == link.first)
                {
                    string outmodelstr_ = link.second.get<string>("<xmlattr>.model", "����δ����");
                    string outname_ = link.second.get<string>("", "δ����");
                    eraseStringHeadAndEnd(instr_);
                    eraseStringHeadAndEnd(outmodelstr_);
                    eraseStringHeadAndEnd(outname_);
                    _linkers.push_back(make_tuple(instr_, outmodelstr_, outname_));
                }
            }
        }
    }
    return _in.size();
};
//��ȡout
int readout(const ptree &_ptree, vector<string> &_out)
{
    auto params_ = _ptree.get_child("");
    //_param.push_back(make_pair(submodel.second.get<string>("<xmlattr>.name", "����δ����"), vector<pair<char *, string>>()));
    for (auto param_ : params_)
    {
        if (string("subout") == param_.first.data())
        {
            string str = param_.second.get<string>("<xmlattr>.name", "����δ����");
            _out.push_back(str);
        }
    }
    return _out.size();
};
//��ȡdll·��
int readdll(const ptree &_ptree, modelinfo &_model)
{
    auto params_ = _ptree.get_child("");
    //_param.push_back(make_pair(submodel.second.get<string>("<xmlattr>.name", "����δ����"), vector<pair<char *, string>>()));
    for (auto param_ : params_)
    {
        if (string("path") == param_.first.data())
        {
            string str = param_.second.get<string>("", "");
            eraseStringHeadAndEnd(str);
            _model._dllpath = str;
        }
        if (string("name") == param_.first.data())
        {
            string str = param_.second.get<string>("", "");
            eraseStringHeadAndEnd(str);
            _model._funcname = (str);
        }
    }
    return 0;
}
//�õ�һ��model
modelinfo make_model(const ptree &_ptree)
{
    modelinfo themodel_;
    themodel_._modelname = _ptree.get<string>("<xmlattr>.name", "notdef");
    auto submodels = _ptree.get_child("");
    for (auto submodel : submodels)
    {
        if (string("param") == submodel.first.data())
            readparam(submodel.second, themodel_._param_name);
        if (string("stata") == submodel.first.data())
            readx(submodel.second, themodel_._x_name);
        if (string("out") == submodel.first.data())
            readout(submodel.second, themodel_._out_name);
        if (string("in") == submodel.first.data())
            readin(submodel.second, themodel_);
        if (string("function") == submodel.first.data())
            readdll(submodel.second, themodel_);
    }
    return themodel_;
};
projectinfo *readprojectfromxml(const string &_path)
{
    ptree xmltree;
    ptree pt_empty;
    projectinfo *theinfp_ = nullptr;
#ifdef USE_UTF_8
    std::locale utf8Locale(std::locale(), new std::codecvt_utf8<wchar_t>());
    read_xml(_path, xmltree, 0, utf8Locale);
#else
    read_xml(_path, xmltree);
#endif
    theinfp_ = new projectinfo();
    const ptree _models = xmltree.get_child("tarjet");
    for (auto _model : _models)
    {
        //ģ�ͱ�ǩ
        if (string("model") == _model.first.data())
        {
            theinfp_->_models.push_back(make_model(_model.second));
        }
    }
    return theinfp_;
};
int getparamData(const ptree &_tree, model &_model)
{
    auto params_ = _tree.get_child("");
    //_param.push_back(make_pair(submodel.second.get<string>("<xmlattr>.name", "����δ����"), vector<pair<char *, string>>()));
    spfloat *beg_ = _model._data._param;
    spindex index_ = 0;
    for (auto param_ : params_)
    {
        if (string("subparam") == param_.first.data())
        {
            spfloat data = param_.second.get<spfloat>("", 0);
            *beg_ = data;
            beg_++;
            index_++;
        }
    }
    return index_;
};
int getstataData(const ptree &_tree, model &_model)
{
    auto params_ = _tree.get_child("");
    //_param.push_back(make_pair(submodel.second.get<string>("<xmlattr>.name", "����δ����"), vector<pair<char *, string>>()));
    spfloat *beg_ = _model._data._x;
    spindex index_ = 0;
    for (auto param_ : params_)
    {
        if (string("substata") == param_.first.data())
        {
            spfloat data = param_.second.get<spfloat>("", 0);
            *beg_ = data;
            beg_++;
            index_++;
        }
    }
    return index_;
}

int getoutData(const ptree &_tree, model &_model)
{
    auto params_ = _tree.get_child("");
    //_param.push_back(make_pair(submodel.second.get<string>("<xmlattr>.name", "����δ����"), vector<pair<char *, string>>()));
    spfloat *beg_ = _model._data._out;
    spindex index_ = 0;
    for (auto param_ : params_)
    {
        if (string("subout") == param_.first.data())
        {
            spfloat data = param_.second.get<spfloat>("", 0);
            *beg_ = data;
            beg_++;
            index_++;
        }
    }
    return index_;
}

int getinData(const ptree &_tree, model &_model)
{
    auto params_ = _tree.get_child("");
    //_param.push_back(make_pair(submodel.second.get<string>("<xmlattr>.name", "����δ����"), vector<pair<char *, string>>()));
    spfloat *beg_ = _model._data._in;
    spindex index_ = 0;
    for (auto param_ : params_)
    {
        if (string("subin") == param_.first.data())
        {
            spfloat data = param_.second.get<spfloat>("", 0);
            *beg_ = data;
            beg_++;
            index_++;
        }
    }
    return index_;
}
int getevelType(const ptree &_tree, spprojection *_projection)
{
    auto rktype_ = _tree.get("<xmlattr>.name", "rk1");
    eraseStringHeadAndEnd(rktype_);
    if (string("rk1") == rktype_)
        _projection->_evetype = eveltype::rk1;
    if (string("rk2") == rktype_)
        _projection->_evetype = eveltype::rk2;
    if (string("rk4") == rktype_)
        _projection->_evetype = eveltype::rk4;
    if (string("rk8") == rktype_)
        _projection->_evetype = eveltype::rk8;
    auto rktypes_ = _tree.get_child("");
    for (auto rktype_ : rktypes_)
    {
        if (string("step") == rktype_.first)
            _projection->_step = rktype_.second.get<spfloat>("", 0.01);
        if (string("timebeg") == rktype_.first)
            _projection->_begtime = rktype_.second.get<spfloat>("", 0);
        if (string("timeend") == rktype_.first)
            _projection->_endtime = rktype_.second.get<spfloat>("", 1000);
    }
    return 0;
};
int InitalDataFromXml(spprojection *_projection, const string &_path)
{
    if (!_projection)
        return -1;
    auto modeldata = _projection->_models;
    ptree xmltree;
    ptree pt_empty;
#ifdef USE_UTF_8
    std::locale utf8Locale(std::locale(), new std::codecvt_utf8<wchar_t>());
    read_xml(_path, xmltree, 0, utf8Locale);
#else
    read_xml(_path, xmltree);
#endif
    const ptree _models = xmltree.get_child("tarjet");
    spindex index_ = 0;
    for (auto _model : _models)
    {
        //ģ�ͱ�ǩ
        if (string("model") == _model.first.data())
        {
            auto subodels = _model.second.get_child("");
            for (auto submodel : subodels)
            {
                if (string("param") == submodel.first.data())
                    getparamData(submodel.second, modeldata[index_]);
                if (string("stata") == submodel.first.data())
                    getstataData(submodel.second, modeldata[index_]);
                if (string("out") == submodel.first.data())
                    getoutData(submodel.second, modeldata[index_]);
                if (string("in") == submodel.first.data())
                    getinData(submodel.second, modeldata[index_]);
            }
            ++index_;
        }
        if (string("eveltype") == _model.first.data())
        {
            getevelType(_model.second, _projection);
        }
    }
};