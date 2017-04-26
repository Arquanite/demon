#!/bin/bash
echo "Tworzenie katalogu źródłowego i docelowego"
mkdir ~/source ~/destination

echo "Tworzene drzewa plików do synchronizacji"
mkdir ~/source/a ~/source/b ~/source/b/bb ~/source/c
touch ~/source/jedenplik ~/source/a/drugiplik ~/source/b/trzeciplik

echo "Uruchamiam daemona, z rekursywną synchronizacją co 10 sekund"
demon -R -s 10 ~/source ~/destination

echo "Porównuję katalog źródłowy z docelowym: "
diff ~/source ~/destination
