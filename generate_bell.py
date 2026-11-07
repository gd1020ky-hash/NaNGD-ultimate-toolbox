import os
import wave
import math
import struct

os.makedirs("resources", exist_ok=True)

SAMPLE_RATE = 44100
DURATION = 1.0
TOTAL_SAMPLES = int(SAMPLE_RATE * DURATION)

HARMONICS = [
    (2093.0, 1.0, 3.5),
    (4186.0, 0.45, 5.0),
    (6280.0, 0.25, 7.0),
    (8372.0, 0.15, 9.0),
]

audio_data = []

for i in range(TOTAL_SAMPLES):
    t = i / SAMPLE_RATE
    sample = 0.0
    attack = min(1.0, t * 1000.0)

    for freq, amp, decay in HARMONICS:
        env = math.exp(-decay * t)
        sample += amp * env * math.sin(2.0 * math.pi * freq * t)

    sample *= attack * 0.8
    sample = max(-1.0, min(1.0, sample))
    audio_data.append(int(sample * 32767.0))

with wave.open("resources/bell.wav", "wb") as wav_file:
    wav_file.setnchannels(1)
    wav_file.setsampwidth(2)
    wav_file.setframerate(SAMPLE_RATE)
    for s in audio_data:
        wav_file.writeframes(struct.pack("<h", s))

print("Created resources/bell.wav successfully!")
