package com.example.home.ui.device;

import org.json.JSONObject;

public class Device {
    private String name;
    private String type;

    private String SSID;
    private String password;

    private JSONObject data;

    public Device(String name, String type) {
        this.name = name;
        this.type = type;
    }


    public String getName() {
        return name;
    }

    public String getType() {
        return type;
    }
}
