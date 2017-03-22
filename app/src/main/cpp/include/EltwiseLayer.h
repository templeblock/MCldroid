//
// Created by 神经元002 on 2017/3/22.
//

#ifndef MCLDROID_ELTWISELAYER_H
#define MCLDROID_ELTWISELAYER_H

#include <BaseLayer.h>
#include <MultiDimensionData.h>

class EltwiseLayer : public BaseLayer{

public:
    enum Type {product,sum,max};

    EltwiseLayer(const std::string &name, Type type, std::vector<float> coefficients)
            : BaseLayer(name), type(type), coefficients(coefficients) { }
    void compute(MultiDimensionData<float> *input, MultiDimensionData<float> *output);

protected:
    void computeOutputShape(MultiDimensionData<float> *input, MultiDimensionData<float> * output);

private:
    std::vector<float> coefficients;
    Type type;
};


#endif //MCLDROID_ELTWISELAYER_H
