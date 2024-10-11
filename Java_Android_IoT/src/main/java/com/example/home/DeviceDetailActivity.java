package com.example.home;

import android.os.Bundle;
import android.widget.TextView;



import androidx.appcompat.app.AppCompatActivity;

public class DeviceDetailActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_device_detail);

        String deviceName = getIntent().getStringExtra("device_name");
        if (deviceName == null) {
            deviceName = "Unknown Device";
        }

        TextView deviceNameTextView = findViewById(R.id.textView3);
        if (deviceNameTextView != null) {
            deviceNameTextView.setText(deviceName);
        }
    }
}