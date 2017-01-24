package com.example.xxx.myopenglportandroid;

import android.app.Activity;
import android.app.ActivityManager;
import android.content.Context;
import android.content.pm.ConfigurationInfo;
import android.opengl.GLSurfaceView;
import android.opengl.GLSurfaceView.Renderer;
import android.os.Build;
import android.os.Bundle;
import android.util.Log;
import android.view.MotionEvent;
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


        copyAssets();


        setAppPath(getFilesDir().getPath());


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
            glSurfaceView.setPreserveEGLContextOnPause(true); //prevents from recreating context if app loses focus
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

    @Override
    public boolean onTouchEvent(MotionEvent e) {
        // MotionEvent reports input details from the touch screen
        // and other input controls. In this case, you are only
        // interested in events where the touch position changed.

        int x = (int)e.getX();
        int y = (int)e.getY();

        switch (e.getAction()) {
            case MotionEvent.ACTION_MOVE:



                int difx = x - touchBeginX;
                int dify = y - touchBeginY;
               // Log.d("difx", "value X: " + difx);
               // Log.d("dify", "value Y: " + dify);

                double distanceX = Math.sqrt(difx*difx);
                double distanceY = Math.sqrt(dify*dify);

                ////////////this code is fore view where we rotate just around y axis//////////
                if(x >= 0)
                    rotateCameraAroundAxis((int)-distanceX);
                else
                    rotateCameraAroundAxis((int)distanceX);
                ///////////////////////////////////////////////////////////////////////////////

                distanceX /= 250.0f; //deviding by 250 simply to slowdown user input rotation
                distanceY /= 250.0f;

                float angleX = getAngleX();
                float angleY = getAngleY();

                if(difx >= 0)
                    setAngleY(angleY += distanceX);
                    //when we slide horizontally we expect object to rotate arount its x axis
                else
                    setAngleY(angleY -= distanceX);

                if(dify >= 0)
                    setAngleX(angleX += distanceY);
                else
                    setAngleX(angleX -= distanceY);

                touchBeginX = x;
                touchBeginY = y;

                break;
            case MotionEvent.ACTION_DOWN:

                 touchBeginX = x;
                 touchBeginY = y;
                break;

            case MotionEvent.ACTION_UP:
                touchBeginX = x;
                touchBeginY = y;
                break;

            case MotionEvent.ACTION_POINTER_DOWN:

                Log.d("difx", "value X0: " + e.getX(0));
                Log.d("dify", "value X1: " + e.getX(1));
                break;

        }


        return true;
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

   private void copyAssets() {

        //descript and copy files to cache directory
        String filepath = FileMgr.copyAsset(this, getFilesDir().getPath(), "QuadFront.png");
        FileMgr.copyAsset(this, getFilesDir().getPath(), "QuadRight.png");
        FileMgr.copyAsset(this, getFilesDir().getPath(), "QuadTop.png");
        FileMgr.copyAsset(this, getFilesDir().getPath(), "QuadPerspective.png");
        FileMgr.copyAsset(this, getFilesDir().getPath(), "invasteranko_d.png");
        FileMgr.copyAsset(this, getFilesDir().getPath(), "podium.png");
        FileMgr.copyAsset(this, getFilesDir().getPath(), "ster2.obj");
        FileMgr.copyAsset(this, getFilesDir().getPath(), "texture2d.vert");
        FileMgr.copyAsset(this, getFilesDir().getPath(), "texture2d.frag");
        FileMgr.copyAsset(this, getFilesDir().getPath(), "texture3d.vert");
        FileMgr.copyAsset(this, getFilesDir().getPath(), "texture3d.frag");
        FileMgr.copyAsset(this, getFilesDir().getPath(), "color.vert");
        FileMgr.copyAsset(this, getFilesDir().getPath(), "color.frag");

        // FileMgr.copyAsset(this, getCacheDir().getPath(), "sonic2.png");
        // FileMgr.copyAsset(this, getCacheDir().getPath(), "sonic2.obj");
        // FileMgr.copyAsset(this, getCacheDir().getPath(), "cube.obj");
        //FileMgr.copyAsset(this, getCacheDir().getPath(), "uvtemplate.png");
    }



    /////
    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */


   // public static native void on_surface_created();

   // public static native void on_surface_changed(int width, int height);

    //public static native void on_draw_frame();
    int touchBeginX = 0;
    int touchBeginY = 0;
    private GLSurfaceView glSurfaceView;
    private boolean rendererSet;
    public static native void on_surface_created();
    public static native void on_surface_changed(int width, int height);

    public static native void on_draw_frame();

    public static native void rotateCameraAroundAxis(int distance);

    public static native float getAngleX();
    public static native float getAngleY();

    public static native void setAngleX(float angle);
    public static native void setAngleY(float angle);


    public static native void setAppPath(String path);
    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("nativeBridge");

    }
}



