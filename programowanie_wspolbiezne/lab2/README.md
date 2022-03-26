# client.cpp
1. Z linii poleceń pobiera nazwę pliku klienta
2. Sprawdza czy istnieje lockfile, jeśli nie to tworzy lockfile, który blokuje innych klientów
3. Wczytuje kolejne linie danych dopóki teskt linii jest różny od "Q", następnie te dane są zapisywane w buforze serwera
4. Oczekuje odpowiedzi od serwera, po której usuwa plik klienta
# server.cpp
1. Sprawdza czy istnieje bufor serwera, jeśli tak to wyświetla go
2. Właściciel serwera wpisuje odpowiedź, końcem pliku jest linia "Q"
3. Jest tworzony plik klienta, do którego jest zapisywana odpowiedź
4. Bufor serwera jest usuwany i lockfile jest usuwany
