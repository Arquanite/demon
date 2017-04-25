#include "help.h"
#include <stdio.h>

void help(bool full){
    printf("Poprawne użycie programu:\n\n");
    printf(" (1) demon <ścieżka źródłowa> <ścieżka docelowa> [opcje]\n");
    printf(" (2) demon <opcja>\n\n");

    printf("Jeżeli chcesz uruchomić demona i rozpocząć synchronizację, użyj rozwiązania (1).\n");
    printf("Jeżeli chcesz wymusić synchronizację lub zatrzymać uruchomionego demona, użyj rozwiązania (2).\n");

    if(!full){
        printf("\nWyświetlenie pomocy: demon -h\n");
        return;
    }

    printf("\nOpis opcji stosowanych w (1) przypadku użycia:\n\n");
    help_params();

    printf("\nOpis opcji stosowanych w (2) przypadku użycia:\n\n");
    help_actions();
}

void help_params(){
    printf(" -R : kopiowanie rekursywne\n\n");

    printf(" -s <liczba>[jednostka] : czas uśpienia demona, jednostki:\n");
    printf("  [ brak ] : to samo co s\n");
    printf("  s : sekundy\n");
    printf("  m : minuty\n");
    printf("  h : godziny\n\n");

    printf(" -m <liczba>[jednostka] : pliki tego rozmiaru i większe będą kopiowane za pomocą mmap, jednostki:\n");
    printf("  [ brak ] : bajty\n");
    printf("  k : kilobajty \n");
    printf("  m : megabajty\n");
    printf("  g : gigabajty\n");
}

void help_actions(){
    printf(" -F : powoduje natychmiastowe wybudzenie demona (synchronizację)\n");
    printf(" -S : bezpieczne zakończenie pracy demona (kończy synchronizację, jeśli rozpoczęta)\n");
    printf(" -K : natychmiastowe zakończenie pracy demona.\n");
}
