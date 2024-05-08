#ifndef BACK_NOTIFICATION_H
#define BACK_NOTIFICATION_H

// не должен включать никакие файлы

#include <iostream>
#include <string>
#include <cstdlib>

void send_notification(std::string &notification_channel, const std::string &message);

void send_critical_urgency_notification(std::string &notification_channel, const std::string &message);

#endif //BACK_NOTIFICATION_H
