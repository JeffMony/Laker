package com.jeffmony.videorender;

import android.graphics.Bitmap;

import java.io.File;
import java.io.FileOutputStream;

public class ImageUtils {

    public static void saveImg(Bitmap bitmap, String path) {
        FileOutputStream fos = null;
        try {
            File jpegFile = new File(path);
            if (jpegFile.exists()) {
                jpegFile.delete();
            }
            fos = new FileOutputStream(jpegFile);
            bitmap.compress(Bitmap.CompressFormat.JPEG, 100, fos);
            fos.flush();
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            if (fos != null) {
                try {
                    fos.close();
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        }
    }

}
