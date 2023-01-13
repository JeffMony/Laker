package com.jeffmony.videorender.listener;

import android.view.Surface;

public interface OnSurfaceListener {

    void onSurfaceCreated(Surface surface);

    void onSurfaceChanged(int width, int height);

    void onSurfaceDestroy();
}
