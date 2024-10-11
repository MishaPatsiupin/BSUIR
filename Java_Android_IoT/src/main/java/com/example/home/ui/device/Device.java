package com.example.home.ui.device;

import org.json.JSONObject;

public class Device {
    private String name;
    private String type;

    private String SSID;
    private String password;

    private JSONObject data;

    public Device(String name) {
        this.name = name;
    }

    public String getName() {
        return name;
    }
}
