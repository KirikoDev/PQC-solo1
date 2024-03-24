import matplotlib.pyplot as plt
import numpy as np
import re

def parse_benchmark_data(file_path):
    with open(file_path, 'r') as file:
        data = file.read()
    pattern = re.compile(r'Keypair time: (\d+)\nEnc time: (\d+)\nDec time: (\d+)')
    matches = pattern.findall(data)
    keypair_times, enc_times, dec_times = zip(*[(int(m[0]), int(m[1]), int(m[2])) for m in matches])
    return np.mean(keypair_times), np.mean(enc_times), np.mean(dec_times)

files = {
    "kyber-512": "kyber512_benchmark.txt",
    "kyber-512-90s": "kyber512-90s-mit-param_benchmark.txt",
    "kyber-768": "kyber768_benchmark.txt"
}

zeiten_ms = {key: parse_benchmark_data(file) for key, file in files.items()}
ms_zu_clock_cycles = 100 * 1000

zeiten_clock_cycles_kilo = {key: [ms * ms_zu_clock_cycles / 1000 for ms in vals] for key, vals in zeiten_ms.items()}
data = np.array(list(zeiten_clock_cycles_kilo.values()))

labels = list(zeiten_ms.keys())
farben = ['SkyBlue', 'IndianRed', 'LightGreen']
operationen = ['Keypair', 'Enc', 'Dec']

fig, ax = plt.subplots()

for i, label in enumerate(labels):
    bottoms = 0
    for j, (op, color) in enumerate(zip(operationen, farben)):
        ax.bar(label, data[i, j], bottom=bottoms, color=color, label=op if i == 0 else "", width=0.5)
        bottoms += data[i, j]

ax.set_ylabel('Clock Cycles (in thousands)')
ax.set_title('Performance Measurement of Kyber Algorithms')
ax.set_xticks(np.arange(len(labels)))
ax.set_xticklabels(labels)
if len(labels) > 0:
    ax.legend()

ax2 = ax.twinx()
ax2.set_ylabel('Time in ms')

total_times_ms = [sum(vals) for vals in zeiten_ms.values()]
ax2.set_ylim(0, max(total_times_ms))
ax2.set_yticks(np.linspace(0, max(total_times_ms), len(ax.get_yticks())))
ax2.set_yticklabels([f"{tick:.0f}" for tick in np.linspace(0, max(total_times_ms), len(ax.get_yticks()))])

plt.savefig('kyber_benchmark_diagram1.png', dpi=300)