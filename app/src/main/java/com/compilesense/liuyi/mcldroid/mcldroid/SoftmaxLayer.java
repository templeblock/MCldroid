package com.compilesense.liuyi.mcldroid.mcldroid;

/**
 * Created by shenjingyuan002 on 2017/3/27.
 */

public class SoftmaxLayer extends BaseLayer {

    private static final String TAG = "SoftmaxLayer";

    public SoftmaxLayer(String name){
        this.name = name;
        nativeObject = createSoftmaxLayer(name);
    }

    @Override
    public Object compute(Object input) {
        return null;
    }

    @Override
    public void releaseLayer() {

    }

    private static native long createSoftmaxLayer(String name);
}
