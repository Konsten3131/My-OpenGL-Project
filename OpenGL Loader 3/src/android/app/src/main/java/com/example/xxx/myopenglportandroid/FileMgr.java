package com.example.xxx.myopenglportandroid;

/**
 * Created by xxx on 2017-01-10.
 */
import android.content.Context;
import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;

public class FileMgr
{
    public static String copyAsset(Context context, String folder, String filename) {
        context = context.getApplicationContext();
        File file = new File(folder + "/" + filename);
        // TODO Manage the size of the cache dir.

        try {
            InputStream inputStream = context.getAssets().open(filename);
            int size = inputStream.available();
            byte[] buffer = new byte[size];
            inputStream.read(buffer);
            inputStream.close();

            FileOutputStream fileOutputStream = new FileOutputStream(file);
            fileOutputStream.write(buffer);
            fileOutputStream.close();
        } catch (Exception e) {
            return "File Not Found";
        }


        return file.getPath();
    }
}
