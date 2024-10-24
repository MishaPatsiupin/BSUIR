package com.example.home.adapter;

import android.content.Context;
import android.content.Intent;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.TextView;

import androidx.annotation.NonNull;

import com.example.home.DeviceDetailActivity;
import com.example.home.R;
import com.example.home.ui.device.Device;

import java.util.ArrayList;

public class DeviceAdapter extends ArrayAdapter<Device> {
    private final Context context;
    private final ArrayList<Device> devices;

    public DeviceAdapter(Context context, ArrayList<Device> devices) {
        super(context, 0, devices);
        this.context = context;
        this.devices = devices;
    }

    @NonNull
    @Override
    public View getView(int position, View convertView, @NonNull ViewGroup parent) {
        Device device = devices.get(position);

        if (convertView == null) {
            convertView = LayoutInflater.from(context).inflate(R.layout.device_item, parent, false);
        }

        TextView textView = convertView.findViewById(R.id.device_name);
        textView.setText(device.getName());

        // Добавление обработчика нажатия на элемент списка
        convertView.setOnClickListener(v -> {
            Intent intent = new Intent(context, DeviceDetailActivity.class);
            intent.putExtra("device_name", device.getName());
            intent.putExtra("device_type", device.getType()); // Передача типа устройства
            context.startActivity(intent);
        });

        return convertView;
    }
}