//
// Created by 神经元002 on 2017/3/21.
//

#include <jni.h>
#include <SoftmaxLayer.h>
#include <stdlib.h>
#include <utils.h>


extern "C" {


JNIEXPORT void JNICALL
Java_com_compilesense_liuyi_mcldroid_NativeTest_sofymaxLayerTest(JNIEnv *env, jclass type) {
    //二位数据
    SoftmaxLayer softmaxLayer("testSoftmaxLayer");
    float * dataPtr1 = new float[100];
    for (int i = 0; i<100; i++){
        dataPtr1[i] =  (-rand()%51+25);
    }
    std::vector<size_t> shape(4);
    shape[0] = 10;
    shape[1] = 10;
    shape[2] = 1;
    shape[3] = 1;
    MultiDimensionData<float> input(dataPtr1, shape);
    MultiDimensionData<float> outputPtr;
    softmaxLayer.compute(&input, &outputPtr);
    LOGD("output size:%lu",outputPtr.totalSize());
}


}