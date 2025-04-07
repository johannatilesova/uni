import random
import matplotlib.pyplot as plt
import numpy as np
import time
from matplotlib.colors import ListedColormap

# Generovanie počiatočných bodov a bodov s posunom
def generuj_bodiky(pociatocne_body, extra_body, x_min, x_max, y_min, y_max, limit_posunu):
    print(f"Generujem {pociatocne_body + extra_body} bodov...")

    vsetky_body = set()
    while len(vsetky_body) < pociatocne_body:
        x, y = random.randint(x_min, x_max), random.randint(y_min, y_max)
        vsetky_body.add((x, y))

    vsetky_body = list(vsetky_body)

    for i in range(extra_body):
        zaklad_x, zaklad_y = random.choice(vsetky_body)
        x_posun = random.randint(max(-limit_posunu, x_min - zaklad_x), min(limit_posunu, x_max - zaklad_x))
        y_posun = random.randint(max(-limit_posunu, y_min - zaklad_y), min(limit_posunu, y_max - zaklad_y))
        novy_bod = (zaklad_x + x_posun, zaklad_y + y_posun)
        vsetky_body.append(novy_bod)
    return np.array(vsetky_body)


#  Výpočet euklidovskej vzdialenosti
def euklidovska_vzdialenost(jeden_bod, druhy_bod):
    return np.linalg.norm(jeden_bod - druhy_bod)

# Vytvorenie matice vzdialeností pomocou efektívnych NumPy operácií
def vytvor_maticu_vzdialenosti(bodiky):
    rozdiel = bodiky[:, np.newaxis, :] - bodiky[np.newaxis, :, :]
    vzdialenost = np.sqrt(np.sum(rozdiel**2, axis=-1))
    np.fill_diagonal(vzdialenost, np.inf)
    return vzdialenost

# Vyhodnotenie úspešnosti zhlukovania na základe stredovej vzdialenosti
def uspesnost_zhlukovania(klastre, vzdialenost_od_stredu=500):
    uspesne_klastre = 0  # Počítadlo úspešných zhlukov

    for zhluk in klastre:
        # najdenie medoidu
        medoid = min(zhluk, key=lambda p: sum(euklidovska_vzdialenost(p, dalsi) for dalsi in zhluk))
        # Výpočet priemernej vzdialenosti všetkých bodov od medoidu
        priemerna_vzdialenost = np.mean([euklidovska_vzdialenost(bod, medoid) for bod in zhluk])

        # Kontrola, či priemerná vzdialenosť nepresahuje limit
        if priemerna_vzdialenost <= vzdialenost_od_stredu:
            uspesne_klastre += 1

    # Výpočet percentuálnej úspešnosti zhlukovania
    uspesnost = (uspesne_klastre / len(klastre)) * 100
    print(f"\nÚspešnosť zhlukovania: {uspesnost:.2f}%")

    return uspesnost


def zhlukovanie_centroidy(bodiky, vzdialenost_od_stredu=500):
    klastre = [[bod] for bod in bodiky] # pociatocne zhluky s jednotlivymi bodmi
    matica_vzdialenosti = vytvor_maticu_vzdialenosti(bodiky)
    centroidy = [np.mean(zhluk, axis=0) for zhluk in klastre] # vypocet pociatocnych centroidov

    cas_na_zaciatku = time.time()
    celkove_klastre = len(klastre)

    iteracia = 0
    while True:
        minimalna_vzdialenost = np.min(matica_vzdialenosti)
        if minimalna_vzdialenost > vzdialenost_od_stredu:
            break

        # zlucovanie dvoj najblizsich zhlukov
        i, j = np.unravel_index(np.argmin(matica_vzdialenosti), matica_vzdialenosti.shape)
        klastre[i].extend(klastre[j]) #pridanie bodov z klastu j do klastru i
        del klastre[j] #vymyzanie klastru j
        matica_vzdialenosti = np.delete(matica_vzdialenosti, j, axis=0)
        matica_vzdialenosti = np.delete(matica_vzdialenosti, j, axis=1)

        # aktualizuje sa centroid pre zluceny klaster i
        centroidy[i] = np.mean(klastre[i], axis=0)
        del centroidy[j]

        # prepocitanie vzdialenosti medzi zhlukmi
        for k in range(len(klastre)):
            if k != i:
                matica_vzdialenosti[i, k] = matica_vzdialenosti[k, i] = euklidovska_vzdialenost(centroidy[i], centroidy[k])

        iteracia += 1
        if iteracia % 100 == 0:
            percenta_hotovo = 100 * (celkove_klastre - len(klastre)) / celkove_klastre
            aktualny_cas = time.time() - cas_na_zaciatku
            print(f"Iterácia {iteracia}: {percenta_hotovo:.2f}% hotovo "
                  f"Čas = {aktualny_cas:.2f} sekúnd")

    cas_na_konci = time.time() - cas_na_zaciatku
    hodiny, zostatok = divmod(cas_na_konci, 3600)
    minuty, sekundy = divmod(zostatok, 60)
    print(f"\nZhlukovanie na základe centroidov dokončené za {int(hodiny)} hodin, {int(minuty)} minut a {sekundy:.2f} sekúnd")

    uspesnost = uspesnost_zhlukovania(klastre, vzdialenost_od_stredu)
    print(f"Zhlukovanie na základe centroidov pre {len(bodiky)} bodov - Úspešnosť: {uspesnost:.2f}%\n")

    popisky = np.full(len(bodiky), -1, dtype=int)
    for idx, zhluk in enumerate(klastre):
        for bod in zhluk:
            idx_bodu = np.where((bodiky == bod).all(axis=1))[0]
            if idx_bodu.size > 0:
                popisky[idx_bodu[0]] = idx

    return popisky, klastre


def zhlukovanie_medoidy(bodiky, vzdialenost_od_stredu=500):
    klastre = [[bod] for bod in bodiky] # pociatocne zhluky
    matica_vzdialenosti = vytvor_maticu_vzdialenosti(bodiky) # vytvorenie matice medzi vsetkymi bodmi
    cas_na_zaciatku = time.time()
    celkove_klastre = len(klastre)

    iteracia = 0
    while True:
        minimalna_vzdialenost = np.min(matica_vzdialenosti) # najdenie minimalnej vzdialenosti medzi dvoma zhlukmi
        # ak je minimálna vzdialenosť väčšia ako vzdialenost od stredu, ukonci sa cyklus
        if minimalna_vzdialenost > vzdialenost_od_stredu:
            break

        # ziskanie indexov dvoch zhlukov, ktore maju najmensiu vzdialenost
        i, j = np.unravel_index(np.argmin(matica_vzdialenosti), matica_vzdialenosti.shape)
        klastre[i].extend(klastre[j]) # zlucenie zhluku i a j
        del klastre[j]
        
        # aktualizacia matice
        matica_vzdialenosti = np.delete(matica_vzdialenosti, j, axis=0)
        matica_vzdialenosti = np.delete(matica_vzdialenosti, j, axis=1)

        # vypocet noveho medoidu
        medoid = min(klastre[i], key=lambda p: sum(euklidovska_vzdialenost(p, dalsi) for dalsi in klastre[i]))

        for k in range(len(klastre)):
            if k != i:
                matica_vzdialenosti[i, k] = matica_vzdialenosti[k, i] = euklidovska_vzdialenost(medoid, klastre[k][0])

        iteracia += 1
        if iteracia % 100 == 0:
            percenta_hotovo = 100 * (celkove_klastre - len(klastre)) / celkove_klastre
            aktualny_cas = time.time() - cas_na_zaciatku
            print(f"Iterácia {iteracia}: {percenta_hotovo:.2f}% hotovo "
                  f"Čas = {aktualny_cas:.2f} sekúnd")

    cas_na_konci = time.time() - cas_na_zaciatku
    hodiny, zostatok = divmod(cas_na_konci, 3600)
    minuty, sekundy = divmod(zostatok, 60)
    print(f"\nZhlukovanie na báze medoidov dokončené za {int(hodiny)} hodin, {int(minuty)} minut a {sekundy:.2f} sekúnd")

    uspesnost = uspesnost_zhlukovania(klastre, vzdialenost_od_stredu)
    print(f"Zhlukovanie na báze medoidov pre {len(bodiky)} bodov - Úspešnosť: {uspesnost:.2f}%")

    popisky = np.full(len(bodiky), -1, dtype=int)
    for idx, zhluk in enumerate(klastre):
        for bod in zhluk:
            idx_bodu = np.where((bodiky == bod).all(axis=1))[0]
            if idx_bodu.size > 0:
                popisky[idx_bodu[0]] = idx

    return popisky, klastre


# Vizualizácia klastrov s farebným odlíšením
def vizualizuj_klastre(bodiky, popisky, nadpis):
    plt.figure(figsize=(10, 10))
    unikatne_popisky = set(popisky)
    farby = ListedColormap(plt.get_cmap("hsv")(np.linspace(0, 1, len(unikatne_popisky))))

    for id_klastra in unikatne_popisky:
        body_klastra = bodiky[popisky == id_klastra]
        plt.scatter(body_klastra[:, 0], body_klastra[:, 1], color=farby(id_klastra / len(unikatne_popisky)), s=10)

        stred = np.mean(body_klastra, axis=0)
        plt.plot(stred[0], stred[1], 'o', color='black', markersize=8)

    plt.title(nadpis)
    plt.xlabel("X súradnice")
    plt.ylabel("Y súradnice")
    plt.grid(True)
    plt.show()


# Generovanie bodov a vykonanie zhlukovania
bodiky = generuj_bodiky(20, 10000, -5000, 5000, -5000, 5000, 100)
nazov_centroidy, klastre_centroidy = zhlukovanie_centroidy(bodiky, vzdialenost_od_stredu=500)
vizualizuj_klastre(bodiky, nazov_centroidy, "Aglomeratívne zhlukovanie s centroidmi")

bodiky = bodiky[:10000]
nazov_medoidy, klastre_medoidy = zhlukovanie_medoidy(bodiky, vzdialenost_od_stredu=500)
vizualizuj_klastre(bodiky, nazov_medoidy, "Aglomeratívne zhlukovanie s medoidmi")
