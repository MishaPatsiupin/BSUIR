package com.example.home;

import android.gesture.Gesture;
import android.gesture.GestureLibraries;
import android.gesture.GestureLibrary;
import android.gesture.GestureOverlayView;
import android.gesture.Prediction;
import android.graphics.Color;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;

import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.JsonObjectRequest;
import com.android.volley.toolbox.Volley;
import com.github.mikephil.charting.charts.LineChart;
import com.github.mikephil.charting.components.Legend;
import com.github.mikephil.charting.data.Entry;
import com.github.mikephil.charting.data.LineData;
import com.github.mikephil.charting.data.LineDataSet;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.Locale;

public class DeviceDetailActivity extends AppCompatActivity {

    private TextView deviceNameTextView, temperatureTextView, humidityTextView, updateTimeTextView, waterPumpTextView, windowTextView, moisture1TextView, moisture2TextView, liquid_sensor_waterTextView, liquid_sensor_plantTextView, pressureTextView;
    private Button buttonToggleWaterPump, buttonToggleVentilation;
    private Handler handler = new Handler();
    private Runnable runnable;
    private String deviceName;
    private String deviceType; // Добавлено поле для типа устройства
    private RequestQueue requestQueue;
    // Для графиков
    private LineChart lineChart;
    private ArrayList<Entry> moisture1Data; // Для первой линии
    private ArrayList<Entry> moisture2Data; // Для второй линии

    private GestureLibrary gestureLib;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_device_detail);
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
            getWindow().setStatusBarColor(getResources().getColor(R.color.black));
        }

        deviceName = getIntent().getStringExtra("device_name");
        deviceType = getIntent().getStringExtra("device_type"); // Получаем тип устройства

        if (deviceName == null) {
            deviceName = "Unknown Device";
        }
        if (deviceType == null) {
            deviceName = "tepliza";
        }

        deviceNameTextView = findViewById(R.id.textView3);
        deviceNameTextView.setText(deviceName);

//        if (deviceType.equals("tepliza")) {

            updateTimeTextView = findViewById(R.id.updateTimeTextView);
            waterPumpTextView = findViewById(R.id.waterPumpTextView);
            windowTextView = findViewById(R.id.windowTextView);
            moisture1TextView = findViewById(R.id.moisture1TextView);
            moisture2TextView = findViewById(R.id.moisture2TextView);
            temperatureTextView = findViewById(R.id.temperatureTextView);
            humidityTextView = findViewById(R.id.humidityTextView);
            liquid_sensor_waterTextView = findViewById(R.id.liquid_sensor_waterTextView);
            liquid_sensor_plantTextView = findViewById(R.id.liquid_sensor_plantTextView);

            // Инициализация графика
            lineChart = findViewById(R.id.lineChart);
            moisture1Data = new ArrayList<>();
            moisture2Data = new ArrayList<>();
//        } else if (deviceType.equals("gradka")) {
//            lineChart = findViewById(R.id.lineChart);
//
//            updateTimeTextView = findViewById(R.id.updateTimeTextView);
//            temperatureTextView = findViewById(R.id.temperatureTextView);
//            humidityTextView = findViewById(R.id.humidityTextView);
//            moisture1TextView = findViewById(R.id.moisture1TextView);
            pressureTextView = findViewById(R.id.moisture2TextView);
//        }

        requestQueue = Volley.newRequestQueue(this);

        buttonToggleWaterPump = findViewById(R.id.buttonToggleWaterPump);
        buttonToggleVentilation = findViewById(R.id.buttonToggleVentilation);
        buttonToggleWaterPump.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                toggleWaterPump();
            }
        });

        buttonToggleVentilation.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                toggleVentilation();
            }
        });

        GestureOverlayView gestureview = (GestureOverlayView) findViewById(R.id.gestures3);

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
                if (prediction.score > 2.5) {
                    if (prediction.name.equals("ges_export")){
                        exportData();
                    }
                }
            }

        }
    };


    private void exportData() {
        StringBuilder exportData = new StringBuilder();
        try {
            exportData.append("Назва прылады: ").append(deviceName).append("\n");
            exportData.append("Тып прылады: ").append(deviceType).append("\n");

            if (deviceType.equals("tepliza")) {
                exportData.append("Вільготнасць 1: ").append(moisture1TextView.getText().toString()).append("\n");
                exportData.append("Вільготнасць 2: ").append(moisture2TextView.getText().toString()).append("\n");
                exportData.append("Тэмпература: ").append(temperatureTextView.getText().toString()).append("\n");
                exportData.append("Вільготнасць: ").append(humidityTextView.getText().toString()).append("\n");
                exportData.append("Сэнсар вадкасці (вада): ").append(liquid_sensor_waterTextView.getText().toString()).append("\n");
                exportData.append("Сэнсар вадкасці (расліна): ").append(liquid_sensor_plantTextView.getText().toString()).append("\n");
                exportData.append("Час абнаўлення: ").append(updateTimeTextView.getText().toString()).append("\n");
            } else if (deviceType.equals("gradka")) {
                exportData.append("Вільготнасць 1: ").append(moisture1TextView.getText().toString()).append("\n");
                exportData.append("Ціск: ").append(pressureTextView.getText().toString()).append("\n");
                exportData.append("Тэмпература: ").append(temperatureTextView.getText().toString()).append("\n");
                exportData.append("Вільготнасць: ").append(humidityTextView.getText().toString()).append("\n");
                exportData.append("Час абнаўлення: ").append(updateTimeTextView.getText().toString()).append("\n");
            }

            // Захоўваем дадзеныя ў файл
            saveDataToFile(exportData.toString());

        } catch (Exception e) {
            e.printStackTrace();
            Toast.makeText(this, "Памылка экспарту дадзеных", Toast.LENGTH_SHORT).show();
        }
    }

    private void saveDataToFile(String data) {
        String currentDateTime = new SimpleDateFormat("yyyyMMdd_HHmmss", Locale.getDefault()).format(new Date());
        String fileName = deviceName + "_" + currentDateTime + ".txt";

        File directory = Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOCUMENTS);
        File homeIoTDirectory = new File(directory, "home_iot");

        if (!homeIoTDirectory.exists()) {
            boolean wasCreated = homeIoTDirectory.mkdirs(); // Ствараем папку, калі яна не існуе
        }

        File file = new File(homeIoTDirectory, fileName);
        FileOutputStream fos = null;

        try {
            fos = new FileOutputStream(file);
            fos.write(data.getBytes());
            Toast.makeText(this, "Дадзеныя паспяхова захаваны ў " + file.getAbsolutePath(), Toast.LENGTH_SHORT).show();
        } catch (IOException e) {
            e.printStackTrace();
            Toast.makeText(this, "Памылка захавання дадзеных: " + e.getMessage(), Toast.LENGTH_SHORT).show();
        } finally {
            if (fos != null) {
                try {
                    fos.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    @Override
    protected void onStart() {
        super.onStart();
        startDataUpdates(); // Начинаем обновление данных
    }

    @Override
    protected void onStop() {
        super.onStop();
        handler.removeCallbacks(runnable); // Останавливаем обновление данных
    }

    private void startDataUpdates() {
        runnable = new Runnable() {
            @Override
            public void run() {
                fetchData();
                handler.postDelayed(this, 2000); // обновление каждые 2 секунды
            }
        };
        handler.post(runnable);
    }

    private void fetchData() {
        if(deviceType.equals("tepliza")) {
            fetchCurrentData();
            fetchStateData();
        } else if (deviceType.equals("gradka")) {
            fetchCurrentData();
        }
    }

    private void fetchCurrentData() {
        String url = "http://192.168.150.43:5000/" + deviceType + "/data"; // URL для получения данных с типом устройства

        JsonObjectRequest jsonObjectRequest = new JsonObjectRequest(Request.Method.GET, url, null,
                new Response.Listener<JSONObject>() {
                    @Override
                    public void onResponse(JSONObject response) {
                        try {
                            if (deviceType.equals("tepliza")) {
                                // Получаем массивы данных
                                JSONArray moisture1Graf = response.getJSONArray("moisture1_graf");
                                JSONArray moisture2Graf = response.getJSONArray("moisture2_graf");

                                // Очищаем данные перед добавлением новых
                                moisture1Data.clear();
                                moisture2Data.clear();

                                // Добавляем данные в график
                                for (int i = 0; i < moisture1Graf.length(); i++) {
                                    float moistureValue = (float) moisture1Graf.getDouble(i);
                                    moisture1Data.add(new Entry(i, moistureValue));
                                }

                                for (int i = 0; i < moisture2Graf.length(); i++) {
                                    float temperatureValue = (float) moisture2Graf.getDouble(i);
                                    moisture2Data.add(new Entry(i, temperatureValue));
                                }

                                updateChart();

                                String time = response.getString("this_time");

                                String moisture1 = response.getString("moisture1");
                                String moisture2 = response.getString("moisture2");

                                String temperature = response.getString("temperature");
                                String humidity = response.getString("humidity");

                                String liquid_sensor_water;
                                if (response.getBoolean("liquid_sensor_water")) {
                                    liquid_sensor_water = "ёсць";
                                } else {
                                    liquid_sensor_water = "няма";
                                }
                                String liquid_sensor_plant;
                                if (response.getBoolean("liquid_sensor_plant")) {
                                    liquid_sensor_plant = "ёсць";
                                } else {
                                    liquid_sensor_plant = "няма";
                                }

                                updateTimeTextView.setText("Час абнаўлення ->" + time);
                                moisture1TextView.setText("Віль. грунту 1 ->" + moisture1 + " %");
                                moisture2TextView.setText("Віль. грунту 2 ->" + moisture2 + " %");
                                temperatureTextView.setText("Температура ->" + temperature + " °C");
                                humidityTextView.setText("Віль. паветра ->" + humidity + " %");
                                liquid_sensor_waterTextView.setText("Жидкасць поліву ->" + liquid_sensor_water);
                                liquid_sensor_plantTextView.setText("Перелів ->" + liquid_sensor_plant);

                            } else if (deviceType.equals("gradka")) {
                                String time = response.getString("this_time");

                                String moisture1 = response.getString("moisture1");
                                String pressure = response.getString("pressure");
                                String temperature = response.getString("temperature");
                                String humidity = response.getString("humidity");


                                updateTimeTextView.setText("Час абнаўлення ->" + time);
                                moisture1TextView.setText("Віль. грунту 1 ->" + moisture1 + " %");
                                pressureTextView.setText("Давленіе ->" + pressure + " %");
                                temperatureTextView.setText("Температура ->" + temperature + " °C");
                                humidityTextView.setText("Віль. паветра ->" + humidity + " %");
                                liquid_sensor_waterTextView.setVisibility(View.GONE);
                                liquid_sensor_plantTextView.setVisibility(View.GONE);
                                lineChart.setVisibility(View.GONE);
                                windowTextView.setVisibility(View.GONE);
                                waterPumpTextView.setVisibility(View.GONE);
                                buttonToggleVentilation.setVisibility(View.GONE);
                                buttonToggleWaterPump.setVisibility(View.GONE);
                            }

                        } catch (JSONException e) {
                            e.printStackTrace();
                            Toast.makeText(DeviceDetailActivity.this, "Error parsing data", Toast.LENGTH_SHORT).show();
                        }
                    }
                },
                new Response.ErrorListener() {
                    @Override
                    public void onErrorResponse(VolleyError error) {
                        error.printStackTrace();
                        Toast.makeText(DeviceDetailActivity.this, "Error fetching data", Toast.LENGTH_SHORT).show();
                    }
                });

        requestQueue.add(jsonObjectRequest);
    }

    private void fetchStateData() {
        String url = "http://192.168.150.43:5000/" + deviceType + "/state"; // URL для получения состояния с типом устройства

        JsonObjectRequest jsonObjectRequest = new JsonObjectRequest(Request.Method.GET, url, null,
                new Response.Listener<JSONObject>() {
                    @Override
                    public void onResponse(JSONObject response) {
                        try {
                            if (deviceType.equals("tepliza")) {
                                boolean windowFlag = response.getBoolean("window_flag");
                                boolean waterPumpFlag = response.getBoolean("water_pomp_flag");

                                String windowState = windowFlag ? "Адкрыта" : "Зачынена";
                                String waterPumpState = waterPumpFlag ? "Уключана" : "Выключана";

                                windowTextView.setText("Стан вентыляцыі ->" + windowState);
                                waterPumpTextView.setText("Стан паліву ->" + waterPumpState);
                            } else {
                                windowTextView.setVisibility(View.GONE);
                                waterPumpTextView.setVisibility(View.GONE);
                                buttonToggleVentilation.setVisibility(View.GONE);
                                buttonToggleWaterPump.setVisibility(View.GONE);
                            }
                        } catch (JSONException e) {
                            e.printStackTrace();
                        }
                    }
                },
                new Response.ErrorListener() {
                    @Override
                    public void onErrorResponse(VolleyError error) {
                        if (error.networkResponse != null) {
                            int statusCode = error.networkResponse.statusCode;
                            String errorMessage = new String(error.networkResponse.data);
                            Log.e("Volley Error", "Status Code: " + statusCode + ", Message: " + errorMessage);
                        } else {
                            Log.e("Volley Error", "No network response: " + error.getMessage());
                        }
                        Toast.makeText(DeviceDetailActivity.this, "Error fetching data", Toast.LENGTH_SHORT).show();
                    }
                });

        requestQueue.add(jsonObjectRequest);
    }

    private void updateChart() {
        LineDataSet moistureDataSet = new LineDataSet(moisture1Data, "Віль. грунту 1"); // Название линии 1
        moistureDataSet.setColor(Color.BLUE);
        moistureDataSet.setValueTextColor(Color.BLUE); // Цвет текста значений

        LineDataSet temperatureDataSet = new LineDataSet(moisture2Data, "Віль. грунту 2"); // Название линии 2
        temperatureDataSet.setColor(Color.RED);
        temperatureDataSet.setValueTextColor(Color.RED); // Цвет текста значений

        // Установка цвета фона для легенды
        lineChart.getLegend().setEnabled(true); // Включение легенды
        lineChart.getLegend().setWordWrapEnabled(true); // Автообтекание текста
        lineChart.getLegend().setTextColor(Color.rgb(196, 196, 196)); // Цвет текста легенды
        //lineChart.getLegend().setForm(Legend.LegendForm.LINE); // Форма легенды
        lineChart.getDescription().setEnabled(false); // Отключить описание
        LineData lineData = new LineData(moistureDataSet, temperatureDataSet);
        lineChart.setData(lineData);
        lineChart.setBackgroundColor(Color.rgb(66, 66, 66));


        lineChart.invalidate(); // Обновление графика
    }

    private void toggleWaterPump() {
        String url = "http://192.168.150.43:5000/" + deviceType + "/set/pomp"; // URL для переключения состояния полива
        // Получите текущее состояние полива
        boolean currentState = waterPumpTextView.getText().toString().contains("Уключана");

        // Создание JSON для отправки на сервер
        JSONObject jsonBody = new JSONObject();
        try {
            jsonBody.put("state", !currentState); // Переключаем состояние
        } catch (JSONException e) {
            e.printStackTrace();
        }

        JsonObjectRequest jsonObjectRequest = new JsonObjectRequest(Request.Method.POST, url, jsonBody,
                new Response.Listener<JSONObject>() {
                    @Override
                    public void onResponse(JSONObject response) {
                        // Обработка ответа сервера
                        fetchStateData(); // Обновление состояния после переключения
                    }
                },
                new Response.ErrorListener() {
                    @Override
                    public void onErrorResponse(VolleyError error) {
                        Toast.makeText(DeviceDetailActivity.this, "Error changing water pump state", Toast.LENGTH_SHORT).show();
                    }
                });

        requestQueue.add(jsonObjectRequest);
    }

    private void toggleVentilation() {
        String url = "http://192.168.150.43:5000/" + deviceType + "/set/ventilation"; // URL для переключения состояния вентиляции
        // Получите текущее состояние вентиляции
        boolean currentState = windowTextView.getText().toString().contains("Адкрыта");

        // Создание JSON для отправки на сервер
        JSONObject jsonBody = new JSONObject();
        try {
            jsonBody.put("state", !currentState); // Переключаем состояние
        } catch (JSONException e) {
            e.printStackTrace();
        }

        JsonObjectRequest jsonObjectRequest = new JsonObjectRequest(Request.Method.POST, url, jsonBody,
                new Response.Listener<JSONObject>() {
                    @Override
                    public void onResponse(JSONObject response) {
                        // Обработка ответа сервера
                        fetchStateData(); // Обновление состояния после переключения
                    }
                },
                new Response.ErrorListener() {
                    @Override
                    public void onErrorResponse(VolleyError error) {
                        Toast.makeText(DeviceDetailActivity.this, "Error changing ventilation state", Toast.LENGTH_SHORT).show();
                    }
                });

        requestQueue.add(jsonObjectRequest);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        handler.removeCallbacks(runnable); // Остановить обновление при уничтожении активности
    }
}