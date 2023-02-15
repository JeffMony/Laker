# VideoRenderSdk
支持播放器接入的视频渲染SDK：
- 可以支持MediaPlayer/ijkplayer/Exoplayer，具体可以参考: [播放器渲染例子](https://github.com/JeffMony/PlayerSDK)
- 支持SurfaceView和TextureView
- 使用OpenGL渲染视频
- 支持设置水印
- 支持调整亮度、对比度、色温、色调、锐度、颗粒度等等
- 支持上下翻转视频
- 支持左右翻转视频
- 支持设置特定滤镜

### 接入SDK
工程的build.gradle中添加
```
allprojects {
    repositories {
        google()
        jcenter()
        maven {
            url 'https://jitpack.io'
        }
    }
}
```
app的build.gradle中添加
```
dependencies {
    implementation 'com.github.JeffMony:Laker:1.0.4'
}
```
目前最新的版本是1.0.4，后续直接看tag上版本的更新,

具体接入的流程可以查看我在播放器SDK中应用的例子：
[播放器渲染例子](https://github.com/JeffMony/PlayerSDK)

### 设置贴纸
```
{
    "effect":[
        {
            "type":"sticker",
            "path":"/sdcard/Pictures/icon.png",
            "center_x":0.2,
            "center_y":0.2,
            "scale":1,
            "rotate":30
        }
    ]
}
```
- center_x和center_y的范围是0.f ~ 1.f
- scale 表示放缩情况
- rotate表示旋转角度
### 设置亮度
```
{
    "effect":[
        {
            "type":"color_adjust",
            "method_bit":1,
            "brightness_level":30
        }
    ]
}
```
调节范围：-100 ~ 100
### 设置对比度
```
{
    "effect":[
        {
            "type":"color_adjust",
            "method_bit":2,
            "contrast_level":30
        }
    ]
}
```
调节范围：-100 ~ 100
### 设置色温
```
{
    "effect":[
        {
            "type":"color_adjust",
            "method_bit":4,
            "temperature_level":30
        }
    ]
}
```
调节范围：-100 ~ 100
### 设置饱和度
```
{
    "effect":[
        {
            "type":"color_adjust",
            "method_bit":8,
            "saturation_level":30
        }
    ]
}
```
调节范围：-100 ~ 100
### 设置颗粒度
```
{
    "effect":[
        {
            "type":"color_adjust",
            "method_bit":16,
            "grain_level":30
        }
    ]
}
```
调节范围：0 ~ 100
### 设置锐度
```
{
    "effect":[
        {
            "type":"color_adjust",
            "method_bit":32,
            "sharpness_level":30
        }
    ]
}
```
调节范围：0 ~ 100
### 色彩调节组合
1 + 4 + 8 + 32 = 45
```
{
    "effect":[
        {
            "type":"color_adjust",
            "method_bit":45,
            "brightness_level":10,
            "temperature_level":5,
            "saturation_level":20,
            "sharpness_level":10
        }
    ]
}
```


