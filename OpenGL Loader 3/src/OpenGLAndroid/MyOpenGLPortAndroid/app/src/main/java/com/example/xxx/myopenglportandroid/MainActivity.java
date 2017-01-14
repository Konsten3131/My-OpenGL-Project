package com.example.xxx.myopenglportandroid;

import android.app.Activity;
import android.app.ActivityManager;
import android.content.Context;
import android.content.pm.ConfigurationInfo;
import android.content.res.AssetManager;
import android.opengl.GLSurfaceView;
import android.opengl.GLSurfaceView.Renderer;
import android.os.Build;
import android.os.Bundle;
import android.view.WindowManager;
import android.widget.TextView;
import android.widget.Toast;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;
public class MainActivity extends Activity{ //AppCompatActivity {  //If extended from AppCompatActivity than we get bar with app name on top of screen

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,//setting full screen of app
                WindowManager.LayoutParams.FLAG_FULLSCREEN);

    // Example of a call to a native method
    TextView tv = (TextView) findViewById(R.id.sample_text);
               AssetManager mngr=getAssets();

        /*String [] list;
        try {
            list = mngr.list("");
            if (list.length > 0) {
                // This is a folder
                for (String file : list) {
                    Toast.makeText(this, list.toString(),
                              Toast.LENGTH_LONG).show();
                }
            }
        }catch (Exception e ) {

        }*/


       //descript and copy files to cache directory
        String filepath = FileMgr.copyAsset(this, getCacheDir().getPath(), "QuadFront.png");
        FileMgr.copyAsset(this, getCacheDir().getPath(), "QuadRight.png");
        FileMgr.copyAsset(this, getCacheDir().getPath(), "QuadTop.png");
        FileMgr.copyAsset(this, getCacheDir().getPath(), "QuadPerspective.png");
        FileMgr.copyAsset(this, getCacheDir().getPath(), "invasteranko_d.png");
        FileMgr.copyAsset(this, getCacheDir().getPath(), "podium.png");
        FileMgr.copyAsset(this, getCacheDir().getPath(), "ster2.obj");
        FileMgr.copyAsset(this, getCacheDir().getPath(), "texture2d.vert");
        FileMgr.copyAsset(this, getCacheDir().getPath(), "texture2d.frag");
        FileMgr.copyAsset(this, getCacheDir().getPath(), "texture3d.vert");
        FileMgr.copyAsset(this, getCacheDir().getPath(), "texture3d.frag");
        FileMgr.copyAsset(this, getCacheDir().getPath(), "color.vert");
        FileMgr.copyAsset(this, getCacheDir().getPath(), "color.frag");

       // FileMgr.copyAsset(this, getCacheDir().getPath(), "sonic2.png");
       // FileMgr.copyAsset(this, getCacheDir().getPath(), "sonic2.obj");


       // FileMgr.copyAsset(this, getCacheDir().getPath(), "cube.obj");
        //FileMgr.copyAsset(this, getCacheDir().getPath(), "uvtemplate.png");






        setAppPath(getCacheDir().getPath());



        // InputStream is = getAssets().open("path/file.ext");
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
           on_surface_created();
        }

        @Override
        public void onSurfaceChanged(GL10 gl, int width, int height) {

            on_surface_changed(width, height);
        }

        @Override
        public void onDrawFrame(GL10 gl) {

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

    public static native void setAppPath(String path);
    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("nativeBridge");

    }
}



