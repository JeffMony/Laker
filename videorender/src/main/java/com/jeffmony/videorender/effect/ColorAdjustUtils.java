package com.jeffmony.videorender.effect;

public class ColorAdjustUtils {

    public static String getBrightEffect(int brightLevel) {
        String str = "{\n" +
                "    \"effect\":[\n" +
                "        {\n" +
                "            \"type\":\"color_adjust\",\n" +
                "            \"method_bit\":1,\n" +
                "            \"brightness_level\":%d\n" +
                "        }\n" +
                "    ]\n" +
                "}";
        return String.format(str, brightLevel);
    }

    public static String getContrastEffect(int contrastLevel) {
        String str = "{\n" +
                "    \"effect\":[\n" +
                "        {\n" +
                "            \"type\":\"color_adjust\",\n" +
                "            \"method_bit\":2,\n" +
                "            \"contrast_level\":%d\n" +
                "        }\n" +
                "    ]\n" +
                "}";
        return String.format(str, contrastLevel);
    }

    public static String getTemperatureEffect(int temperatureLevel) {
        String str = "{\n" +
                "    \"effect\":[\n" +
                "        {\n" +
                "            \"type\":\"color_adjust\",\n" +
                "            \"method_bit\":4,\n" +
                "            \"temperature_level\":%d\n" +
                "        }\n" +
                "    ]\n" +
                "}";
        return String.format(str, temperatureLevel);
    }

    public static String getSaturationEffect(int saturationLevel) {
        String str = "{\n" +
                "    \"effect\":[\n" +
                "        {\n" +
                "            \"type\":\"color_adjust\",\n" +
                "            \"method_bit\":8,\n" +
                "            \"saturation_level\":%d\n" +
                "        }\n" +
                "    ]\n" +
                "}";
        return String.format(str, saturationLevel);
    }

    public static String getGrainEffect(int grainLevel) {
        String str = "{\n" +
                "    \"effect\":[\n" +
                "        {\n" +
                "            \"type\":\"color_adjust\",\n" +
                "            \"method_bit\":16,\n" +
                "            \"grain_level\":%d\n" +
                "        }\n" +
                "    ]\n" +
                "}";
        return  String.format(str, grainLevel);
    }

    public static String getSharpEffect(int sharpLevel) {
        String str = "{\n" +
                "    \"effect\":[\n" +
                "        {\n" +
                "            \"type\":\"color_adjust\",\n" +
                "            \"method_bit\":32,\n" +
                "            \"sharpness_level\":%d\n" +
                "        }\n" +
                "    ]\n" +
                "}";
        return String.format(str, sharpLevel);
    }

    public static String getColorEffect(int brightLevel, int contrastLevel, int temperatureLevel, int saturationLevel, int grainLevel, int sharpLevel) {
        String str = "{\n" +
                "    \"effect\":[\n" +
                "        {\n" +
                "            \"type\":\"color_adjust\",\n" +
                "            \"method_bit\":63,\n" +
                "            \"brightness_level\":%d,\n" +
                "            \"contrast_level\":%d,\n" +
                "            \"temperature_level\":%d,\n" +
                "            \"saturation_level\":%d,\n" +
                "            \"grain_level\":%d,\n" +
                "            \"sharpness_level\":%d\n" +
                "        }\n" +
                "    ]\n" +
                "}";
        return String.format(str, brightLevel, contrastLevel, temperatureLevel, saturationLevel, grainLevel, sharpLevel);
    }
}
