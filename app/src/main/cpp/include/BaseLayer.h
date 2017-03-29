//
// Created by 神经元002 on 2017/3/20.
//

#ifndef CAFFEMODELINPUT_BASELAYER_H
#define CAFFEMODELINPUT_BASELAYER_H

#include <MultiDimensionData.h>
#include <string>

class BaseLayer {
public:
    BaseLayer() : name("baseLayer") { }
    BaseLayer(const std::string &name) : name(name) { }
    void setName(const std::string &name){
        this->name = name;
    }
    std::string getName(){
        return name;
    }
    /**
     * 网络中每一层的计算。
     */
    virtual void compute(MultiDimensionData<float> *input, MultiDimensionData<float> *output) = 0;
protected:
    /**
     * 计算每一次输出的Shape, 并申请输出需站的内存。
     */
    virtual void computeOutputShape(MultiDimensionData<float> *input, MultiDimensionData<float> * output) = 0;
    std::string name;
};
#endif //CAFFEMODELINPUT_BASELAYER_H
