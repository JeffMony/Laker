package com.jeffmony.playerdemo;

import android.graphics.Bitmap;
import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.Surface;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.Toast;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

//import com.jeffmony.playersdk.CommonPlayer;
//import com.jeffmony.playersdk.IPlayer;
//import com.jeffmony.playersdk.PlayerParams;
//import com.jeffmony.playersdk.PlayerType;
//import com.jeffmony.playersdk.utils.ScreenUtils;
//import com.jeffmony.videorender.IRenderProcess;
//import com.jeffmony.videorender.ImageUtils;
//import com.jeffmony.videorender.LogTag;
//import com.jeffmony.videorender.MirrorType;
//import com.jeffmony.videorender.RenderSdk;
//import com.jeffmony.videorender.listener.OnCaptureListener;
//import com.jeffmony.videorender.listener.OnSurfaceListener;
//import com.jeffmony.videorender.view.VideoSurfaceView;

public class SurfaceViewActivity extends AppCompatActivity {

//    private final static int MSG_CAPTURE_FRAME_SUCCESS = 100;
//    private final static int MSG_CAPTURE_FRAME_FAILED = 101;
//    private final static int MSG_INIT_PLAYER = 102;
//    private VideoSurfaceView mVideoSurfaceView;
//    private IRenderProcess mRenderProcess;
//    private Surface mSurface;
//    private CommonPlayer mPlayer;
//    private int mBackgroundEffectId = -1;
//    private int mColorEffectId = -1;
//    private int mFilterId = -1;
//    private int mStickerId = -1;
//    private int mColorId = -1;
//    private Button mPlayPauseBtn;
//    private Handler mMainHandler = new Handler(msg -> {
//        int what = msg.what;
//        if (what == MSG_CAPTURE_FRAME_SUCCESS) {
//
//        } else if (what == MSG_CAPTURE_FRAME_FAILED) {
//
//        } else if (what == MSG_INIT_PLAYER) {
//            initPlayer();
//        }
//        return false;
//    });

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
//        setContentView(R.layout.activity_surface_view);
//        mVideoSurfaceView = findViewById(R.id.video_surface_view);
//        mRenderProcess = RenderSdk.createRenderProcess();
//        mRenderProcess.setSurfaceView(mVideoSurfaceView);
//        mRenderProcess.setOnSurfaceListener(new OnSurfaceListener() {
//            @Override
//            public void onSurfaceCreated(Surface surface) {
//                mSurface = surface;
//                if (mBackgroundEffectId == -1) {
//                    mBackgroundEffectId = mRenderProcess.addEffect("{\n" +
//                            "    \"effect\":[\n" +
//                            "        {\n" +
//                            "            \"type\":\"background\",\n" +
//                            "            \"backgroundType\":1,\n" +
//                            "            \"blur\":10,\n" +
//                            "            \"renderFrameType\":0,\n" +
//                            "            \"z_order\":1\n" +
//                            "        }\n" +
//                            "    ]\n" +
//                            "}");
//                }
//                mMainHandler.sendEmptyMessage(MSG_INIT_PLAYER);
//                initPlayer();
//            }
//
//            @Override
//            public void onSurfaceChanged(int width, int height) {
//
//            }
//
//            @Override
//            public void onSurfaceDestroy() {
//
//            }
//        });
//        findViewById(R.id.btn_capture).setOnClickListener(v -> {
//            if (mRenderProcess != null) {
//                mRenderProcess.captureFrame(new OnCaptureListener() {
//                    @Override
//                    public void onSuccess(Bitmap bitmap) {
//                        Log.i(LogTag.TAG, "captureFrame onSuccess");
//                        if (bitmap != null) {
//                            ImageUtils.saveImg(bitmap, "/sdcard/DCIM/Camera/result_ltp.jpg");
//                            bitmap.recycle();
//                            mMainHandler.post(() -> {
//                                Toast.makeText(SurfaceViewActivity.this, "截图成功", Toast.LENGTH_SHORT).show();
//                            });
//                        }
//                    }
//
//                    @Override
//                    public void onError(int code) {
//                        Log.e(LogTag.TAG, "captureFrame onError");
//                        mMainHandler.post(() -> {
//                            Toast.makeText(SurfaceViewActivity.this, "截图失败", Toast.LENGTH_SHORT).show();
//                        });
//                    }
//                });
//            }
//        });
//        findViewById(R.id.btn_horizontal_mirror).setOnClickListener(v -> {
//            mRenderProcess.setMirror(MirrorType.HORIZONTAL);
//            if (!mPlayer.isPlaying()) {
//                mRenderProcess.updateFrame();
//            }
//        });
//        findViewById(R.id.btn_vertical_mirror).setOnClickListener(v -> {
//            mRenderProcess.setMirror(MirrorType.VERTICAL);
//            if (!mPlayer.isPlaying()) {
//                mRenderProcess.updateFrame();
//            }
//        });
//        findViewById(R.id.btn_mirror_none).setOnClickListener(v -> {
//            mRenderProcess.setMirror(MirrorType.NONE);
//            if (!mPlayer.isPlaying()) {
//                mRenderProcess.updateFrame();
//            }
//        });
//        findViewById(R.id.btn_filter).setOnClickListener(v -> {
//            if (mRenderProcess != null) {
//                if (mFilterId == -1) {
//                    mFilterId = mRenderProcess.addFilter("/sdcard/poizon/Filter/Filter_32_Po10");
//                } else {
//                    mRenderProcess.deleteFilter(mFilterId);
//                    mFilterId = -1;
//                }
//                if (!mPlayer.isPlaying()) {
//                    mRenderProcess.updateFrame();
//                }
//            }
//        });
//        findViewById(R.id.btn_bright).setOnClickListener(v -> {
//            if (mRenderProcess != null) {
//                if (mColorEffectId == -1) {
//                    mColorEffectId = mRenderProcess.addEffect("{\n" +
//                            "    \"effect\":[\n" +
//                            "        {\n" +
//                            "            \"type\":\"color_adjust\",\n" +
//                            "            \"method_bit\":1,\n" +
//                            "            \"contrast_level\":0,\n" +
//                            "            \"brightness_level\":30,\n" +
//                            "            \"temperature_level\":0,\n" +
//                            "            \"saturation_level\":0,\n" +
//                            "            \"grain_level\":0,\n" +
//                            "            \"sharpness_level\":0\n" +
//                            "        }\n" +
//                            "    ]\n" +
//                            "}");
//                } else {
//                    mRenderProcess.deleteEffect(mColorEffectId);
//                    mColorEffectId = -1;
//                }
//                if (!mPlayer.isPlaying()) {
//                    mRenderProcess.updateFrame();
//                }
//            }
//        });
//        findViewById(R.id.btn_contrast).setOnClickListener(v -> {
//            if (mRenderProcess != null) {
//                if (mColorEffectId == -1) {
//                    mColorEffectId = mRenderProcess.addEffect("{\n" +
//                            "    \"effect\":[\n" +
//                            "        {\n" +
//                            "            \"type\":\"color_adjust\",\n" +
//                            "            \"method_bit\":2,\n" +
//                            "            \"contrast_level\":30,\n" +
//                            "            \"brightness_level\":0,\n" +
//                            "            \"temperature_level\":0,\n" +
//                            "            \"saturation_level\":0,\n" +
//                            "            \"grain_level\":0,\n" +
//                            "            \"sharpness_level\":0\n" +
//                            "        }\n" +
//                            "    ]\n" +
//                            "}");
//                } else {
//                    mRenderProcess.deleteEffect(mColorEffectId);
//                    mColorEffectId = -1;
//                }
//                if (!mPlayer.isPlaying()) {
//                    mRenderProcess.updateFrame();
//                }
//            }
//        });
//        findViewById(R.id.btn_temperature).setOnClickListener(v -> {
//            if (mRenderProcess != null) {
//                if (mColorEffectId == -1) {
//                    mColorEffectId = mRenderProcess.addEffect("{\n" +
//                            "    \"effect\":[\n" +
//                            "        {\n" +
//                            "            \"type\":\"color_adjust\",\n" +
//                            "            \"method_bit\":4,\n" +
//                            "            \"contrast_level\":0,\n" +
//                            "            \"brightness_level\":0,\n" +
//                            "            \"temperature_level\":-100,\n" +
//                            "            \"saturation_level\":0,\n" +
//                            "            \"grain_level\":0,\n" +
//                            "            \"sharpness_level\":0\n" +
//                            "        }\n" +
//                            "    ]\n" +
//                            "}");
//                } else {
//                    mRenderProcess.deleteEffect(mColorEffectId);
//                    mColorEffectId = -1;
//                }
//                if (!mPlayer.isPlaying()) {
//                    mRenderProcess.updateFrame();
//                }
//            }
//        });
//        findViewById(R.id.btn_saturation).setOnClickListener(v -> {
//            if (mRenderProcess != null) {
//                if (mColorEffectId == -1) {
//                    mColorEffectId = mRenderProcess.addEffect("{\n" +
//                            "    \"effect\":[\n" +
//                            "        {\n" +
//                            "            \"type\":\"color_adjust\",\n" +
//                            "            \"method_bit\":8,\n" +
//                            "            \"contrast_level\":0,\n" +
//                            "            \"brightness_level\":0,\n" +
//                            "            \"temperature_level\":0,\n" +
//                            "            \"saturation_level\":100,\n" +
//                            "            \"grain_level\":0,\n" +
//                            "            \"sharpness_level\":0\n" +
//                            "        }\n" +
//                            "    ]\n" +
//                            "}");
//                } else {
//                    mRenderProcess.deleteEffect(mColorEffectId);
//                    mColorEffectId = -1;
//                }
//                if (!mPlayer.isPlaying()) {
//                    mRenderProcess.updateFrame();
//                }
//            }
//        });
//        findViewById(R.id.btn_grain).setOnClickListener(v -> {
//            if (mRenderProcess != null) {
//                if (mColorEffectId == -1) {
//                    mColorEffectId = mRenderProcess.addEffect("{\n" +
//                            "    \"effect\":[\n" +
//                            "        {\n" +
//                            "            \"type\":\"color_adjust\",\n" +
//                            "            \"method_bit\":16,\n" +
//                            "            \"contrast_level\":0,\n" +
//                            "            \"brightness_level\":0,\n" +
//                            "            \"temperature_level\":0,\n" +
//                            "            \"saturation_level\":0,\n" +
//                            "            \"grain_level\":100,\n" +
//                            "            \"sharpness_level\":0\n" +
//                            "        }\n" +
//                            "    ]\n" +
//                            "}");
//                } else {
//                    mRenderProcess.deleteEffect(mColorEffectId);
//                    mColorEffectId = -1;
//                }
//                if (!mPlayer.isPlaying()) {
//                    mRenderProcess.updateFrame();
//                }
//            }
//        });
//        findViewById(R.id.btn_sharp).setOnClickListener(v -> {
//            if (mRenderProcess != null) {
//                if (mColorEffectId == -1) {
//                    mColorEffectId = mRenderProcess.addEffect("{\n" +
//                            "    \"effect\":[\n" +
//                            "        {\n" +
//                            "            \"type\":\"color_adjust\",\n" +
//                            "            \"method_bit\":32,\n" +
//                            "            \"contrast_level\":0,\n" +
//                            "            \"brightness_level\":0,\n" +
//                            "            \"temperature_level\":0,\n" +
//                            "            \"saturation_level\":0,\n" +
//                            "            \"grain_level\":0,\n" +
//                            "            \"sharpness_level\":100\n" +
//                            "        }\n" +
//                            "    ]\n" +
//                            "}");
//                } else {
//                    mRenderProcess.deleteEffect(mColorEffectId);
//                    mColorEffectId = -1;
//                }
//                if (!mPlayer.isPlaying()) {
//                    mRenderProcess.updateFrame();
//                }
//            }
//        });
//        findViewById(R.id.btn_sticker).setOnClickListener(v -> {
//            if (mRenderProcess != null) {
//                if (mStickerId == -1) {
//                    mStickerId = mRenderProcess.addEffect("{\n" +
//                            "    \"effect\":[\n" +
//                            "        {\n" +
//                            "            \"type\":\"sticker\",\n" +
//                            "            \"path\":\"/sdcard/poizon/image/scale.jpg\",\n" +
//                            "            \"center_x\":0.5,\n" +
//                            "            \"center_y\":0.5,\n" +
//                            "            \"scale\":0.5\n" +
//                            "        }\n" +
//                            "    ]\n" +
//                            "}");
//                } else {
//                    mRenderProcess.deleteEffect(mStickerId);
//                    mStickerId = -1;
//                }
//                if (!mPlayer.isPlaying()) {
//                    mRenderProcess.updateFrame();
//                }
//            }
//        });
//        findViewById(R.id.btn_color).setOnClickListener(v -> {
//            if (mRenderProcess != null) {
//                if (mColorId == -1) {
//                    mColorId = mRenderProcess.addEffect("{\n" +
//                            "    \"effect\":[\n" +
//                            "        {\n" +
//                            "            \"type\":\"color_adjust\",\n" +
//                            "            \"method_bit\":41,\n" +
//                            "            \"contrast_level\":0,\n" +
//                            "            \"brightness_level\":10,\n" +
//                            "            \"temperature_level\":0,\n" +
//                            "            \"saturation_level\":20,\n" +
//                            "            \"grain_level\":0,\n" +
//                            "            \"sharpness_level\":15\n" +
//                            "        }\n" +
//                            "    ]\n" +
//                            "}");
//                } else {
//                    mRenderProcess.deleteEffect(mColorId);
//                    mColorId = -1;
//                }
//                if (!mPlayer.isPlaying()) {
//                    mRenderProcess.updateFrame();
//                }
//            }
//        });
//        mPlayPauseBtn = findViewById(R.id.btn_play_pause);
//        mPlayPauseBtn.setOnClickListener(v-> {
//            if (mPlayer.isPlaying()) {
//                mPlayer.pause();
//                mPlayPauseBtn.setText("播放");
//            } else {
//                mPlayer.start();
//                mPlayPauseBtn.setText("暂停");
//            }
//        });
    }

//    private void initPlayer() {
//        PlayerParams params = new PlayerParams();
//        params.setUseOkHttp(false);
//        mPlayer = new CommonPlayer(this, PlayerType.IJK_PLAYER, params);
//        try {
//            mPlayer.setDataSource(this, Uri.parse("http://videoconverter.vivo.com.cn/201706/655_1498479540118.mp4.f40.m3u8"));
//        } catch (Exception e) {
//            Log.w(LogTag.TAG, "setDataSource failed, exception = " + e.getMessage());
//            return;
//        }
//        mPlayer.setSurface(mSurface);
//        mPlayer.setOnPreparedListener(mPrepareListener);
//        mPlayer.setOnVideoSizeChangedListener(mSizeChangedListener);
//        mPlayer.prepareAsync();
//    }
//
//    private IPlayer.OnPreparedListener mPrepareListener = mp -> {
//        mPlayer.start();
//    };
//
//    private IPlayer.OnVideoSizeChangedListener mSizeChangedListener = new IPlayer.OnVideoSizeChangedListener() {
//        @Override
//        public void onVideoSizeChanged(IPlayer mp, int width, int height, int rotationDegree, float pixelRatio, float darRatio) {
//            if (mRenderProcess != null && width != 0 && height != 0) {
//                mRenderProcess.setVideoSize(width, height);
//
//                ViewGroup.LayoutParams layoutParams = mVideoSurfaceView.getLayoutParams();
//                layoutParams.width = ViewGroup.LayoutParams.MATCH_PARENT;
//                layoutParams.height = (int) (ScreenUtils.getScreenWidth(SurfaceViewActivity.this) * height * 1.0f / width);
//                mVideoSurfaceView.setLayoutParams(layoutParams);
//            }
//        }
//    };
//
//    @Override
//    protected void onPause() {
//        super.onPause();
//        if (mPlayer != null) {
//            mPlayer.pause();
//            mPlayPauseBtn.setText("播放");
//        }
//    }
//
//    @Override
//    protected void onDestroy() {
//        super.onDestroy();
//        if (mPlayer != null) {
//            mPlayer.release();
//            mPlayer = null;
//        }
//        if (mRenderProcess != null) {
//            mRenderProcess.destroy();
//        }
//    }
}
