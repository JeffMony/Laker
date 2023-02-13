package com.jeffmony.videorender;

import android.view.TextureView;

import com.jeffmony.videorender.listener.OnCaptureListener;
import com.jeffmony.videorender.listener.OnRenderListener;
import com.jeffmony.videorender.listener.OnSurfaceListener;
import com.jeffmony.videorender.view.VideoSurfaceView;

public interface IRenderProcess {

    void setOnSurfaceListener(OnSurfaceListener listener);

    void setOnRenderListener(OnRenderListener listener);

    void setTextureView(TextureView view);

    void setSurfaceView(VideoSurfaceView view);

    void setVideoSize(int width, int height);

    int addEffect(String config);

    void updateEffect(int id, String config);

    void deleteEffect(int id);

    int addFilter(String config);

    void updateFilter(int id, String config);

    void updateFilterIntensity(int id, int intensity);

    void deleteFilter(int id);

    void updateFrame();

    void captureFrame(OnCaptureListener listener);

    void setMirror(MirrorType type);

    MirrorType getMirrorType();

    void destroy();
}
