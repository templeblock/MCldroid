//
// Created by 神经元002 on 2017/3/23.
//

#ifndef MCLDROID_NET_H
#define MCLDROID_NET_H


#include <vector>
#include <BaseLayer.h>

class Net {
public:
    Net():layers(){};
    void setUpNet( std::vector<BaseLayer*> layers){
        if (this->layers.capacity() < layers.size()){
            this->layers.reserve(layers.size());
        }
        this->layers.assign(layers.begin(), layers.end());
    };
    void forward(MultiDimensionData<float> *input, MultiDimensionData<float> *output);
private:
    std::vector<BaseLayer*> layers;
};


#endif //MCLDROID_NET_H
