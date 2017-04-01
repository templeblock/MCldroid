//
// Created by 神经元002 on 2017/3/30.
//

#include "logUtil.h"
#include <string>
#include <sstream>
#include <fcntl.h>
#include <ctime>
#include <jni.h>
#include <MultiDimensionData.h>
#include <android/bitmap.h>
#include <BitmapProcess.h>

std::string logPath = "/storage/emulated/0/MCLdroidTestLog/";

void int2str(const int &int_temp,std::string &string_temp)
{
    std::stringstream stream;
    stream<<int_temp;
    string_temp=stream.str();   //此处也可以用 stream>>string_temp
}

void long2str(const long &int_temp,std::string &string_temp)
{
    std::stringstream stream;
    stream<<int_temp;
    string_temp=stream.str();   //此处也可以用 stream>>string_temp
}

const char* getTime(){
    // 基于当前系统的当前日期/时间
    time_t now = time(0);

    // 把 now 转换为字符串形式
    const char* dt = ctime(&now);
    return dt;
}

void initLogUtil(std::string logPath_){
    logPath = logPath_;
}

void logString(std::string info){
    int fd = open(logPath.data(),O_RDWR|O_CREAT|O_APPEND);
    std::string s1 = "log time:";
    s1.append(getTime());
    write(fd, s1.data(), s1.size());
    write(fd, info.data(), info.size());
    close(fd);
}

void logMDData(MultiDimensionData<float> *data,const char* dataTitleInfo){

    float * dataPtr = data->data_ptr;

    std::string file = logPath;
    file.append(dataTitleInfo);

    int fd = open(file.data(),O_RDWR|O_CREAT|O_TRUNC);//写,创建,覆盖.
    std::string tempS;
    std::stringstream stream;

    std::string s1 = "log time: ";
    s1.append(getTime()).append("\n");
    write(fd, s1.data(), s1.size());

    std::string s3 = "data shape: ";

    const size_t nSize = data->get_n();
    const size_t cSize = data->get_c();
    const size_t hSize = data->get_h();
    const size_t wSize = data->get_w();

    const size_t h_data_size = wSize;
    const size_t c_data_size = h_data_size * hSize;
    const size_t n_data_size = c_data_size * cSize;


    stream<<nSize<<"*"<<cSize<<"*"<<hSize<<"*"<<wSize<<std::endl;

    s3.append(stream.str().append("\n"));
    stream.str("");
    stream.clear();
    write(fd, s3.data(), s3.size());


    std::string temp = "data :\n";
    write(fd, temp.data(), temp.size());

    for (int indexN = 0; indexN < nSize; ++indexN){
        for (int indexC = 0; indexC < cSize; ++indexC) {
            for (int indexH = 0; indexH < hSize; ++indexH) {
                size_t baseIndex = indexN*(n_data_size) + indexC*(c_data_size) + indexH*(h_data_size);

                //先打印这一排的范围
                stream.str("");
                stream.clear();
                stream<<"["<<indexN<<"]["<<indexC<<"]["<<indexH<<"][0~"<<(wSize-1)<<"]";
                size_t be = baseIndex + wSize -1;
                stream<<" / "<<baseIndex <<"~"<<be<<std::endl;
                tempS = stream.str();
                stream.str("");
                stream.clear();
                write(fd, tempS.data(), tempS.size());

                //打印数据
                for (int indexW = 0; indexW < wSize; ++indexW){
                    stream<<dataPtr[baseIndex + indexW]<<"\t";
                }

                stream<<std::endl;
                tempS = stream.str();
                write(fd, tempS.data(), tempS.size());
                stream.str("");
                stream.clear();
            }
        }
    }

    close(fd);
}

extern "C"{

JNIEXPORT void JNICALL
Java_com_compilesense_liuyi_mcldroid_NativeTest_logSystemTest(JNIEnv *env, jclass type,
        jstring path_, jobject bitmap) {
    const char *path = env->GetStringUTFChars(path_, 0);
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
    AndroidBitmap_unlockPixels(env, bitmap);

    MultiDimensionData<float> MDData;
    pixels2MultiDimensionData(&info, pixels, &MDData);
    logMDData(&MDData,"bitmap-input");

    MDData.releaseData();
    pixels2MultiDimensionDataNeon(&info, pixels, &MDData);
    logMDData(&MDData,"bitmap-input_neon");

    env->ReleaseStringUTFChars(path_, path);
}

}
