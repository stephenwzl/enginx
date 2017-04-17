package cc.stephenw.enginxandroiddemo;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.TextView;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.UnsupportedEncodingException;

import cc.stephenw.enginxandroid.EnginxAndroid;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        EnginxAndroid.loadLibrary();
        setContentView(R.layout.activity_main);
        String result = EnginxAndroid.loadConfig(this.configString());
        TextView t = (TextView) findViewById(R.id.enginx_hello);
        t.setText(result.isEmpty() ? "config load success" : result);
        String rewrited = EnginxAndroid.rewriteURL("https://h5.ele.me/shop/#geohash=wtw3dn0w04zkbhsfnqnh3c&id=166657");
        t.setText(rewrited);
    }

    protected String configString() {
        InputStream inputStream = getResources().openRawResource(R.raw.enginx_config);
        InputStreamReader inputStreamReader = null;
        try {
            inputStreamReader = new InputStreamReader(inputStream, "utf-8");

        } catch (UnsupportedEncodingException e1) {
            e1.printStackTrace();
        }
        BufferedReader reader = new BufferedReader(inputStreamReader);
        StringBuffer sb = new StringBuffer("");
        String line;
        try {
            while ((line = reader.readLine()) != null) {
                sb.append(line);
                sb.append("\n");
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        return sb.toString();
    }
}
