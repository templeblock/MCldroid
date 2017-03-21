package com.compilesense.liuyi.mcldroid.caffecnn;

/**
 * Created by shenjingyuan002 on 2017/3/3.
 */

public interface Layer {
    Object compute(Object input);
    void releaseLayer();
}
