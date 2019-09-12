//package com.android.camera;
package com.example.a2_belcher.spyapp;


import android.content.Context;
import android.content.res.Configuration;
import android.hardware.Camera;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
//import android.graphics.Camera;
import android.media.MediaRecorder;
import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.widget.FrameLayout;
import android.widget.ImageView;
import android.widget.Toast;
import java.io.File;
import java.io.IOException;

import static com.example.a2_belcher.spyapp.SpyApp.TAG;


public class ShowCamera extends SurfaceView implements SurfaceHolder.Callback
{
    private SurfaceHolder holdMe;
    private Camera theCamera;

    public ShowCamera(Context context, Camera camera)
    {
        super(context);
        theCamera = camera;
        holdMe = getHolder();
        holdMe.addCallback(this);
    }

    @Override
    public void surfaceChanged(SurfaceHolder arg0, int arg1, int arg2, int arg3)
    {
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder)
    {
        Camera.Parameters parameters = theCamera.getParameters();

        if(this.getResources().getConfiguration().orientation != Configuration.ORIENTATION_LANDSCAPE) {
            parameters.set("orientation", "portrait");
            theCamera.setDisplayOrientation(90);
        }
        try {
            Log.i(TAG, "holder: " + holder + "camera: " + theCamera);


            //inside surfaceCreated(SurfaceHolder holder)
            Camera.Parameters params = theCamera.getParameters();
            Camera.Size prevSize = getOptimalSize(params, getWidth(), getHeight());
            //prevSize should be still in the camera's orientation. In your and my cases - landscape
            //params.setPreviewSize(prevSize.width, prevSize.height);
            theCamera.setParameters(params);



            theCamera.setPreviewDisplay(holder);
            theCamera.startPreview();
        } catch (IOException ignored) {
        }
    }

    private Camera.Size getOptimalSize(Camera.Parameters params, int w, int h) {

        final double ASPECT_TH = 2; // Threshold between camera supported ratio and screen ratio.
        double minDiff = Double.MAX_VALUE; //  Threshold of difference between screen height and camera supported height.
        double targetRatio = 0;
        int targetHeight = h;
        double ratio;
        Camera.Size optimalSize = null;

        // check if the orientation is portrait or landscape to change aspect ratio.
        if (getResources().getConfiguration().orientation == Configuration.ORIENTATION_PORTRAIT) {
            targetRatio = (double) h / (double) w;
        } else if (getResources().getConfiguration().orientation == Configuration.ORIENTATION_LANDSCAPE) {
            targetRatio = (double) w / (double) h;
        }

        // loop through all supported preview sizes to set best display ratio.
        for (Camera.Size s : params.getSupportedPreviewSizes()) {

            Log.i(TAG,"supported preview size h: " + s.height + " w: " + s.width);
            ratio = (double) s.width / (double) s.height;
            Log.i(TAG, "ratio: " + ratio);
            Log.i(TAG,"if : " + Math.abs(ratio - targetRatio) + " th: " + ASPECT_TH);

            if (Math.abs(ratio - targetRatio) <= ASPECT_TH) {
                Log.i(TAG,"\n\n if!!\n");
                if (Math.abs(targetHeight - s.height) < minDiff) {
                    optimalSize = s;
                    minDiff = Math.abs(targetHeight - s.height);
                }
            }
        }
        return optimalSize;
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder arg0)
    {
        if (theCamera != null) {
            theCamera.stopPreview();
            theCamera.release();
            theCamera = null;
        }
    }

};