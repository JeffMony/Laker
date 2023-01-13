package com.jeffmony.videorender.view;

import android.content.Context;
import android.util.AttributeSet;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

import com.jeffmony.videorender.listener.OnLayoutChanged;

import java.util.ArrayList;
import java.util.List;

public class VideoSurfaceView extends SurfaceView implements SurfaceHolder.Callback {

    private int mWidth = -1;
    private int mHeight = -1;

    /**
     * SurfaceView回调的集合, 可以设置多个
     */
    private final List<SurfaceViewCallback> mSurfaceViewCallback = new ArrayList<>();

    /**
     * view的宽高固定时的回调
     */
    private OnLayoutChanged mOnLayoutChanged;

    public VideoSurfaceView(Context context) {
        this(context, null);
    }

    public VideoSurfaceView(Context context, AttributeSet attrs) {
        this(context, attrs, 0);
    }

    public VideoSurfaceView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        init(context);
    }

    private void init(Context context) {
        getHolder().addCallback(this);
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        for (SurfaceViewCallback callback : mSurfaceViewCallback) {
            callback.surfaceCreate(holder);
        }
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
        for (SurfaceViewCallback callback : mSurfaceViewCallback) {
            callback.surfaceChanged(holder, format, width, height);
        }
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        for (SurfaceViewCallback callback : mSurfaceViewCallback) {
            callback.surfaceDestroyed(holder);
        }
    }

    @Override
    protected void onDetachedFromWindow() {
        super.onDetachedFromWindow();
        mSurfaceViewCallback.clear();
    }

    @Override
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
        if (mWidth == -1 || mHeight == -1) {
            super.onMeasure(widthMeasureSpec, heightMeasureSpec);
        } else {
            setMeasuredDimension(mWidth, mHeight);
        }
    }

    @Override
    protected void onLayout(boolean changed, int left, int top, int right, int bottom) {
        super.onLayout(changed, left, top, right, bottom);
        if (changed && mOnLayoutChanged != null) {
            mOnLayoutChanged.onLayout(right - left, bottom - top);
        }
    }

    public void addSurfaceCallback(SurfaceViewCallback callback) {
        mSurfaceViewCallback.add(callback);
    }

    public void setOnLayoutChanged(OnLayoutChanged changed) {
        mOnLayoutChanged = changed;
    }

    public interface SurfaceViewCallback {

        /**
         * surfaceCreate 回调
         * @param holder holder对象
         */
        void surfaceCreate(SurfaceHolder holder);

        /**
         * surfaceChanged 回调
         * @param holder holder对象
         * @param format -
         * @param width surface宽
         * @param height surface高
         */
        void surfaceChanged(SurfaceHolder holder, int format, int width, int height);

        /**
         * surfaceDestroyed 回调
         * @param holder holder对象
         */
        void surfaceDestroyed(SurfaceHolder holder);
    }
}
