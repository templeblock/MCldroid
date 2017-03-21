//
// Created by 神经元002 on 2017/3/10.
//

#include "ActivationLayerTest.h"
#include <ComputeTool.h>


void ActivationLayer::compute(MultiDimensionData<float> *input) {
    switch (type){
        case ReLU:
            relu(input->data_ptr, input->totalSize());
            break;
        case PReLU:
            prelu(input->data_ptr, input->totalSize());
            break;
        case TanH:
            tanh(input->data_ptr, input->totalSize());
            break;
        case Abs:
            abs(input->data_ptr, input->totalSize());
            break;
    }
}