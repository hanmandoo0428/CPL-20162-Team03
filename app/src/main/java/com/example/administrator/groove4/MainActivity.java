package com.example.administrator.groove4;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

public class MainActivity extends Activity implements OnClickListener {
    // Debugging
    private static final String TAG = "Main";
    // Intent request code
    private static final int REQUEST_CONNECT_DEVICE = 1;
    private static final int REQUEST_ENABLE_BT = 2;
    // Layout
    private Button btn_Connect,send;
    private TextView txt_Result;
    private EditText entry;
    private BluetoothService btService = null;


        private final Handler mHandler = new Handler() {

            @Override
            public void handleMessage(Message msg) {
                super.handleMessage(msg);
            }

    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Log.e(TAG, "onCreate");

        setContentView(R.layout.activity_main);

        btn_Connect = (Button) findViewById(R.id.btn_connect);
        txt_Result = (TextView) findViewById(R.id.txt_result);
        send = (Button)findViewById(R.id.send);
        entry = (EditText)findViewById(R.id.entry);

        btn_Connect.setOnClickListener(this);

        // BluetoothService 클래스 생성
        if(btService == null) {
            btService = new BluetoothService(this, mHandler);
        }
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()){
            case R.id.btn_connect:
                if(btService.getDeviceState()) {
                    Log.d(TAG, "shl-go to getDeviceState()");
                    // 블루투스가 지원 가능한 기기일 때
                    btService.enableBluetooth();
                    Log.d(TAG, "shl-go to enableBluetooth()");
                } else {
                    finish();
                }
                break;
            case R.id.send:
                break;


        }

    }

    public void onActivityResult(int requestCode, int resultCode, Intent data) {
        Log.d(TAG, "shl-onActivityResult " + resultCode);

        switch (requestCode) {
            case REQUEST_CONNECT_DEVICE:
                // When DeviceListActivity returns with a device to connect
                if (resultCode == Activity.RESULT_OK) {
                    btService.getDeviceInfo(data);
                }
                break;
            case REQUEST_ENABLE_BT:
                // When the request to enable Bluetooth returns
                if (resultCode == Activity.RESULT_OK) {
                    // Next Step
                    btService.scanDevice(); // 권한요청 허락이 떨어지면 블루투스 장치 탐색
                } else {

                    Log.d(TAG, "shl-Bluetooth is not enabled");
                    Toast.makeText(this,"연결불가",Toast.LENGTH_SHORT).show();
                }
                break;
        }
    }

}
