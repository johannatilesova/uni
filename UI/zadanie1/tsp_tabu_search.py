import tkinter as tk
import random
import math
from copy import deepcopy
import time

POCET_MIEST = 25
ITERACIE = 1000
TABU_SIZE = 50
CAS = 0.02
CANVAS = 500

root = tk.Tk()
root.title("tsp - tabu search")

canvas_width, canvas_height = CANVAS, CANVAS

canvas = tk.Canvas(root, width=canvas_width, height=canvas_height)
canvas.pack()

#generovanie nahodnych mesteciek
def generuj_mesta(pocet_miest=POCET_MIEST):
    return[(random.randint(50, canvas_width - 50), random.randint(50, canvas_height - 50)) for i in range(pocet_miest)]

#vypocet vzdialenosti medzi vsetkymi mestami
def vypocet_vzdialenost(mesta):
    n = len(mesta)
    vzdialenosti = [[0] * n for i in range(n)]
    for i in range(n):
        for j in range(n):
            vzdialenosti[i][j] = math.sqrt((mesta[i][0] - mesta[j][0]) ** 2 + (mesta[i][1] - mesta[j][1]) ** 2)
    return vzdialenosti

#vypocet celkovej dlzky trasy
def celkova_dlzka(trasa, vzdialenosti):
    return sum(vzdialenosti[trasa[i]][trasa[i+1]]
               for i in range (len(trasa) - 1)) + vzdialenosti[trasa[-1]][trasa[0]]

#vymena miest v trase
def two_opt_swap(trasa, i, j):
    nova_trasa = trasa[:i] + trasa[i:j+1][::-1] + trasa[j+1:]
    return nova_trasa

#aktualizacia vykreslovania
def kresli(mesta, trasa):
    canvas.delete("cesta")
    for i in range(len(trasa) - 1):
        canvas.create_line(mesta[trasa[i]][0], mesta[trasa[i]][1],
                           mesta[trasa[i + 1]][0], mesta[trasa[i + 1]][1],
                           fill="blue", width=2, tags="cesta")
    canvas.create_line(mesta[trasa[-1]][0], mesta[trasa[-1]][1],
                       mesta[trasa[0]][0], mesta[trasa[0]][1],
                       fill="blue", width=2, tags="cesta")

    # Označenie miest písmenami abecedy
    for index, mesto in enumerate(mesta):
        x, y = mesto
        canvas.create_text(x, y - 15, text=f"{chr(65 + index)}")
    root.update()


#tabu search
def tabu_search(mesta, max_iteracii = ITERACIE, tabu_velkost = TABU_SIZE, pauza = CAS, iteracie_bez_zmeny = 1):
    vzialenosti = vypocet_vzdialenost(mesta)
    aktualne_riesenie = list(range(len(mesta)))
    random.shuffle(aktualne_riesenie)
    najlepsie_riesenie = deepcopy(aktualne_riesenie)
    najlepsia_cena = celkova_dlzka(najlepsie_riesenie, vzialenosti)
    tabu_mnozina = set()

    pocet_bez_zmeny = 0

    for iteracia in range(max_iteracii):
        susedia = []

        #generovanie susednych miest pomocou 2-opt swap
        for i in range(len(aktualne_riesenie)):
            for j in range(i+1, len(aktualne_riesenie)):
                sused = two_opt_swap(aktualne_riesenie, i, j)
                cena = celkova_dlzka(sused, vzialenosti)
                if (i,j) not in tabu_mnozina or cena < najlepsia_cena:
                    susedia.append((sused, cena))

        if not susedia:
            break

        #vyber najlepsieho suseda
        najlepsi_sused = min(susedia, key = lambda x: x[1])
        aktualne_riesenie = najlepsi_sused[0]

        #aktualizácia tabu zoznamu
        tabu_mnozina.add((aktualne_riesenie.index(najlepsi_sused[0][0]), aktualne_riesenie.index(najlepsi_sused[0][1])))
        if len(tabu_mnozina) > tabu_velkost:
            tabu_mnozina.pop()

        aktualna_cena = najlepsi_sused[1]
        if aktualna_cena < najlepsia_cena:
            najlepsie_riesenie = deepcopy(aktualne_riesenie)
            najlepsia_cena = aktualna_cena
            pocet_bez_zmeny = 0
        else:
            pocet_bez_zmeny += 1

        #koniec ak sa ta ista iteracia X-ty krat opakuje
        if pocet_bez_zmeny >= iteracie_bez_zmeny:
            break

        #zobrazovanie priebeznych vysledkov
        print(f"iteracia {iteracia + 1}: dlzka trasy = {aktualna_cena:.2f} km")
        kresli(mesta, aktualne_riesenie)
        time.sleep(pauza)

    return najlepsie_riesenie, najlepsia_cena

# TSP pomocou tabu search
def tsp_tabu(mesta):
    if not mesta:
        return
    najlepsia_trasa, najlepsia_vzdialenost = tabu_search(mesta)
    print(f"\nKonečná dĺžka trasy {najlepsia_vzdialenost:.2f} km")

    # Výpis súradníc
    print("\nTrasa ide cez mestá:")
    for i in range(len(najlepsia_trasa)):
        mesto_index = najlepsia_trasa[i]
        dalsie_mesto_index = najlepsia_trasa[(i + 1) % len(najlepsia_trasa)]
        mesto = mesta[mesto_index]
        dalsie_mesto = mesta[dalsie_mesto_index]
        vzdialenost = math.sqrt((mesto[0] - dalsie_mesto[0]) ** 2 + (mesto[1] - dalsie_mesto[1]) ** 2)

        # Získanie písmenka pre aktuálne mesto a ďalšie mesto
        mesto_pismeno = chr(65 + mesto_index)  # A, B, C, ...
        dalsie_mesto_pismeno = chr(65 + dalsie_mesto_index)  # A, B, C, ...

        print(f"{mesto_pismeno} {mesto} --> {dalsie_mesto_pismeno} {dalsie_mesto} = {vzdialenost:.2f} km")

    # Zobrazenie trasy na plátne
    for i in range(len(najlepsia_trasa) - 1):
        canvas.create_line(mesta[najlepsia_trasa[i]][0], mesta[najlepsia_trasa[i]][1],
                           mesta[najlepsia_trasa[i + 1]][0], mesta[najlepsia_trasa[i + 1]][1],
                           fill="blue", width=2)

    # Prepojenie späť na začiatok
    canvas.create_line(mesta[najlepsia_trasa[-1]][0], mesta[najlepsia_trasa[-1]][1],
                       mesta[najlepsia_trasa[0]][0], mesta[najlepsia_trasa[0]][1], fill="blue", width=2)

#generovanie miest
mesta = generuj_mesta()
for x, y in mesta:
    canvas.create_rectangle(x - 5, y - 5, x + 5, y + 5, fill = "pink")

tsp_tabu(mesta)
root.mainloop()
