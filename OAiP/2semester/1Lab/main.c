#include "Hurra_Die_Weit_Geht_Unter.h"
//https://5element.by/catalog/581-tostery
//c-text



int main() {
    pToaster toast;
    toast = (pToaster) calloc(amount, sizeof(Toaster));

    baza(toast);

    void (*func[5])(pToaster) = {print, sort, add, change, remove_};
    int kolichestvo = 5;
    int choice = -1;
    while (choice != 0) {
        print_menu();
        choice = get_choice(kolichestvo);
        if (choice != 0) {
            func[choice - 1](toast);
        }
    }

    fflush(stdin);
    getchar();
    free(toast);
    return 0;
}
