//
// Created by 神经元002 on 2017/3/16.
//

#ifndef CAFFEMODELINPUT_POOLINGLAYER_H
#define CAFFEMODELINPUT_POOLINGLAYER_H


#include <string>
#include <MultiDimensionData.h>
#include <utils.h>
#include <BaseLayer.h>

class PoolingLayer : public BaseLayer {
public:
    enum KernelType{
        mean,max
    };
    class Param {
    public:
        std::vector<size_t> pad;
        std::vector<size_t> stride;
        std::vector<size_t> kernelSize;//{h,w}
        Param(const std::vector<size_t> &pad, const std::vector<size_t> &stride,
              const std::vector<size_t> &kernelSize) : pad(pad), stride(stride),
                                                    kernelSize(kernelSize) { }
    };

    PoolingLayer(const std::string &name, KernelType kernelType, const Param params )
            : BaseLayer(name), kernelType(kernelType), params(params){ }

    ~PoolingLayer(){
        LOGD(" ~PoolingLayer()");
    }
    void compute(MultiDimensionData<float> *input, MultiDimensionData<float> *output);
protected:
    void computeOutputShape(MultiDimensionData<float> *input, MultiDimensionData<float> * output);
private:
    std::string name;
    Param params;
    KernelType kernelType;
};


#endif //CAFFEMODELINPUT_POOLINGLAYER_H
