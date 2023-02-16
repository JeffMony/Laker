package com.jeffmony.videorender.effect;

public class StickerUtils {

    public static String createStickerStr(String path, float centerX, float centerY, float scale, float rotate) {
        String str = "{\n" +
                "    \"effect\":[\n" +
                "        {\n" +
                "            \"type\":\"sticker\",\n" +
                "            \"path\":\"%s\",\n" +
                "            \"center_x\":%f,\n" +
                "            \"center_y\":%f,\n" +
                "            \"scale\":%f,\n" +
                "            \"rotate\":%f\n" +
                "        }\n" +
                "    ]\n" +
                "}";
        return String.format(str, path, centerX, centerY, scale, rotate);
    }

    public static String updateStickerStr(float centerX, float centerY, float scale, float rotate) {
        String str = "{\n" +
                "    \"effect\":[\n" +
                "        {\n" +
                "            \"type\":\"sticker\",\n" +
                "            \"center_x\":0.1,\n" +
                "            \"center_y\":0.2,\n" +
                "            \"scale\":1,\n" +
                "            \"rotate\":0\n" +
                "        }\n" +
                "    ]\n" +
                "}";
        return String.format(str, centerX, centerY, scale, rotate);
    }
}
