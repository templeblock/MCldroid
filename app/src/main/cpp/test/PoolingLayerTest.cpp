//
// Created by 神经元002 on 2017/3/21.
//
#include <jni.h>
#include <PoolingLayer.h>
#include <stdlib.h>
#include <utils.h>

PoolingLayer * poolingLayerPtr = NULL;
float * dataPtr1 = NULL;
MultiDimensionData<float> input;
MultiDimensionData<float> output;

void initEnv(){
    std::vector<size_t> pad(2);
    pad[0] = 0;
    pad[1] = 0;
    std::vector<size_t> stride(2);
    stride[0] = 1;
    stride[1] = 1;
    std::vector<size_t> kernel(2);
    kernel[0] = 3;
    kernel[1] = 3;
    PoolingLayer::Param param(pad, stride, kernel);
    poolingLayerPtr = new PoolingLayer("testPoolingLayer", PoolingLayer::mean, param);
    dataPtr1 = new float[25];
    for (int i = 0; i<25; i++){
        dataPtr1[i] =  (-rand()%51+25);
        LOGD("input data [%i] : %f",i, dataPtr1[i]);
    }
    input.setData(dataPtr1,1, 1, 5, 5);
}

extern "C"{

JNIEXPORT void JNICALL
Java_com_compilesense_liuyi_mcldroid_NativeTest_poolingLayerTest(JNIEnv *env, jclass type) {
    initEnv();
    poolingLayerPtr->compute(&input, &output);
    size_t outputTotalSize = output.totalSize();
    for (int i = 0; i < outputTotalSize; i++){
        LOGD("out data [%i] : %f",i,  output.data_ptr[i]);
    }
    delete poolingLayerPtr;
}


}