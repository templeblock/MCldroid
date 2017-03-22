//
// Created by 神经元002 on 2017/3/20.
//

#ifndef CAFFEMODELINPUT_SOFTMAXLAYER_H
#define CAFFEMODELINPUT_SOFTMAXLAYER_H

#include "MultiDimensionData.h"
#include "BaseLayer.h"

class SoftmaxLayer : public BaseLayer {
public:
    SoftmaxLayer(const std::string &name) : BaseLayer(name) { }
    void compute(MultiDimensionData<float> *inputData, MultiDimensionData<float> *outputData);

protected:
    void computeOutputShape(MultiDimensionData<float> *inputData, MultiDimensionData<float> *outputData);
};


#endif //CAFFEMODELINPUT_SOFTMAXLAYER_H
