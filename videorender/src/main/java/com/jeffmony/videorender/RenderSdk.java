package com.jeffmony.videorender;

public class RenderSdk {
    private static boolean sLoadLibrary = false;

    static {
        load();
    }

    public static void load() {
        if (sLoadLibrary) {
            return;
        }
        System.loadLibrary("rendersdk");
        System.loadLibrary("c++_shared");
        sLoadLibrary = true;
    }

    public static IRenderProcess createRenderProcess() {
        load();
        return new RenderProcessImpl();
    }
}
