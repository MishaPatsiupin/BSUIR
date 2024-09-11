#include "procces.h"

void printUsage() {
    std::cout << "Usage:\n";
    std::cout << "  --hibernation  -h   Execute hibernation\n";
    std::cout << "  --shutdown     -s   Execute shutdown\n";
    std::cout << "  *none flags*        Display power information\n";
}

int main(int argc, char *argv[]) {
    if (argc > 1) {
        std::vector<std::string> flags(argv + 1, argv + argc);
        for (const auto &flag: flags) {
            if (flag == "--hibernation" || flag == "-h") {
                hibernation();
                return 0;
            } else if (flag == "--shutdown" || flag == "-s") {
                shutdown();
                return 0;
            } else {
                std::cout << "Unknown flag: " << flag << "\n";
                printUsage();
                return 1;
            }
        }
    } else {
        displayPowerInfo();
    }

    return 0;
}