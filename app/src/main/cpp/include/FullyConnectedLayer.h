//
// Created by 神经元002 on 2017/3/7.
//

#ifndef CAFFEMODELINPUT_FULLYCONECTEDLAYER_H
#define CAFFEMODELINPUT_FULLYCONECTEDLAYER_H


#include <BaseLayer.h>
#include <MultiDimensionData.h>

class FullyConnectedLayer: BaseLayer{
public:
    FullyConnectedLayer(const std::string &name, bool nonLinear = false)
            :BaseLayer(name),nonLinear(nonLinear) {
        this->paramHadLoad = false;
    };
    ~FullyConnectedLayer();
    void setKernel(float *weight, int weightSize, float *bias, int biasSize);
    void releaseKernel();
    void compute(MultiDimensionData<float > *input, MultiDimensionData<float > *output);

protected:
    /**
     * 计算每一次输出的Shape, 并申请输出需站的内存。
     */
    void computeOutputShape(MultiDimensionData<float> *input, MultiDimensionData<float> * output){
        //空
    }

private:
    bool nonLinear;

    bool paramHadLoad;
    MultiDimensionData<float> weight;
    MultiDimensionData<float> bias;
};


#endif //CAFFEMODELINPUT_FULLYCONECTEDLAYER_H
