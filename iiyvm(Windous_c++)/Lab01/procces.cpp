
#include "procces.h"


void hibernation() {
    SetSuspendState(FALSE, FALSE, FALSE);
}

void shutdown() {
    SetSuspendState(TRUE, FALSE, FALSE);
}

int extractChemistryValue(const std::string &input) {
    std::istringstream stream(input);
    std::string line;

    std::getline(stream, line);

    std::getline(stream, line);

    // Убираем лишние пробелы и преобразуем в int
    try {
        return std::stoi(line); // Преобразуем строку в целое число
    } catch (const std::invalid_argument &) {
        return -1; // Возвращаем -1 в случае ошибки преобразования
    } catch (const std::out_of_range &) {
        return -1; // Возвращаем -1 в случае переполнения
    }
}

std::string executeCommand(const std::string &command) {
    char buffer[128];
    std::string result;

    // Открываем процесс для выполнения команды
    FILE *pipe = popen(command.c_str(), "r");
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }

    // Читаем вывод команды
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }

    pclose(pipe);
    return result;
}

std::string GetChemistryInfo() {
    std::string command = "wmic path Win32_Battery get Chemistry";
    std::string output = executeCommand(command);

    int chemistryValue = extractChemistryValue(output);
//    return chemistryValue;

    if (chemistryValue == 1) return "NiCd (Nickel-Cadmium)\n";
    if (chemistryValue == 2) return "Li-poly (Lithium-Polymer)\n";
    if (chemistryValue == 3) return "Li-ion (Lithium-Ion)\n";
    if (chemistryValue == 4) return "NiMH (Nickel-Metal Hydride)\n";
    if (chemistryValue == 5) return "Li-Fe (Lithium Iron Phosphate)\n";
    if (chemistryValue == 6) return "Lead Acid\n";
    if (chemistryValue == 7) return "Nickel-Metal Hydride (Chargeable)\n";

    return "Unknown Chemistry";
}

std::string updateStatus(std::atomic<bool> &acLineStatus, int &timer) {
    if (acLineStatus.load()) {
        timer = 0;
        std::cout << "The laptop is powered from the mains";
        return "The laptop is powered from the mains";
    } else {
        timer += 1;
        std::cout << "The laptop is already unplugged for " << timer << " sec." << std::endl;
        return std::to_string(timer);
    }
}


void displayPowerInfo() {
    SYSTEM_POWER_STATUS status;
    std::atomic<bool> acLineStatus(true);
    int timer = 0;

    while (true) {
        if (GetSystemPowerStatus(&status)) {
            system("cls");

            acLineStatus = (status.ACLineStatus == 1);

            std::cout << "Power Source: ";
            if (status.ACLineStatus == 0) {
                std::cout << "Battery\n";
            } else if (status.ACLineStatus == 1) {
                std::cout << "AC Power\n";
            }

            std::cout << "Battery Chemistry: " << GetChemistryInfo();

            std::cout << "Battery Life: ";
            if (status.BatteryLifePercent != 255) {
                std::cout << static_cast<int>(status.BatteryLifePercent) << "%\n";
            } else {
                std::cout << "Unknown\n";
            }

            std::cout << "Power Saving Mode: ";
            if (status.SystemStatusFlag == 1) {
                std::cout << "ON\n";
            } else {
                std::cout << "OFF\n";
            }

            std::cout << "Battery Life Time: ";
            if (status.BatteryLifeTime != -1) {
                std::cout << "EST " << status.BatteryLifeTime / 60 << " minutes\n";
            } else {
                std::cout << "AC Power\n";
            }
            updateStatus(acLineStatus, timer);
        } else {
            std::cerr << "Failed to get power status\n";
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
