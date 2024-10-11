// MainActivity.java
package com.example.home;

import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.os.Parcelable;
import android.view.Menu;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;
import androidx.drawerlayout.widget.DrawerLayout;
import androidx.navigation.NavController;
import androidx.navigation.Navigation;
import androidx.navigation.ui.AppBarConfiguration;
import androidx.navigation.ui.NavigationUI;

import com.example.home.adapter.DeviceAdapter;
import com.example.home.databinding.ActivityMainBinding;
import com.example.home.ui.device.Device;
import com.google.firebase.Firebase;
import com.google.firebase.auth.FirebaseAuth;
import com.google.gson.Gson;
import com.google.gson.reflect.TypeToken;

import java.lang.reflect.Type;
import java.util.ArrayList;

public class MainActivity extends AppCompatActivity {

    private AppBarConfiguration mAppBarConfiguration;
    private ArrayList<Device> deviceList;
    private DeviceAdapter adapter;
    private static final String PREFS_NAME = "device_prefs";
    private static final String DEVICE_LIST_KEY = "device_list";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        ActivityMainBinding binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        setSupportActionBar(binding.appBarMain.toolbar);

        DrawerLayout drawer = binding.drawerLayout;
        mAppBarConfiguration = new AppBarConfiguration.Builder(R.id.nav_home)
                .setOpenableLayout(drawer)
                .build();
        NavController navController = Navigation.findNavController(this, R.id.nav_host_fragment_content_main);
        NavigationUI.setupActionBarWithNavController(this, navController, mAppBarConfiguration);
        NavigationUI.setupWithNavController(binding.navView, navController);

        ListView deviceListView = binding.navView.findViewById(R.id.device_list);
        Button addDeviceButton = binding.navView.findViewById(R.id.add_device);

        loadDeviceList(); // Загружаем спіс прылад пры стварэнні актыўнасці

        adapter = new DeviceAdapter(this, deviceList);
        deviceListView.setAdapter(adapter);

        addDeviceButton.setOnClickListener(v -> addDevice());

        deviceListView.setOnItemClickListener((parent, view, position, id) -> {
            Device device = deviceList.get(position);
            Intent intent = new Intent(MainActivity.this, DeviceDetailActivity.class);
            intent.putExtra("device_name", device.getName());
            startActivity(intent);
        });

        deviceListView.setOnItemLongClickListener((parent, view, position, id) -> {
            showDeleteConfirmationDialog(position);
            return true;
        });
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }

    @Override
    protected void onPause() {
        super.onPause();
        saveDeviceList(); // Захоўваем спіс перад прыпыненнем актыўнасці
    }

    private void saveDeviceList() {
        SharedPreferences sharedPreferences = getSharedPreferences(PREFS_NAME, MODE_PRIVATE);
        SharedPreferences.Editor editor = sharedPreferences.edit();
        Gson gson = new Gson();
        String json = gson.toJson(deviceList);
        String userKey = getCurrentUserKey();
        editor.putString(userKey, json);
        editor.apply();
    }

    private void loadDeviceList() {
        SharedPreferences sharedPreferences = getSharedPreferences(PREFS_NAME, MODE_PRIVATE);
        Gson gson = new Gson();
        String userKey = getCurrentUserKey();
        String json = sharedPreferences.getString(userKey, null);
        Type type = new TypeToken<ArrayList<Device>>() {}.getType();
        deviceList = gson.fromJson(json, type);

        if (deviceList == null) {
            deviceList = new ArrayList<>(); // Калі дадзеных няма, ствараем новы спіс
        }
    }

    private String getCurrentUserKey() {
        String userId = FirebaseAuth.getInstance().getCurrentUser().getUid();
        return DEVICE_LIST_KEY + "_" + userId;
    }

    public void addDevice() {
        EditText deviceNameInput = new EditText(this);

        new AlertDialog.Builder(this)
                .setMessage(getString(R.string.enter_device_name))
                .setView(deviceNameInput)
                .setPositiveButton(getString(R.string.add), (dialog, which) -> {
                    String newDeviceName = deviceNameInput.getText().toString().trim();
                    if (!newDeviceName.isEmpty()) {
                        boolean exists = false;
                        for (Device device : deviceList) {
                            if (device.getName().equals(newDeviceName)) {
                                exists = true;
                                break;
                            }
                        }

                        if (exists) {
                            Toast.makeText(this, getString(R.string.device_name_exists), Toast.LENGTH_SHORT).show();
                        } else {
                            Device newDevice = new Device(newDeviceName);
                            deviceList.add(newDevice);
                            adapter.notifyDataSetChanged();
                            Toast.makeText(this, getString(R.string.added_device, newDeviceName), Toast.LENGTH_SHORT).show();
                        }
                    } else {
                        Toast.makeText(this, getString(R.string.device_name_empty), Toast.LENGTH_SHORT).show();
                    }
                })
                .setNegativeButton(getString(R.string.cancel), (dialog, which) -> dialog.cancel())
                .show();
    }

    private void showDeleteConfirmationDialog(int position) {
        new AlertDialog.Builder(this)
                .setTitle("Выдаліць прыладу")
                .setMessage("Вы ўпэўнены, што хочаце выдаліць гэтую прыладу?")
                .setPositiveButton("Так", (dialog, which) -> deleteDevice(position))
                .setNegativeButton("Не", null)
                .show();
    }

    private void deleteDevice(int position) {
        deviceList.remove(position);
        adapter.notifyDataSetChanged();
        saveDeviceList();
    }

    @Override
    public boolean onSupportNavigateUp() {
        NavController navController = Navigation.findNavController(this, R.id.nav_host_fragment_content_main);
        return NavigationUI.navigateUp(navController, mAppBarConfiguration)
                || super.onSupportNavigateUp();
    }
}