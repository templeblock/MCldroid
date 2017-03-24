//
// Created by 神经元002 on 2017/3/3.
//

#include <string>
#include <jni.h>
#include "utils.h"
#include "ConvolutionLayer.h"
#include <math.h>
#include <ComputeTool.h>




void ConvolutionLayer::setKernel(float *weight, std::vector<size_t> weightShape,
                                  float *bias, int biasSize){

    this->weight.setData(weight, weightShape);
    this->bias.setData(bias, 1,1,1,(size_t)biasSize);
    this->paramHadLoad = true;
}

void ConvolutionLayer::setKernel(
        bool shouldRelease,
        float *weight, std::vector<size_t> weightShape,
        float *bias, int biasSize){
    this->kernelSholdRelease = shouldRelease;
    this->weight.setData(weight, weightShape, shouldRelease);
    this->bias.setData(bias, 1,1,1,(size_t)biasSize, shouldRelease);
    this->paramHadLoad = true;
}

void ConvolutionLayer::releaseKernel() {
    if (this->paramHadLoad && kernelSholdRelease){
        LOGD("releaseKernel weight");
        weight.releaseData();
        LOGD("releaseKernel bias");
        bias.releaseData();
    }
    this->paramHadLoad = false;
}

/**
 * group 为1,
 */

void ConvolutionLayer::compute(MultiDimensionData<float> *input, MultiDimensionData<float> *output){
    size_t pad_ = params.pad;
    size_t stride_h = params.stride_h;
    size_t stride_w = params.stride_w;

    //input
    size_t n_i = input->shape[0];//数量
    size_t c_i = input->shape[1];//通道
    size_t h_i = input->shape[2];//高度
    size_t w_i = input->shape[3];//高度

    //kernel
    size_t n_k = weight.shape[0];//数量
    size_t c_k = weight.shape[1];//通道
    size_t h_k = weight.shape[2];//高度
    size_t w_k = weight.shape[3];//宽度

    //output
    size_t n_o = n_i;
    size_t h_o = (size_t) (ceil((h_i + 2 * pad_ - h_k) / ((float) (stride_h))) + 1);
    size_t w_o = (size_t) (ceil((w_i + 2 * pad_ - w_k) / ((float) (stride_w))) + 1);
    size_t c_o = n_k;

    float *outPtr = new float[n_o * c_o * h_o * w_o];
    output->setData(outPtr, n_o, c_o, h_o, w_o);

    //利用 stride 对 input 进行处理。
    if (stride_h > 1){
        h_i = h_i/stride_h;
    }
    if (stride_w > 1){
        w_i = w_i/stride_w;
    }
    convNnpack(output->data_ptr, input->data_ptr, weight.data_ptr, bias.data_ptr,
               n_i, c_i, h_i, w_i,
               n_o, c_o, h_o, w_o,
               pad_,stride_h,h_k, w_k);

}

void ConvolutionLayer::computeOutputShape(MultiDimensionData<float> *input, MultiDimensionData<float> * output){
    size_t pad_ = params.pad;
    size_t stride_h = params.stride_h;
    size_t stride_w = params.stride_w;

    //input
    size_t n_i = input->shape[0];//数量
    size_t c_i = input->shape[1];//通道
    size_t h_i = input->shape[2];//高度
    size_t w_i = input->shape[3];//高度

    //kernel
    size_t n_k = weight.shape[0];//数量
    size_t c_k = weight.shape[1];//通道
    size_t h_k = weight.shape[2];//高度
    size_t w_k = weight.shape[3];//宽度

    //output
    size_t n_o = n_i;
    size_t h_o = (size_t) (ceil((h_i + 2 * pad_ - h_k) / ((float) (stride_h))) + 1);
    size_t w_o = (size_t) (ceil((w_i + 2 * pad_ - w_k) / ((float) (stride_w))) + 1);
    size_t c_o = n_k;
}

#ifdef __cplusplus
extern "C" {
#endif

//构造一个 conv layer 实例
JNIEXPORT jlong JNICALL
Java_com_compilesense_liuyi_mcldroid_mcldroid_ConvolutionLayer_createConvolutionLayer(
        JNIEnv *env, jobject instance,
        jstring name_,
        jintArray stride_,
        jintArray pad_,
        jint group_,
        jboolean nonLinear_
){
    //name
    const char * name = env->GetStringUTFChars(name_,NULL);
    //stride
    int strideSize = 0;
    int * stride = jIntArray2prt(env, stride_, &strideSize);
    //pad
    int padSize = 0;
    int * pad = jIntArray2prt(env, pad_, &padSize);
    //group
    int group = group_;
    //nonLinear
    bool nonLinear = nonLinear_;
    ConvolutionLayer * convLayerPrt =  new ConvolutionLayer(
            name,
            (unsigned int)stride[0],
            (unsigned int)pad[0],
            group,
            nonLinear
    );
    env->ReleaseStringUTFChars(name_,name);
    return (jlong) convLayerPrt;
}

//析构一个实例
JNIEXPORT void JNICALL
Java_com_compilesense_liuyi_mcldroid_mcldroid_ConvolutionLayer_deleteConvolutionLayer(
        JNIEnv *env, jobject instance,
        jlong objectPrt_
){
    ConvolutionLayer *objectPrt = (ConvolutionLayer *) objectPrt_;
    objectPrt->releaseKernel();
    delete(objectPrt);
}

//设置参数
JNIEXPORT void JNICALL
Java_com_compilesense_liuyi_mcldroid_mcldroid_ConvolutionLayer_setKernel(
        JNIEnv *env, jobject instance,
        jlong objectPrt_,
        jfloatArray weight_,
        jintArray weightShape_,//因为 int 是用 GetIntArrayRegion, 来访问的不需要专门释放 JVM。
        jfloatArray bias_
){
    ConvolutionLayer *objectPrt = (ConvolutionLayer *) objectPrt_;

    int weightShapeLength = 0;
    int* weightShape = jIntArray2prt(env, weightShape_, &weightShapeLength);

    int weightArrayLength = 0;
    float * weight = jFloatArray2prtFast(env, weight_, &weightArrayLength);

    int biasArrayLength = 0;
    float* bias = jFloatArray2prtFast(env, bias_, &biasArrayLength);
//
//    objectPrt->setKernel(weight, weightArrayLength,
//                         weightShape, weightShapeLength,
//                         bias, biasArrayLength);

    jFloatArrayRelease(env, weight_, weight);
    jFloatArrayRelease(env, bias_, bias);
}


#ifdef __cplusplus
}
#endif