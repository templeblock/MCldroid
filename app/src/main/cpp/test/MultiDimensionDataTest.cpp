//
// Created by 神经元002 on 2017/3/21.
//
#include <jni.h>
#include <MultiDimensionData.h>
#include <utils.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL
Java_com_compilesense_liuyi_mcldroid_NativeTest_multiDimensionDataTest(JNIEnv *env, jclass type) {
    //int
    int * dataPtr1 = new int[100];
    for (int i = 0; i<100; i++){
        dataPtr1[i] =  (-rand()%51+25);
    }
    MultiDimensionData<int> mdData1(dataPtr1, 1, 1, 10, 10);
    LOGD("mdData1 data[0]:%i, total size:%lu",mdData1.data_ptr[0],mdData1.totalSize());
    float * dataPtr2 = new float[1000];
    for (int i = 0; i<1000; i++){
        dataPtr2[i] =  (-rand()%51+25);
    }
    std::vector<size_t> shape(4);
    shape[0] = 10;
    shape[1] = 10;
    shape[2] = 10;
    shape[3] = 1;
    MultiDimensionData<float> mdData2(dataPtr2, shape);
    LOGD("mdData2 data[0]:%f, total size:%lu",mdData2.data_ptr[0],mdData2.totalSize());

}

#ifdef __cplusplus
}
#endif