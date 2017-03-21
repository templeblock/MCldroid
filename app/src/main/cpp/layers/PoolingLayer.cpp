//
// Created by 神经元002 on 2017/3/16.
//

#include "PoolingLayer.h"
#include "math.h"
#include "ComputeTool.h"

//在这个实现就不直接使用了。
void PoolingLayer::computeOutputShape(MultiDimensionData<float> *input,
                                      MultiDimensionData<float> *output) {
    size_t n_i = input->get_n();
    size_t c_i = input->get_c();
    size_t h_i = input->get_h();
    size_t w_i = input->get_w();

    size_t h_pad = params.pad[0];
    size_t w_pad = params.pad[1];

    size_t h_stride = params.stride[0];
    size_t w_stride = params.stride[1];

    size_t h_kernel = params.kernelSize[0];
    size_t w_kernel = params.kernelSize[1];

    size_t n_o = n_i;
    size_t c_o = c_i;
    size_t h_o = (size_t) (ceilf((h_i + 2 * h_pad - h_kernel) / h_stride) + 1);
    size_t w_o = (size_t) (ceilf((w_i + 2 * w_pad - w_kernel) / w_stride) + 1);

    float *outputPtr = new float[n_o * c_o * h_o * w_o];
    output->setData(outputPtr, n_o, c_o, h_o, w_o);
}

void PoolingLayer::compute(MultiDimensionData<float> *input, MultiDimensionData<float> *output){
    float *inputPtr = input->data_ptr;
    size_t n_i = input->get_n();
    size_t c_i = input->get_c();
    size_t h_i = input->get_h();
    size_t w_i = input->get_w();

    size_t h_pad = params.pad[0];
    size_t w_pad = params.pad[1];

    size_t h_stride = params.stride[0];
    size_t w_stride = params.stride[1];

    size_t h_kernel = params.kernelSize[0];
    size_t w_kernel = params.kernelSize[1];

    size_t n_o = n_i;
    size_t c_o = c_i;
    size_t h_o = (size_t) (ceilf((h_i + 2 * h_pad - h_kernel) / h_stride) + 1);
    size_t w_o = (size_t) (ceilf((w_i + 2 * w_pad - w_kernel) / w_stride) + 1);

    // Initialize the result.
    float *outputPtr = new float[n_o * c_o * h_o * w_o];
    output->setData(outputPtr, n_o, c_o, h_o, w_o);

    switch (kernelType){
        case mean:
            meanPoolingC(outputPtr, inputPtr,
                         n_i, c_i, h_i, w_i,
                         n_o, c_o, h_o, w_o,
                         h_stride, w_stride,
                         h_pad, w_pad,
                         h_kernel, w_kernel);
            break;
        case max:
            maxPoolingNnpack(
                    outputPtr, inputPtr,
                    n_i, c_i, h_i, w_i,
                    h_stride, w_stride,
                    h_pad, w_pad,
                    h_kernel ,w_kernel);
            break;
    }
}