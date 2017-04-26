# Demon synchronizujący dwa katalogi (na przykład A i B)
Poprawne użycie programu:

 (1) ```demon <ścieżka źródłowa> <ścieżka docelowa> [opcje]```
 
 (2) ```demon <opcja>```

Jeżeli chcesz uruchomić demona i rozpocząć synchronizację, użyj rozwiązania (1).
Jeżeli chcesz wymusić synchronizację lub zatrzymać uruchomionego demona, użyj rozwiązania (2).
Jeżeli chcesz wyświetlić pomoc wpisz ``` demon -h ```

Opis opcji stosowanych w (1) przypadku użycia:

 -R : kopiowanie rekursywne

 ```-s <liczba>[jednostka]``` : czas uśpienia demona, jednostki (domyślnie sekundy):
 * s : sekundy
 * m : minuty
 * h : godziny

 ```-m \<liczba>[jednostka]``` : pliki tego rozmiaru i większe będą kopiowane za pomocą mmap, jednostki (domyślnie bajty):
 * k : kilobajty 
 * m : megabajty
 * g : gigabajty

Opis opcji stosowanych w (2) przypadku użycia:

 * -F : powoduje natychmiastowe wybudzenie demona (synchronizację)
 * -S : bezpieczne zakończenie pracy demona (kończy synchronizację, jeśli rozpoczęta)
 * -K : natychmiastowe zakończenie pracy demona.
