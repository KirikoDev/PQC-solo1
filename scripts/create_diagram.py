import matplotlib.pyplot as plt
import numpy as np
import re

def parse_full_benchmark_data(file_path):
    with open(file_path, 'r') as file:
        data = file.read()
    pattern = re.compile(r'Keypair time: (\d+)\nEnc time: (\d+)\nDec time: (\d+)')
    matches = pattern.findall(data)
    all_times = np.array([[int(m[0]), int(m[1]), int(m[2])] for m in matches])
    return all_times

data_files = {
    "kyber-512": "kyber512_benchmark.txt",
    "kyber-512-90s": "kyber512-90s-mit-param_benchmark.txt",
    "kyber-768": "kyber768_benchmark_v1.txt"
}

# 80MHz
frequency = 80 * 1000 * 1000
labels = list(data_files.keys())
colors = ['SkyBlue', 'IndianRed', 'LightGreen']
operations = ['KeyGen', 'Encaps', 'Decaps']

fig, ax = plt.subplots()
bar_width = 0.5
x = np.arange(len(labels))

data_phases = {label: parse_full_benchmark_data(filepath) for label, filepath in data_files.items()}

total_times_ms = []

for i, label in enumerate(labels):
    times = data_phases[label]
    mean_times = times.mean(axis=0) / 1000 * frequency / 1000
    min_times = times.min(axis=0) / 1000 * frequency / 1000
    max_times = times.max(axis=0)  / 1000 * frequency / 1000
    
    bottoms = 0
    for j, (mean_time, color) in enumerate(zip(mean_times, colors)):
        ax.bar(x[i], mean_time, bar_width, bottom=bottoms, color=color, label=operations[j] if i == 0 else "")
        bottoms += mean_time

    total_min_time = min_times.sum()
    total_max_time = max_times.sum()
    error_margin = [(bottoms - total_min_time, total_max_time - bottoms)]
    ax.errorbar(x[i], bottoms, yerr=np.array(error_margin).T, fmt='none', ecolor='black', capsize=5)

    total_times_ms.append(times.sum(axis=1).mean())

ax.set_xticks(x)
ax.set_xticklabels(labels)
ax.set_ylabel('Taktzyklen (in Tausend)')
ax.set_title('Performanzmessung der Crystals-Kyber Algorithmen')
ax.legend()

ax2 = ax.twinx()
ax2.set_ylabel('Zeit in ms')
max_time_ms_adjusted = np.ceil(max(total_times_ms) / 10) * 10
ax2.set_ylim(0, max_time_ms_adjusted)

plt.tight_layout()
plt.savefig('kyber_benchmark_variation.png', dpi=300)
