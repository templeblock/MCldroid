//
// Created by 神经元002 on 2017/3/10.
//

#ifndef CAFFEMODELINPUT_ACTIVATIONLAYER_H
#define CAFFEMODELINPUT_ACTIVATIONLAYER_H

#include <MultiDimensionData.h>
#include <string>
#include <BaseLayer.h>

class ActivationLayer : public BaseLayer{
public:
    enum Type {ReLU,PReLU,TanH,Abs};
    ActivationLayer(const std::string name, Type type_)
            : BaseLayer(name),type(type_) { };
    void compute(MultiDimensionData<float> *input);
    void compute(MultiDimensionData<float> *input, MultiDimensionData<float> *output){
        compute(input);
    }
protected:
    void computeOutputShape(MultiDimensionData<float> *input, MultiDimensionData<float> * output){
        //空
    }
private:
    Type type;
};


#endif //CAFFEMODELINPUT_ACTIVATIONLAYER_H
