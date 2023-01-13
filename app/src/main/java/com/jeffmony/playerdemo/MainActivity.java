package com.jeffmony.playerdemo;

import androidx.appcompat.app.AppCompatActivity;

import android.Manifest;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

import com.github.florent37.runtimepermission.RuntimePermission;

public class MainActivity extends AppCompatActivity implements View.OnClickListener {

    private EditText mUrlText;
    private Button mTextureViewBtn;
    private Button mSurfaceViewBtn;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        RuntimePermission.askPermission(this)
                .request(Manifest.permission.READ_EXTERNAL_STORAGE,
                        Manifest.permission.WRITE_EXTERNAL_STORAGE,
                        Manifest.permission.CAMERA)
                .onAccepted((result -> {
                    /**
                     * 权限已经申请成功
                     */
                })).ask();
        initViews();
    }

    private void initViews() {
        mUrlText = findViewById(R.id.video_url_text);
        mTextureViewBtn = findViewById(R.id.texture_view_btn);
        mSurfaceViewBtn = findViewById(R.id.surface_view_btn);
        //http://videocdn.mviplay.cn/video/283665.mp4
        //http://videoconverter.vivo.com.cn/201706/655_1498479540118.mp4.main.m3u8
        mUrlText.setText("http://videoconverter.vivo.com.cn/201706/655_1498479540118.mp4.main.m3u8");
        mTextureViewBtn.setOnClickListener(this);
        mSurfaceViewBtn.setOnClickListener(this);
    }


    @Override
    public void onClick(View v) {
        if (v == mTextureViewBtn) {
            playWithTextureView();
        } else if (v == mSurfaceViewBtn) {
            playWithSurfaceView();
        }
    }

    private void playWithTextureView() {
        Intent intent = new Intent(MainActivity.this, TextureViewActivity.class);
        startActivity(intent);
    }

    private void playWithSurfaceView() {
        Intent intent = new Intent(MainActivity.this, SurfaceViewActivity.class);
        startActivity(intent);
    }
}
