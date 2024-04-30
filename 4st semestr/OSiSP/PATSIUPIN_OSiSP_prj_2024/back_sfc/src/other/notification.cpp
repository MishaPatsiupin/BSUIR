#include "notification.h"


void send_notification(std::string &notification_channel, const std::string &message) {
    if (notification_channel == "system") {
        std::string cmd = "notify-send 'System File Control' '" + message + "'";
        system(cmd.c_str());
    }
}

void send_critical_urgency_notification(std::string &notification_channel, const std::string &message) {
    if (notification_channel == "system") {
        std::string cmd = "notify-send -u critical 'System File Control' '" + message + "'";
        system(cmd.c_str());
    }
}