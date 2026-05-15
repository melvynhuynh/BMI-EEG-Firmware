from scipy.signal import iirfilter, iirnotch, tf2sos
import numpy as np

fs = 333  # Hz — DOIT matcher EEG_FS dans eeg_features.h

sos_bp = iirfilter(N=4, Wn=[1, 50], btype='bandpass', ftype='butter', fs=fs, output='sos')

b50, a50 = iirnotch(50, Q=30, fs=fs)
sos_n50 = tf2sos(b50, a50)

b100, a100 = iirnotch(100, Q=30, fs=fs)
sos_n100 = tf2sos(b100, a100)

def to_cmsis(sos, name):
    print(f"static const float {name}[{len(sos)*5}] = {{")
    for row in sos:
        b0, b1, b2, _, a1, a2 = row
        print(f"    {b0:+.10f}f, {b1:+.10f}f, {b2:+.10f}f, {-a1:+.10f}f, {-a2:+.10f}f,")
    print("};")

to_cmsis(sos_bp,   "bp_coefs")
print(f"#define NUM_BP_STAGES {len(sos_bp)}")
print()
to_cmsis(sos_n50,  "n50_coefs")
to_cmsis(sos_n100, "n100_coefs")