import numpy as np
import re

def parse_full_benchmark_data(file_path):
    with open(file_path, 'r') as file:
        data = file.read()
    pattern = re.compile(r'Keypair time: (\d+)\nEnc time: (\d+)\nDec time: (\d+)')
    matches = pattern.findall(data)
    all_times = np.array([[int(m[0]), int(m[1]), int(m[2])] for m in matches])
    return all_times

def calculate_adjusted_statistics(times_array, expected_points):
    adjustment_factor = len(times_array) / expected_points
    mittelwerte = np.mean(times_array, axis=0)
    mediane = np.median(times_array, axis=0)
    standardabweichungen = np.std(times_array, axis=0) * np.sqrt(adjustment_factor)
    minima = np.min(times_array, axis=0)
    maxima = np.max(times_array, axis=0)
    varianzen = np.var(times_array, axis=0) * adjustment_factor
    return mittelwerte, mediane, standardabweichungen, minima, maxima, varianzen

data_files = {
    "kyber-512": "kyber512_benchmark.txt",  
    "kyber-512-90s": "kyber512-90s-mit-param_benchmark.txt",  
    "kyber-768": "kyber768_benchmark_v1.txt" 
}

expected_data_points = {
    "kyber-512": 1000,
    "kyber-512-90s": 1000,
    "kyber-768": 100
}

statistiken = {}
for label, filepath in data_files.items():
    times = parse_full_benchmark_data(filepath)
    statistiken[label] = calculate_adjusted_statistics(times, expected_data_points[label])

for label, stats in statistiken.items():
    print(f"{label} Statistiken:")
    stat_labels = ['Mittelwerte', 'Mediane', 'Standardabweichungen', 'Minima', 'Maxima', 'Varianzen']
    for stat_label, stat_value in zip(stat_labels, stats):
        print(f"{stat_label}: {np.round(stat_value, decimals=4)}")
    print()  # Fügt eine Leerzeile hinzu für bessere Lesbarkeit
