package com.compilesense.liuyi.mcldroid;

/**
 * Created by shenjingyuan002 on 2017/3/21.
 */

public class NativeTest {
    static public native void testConvolutionLayerCompute();
    static public native void poolingLayerTest();
    static public native void multiDimensionDataTest();
    static public native void sofymaxLayerTest();

    static public native void testComputeRelu();
    static public native void testComputePrelu();
    static public native void testComputeTanh();
    static public native void testEltwiseLayer();

}
