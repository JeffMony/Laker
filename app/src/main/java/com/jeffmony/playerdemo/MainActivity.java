package com.jeffmony.playerdemo;

import androidx.appcompat.app.AppCompatActivity;

import android.Manifest;
import android.content.Intent;
import android.os.Bundle;
import android.widget.Button;
import android.widget.EditText;

import com.github.florent37.runtimepermission.RuntimePermission;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        RuntimePermission.askPermission(this)
                .request(Manifest.permission.READ_EXTERNAL_STORAGE,
                        Manifest.permission.WRITE_EXTERNAL_STORAGE)
                .onAccepted((result -> {
                    /**
                     * 权限已经申请成功
                     */
                })).ask();
        EditText urlText = findViewById(R.id.video_url_text);
        Button textureViewBtn = findViewById(R.id.texture_view_btn);
        Button surfaceViewBtn = findViewById(R.id.surface_view_btn);
        urlText.setText("http://videoconverter.vivo.com.cn/201706/655_1498479540118.mp4.f40.m3u8");

        textureViewBtn.setOnClickListener(v -> {
            Intent intent = new Intent(MainActivity.this, TextureViewActivity.class);
            intent.putExtra("video_uri", urlText.getText().toString());
            startActivity(intent);
        });

        surfaceViewBtn.setOnClickListener(v -> {
            Intent intent = new Intent(MainActivity.this, SurfaceViewActivity.class);
            intent.putExtra("video_uri", urlText.getText().toString());
            startActivity(intent);
        });
    }

}
