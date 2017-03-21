//
// Created by 神经元002 on 2017/3/21.
//

#include <jni.h>
#include <utils.h>
#include <ConvolutionLayer.h>

ConvolutionLayer *convolutionLayer;
MultiDimensionData<float> inputData;
MultiDimensionData<float> outputData;
extern "C" {


//计算测试 输入数据不应该是每个 layer 从 java 传进来,效率太低
JNIEXPORT void JNICALL
Java_com_compilesense_liuyi_mcldroid_NativeTest_testConvolutionLayerCompute(
        JNIEnv *env, jobject instance
){
    LOGD("init test");
    unsigned int stride = 1;
    unsigned int pad = 0;

    ConvolutionLayer::Params params(pad,stride);
    convolutionLayer = new ConvolutionLayer("testConvolutionLayer", params, false);

    float kernel[3*3] = {1,2,3,
                         4,5,6,
                         7,8,9};
    float bias[1] = {0};
    std::vector<size_t> kernelShapeV(4);
    kernelShapeV[0] = 1;
    kernelShapeV[1] = 1;
    kernelShapeV[2] = 3;
    kernelShapeV[3] = 3;

    convolutionLayer->setKernel(false, kernel, kernelShapeV, bias, 1);

    float * input = new float[25];
    float arrayIn[25] =
            {1,2,3,4,5,
             1,2,3,4,5,
             1,2,3,4,5,
             1,2,3,4,5,
             1,2,3,4,5,};

    for (int i = 0; i< 25; i++){
        input[i] = arrayIn[i];
    }
    std::vector<size_t> inputShape(4);
    inputShape[0] = 1;
    inputShape[1] = 1;
    inputShape[2] = 5;
    inputShape[3] = 5;

    inputData.setData(input, inputShape);

    LOGD("compute test");
    convolutionLayer->compute(&inputData, &outputData);
    for (int i = 0; i < 9; ++i) {
        LOGD("result:%f",outputData.data_ptr[i]);
    }

    convolutionLayer->releaseKernel();
    delete convolutionLayer;
}

}
