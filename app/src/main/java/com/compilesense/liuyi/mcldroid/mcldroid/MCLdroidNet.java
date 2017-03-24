package com.compilesense.liuyi.mcldroid.mcldroid;

import android.content.Context;
import android.graphics.Bitmap;

import com.compilesense.liuyi.mcldroid.messagepack.ModelInput;

/**
 * Created by shenjingyuan002 on 2017/3/24.
 */

public class MCLdroidNet {
    private MCLdroidNet(){}
    private final static MCLdroidNet mcl = new MCLdroidNet();
    public static MCLdroidNet getInstance() {
        return mcl;
    }

    void setUpNet(Context context){
        try {
            ModelInput.getInstance().readNetFileFromAssert(context,"Cifar10_def.txt");
        }catch (Exception e){
            e.printStackTrace();
        }

    }


    native void bitmapProcess(Bitmap bitmap);
}
