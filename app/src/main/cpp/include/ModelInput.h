//
// Created by 神经元002 on 2017/4/6.
//

#ifndef MCLDROID_MODELINPUT_H
#define MCLDROID_MODELINPUT_H

#include <MultiDimensionData.h>
#include <string>

void loadFullyConnectKernel(std::string filePath, MultiDimensionData<float> *MDDweight, MultiDimensionData<float> *MDDbias);

void loadConvolutionKernel(std::string filePath, MultiDimensionData<float> *weightMDD, MultiDimensionData<float> *biasMDD);

#endif //MCLDROID_MODELINPUT_H
