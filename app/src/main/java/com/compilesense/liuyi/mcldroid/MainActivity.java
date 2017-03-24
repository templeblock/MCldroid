package com.compilesense.liuyi.mcldroid;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.drawable.Drawable;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("MCLdroid-lib");
    }

    ImageView imageView;
    Bitmap bmp;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        Button bt = (Button) findViewById(R.id.bt_test);
        bt.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                testBitmap();
            }
        });

        imageView = (ImageView) findViewById(R.id.img_test);
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
}
