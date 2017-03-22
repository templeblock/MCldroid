//
// Created by 神经元002 on 2017/3/22.
//

#include "EltwiseLayer.h"
#include <ComputeTool.h>
void EltwiseLayer::compute(MultiDimensionData<float> *input, MultiDimensionData<float> *output){
    const size_t arraySize = coefficients.size();
    std::vector<float*> inputArray(arraySize);
    for (int i = 0; i < arraySize; i++){
        inputArray[i] = input[i].data_ptr;
    }

    computeOutputShape(input, output);

    switch (type){
        case sum:
            elementSumNeon(inputArray.data(),
                           output->data_ptr,
                           coefficients.data(),
                           arraySize,
                           output->totalSize());
            break;

        case product:
            elementProductNeon(inputArray.data(),
                           output->data_ptr,
                           coefficients.data(),
                           arraySize,
                           output->totalSize());
            break;

        case max:
            elementMaxNeon(inputArray.data(),
                           output->data_ptr,
                           coefficients.data(),
                           arraySize,
                           output->totalSize());
            break;
    }
}

void EltwiseLayer::computeOutputShape(MultiDimensionData<float> *input, MultiDimensionData<float> * output){
    size_t size = input->totalSize();
    float *dataPtr = new float[size];
    output->setData(dataPtr, input->shape);
}
