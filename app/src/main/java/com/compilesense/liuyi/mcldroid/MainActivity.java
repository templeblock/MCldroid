package com.compilesense.liuyi.mcldroid;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.drawable.Drawable;
import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;

import com.compilesense.liuyi.mcldroid.mcldroid.MCLdroidNet;

import java.io.File;
import java.io.FileInputStream;
import java.io.InputStream;

import static android.graphics.Color.blue;
import static android.graphics.Color.green;
import static android.graphics.Color.red;


public class MainActivity extends AppCompatActivity {
    String TAG = "MainActivity";
    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("MCLdroid-lib");
    }

    ImageView imageView;
    Bitmap bmp;
    boolean hadSetupNet = false;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        final Button bt = (Button) findViewById(R.id.bt_test);



        bt.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                long st = System.currentTimeMillis();
//                NativeTest.testModelInput("sss");



                MCLdroidNet.getInstance().setUpNet(MainActivity.this);

//                st = System.currentTimeMillis();
////                logBitmapData(bmp);


//                MCLdroidNet.getInstance().testInputBitmap(bmp);
                Log.d("MainActivity","计算时(ms):"+ (System.currentTimeMillis() - st));
////                bt.postDelayed(
////                        new Runnable() {
////                            @Override
////                            public void run() {
////                                imageView.setImageBitmap(bmp);
////                            }
////                        },1000
////                );
            }
        });
        imageView = (ImageView) findViewById(R.id.img_test);
    }

    @Override
    protected void onResume() {
        super.onResume();

        try {
            bmp = getBitmapFromCache(this);
            imageView.setImageBitmap(bmp);

        }catch (Exception e){
            e.printStackTrace();
        }
    }

    void testBitmap(){
        bmp = BitmapFactory.decodeResource(getResources(), R.drawable.jugg);
        if (bmp == null){
            return;
        }
        NativeTest.bitmapProcess(bmp);
        imageView.setImageBitmap(bmp);
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    public static Bitmap getBitmapFromCache(Context context){
        try {
            File appDir = new File(Environment.getExternalStorageDirectory(), "MCLdoirdTestImage");
            String fileName =  "test.jpg";
            File file = new File(appDir, fileName);
            InputStream in = new FileInputStream(file);
            Bitmap bitmap = BitmapFactory.decodeStream(in);
            return bitmap;
        }catch (Exception e){
            e.printStackTrace();
        }
        return null;
    }

    void logBitmapData(Bitmap bitmap){
        int hSize = bitmap.getHeight();
        int wSize = bitmap.getWidth();
        int cSize = 3;
        int nSize = 1;

        float[][][][] data = new float[nSize][cSize][hSize][wSize];
        for (int h = 0; h < hSize; h++){
            for (int w = 0; w < wSize; w++) {
                int color = bitmap.getPixel(w, h);
                data[0][0][h][w] = (float) red(color);
                data[0][1][h][w] = (float) green(color);
                data[0][2][h][w] = (float) blue(color);
            }
        }
        int[] index = {nSize,cSize,hSize,wSize};//{n,c,h,w}

        try {
            LogUtile.saveLogData("MCLdroid-bitmap-java", data, index);
        }catch (Exception e){
            e.printStackTrace();
        }

    }
}
