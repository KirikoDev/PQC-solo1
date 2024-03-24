# Definieren Sie die Funktion parse_full_benchmark_data speziell für kyber-768, die mit dem gegebenen Datenformat umgehen kann.
import numpy as np
import re

def parse_full_benchmark_data_kyber_768(file_path):
    with open(file_path, 'r') as file:
        data = file.read()
    # Berücksichtigen Sie das spezifische Format, inklusive "reconnecting" Zeilen und mögliche Wiederholungen der Iterationszählung
    pattern = re.compile(r'Keypair time: (\d+)\nEnc time: (\d+)\nDec time: (\d+)')
    matches = pattern.findall(data)
    all_times = np.array([[int(m[0]), int(m[1]), int(m[2])] for m in matches])
    return all_times

def calculate_statistics(times_array):
    # Berechnen Sie Mittelwert, Median, Standardabweichung, Minima, Maxima und Varianz für die übergebene Zeiten-Matrix
    mittelwerte = np.mean(times_array, axis=0)
    mediane = np.median(times_array, axis=0)
    standardabweichungen = np.std(times_array, axis=0)
    minima = np.min(times_array, axis=0)
    maxima = np.max(times_array, axis=0)
    varianzen = np.var(times_array, axis=0)
    return mittelwerte, mediane, standardabweichungen, minima, maxima, varianzen

# Pfad zur kyber-768 Datenfile (bitte entsprechend anpassen)
file_path_kyber_768 = "kyber768_benchmark_v1.txt"  # Angenommener Pfad

# Parsing der Daten und Berechnung der Statistiken für kyber-768
times_kyber_768 = parse_full_benchmark_data_kyber_768(file_path_kyber_768)
stats_kyber_768 = calculate_statistics(times_kyber_768)

# Ausgabe der berechneten Statistiken
stat_labels = ['Mittelwerte', 'Mediane', 'Standardabweichungen', 'Minima', 'Maxima', 'Varianzen']
print("Kyber-768 Statistiken:")
for label, stat in zip(stat_labels, stats_kyber_768):
    print(f"{label}: {stat}")

# Beachten Sie, dass dieses Skript lokal ausgeführt werden muss, da es Zugriff auf die Dateisystem benötigt, um die Daten zu lesen.
