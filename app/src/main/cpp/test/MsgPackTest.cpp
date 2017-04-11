//
// Created by 神经元002 on 2017/4/10.
//

#include <jni.h>
#include <fstream>
#include <iostream>
#include <msgpack.hpp>
#include <sstream>

using std::ios;
using std::ifstream;

class myclass {
private:
    std::vector<float> p;
//    std::vector<float> bias;
public:
    MSGPACK_DEFINE( p);
};

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wfor-loop-analysis"
void load_from_file(const char* binary_file) {
    ifstream binaryFstream(binary_file,ios::in|ios::binary|ios::ate);
    size_t size = (size_t) binaryFstream.tellg();
    char* buff = new char[size];
    binaryFstream.seekg(0, ios::beg);
    binaryFstream.read(buff, size);
    binaryFstream.close();

    std::size_t off = 0;
    std::vector<std::vector<float> *> list;

    while (off != size) {
        msgpack::unpacked result;
        unpack(result, buff, size, off);
        msgpack::object obj(result.get());
        // Use obj
        std::vector<float> *t = new std::vector<float>();
        obj.convert(*t);
        list.push_back(t);
    }

    size_t s =  list.size();

    for (int i = 0; i< s; i++){
        delete list[i];
    }
    delete[] buff;
}
#pragma clang diagnostic pop

extern "C"{



JNIEXPORT void JNICALL
Java_com_compilesense_liuyi_mcldroid_NativeTest_testMsgPack(JNIEnv *env, jclass type,
        jstring filePath_) {
    const char *filePath = env->GetStringUTFChars(filePath_, 0);


    load_from_file("/sdcard/cnn_model/Data_Cifar10/model_param_ip1.msg");

    env->ReleaseStringUTFChars(filePath_, filePath);
}

}