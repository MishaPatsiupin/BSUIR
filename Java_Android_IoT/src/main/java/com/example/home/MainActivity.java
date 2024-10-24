// MainActivity.java
package com.example.home;

import android.content.Intent;
import android.content.SharedPreferences;
import android.gesture.Gesture;
import android.gesture.GestureLibraries;
import android.gesture.GestureLibrary;
import android.gesture.GestureOverlayView;
import android.gesture.Prediction;
import android.os.Build;
import android.os.Bundle;
import android.os.Parcelable;
import android.view.Menu;
import android.widget.Button;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.ListView;
import android.widget.TextView;
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
import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.auth.FirebaseUser;
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

    private GestureLibrary gestureLib;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
            getWindow().setStatusBarColor(getResources().getColor(R.color.black));
        }

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

        // Получаем текущего пользователя
        FirebaseUser currentUser = FirebaseAuth.getInstance().getCurrentUser();
        if (currentUser != null) {
            String email = currentUser.getEmail();

            // Устанавливаем почту в TextView
            TextView emailTextView = findViewById(R.id.textView);
            if (emailTextView != null) {//почемуто нулл
                emailTextView.setText(email);
            }
        }

        GestureOverlayView gestureview = (GestureOverlayView) findViewById(R.id.gestures1);

        gestureLib = GestureLibraries.fromRawResource(this, R.raw.gestures);
        if (!gestureLib.load()) {
            finish();
        }

        gestureview.addOnGesturePerformedListener(handleGestureListener);
    }

    private GestureOverlayView.OnGesturePerformedListener handleGestureListener = new GestureOverlayView.OnGesturePerformedListener() {
        public void onGesturePerformed(GestureOverlayView gestureView,
                                       Gesture gesture) {

            ArrayList<Prediction> predictions = gestureLib.recognize(gesture);

            if (predictions.size() > 0) {
                Prediction prediction = predictions.get(0);
                if (prediction.score > 5.0) {
                    Toast.makeText(MainActivity.this,
                            "Жэст : " + prediction.name,
                            Toast.LENGTH_LONG).show();
                    if (prediction.name.equals("ges_add")){
                        addDevice();
                    } else if (prediction.name.equals("ges_del")) {
                        gesDevice();
                    }
                }
            }

        }
    };

    private void gesDevice() {
        // Создаем EditText для ввода названия устройства
        EditText deviceNameInput = new EditText(this);
        deviceNameInput.setHint("назва");

        new AlertDialog.Builder(this)
                .setTitle("Выдаленне прылады")
                .setMessage("Каб выдаліць прыладу, увядзіце яе назву:")
                .setView(deviceNameInput)
                .setPositiveButton("Выдаліць", (dialog, which) -> {
                    String deviceNameToDelete = deviceNameInput.getText().toString().trim();
                    // Поиск устройства в списке
                    boolean found = false;
                    for (Device device : deviceList) {
                        if (device.getName().equals(deviceNameToDelete)) {
                            found = true;
                            deleteDevice(deviceList.indexOf(device)); // Удаляем устройство
                            Toast.makeText(this, "Прылада " + deviceNameToDelete + " выдалена", Toast.LENGTH_SHORT).show();
                            break;
                        }
                    }
                    if (!found) {
                        Toast.makeText(this, "Прылада з такой назвай не знойдзена", Toast.LENGTH_SHORT).show();
                    }
                })
                .setNegativeButton("Скасаванне", (dialog, which) -> dialog.cancel())
                .show();
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
        // Создаем LinearLayout для размещения EditText
        LinearLayout layout = new LinearLayout(this);
        layout.setOrientation(LinearLayout.VERTICAL);

        EditText deviceNameInput = new EditText(this);
        deviceNameInput.setHint(getString(R.string.enter_device_name));

        EditText deviceTypeInput = new EditText(this);
        deviceTypeInput.setHint(getString(R.string.enter_device_type));

        layout.addView(deviceNameInput);
        layout.addView(deviceTypeInput);

        new AlertDialog.Builder(this)
                .setTitle(getString(R.string.add_device)) // Заголовок диалога
                .setView(layout)
                .setPositiveButton(getString(R.string.add), (dialog, which) -> {
                    String newDeviceName = deviceNameInput.getText().toString().trim();
                    String newDeviceType = deviceTypeInput.getText().toString().trim();

                    if (!newDeviceName.isEmpty() && !newDeviceType.isEmpty()) {
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
                            Device newDevice = new Device(newDeviceName, newDeviceType); // Предполагается, что конструктор Device принимает тип
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