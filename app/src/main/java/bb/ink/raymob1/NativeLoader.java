package bb.ink.raymob1;

import android.app.NativeActivity;
import android.os.Bundle;
import android.view.KeyEvent;

public class NativeLoader extends NativeActivity {

    public DisplayManager displayManager;
    public SoftKeyboard softKeyboard;
    public boolean initCallback = false;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        displayManager = new DisplayManager(this);
        softKeyboard = new SoftKeyboard(this);
        System.loadLibrary("raymob1");
    }

    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        super.onWindowFocusChanged(hasFocus);
        if (BuildConfig.FEATURE_DISPLAY_IMMERSIVE && hasFocus) {
            displayManager.setImmersiveMode();
        }
    }

    @Override
    public boolean onKeyUp(int keyCode, KeyEvent event) {
        softKeyboard.onKeyUpEvent(event);
        return super.onKeyUp(keyCode, event);
    }

    @Override
    protected void onStart() {
        super.onStart();
        if (initCallback) {
            onAppStart();
        }
    }

    @Override
    protected void onResume() {
        super.onResume();
        if (initCallback) {
            onAppResume();
        }
    }

    @Override
    protected void onPause() {
        super.onPause();
        if (initCallback) {
            onAppPause();
        }
    }

    @Override
    protected void onStop() {
        super.onStop();
        if (initCallback) {
            onAppStop();
        }
    }

    private native void onAppStart();
    private native void onAppResume();
    private native void onAppPause();
    private native void onAppStop();
}