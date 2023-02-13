package com.jeffmony.videorender;

import android.graphics.SurfaceTexture;
import android.os.Handler;
import android.os.Looper;
import android.util.Log;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.TextureView;

import com.jeffmony.videorender.listener.OnCaptureListener;
import com.jeffmony.videorender.listener.OnLayoutChanged;
import com.jeffmony.videorender.listener.OnRenderListener;
import com.jeffmony.videorender.listener.OnSurfaceListener;
import com.jeffmony.videorender.view.VideoSurfaceView;

public class RenderProcessImpl implements IRenderProcess, VideoSurfaceView.SurfaceViewCallback,
        OnLayoutChanged, SurfaceTexture.OnFrameAvailableListener, TextureView.SurfaceTextureListener {

    private long mId = 0;
    /**
     * 回调给开发者, 告知其Surface改变
     */
    private OnSurfaceListener mOnSurfaceListener;
    private OnRenderListener mOnRenderListener;
    private int mSurfaceWidth;
    private int mSurfaceHeight;
    private MirrorType mMirrorType = MirrorType.NONE;
    /**
     * 设置给MediaCodec的SurfaceTexture
     */
    private SurfaceTexture mVideoTexture;
    private final float[] mMatrix = new float[16];
    private final Handler mMainHandler = new Handler(Looper.getMainLooper());
    private TextureView mTextureView = null;
    private SurfaceTexture mTextureSurfaceTexture = null;

    public RenderProcessImpl() {
        mId = create();
    }

    @Override
    public void setOnSurfaceListener(OnSurfaceListener listener) {
        mOnSurfaceListener = listener;
    }

    @Override
    public void setOnRenderListener(OnRenderListener listener) {
        mOnRenderListener = listener;
    }

    @Override
    public void setTextureView(TextureView view) {
        if (view != null) {
            mTextureView = view;
            view.setSurfaceTextureListener(this);
        }
    }

    @Override
    public void onSurfaceTextureAvailable(SurfaceTexture surface, int width, int height) {
        if (mId == 0) {
            return;
        }
        Log.i(LogTag.TAG, "onSurfaceTextureAvailable surface="+surface+", width="+width+", height="+height);
        mTextureSurfaceTexture = surface;
        mSurfaceWidth = width;
        mSurfaceHeight = height;
        surfaceCreate(mId, new Surface(mTextureSurfaceTexture));
        surfaceChanged(mId, width, height);
    }

    @Override
    public void onSurfaceTextureSizeChanged(SurfaceTexture surface, int width, int height) {
        if (mId == 0) {
            return;
        }
        Log.i(LogTag.TAG, "onSurfaceTextureSizeChanged surface="+surface+", width="+width+", height="+height);
        mSurfaceWidth = width;
        mSurfaceHeight = height;
        surfaceChanged(mId, width, height);
    }

    @Override
    public void onSurfaceTextureUpdated(SurfaceTexture surface) {
        // Log.i(LogTag.TAG, "onSurfaceTextureUpdated surface="+surface);
    }

    @Override
    public boolean onSurfaceTextureDestroyed(SurfaceTexture surface) {
        Log.i(LogTag.TAG, "onSurfaceTextureDestroyed surface="+surface);
        mTextureSurfaceTexture = null;
        if (mId != 0) {
            surfaceDestroyed(mId);
        }
        return false;
    }

    @Override
    public void setSurfaceView(VideoSurfaceView view) {
        if (view != null) {
            view.addSurfaceCallback(this);
            view.setOnLayoutChanged(this);
        }
    }

    @Override
    public void setVideoSize(int width, int height) {
        if (mId == 0) {
            return;
        }
        setVideoSize(mId, width, height);
    }

    @Override
    public void surfaceCreate(SurfaceHolder holder) {
        if (mId == 0) {
            return;
        }
        surfaceCreate(mId, holder.getSurface());
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
        if (mId == 0) {
            return;
        }
        // holder.setFixedSize(width, height);
        if (mSurfaceWidth == 0 || mSurfaceHeight == 0) {
            surfaceChanged(mId, width, height);
        } else {
            surfaceChanged(mId, mSurfaceWidth, mSurfaceHeight);
        }
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        if (mId == 0) {
            return;
        }
        surfaceDestroyed(mId);
    }

    @Override
    public void onLayout(int width, int height) {
        if (mId == 0) {
            return;
        }
        mSurfaceWidth = width;
        mSurfaceHeight = height;
        setFrameSize(mId, width, height);
    }

    @Override
    public void onFrameAvailable(SurfaceTexture surfaceTexture) {
        if (mId == 0) {
            return;
        }
        updateFrame(mId);
    }

    @Override
    public int addEffect(String config) {
        if (mId == 0) {
            return -1;
        }
        return addEffect(mId, config);
    }

    @Override
    public void updateEffect(int id, String config) {
        if (mId == 0) {
            return;
        }
        updateEffect(mId, id, config);
    }

    @Override
    public void deleteEffect(int id) {
        if (mId == 0) {
            return;
        }
        deleteEffect(mId, id);
    }

    @Override
    public int addFilter(String config) {
        if (mId == 0) {
            return -1;
        }
        return addFilter(mId, config);
    }

    @Override
    public void updateFilter(int id, String config) {
        if (mId == 0) {
            return;
        }
        updateFilter(mId, id, config);
    }

    @Override
    public void updateFilterIntensity(int id, int intensity) {
        if (mId == 0) {
            return;
        }
        updateFilterIntensity(mId, id, intensity);
    }

    @Override
    public void deleteFilter(int id) {
        if (mId == 0) {
            return;
        }
        deleteFilter(mId, id);
    }

    @Override
    public void updateFrame() {
        if (mId == 0) {
            return;
        }
        updateFrame(mId);
    }

    @Override
    public void captureFrame(OnCaptureListener listener) {
        if (mId == 0) {
            return;
        }
        if (listener == null) {
            return;
        }
        captureFrame(mId, listener);
    }

    @Override
    public void setMirror(MirrorType type) {
        if (mId == 0) {
            return;
        }
        mMirrorType = type;
        setMirror(mId, type.ordinal());
    }

    @Override
    public MirrorType getMirrorType() {
        return mMirrorType;
    }

    @Override
    public void destroy() {
        if (mId == 0) {
            return;
        }
        destroy(mId);
        mId = 0;
    }

    /**
     * native回调到Java层
     */
    private void onGenerateOesTexture(int oesTexture) {
        mVideoTexture = new SurfaceTexture(oesTexture);
        mVideoTexture.setOnFrameAvailableListener(this);
        Surface surface = new Surface(mVideoTexture);
        mMainHandler.post(() -> {
            if (mOnSurfaceListener != null) {
                mOnSurfaceListener.onSurfaceCreated(surface);
            }
        });
    }

    private void onSurfaceChanged(int width, int height) {
        mMainHandler.post(() -> {
            if (mOnSurfaceListener != null) {
                mOnSurfaceListener.onSurfaceChanged(width, height);
            }
        });
    }

    private void onSurfaceDestroy() {
        mMainHandler.post(() -> {
            if (mOnSurfaceListener != null) {
                mOnSurfaceListener.onSurfaceDestroy();
            }
        });
    }

    private void updateTexImage() {
        if (mVideoTexture != null) {
            mVideoTexture.updateTexImage();
        }
    }

    private float[] getTextureMatrix() {
        if (mVideoTexture == null) {
            return null;
        }
        mVideoTexture.getTransformMatrix(mMatrix);
        return mMatrix;
    }

    private void onError(int code) {
        mMainHandler.post(() -> {
            if (mOnRenderListener != null) {
                mOnRenderListener.onError(code);
            }
        });
    }

    /**
     * java层调用到native方法
     */
    private native long create();
    private native void surfaceCreate(long id, Surface surface);
    private native void surfaceChanged(long id, int width, int height);
    private native void setFrameSize(long id, int width, int height);
    private native void surfaceDestroyed(long id);
    private native void setVideoSize(long id, int width, int height);
    private native void updateFrame(long id);
    private native int addEffect(long id, String config);
    private native void updateEffect(long id, int effectId, String config);
    private native void deleteEffect(long id, int effectId);
    private native int addFilter(long id, String config);
    private native void updateFilter(long id, int filterId, String config);
    private native void updateFilterIntensity(long id, int filterId, int intensity);
    private native void deleteFilter(long id, int filterId);
    private native void captureFrame(long id, OnCaptureListener listener);
    private native void setMirror(long id, int type);
    private native void destroy(long id);
}
