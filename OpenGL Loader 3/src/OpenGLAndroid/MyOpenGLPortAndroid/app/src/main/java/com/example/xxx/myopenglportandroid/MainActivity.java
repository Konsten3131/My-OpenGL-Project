package com.example.xxx.myopenglportandroid;

import android.app.ActivityManager;
import android.content.Context;
import android.content.pm.ConfigurationInfo;
import android.opengl.GLSurfaceView;
import android.opengl.GLSurfaceView.Renderer;
import android.os.Build;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.widget.TextView;
import android.widget.Toast;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

    // Example of a call to a native method
    TextView tv = (TextView) findViewById(R.id.sample_text);

        //int a = myCustomFunc();
      //  Integer.toString(a)
       // tv.setText( Integer.toString(stringFromJNI()));
       // Toast.makeText(this, Integer.toString(a) +" " +Integer.toString(stringFromJNI()),
              //  Toast.LENGTH_LONG).show();

        //////start opengl related code
        ActivityManager activityManager
                = (ActivityManager) getSystemService(Context.ACTIVITY_SERVICE);
        ConfigurationInfo configurationInfo = activityManager.getDeviceConfigurationInfo();
//////
        final boolean supportsEs2 =
                configurationInfo.reqGlEsVersion >= 0x20000 || isProbablyEmulator();

        if (supportsEs2)
        {
            glSurfaceView = new GLSurfaceView(this);

            if (isProbablyEmulator()) {
                // Avoids crashes on startup with some emulator images.
                glSurfaceView.setEGLConfigChooser(8, 8, 8, 8, 16, 0);
            }

            glSurfaceView.setEGLContextClientVersion(2);
            glSurfaceView.setRenderer(new RendererWrapper());
            rendererSet = true;
            setContentView(glSurfaceView);
        } else
        {
            // Should never be seen in production, since the manifest filters
            // unsupported devices.
           Toast.makeText(this, "This device does not support OpenGL ES 2.0.",
                    Toast.LENGTH_LONG).show();
            return;
        }

    }

    @Override
    protected void onPause() {
        super.onPause();

        if (rendererSet) {
            glSurfaceView.onPause();
        }
    }

    @Override
    protected void onResume() {
        super.onResume();

        if (rendererSet) {
            glSurfaceView.onResume();
        }
    }

    public class RendererWrapper implements Renderer
    {
        @Override
        public void onSurfaceCreated(GL10 gl, EGLConfig config) {
            //glClearColor(0.0f, 0.0f, 1.0f, 0.0f);
           on_surface_created();
        }

        @Override
        public void onSurfaceChanged(GL10 gl, int width, int height) {
            // No-op
           on_surface_created();
        }

        @Override
        public void onDrawFrame(GL10 gl) {

            //glClear(GL_COLOR_BUFFER_BIT);
            on_draw_frame();
        }
    }

    private boolean isProbablyEmulator() {
        return Build.VERSION.SDK_INT >= Build.VERSION_CODES.ICE_CREAM_SANDWICH_MR1
                && (Build.FINGERPRINT.startsWith("generic")
                || Build.FINGERPRINT.startsWith("unknown")
                || Build.MODEL.contains("google_sdk")
                || Build.MODEL.contains("Emulator")
                || Build.MODEL.contains("Android SDK built for x86"));
    }


    /////
    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */


   // public static native void on_surface_created();

   // public static native void on_surface_changed(int width, int height);

    //public static native void on_draw_frame();

    private GLSurfaceView glSurfaceView;
    private boolean rendererSet;
    public static native void on_surface_created();
    public static native void on_surface_changed(int width, int height);

    public static native void on_draw_frame();
    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("nativeBridge");
        //System.loadLibrary("game-lib");
    }
}

//class GameLibJNIWrapper {
 //   static {
  //      System.loadLibrary("nativeBridge");
   // }

   // public static native void on_surface_created();

   // public static native void on_surface_changed(int width, int height);

    //public static native void on_draw_frame();
//}
