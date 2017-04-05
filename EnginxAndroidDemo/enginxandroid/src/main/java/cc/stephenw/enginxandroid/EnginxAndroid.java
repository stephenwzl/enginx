package cc.stephenw.enginxandroid;

/**
 * Created by stephenw on 2017/4/5.
 */

public class EnginxAndroid {
    static {
        System.loadLibrary("enginx-android");
    }

    public static native String loadConfig(String config);
    public static native String rewriteURL(String absoluteURLString);
}
