//
// Created by 神经元002 on 2017/3/7.
//

#include <FullyConnectedLayer.h>
#include <ComputeTool.h>
#include <utils.h>


FullyConnectedLayer::~FullyConnectedLayer() {
    LOGD("ConvolutionLayer delete! name: %s",this->name.data());
}
void FullyConnectedLayer::setKernel(float *weight, int weightSize, float *bias, int biasSize) {
    this->weight.setData(weight,1,1,1,(size_t)weightSize);
    this->bias.setData(bias,1,1,1,(size_t)biasSize);
    this->paramHadLoad = true;
    LOGD("ConvolutionLayer setKernel! name: %s",this->name.data());
}

void FullyConnectedLayer::releaseKernel() {
    this->paramHadLoad = false;
    LOGD("ConvolutionLayer releaseParam! name: %s",this->name.data());
}

void FullyConnectedLayer::compute(MultiDimensionData<float> *input, MultiDimensionData<float > *output) {

    //input
    size_t n_i = input->shape[0];//数量
    size_t c_i = input->shape[1];//通道
    size_t h_i = input->shape[2];//高度
    size_t w_i = input->shape[3];//高度
    size_t h_w = bias.totalSize();
    size_t w_w = weight.totalSize() / h_w;
    size_t n_o = n_i;
    size_t c_o = h_w;

    float * outPtr = new float[n_i*c_o];
    std::vector<size_t > outputShape(2);
    outputShape[0] = n_i;
    outputShape[1] = c_o;
    output->setData(outPtr, outputShape);

    size_t inputBathSize = n_i;
    size_t input_channels = c_i;
    size_t output_channels = c_o;

    fullyConnectNnpack(output->data_ptr,
                       input->data_ptr,
                       weight.data_ptr,
                       inputBathSize,
                       input_channels,
                       output_channels);

}

#ifdef __cplusplus
extern "C" {
#endif
//构造一个 conv layer 实例
JNIEXPORT jlong JNICALL
Java_com_compilesense_liuyi_mcldroid_caffecnn_FullyConnectedLayer_createNativeObject(
        JNIEnv *env, jobject instance,
        jstring name_,
        jboolean nonLinear_
){
    //name
    const char * name = env->GetStringUTFChars(name_,NULL);

    //nonLinear
    bool nonLinear = nonLinear_;
    FullyConnectedLayer * fcLayerPrt = new FullyConnectedLayer(
            name,
            nonLinear
    );
    env->ReleaseStringUTFChars(name_,name);
    return (jlong) fcLayerPrt;
}

JNIEXPORT void JNICALL
Java_com_compilesense_liuyi_mcldroid_caffecnn_FullyConnectedLayer_deleteNativeObject(
        JNIEnv *env, jobject instance,
        jlong nativeObjPrt_
){
    FullyConnectedLayer * prt = (FullyConnectedLayer *) nativeObjPrt_;
    prt->releaseKernel();
    delete prt;
}

JNIEXPORT void JNICALL
Java_com_compilesense_liuyi_mcldroid_caffecnn_FullyConnectedLayer_setParam(
        JNIEnv *env, jobject instance,
        jlong nativeObjPrt_,
        jfloatArray weight_,
        jfloatArray bias_
){
    int weightArrayLength = 0;
    float* weight = jFloatArray2prtFast(env, weight_, &weightArrayLength);

    int biasArrayLength = 0;
    float * bias = jFloatArray2prtFast(env, bias_, &biasArrayLength);
    FullyConnectedLayer * prt = (FullyConnectedLayer *) nativeObjPrt_;
    prt->setKernel(weight, weightArrayLength,
                   bias, biasArrayLength);
    jFloatArrayRelease(env, weight_, weight);
    jFloatArrayRelease(env, bias_, bias);
}

#ifdef __cplusplus
}
#endif
