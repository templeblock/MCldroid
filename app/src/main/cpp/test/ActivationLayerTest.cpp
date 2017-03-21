//
// Created by 神经元002 on 2017/3/21.
//

#include <jni.h>
#include <ActivationLayerTest.h>
#include <stdlib.h>
#include <math.h>

extern "C"{
JNIEXPORT void JNICALL
Java_com_compilesense_liuyi_mcldroid_NativeTest_testComputeRelu(
        JNIEnv *env, jobject instance
){


    ActivationLayer activationLayer("activationLayer1",ActivationLayer::ReLU);
    float * data = new float[1000];
    for(int i = 0; i< 1000; i++) {
        data[i] = (-rand()%51+25);
    }
    for(int i = 0; i<20; i++){
        LOGD("input[%i]=%f",i,data[i]);
    }

    std::vector<size_t > shape(1);
    shape[0] = 1000;
    MultiDimensionData<float> inputData(data, shape);

    double st = now_ms();
    activationLayer.compute(&inputData);
    LOGD("time1---------------------------------------------: %f",now_ms() - st);
    for(int i = 0; i<20; i++){
        LOGD("result[%i]=%f",i,data[i]);
    }

    float * data2 = new float[1000];
    for(int i = 0; i< 1000; i++) {
        data[i] = (-rand()%51+25);
    }
    for(int i = 0; i<20; i++){
        LOGD("input[%i]=%f",i,data[i]);
    }
    st = now_ms();
    for(int i = 0; i< 1000; i++) {
        data[i]=(data[i]>0)?data[i]:0;
    }
    LOGD("time2---------------------------------------------: %f",now_ms() - st);
    for(int i = 0; i<20; i++){
        LOGD("result2[%i]=%f",i,data[i]);
    }

    delete[] data2;
}

JNIEXPORT void JNICALL
Java_com_compilesense_liuyi_mcldroid_NativeTest_testComputePrelu(
        JNIEnv *env, jobject instance
){
    ActivationLayer activationLayer("activationLayer1",ActivationLayer::PReLU);
    float * data = new float[1000];
    for(int i = 0; i< 1000; i++) {
        data[i] = (-rand()%51+25);
    }
    for(int i = 0; i<20; i++){
        LOGD("input[%i]=%f",i,data[i]);
    }

    std::vector<size_t > shape(1);
    shape[0] = 1000;
    MultiDimensionData<float> inputData(data, shape);

    double st = now_ms();
    activationLayer.compute(&inputData);
    LOGD("time1---------------------------------------------: %f",now_ms() - st);
    for(int i = 0; i<20; i++){
        LOGD("result[%i]=%f",i,data[i]);
    }

    float * data2 = new float[1000];
    for(int i = 0; i< 1000; i++) {
        data[i] = (-rand()%51+25);
    }
    for(int i = 0; i<20; i++){
        LOGD("input[%i]=%f",i,data[i]);
    }
    st = now_ms();
    for(int i = 0; i< 1000; i++) {
        data[i]=(data[i]>0)?data[i]:0.25f*data[i];
    }
    LOGD("time2---------------------------------------------: %f",now_ms() - st);
    for(int i = 0; i<20; i++){
        LOGD("result2[%i]=%f",i,data[i]);
    }
    delete[] data2;
}

JNIEXPORT void JNICALL
Java_com_compilesense_liuyi_mcldroid_NativeTest_testComputeTanh(
        JNIEnv *env, jobject instance
){


    ActivationLayer activationLayer("activationLayer1",ActivationLayer::TanH);
    float * data = new float[1000];
    float * data2 = new float[1000];
    for(int i = 0; i< 1000; i++) {
        data2[i] = data[i] = (-rand()%51+25);
    }
    for(int i = 0; i<20; i++){
        LOGD("input[%i]=%f",i,data[i]);
    }

    std::vector<size_t > shape(1);
    shape[0] = 1000;
    MultiDimensionData<float> inputData(data, shape);

    double st = now_ms();
    activationLayer.compute(&inputData);
    LOGD("time1---------------------------------------------: %f",now_ms() - st);
    for(int i = 0; i<20; i++){
        LOGD("result[%i]=%f",i,data[i]);
    }
    st = now_ms();
    for(int i = 0; i< 1000; i++) {
        data[i]=tanhf(data[i]);
    }
    LOGD("time2---------------------------------------------: %f",now_ms() - st);
    for(int i = 0; i<20; i++){
        LOGD("result2[%i]=%f",i,data[i]);
    }
    delete[] data2;
}


}