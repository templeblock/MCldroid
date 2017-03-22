//
// Created by 神经元002 on 2017/3/22.
//

#include <jni.h>
#include <EltwiseLayer.h>

extern "C"{
JNIEXPORT void JNICALL
Java_com_compilesense_liuyi_mcldroid_NativeTest_testEltwiseLayer(
        JNIEnv *env, jobject instance
){
    std::vector<float> cf(2);
    cf[0] = 1;
    cf[1] = 2;
    EltwiseLayer eltwiseLayer("testLayer", EltwiseLayer::sum, cf);

    float * inputDataA = new float[1000];
    float * inputDataB = new float[1000];

    for (int i = 0; i< 1000; ++i){
        inputDataA[i] = i;
        inputDataB[i] = i;
    }

    std::vector<MultiDimensionData<float>> inputArray(2);
    inputArray[0].setData(inputDataA,10,10,1,10);
    inputArray[1].setData(inputDataB,10,10,1,10);
//    MultiDimensionData<float> inputA(inputDataA,10,10,1,10);
//    MultiDimensionData<float> inputB(inputDataB,10,10,1,10);
    MultiDimensionData<float> output;

    eltwiseLayer.compute(inputArray.data(), &output);

    for (int i = 0; i< 10; ++i){
        LOGD("output[%i]:%f",i,output.data_ptr[i]);
    }
}
}