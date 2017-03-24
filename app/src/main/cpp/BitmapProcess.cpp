//
// Created by 神经元002 on 2017/3/23.
//

#include "BitmapProcess.h"
#include <arm_neon.h>

void logBitmapInfo(AndroidBitmapInfo info){
    switch (info.format){
        case ANDROID_BITMAP_FORMAT_NONE:
            LOGD("bitmap format: ANDROID_BITMAP_FORMAT_NONE");
            break;
        case ANDROID_BITMAP_FORMAT_RGB_565:
            LOGD("bitmap format: ANDROID_BITMAP_FORMAT_RGB_565");
            break;
        case ANDROID_BITMAP_FORMAT_RGBA_4444:
            LOGD("bitmap format: ANDROID_BITMAP_FORMAT_RGBA_4444");
            break;
        case ANDROID_BITMAP_FORMAT_RGBA_8888:
            LOGD("bitmap format: ANDROID_BITMAP_FORMAT_RGBA_8888");
            break;
        default:
            LOGE("Error bitmap format");
    }

    LOGD("bitmap height*width: %u*%u", info.height, info.width);
    LOGD("bitmap stride: %i, flags:*%u", info.stride, info.flags);
}

void pixels2MultiDimensionData(AndroidBitmapInfo * info, void *pixels, MultiDimensionData<float> *data){
    if (info->format == ANDROID_BITMAP_FORMAT_RGBA_8888){
        float * dataPtr = new float[1*4*(info->width)*(info->height)];
        data->setData(dataPtr,1,4,info->height,info->width);

        unsigned long index = 0;
        unsigned long size = info->height * info->width;

        uint32_t * pixelsData = (uint32_t *)pixels;
        uint32_t pixel = 0;
        int r,g,b,a;
        for (index = 0; index < size ; ++index) {
            pixel = pixelsData[index];
            r = pixel & 0x000000FF;
            g = (pixel & 0x0000FF00)>>8;
            b = (pixel & 0x00FF0000)>>16;
            a = (pixel & 0xFF000000)>>24;
            dataPtr[size*0 + index] = (float) r;
            dataPtr[size*1 + index] = (float) g;
            dataPtr[size*2 + index] = (float) b;
            dataPtr[size*3 + index] = (float) a;
        }
    }
}

//因为在NEON中8位—>32位的类型转换比较繁琐,在这里效率提升比不太大,读取一个 600*600 的图片耗时 16ms左右。
//TODO 想办法优化图片读取速度, 或者改变 MultiDimensionData 的存储结构, 如果采用和 pixels 一样的结构,应该会带来一定的提升。
void pixels2MultiDimensionDataNeon(AndroidBitmapInfo * info, void *pixels, MultiDimensionData<float> *data){
    if (info->format == ANDROID_BITMAP_FORMAT_RGBA_8888){
        float * dataPtr = new float[1*4*(info->width)*(info->height)];
        data->setData(dataPtr,1,4,info->height,info->width);

        unsigned long index = 0;
        unsigned long pixelSize = info->height * info->width;//每贞像素大小
        unsigned long dataSize = pixelSize * 4;//每贞数据大小
        uint8_t * pixelsChanelData = (uint8_t *)pixels;
        for (index = 0; index < dataSize ; index += 8*4) {//每次处理8个像素 8*4=32
            uint8x8x4_t rgba = vld4_u8(pixelsChanelData + index);
            for (int indexChanel = 0; indexChanel < 4; indexChanel++){
                uint8x8_t chanel_temp = rgba.val[indexChanel];
                uint16x8_t temp = vmovl_u8(chanel_temp);
                uint16x4_t temp_h =  vget_high_u16(temp);
                uint16x4_t temp_l =  vget_low_u16(temp);
                uint32x4_t temp_h_long = vmovl_u16(temp_h);
                uint32x4_t temp_l_long = vmovl_u16(temp_l);
                float32x4_t temp_h_long_f = vcvtq_f32_u32(temp_h_long);
                float32x4_t temp_l_long_f = vcvtq_f32_u32(temp_l_long);
                //indexPixel = index /4;
                vst1q_f32(dataPtr + (indexChanel * pixelSize) + (index / 4), temp_l_long_f);
                vst1q_f32(dataPtr + (indexChanel * pixelSize) + (index / 4) + 4 , temp_h_long_f);
            }
        }
    }
}

void multiDimensionData2Pixels(AndroidBitmapInfo * info, void *pixels, MultiDimensionData<float> *data){
    if (data->get_c() == 4){
        unsigned long index = 0;
        unsigned long size = data->get_h() * data->get_w();
        float * dataPtr = data->data_ptr;
        if (info->width*info->height != size){
            return;
        }
        uint32_t * pixelsData = (uint32_t *)pixels;
        uint32_t pixel = 0;
        uint32_t r,g,b,a;
        for (index = 0; index < size ; ++index) {
            r = (uint32_t) dataPtr[size*0 + index];
            g = (uint32_t) dataPtr[size*1 + index];
            b = (uint32_t) dataPtr[size*2 + index];
            a = (uint32_t) dataPtr[size*3 + index];
            pixel = r | (g << 8) | (b << 16) |(a << 24);
            pixelsData[index] = pixel;
        }

    }
}

void gray(MultiDimensionData<float> *data){
    size_t size = data->get_h()*data->get_w();
    float * dataPtr = data->data_ptr;
    float temp = 0;
    float r,g,b,a;
    for (int i = 0; i < size; i++){
        temp += dataPtr[size*0 + i];
        temp += dataPtr[size*1 + i];
        temp += dataPtr[size*2 + i];
//        temp += dataPtr[size*3 + i];
        temp /= 3;
        dataPtr[size*0 + i] = temp;
        dataPtr[size*1 + i] = temp;
        dataPtr[size*2 + i] = temp;
//        dataPtr[size*3 + i] = temp;
        temp = 0;
    }
}


extern "C" {
JNIEXPORT void JNICALL
Java_com_compilesense_liuyi_mcldroid_NativeTest_bitmapProcess(JNIEnv *env, jclass type,jobject bitmap) {
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
    MultiDimensionData<float> multiDimensionData;

    double st =now_ms();
//    pixels2MultiDimensionData(&info, pixels, &multiDimensionData);

    pixels2MultiDimensionDataNeon(&info, pixels, &multiDimensionData);

    LOGD("time: %f",now_ms() - st);
//    gray(&multiDimensionData);

    multiDimensionData2Pixels(&info, pixels, &multiDimensionData);
    AndroidBitmap_unlockPixels(env, bitmap);


}

}