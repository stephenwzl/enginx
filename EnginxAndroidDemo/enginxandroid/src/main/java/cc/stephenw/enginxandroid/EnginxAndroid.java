package cc.stephenw.enginxandroid;

/**
 * Created by stephenw on 2017/4/5.
 */

public class EnginxAndroid {
    public interface EnginxLoadHandler {
        void loadLibrary(String libraryName);
    }

    public static class EnginxDefaultHandler implements EnginxLoadHandler {
        @Override
        public void loadLibrary(String libraryName) {
            System.loadLibrary(libraryName);
        }
    }

    private static volatile EnginxLoadHandler defaultHandler = new EnginxDefaultHandler();

    public static void setLoadHandler(EnginxLoadHandler handler) {
        if (handler == null) {
            throw new NullPointerException("EnginxLoadHandler cannot be null");
        }
        defaultHandler = handler;
    }

    public static void loadLibrary() {
        defaultHandler.loadLibrary("enginx-android");
    }

    public static native String loadConfig(String config);
    public static native String rewriteURL(String absoluteURLString);
}
