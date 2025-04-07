import tkinter as tk
import random
import math
import time

POCET_MIEST = 25
ITERACIE = 1000
START_TEMP = 30
END_TEMP = 0.01
ALPHA = 0.995
CANVAS = 500
CAS = 0.005

root = tk.Tk()
root.title("TSP - Simulated Annealing")

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

# 2-opt swap
def two_opt_swap(trasa, i, j):
    if j < i:  # Ak je j menšie ako i, prehodíme ich
        i, j = j, i
    # Vráti novú trasu, kde je časť trasy medzi i a j obrátená
    return trasa[:i] + trasa[i:j + 1][::-1] + trasa[j + 1:]

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

    for index, mesto in enumerate(mesta):
        x, y = mesto
        canvas.create_text(x, y - 15, text=f"{chr(65 + index)}")
    root.update()

# Simulated Annealing algoritmus
def simulated_annealing(mesta, start_temp=START_TEMP, end_temp=END_TEMP, alpha=ALPHA, max_iter=ITERACIE):
    vzdialenosti = vypocet_vzdialenost(mesta)
    aktualne_riesenie = list(range(len(mesta)))
    random.shuffle(aktualne_riesenie)
    najlepsie_riesenie = aktualne_riesenie[:]
    najlepsia_cena = celkova_dlzka(najlepsie_riesenie, vzdialenosti)

    teplota = start_temp
    aktualna_cena = celkova_dlzka(aktualne_riesenie, vzdialenosti)
    max_iteracie_bez_zlepsenia = 1000
    iteracie_bez_zlepsenia = 0

    for iteracia in range(max_iter):
        # Generovanie susedného riešenia
        i, j = random.sample(range(len(aktualne_riesenie)), 2)

        # Použitie 2-opt swap
        novy_sused = two_opt_swap(aktualne_riesenie, i, j)
        nova_cena = celkova_dlzka(novy_sused, vzdialenosti)

        # Ak je nové riešenie lepšie, akceptujeme ho
        if nova_cena < aktualna_cena or random.random() < math.exp((aktualna_cena - nova_cena) / teplota):
            aktualne_riesenie = novy_sused
            aktualna_cena = nova_cena
            if aktualna_cena < najlepsia_cena:
                najlepsie_riesenie = aktualne_riesenie[:]
                najlepsia_cena = aktualna_cena
                iteracie_bez_zlepsenia = 0
            else:
                iteracie_bez_zlepsenia += 1
        else:
            iteracie_bez_zlepsenia += 1

        # Znižovanie teploty
        teplota *= alpha

        # Zobrazovanie priebežných výsledkov
        if iteracia % 10 == 0:  # Zobrazujeme každých 10 iterácií
            print(f"Iterácia {iteracia + 1}: dĺžka trasy = {najlepsia_cena:.2f} km, teplota: {teplota:.2f}")
            kresli(mesta, aktualne_riesenie)
            time.sleep(CAS)

        # zastavi sa, ak iteracie dosiahnu maximum bez zlepsenia
        if iteracie_bez_zlepsenia >= max_iteracie_bez_zlepsenia:
            break

    return najlepsie_riesenie, najlepsia_cena

# TSP s pouzitim simulated annealing
def tsp_simulated_annealing(mesta):
    if not mesta:
        return
    najlepsia_trasa, najlepsia_vzdialenost = simulated_annealing(mesta)
    print(f"\nKonečná dĺžka trasy {najlepsia_vzdialenost:.2f} km")

    # Výpis súradníc
    print("\nTrasa ide cez mestá:")
    for i in range(len(najlepsia_trasa)):
        mesto_index = najlepsia_trasa[i]
        dalsie_mesto_index = najlepsia_trasa[(i + 1) % len(najlepsia_trasa)]
        mesto = mesta[mesto_index]
        dalsie_mesto = mesta[dalsie_mesto_index]
        vzdialenost = math.sqrt((mesto[0] - dalsie_mesto[0]) ** 2 + (mesto[1] - dalsie_mesto[1]) ** 2)

        mesto_pismeno = chr(65 + mesto_index)
        dalsie_mesto_pismeno = chr(65 + dalsie_mesto_index)

        print(f"{mesto_pismeno} {mesto} --> {dalsie_mesto_pismeno} {dalsie_mesto} = {vzdialenost:.2f} km")

    # Zobrazenie trasy na plátne
    for i in range(len(najlepsia_trasa) - 1):
        canvas.create_line(mesta[najlepsia_trasa[i]][0], mesta[najlepsia_trasa[i]][1],
                           mesta[najlepsia_trasa[i + 1]][0], mesta[najlepsia_trasa[i + 1]][1],
                           fill="blue", width=2)

    # Prepojenie späť na začiatok
    canvas.create_line(mesta[najlepsia_trasa[-1]][0], mesta[najlepsia_trasa[-1]][1],
                       mesta[najlepsia_trasa[0]][0], mesta[najlepsia_trasa[0]][1], fill="blue", width=2)

# generovanie miest
mesta = generuj_mesta()
for x, y in mesta:
    canvas.create_rectangle(x - 5, y - 5, x + 5, y + 5, fill="pink")

tsp_simulated_annealing(mesta)
root.mainloop()
