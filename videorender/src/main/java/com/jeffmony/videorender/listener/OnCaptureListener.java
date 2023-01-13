package com.jeffmony.videorender.listener;

import android.graphics.Bitmap;

public interface OnCaptureListener {

    void onSuccess(Bitmap bitmap);

    void onError(int code);
}
