//
// Created by 神经元002 on 2017/3/23.
//

#include <BitmapProcess.h>
#include "MCLdroidNet.h"
MultiDimensionData<float> inputBitmapMap;
void Net::forward(MultiDimensionData<float> *input, MultiDimensionData<float> *output){
    if (layers.empty()){
        return;
    }
    if (input == NULL || output == NULL){
        return;
    }
    std::vector<BaseLayer*>::iterator layerPtr = layers.begin();
    while (layerPtr != layers.end()){
        layerPtr.operator*()->compute(input, output);
        layerPtr.operator++();
        input = output;
    }
}

extern "C" {
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

}