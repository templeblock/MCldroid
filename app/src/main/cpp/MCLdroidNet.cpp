//
// Created by 神经元002 on 2017/3/23.
//

#include <BitmapProcess.h>
#include "MCLdroidNet.h"

MultiDimensionData<float> inputBitmapMap;
Net net;

void Net::forward(MultiDimensionData<float> *input, MultiDimensionData<float> *output){
    if (layers == NULL || layerSize <= 0){
        return;
    }
    if (input == NULL || output == NULL){
        return;
    }

    double st;

    BaseLayer* layerPtr = getLayerInOrder();
    while (layerPtr != NULL){
//        LOGD("---------------------compute :%s------------------------",layerPtr->getName().data());
//        LOGD("inputSize:%lu", input->totalSize());
//
//        if (layerIndex==2){
//            LOGD("layerIndex==2");
//        }

        st = now_ms();
        if (output->data_ptr != NULL){
            input->releaseData();
            input->setData(output->data_ptr, output->shape);
            output->setData(NULL,1,1,1,1);//不再保存上一次输出的信息
        }
        layerPtr->compute(input, output);
//        LOGD("outputSize:%lu, n*c*h*w : %lu*%lu*%lu*%lu",
//             input->totalSize(),output->get_n(),output->get_c(),output->get_h(),output->get_w());
//        LOGD("convTime:%f", now_ms() - st);
//        LOGD("---------------------compute end------------------------");

        layerPtr = getLayerInOrder();
    }
}

void meanInput(MultiDimensionData<float> *input, MultiDimensionData<float> *mean){
    size_t cSize = input->get_h()*input->get_w();
    size_t nSize = input->get_c()*cSize;
    size_t h = input->get_h();
    size_t w = input->get_w();

    size_t h_mean = mean->get_h();
    size_t w_mean = mean->get_w();

    float *dataPtr = input->data_ptr;
    float *meanPtr = mean->data_ptr;
    for (int i = 0; i < input->get_n(); i++){
        for (int chanel = 0; chanel<3; chanel++){

            for (int row = 0; row < h; row++){
                for (int column = 0; column < w; column++){
                    float meanData = meanPtr[row*w_mean + column];
                    dataPtr[i*nSize + chanel*cSize + row*w + column] -= meanData;
                }
            }

        }
    }
};

extern "C" {

JNIEXPORT void JNICALL
Java_com_compilesense_liuyi_mcldroid_mcldroid_MCLdroidNet_setInputMean(JNIEnv *env,
                                                                       jobject instance,
                                                                       jfloatArray mean_,
                                                                       jintArray meanShape_) {


    int weightArrayLength = 0;
    float * mean = jFloatArray2prtFast(env, mean_, &weightArrayLength);

    int meanShapeLength = 0;
    int* meanShape = jIntArray2prt(env, meanShape_, &meanShapeLength);
    std::vector<size_t> meanShapeV(4);
//    for (size_t i = 0; i < meanShapeLength; i++){
//        meanShapeV[i] = (size_t) meanShape[i];
//    }
    //TODO meanShape 不是4维的时候出错,这样整个 MultiDimensionData 的维度设计是不好的!
    meanShapeV[0] = (size_t) meanShape[0];
    meanShapeV[1] = (size_t) meanShape[1];
    meanShapeV[2] = (size_t) meanShape[2];
    meanShapeV[3] = (size_t) meanShape[3];

    MultiDimensionData<float> meanMDD(mean, meanShapeV);

    meanInput(&inputBitmapMap, &meanMDD);
    jFloatArrayRelease(env, mean_, mean);
    meanMDD.releaseData();
}


JNIEXPORT void JNICALL
Java_com_compilesense_liuyi_mcldroid_mcldroid_MCLdroidNet_compute(JNIEnv *env, jobject instance) {
    if (inputBitmapMap.data_ptr == NULL){
        return;
    }
    MultiDimensionData<float> outPut;
    net.forward(&inputBitmapMap, &outPut);

    LOGD("outSiez:%lu", outPut.totalSize());
}


JNIEXPORT void JNICALL
Java_com_compilesense_liuyi_mcldroid_mcldroid_MCLdroidNet_addLayer(JNIEnv *env, jobject instance,
                                                                   jlong nativeObject) {
//    net.addLayer((BaseLayer *) nativeObject);
}

JNIEXPORT void JNICALL
Java_com_compilesense_liuyi_mcldroid_mcldroid_MCLdroidNet_setupNet(JNIEnv *env, jobject instance,
                                                                   jlongArray nativeObjectArray_) {
    int layerArrayLength = 0;
    long long * layers = jLongArray2prt(env, nativeObjectArray_, &layerArrayLength);
    net.setUpNet(layers, layerArrayLength);
}


JNIEXPORT void JNICALL
Java_com_compilesense_liuyi_mcldroid_mcldroid_MCLdroidNet_bitmapProcess(JNIEnv *env, jclass type,jobject bitmap) {
    AndroidBitmapInfo  info;
    void*              pixels;//数据
    int                ret;

    if ((ret = AndroidBitmap_getInfo(env, bitmap, &info)) < 0) {
        LOGE("AndroidBitmap_getInfo() failed ! error=%d", ret);
        return;
    }
    logBitmapInfo(info);
    if ((ret = AndroidBitmap_lockPixels(env, bitmap, &pixels)) < 0) {
        LOGE("AndroidBitmap_lockPixels() failed ! error=%d", ret);
    }
    pixels2MultiDimensionDataNeon(&info ,pixels, &inputBitmapMap);
    AndroidBitmap_unlockPixels(env, bitmap);
}

JNIEXPORT void JNICALL
Java_com_compilesense_liuyi_mcldroid_mcldroid_MCLdroidNet_getBitmapProcessed(JNIEnv *env,
                                                                             jobject instance,
                                                                             jobject bitmap) {

    AndroidBitmapInfo  info;
    void*              pixels;//数据
    int                ret;

    if ((ret = AndroidBitmap_getInfo(env, bitmap, &info)) < 0) {
        LOGE("AndroidBitmap_getInfo() failed ! error=%d", ret);
        return;
    }

    if ((ret = AndroidBitmap_lockPixels(env, bitmap, &pixels)) < 0) {
        LOGE("AndroidBitmap_lockPixels() failed ! error=%d", ret);
    }

    multiDimensionData2Pixels(&info, pixels, &inputBitmapMap);
    AndroidBitmap_unlockPixels(env, bitmap);

}
}