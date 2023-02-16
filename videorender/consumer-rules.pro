-keep class com.jeffmony.videorender.RenderSdk { *; }
-keep class com.jeffmony.videorender.RenderProcessImpl { *; }
-keep class com.jeffmony.videorender.ImageUtils { *; }
-keep class com.jeffmony.videorender.effect.ColorAdjustUtils { *; }
-keep class com.jeffmony.videorender.effect.StickerUtils { *; }
-keep class com.jeffmony.videorender.LogTag { *; }
-keep class com.jeffmony.videorender.view.VideoSurfaceView { *; }

-keep interface com.jeffmony.videorender.IRenderProcess { *; }
-keep interface com.jeffmony.videorender.view.VideoSurfaceView$* { *; }
-keep interface com.jeffmony.videorender.listener.OnCaptureListener { *; }
-keep interface com.jeffmony.videorender.listener.OnLayoutChanged { *; }
-keep interface com.jeffmony.videorender.listener.OnRenderListener { *; }
-keep interface com.jeffmony.videorender.listener.OnSurfaceListener { *; }

-keep class com.jeffmony.videorender.view.VideoSurfaceView { *; }
-keep interface com.jeffmony.videorender.view.VideoSurfaceView$* { *; }

-keep enum com.jeffmony.videorender.MirrorType { *; }