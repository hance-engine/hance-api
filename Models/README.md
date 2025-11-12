## Models
A variety of models is available in the *Models* folder. All models are trained on audio material sampled at 48 kHz, but the HANCE Audio Engine will accept other sample rates.

The model names include the purpose of the model along with the total latency and version number, e.g. `speech-denoise-96ms.v26.1.hance` for the speech noise reduction model with 96 milliseconds total latency.

We offer both speech noise reduction models and models that combine noise and reverb reduction for speech. The latter output cleaned dialogue, noise, and reverb estimations as separate busses.

The following table shows the currently available models along with CPU efficiency, file size, and latencies:

| Model                                | File Size | Real-time Factor* | Buffer Latency | Algorithmic Latency |
|--------------------------------------|-----------|-------------------|----------------|---------------------|
| speech-denoise-dereverb-96ms.v26.1   | 926 KB    | 27x               | 21.33 ms       | 74.66 ms            |
| speech-denoise-dereverb-32ms.v26.1   | 854 KB    | 25x               | 21.33 ms       | 10.67 ms            |
| speech-denoise-dereverb-32ms-tiny.v26.1 | 241 KB | 115x              | 21.33 ms       | 10.67 ms            |
| speech-denoise-dereverb-21ms.v26.1   | 851 KB    | 12x               | 21.33 ms       | 0.00 ms             |
| speech-denoise-dereverb-11ms.v26.1   | 615 KB    | 12x               | 10.67 ms       | 0.00 ms             |
| speech-denoise-96ms.v26.1            | 925 KB    | 29x               | 21.33 ms       | 74.66 ms            |
| speech-denoise-32ms.v26.1            | 854 KB    | 27x               | 21.33 ms       | 10.67 ms            |
| speech-denoise-32ms-tiny.v26.1       | 241 KB    | 125x              | 21.33 ms       | 10.67 ms            |
| speech-denoise-21ms.v26.1            | 851 KB    | 13x               | 21.33 ms       | 0.00 ms             |
| speech-denoise-11ms.v26.1            | 615 KB    | 13x               | 10.67 ms       | 0.00 ms             |

\*The real-time factor is measured on a single core of an AMD RYZEN AI MAX+ 395.

---

### Speech Denoise and Dereverberation Model – 96 ms
A high-quality model designed for natural and clear dialogue restoration. It provides excellent noise and reverb reduction, making it ideal when low latency is not critical and the highest speech clarity is desired.

| Model Specifications |                                    |
|----------------------|------------------------------------|
| File name            | speech-denoise-dereverb-96ms.v26.1 |
| Output busses        | Dialogue / Reverb / Noise          |
| File size            | 926 KB                             |
| Real-time factor     | 27x                                |
| Sample rate          | 48 kHz                             |
| Buffer latency       | 21.33 ms                           |
| Algorithmic latency  | 74.66 ms                           |

---

### Speech Denoise and Dereverberation Model – 32 ms
A balanced model that provides strong reverb and noise reduction while keeping latency moderate. Works well for real-time speech enhancement where clarity and responsiveness are both important.

| Model Specifications |                                    |
|----------------------|------------------------------------|
| File name            | speech-denoise-dereverb-32ms.v26.1 |
| Output busses        | Dialogue / Reverb / Noise          |
| File size            | 854 KB                             |
| Real-time factor     | 25x                                |
| Sample rate          | 48 kHz                             |
| Buffer latency       | 21.33 ms                           |
| Algorithmic latency  | 10.67 ms                           |

### Speech Denoise and Dereverberation Model – 32 ms Tiny
A compact 241 KB version of the 32 ms combined model. Provides strong noise and reverb reduction while running at a much higher real-time factor, making it highly efficient for systems where performance and scalability matter. This efficiency allows you to process more simultaneous streams per CPU with only a small trade-off in suppression strength.

| Model Specifications |                                    |
|----------------------|------------------------------------|
| File name            | speech-denoise-dereverb-32ms-tiny.v26.1 |
| Output busses        | Dialogue / Reverb / Noise          |
| File size            | 241 KB                             |
| Real-time factor     | 115x                                |
| Sample rate          | 48 kHz                             |
| Buffer latency       | 21.33 ms                           |
| Algorithmic latency  | 10.67 ms                           |

---

### Speech Denoise and Dereverberation Model – 21 ms
Optimized for responsiveness while maintaining strong reverb and noise suppression. Some transient noise may still appear in highly dynamic environments, but overall performance remains very solid.

| Model Specifications |                                    |
|----------------------|------------------------------------|
| File name            | speech-denoise-dereverb-21ms.v26.1 |
| Output busses        | Dialogue / Reverb / Noise          |
| File size            | 851 KB                             |
| Real-time factor     | 12x                                |
| Sample rate          | 48 kHz                             |
| Buffer latency       | 21.33 ms                           |
| Algorithmic latency  | 0.00 ms                            |

---

### Speech Denoise and Dereverberation Model – 11 ms
Built for ultra-low latency scenarios such as live communication. It effectively reduces noise and reverb but may introduce mild artifacts in very noisy or reverberant conditions.

| Model Specifications |                                    |
|----------------------|------------------------------------|
| File name            | speech-denoise-dereverb-11ms.v26.1 |
| Output busses        | Dialogue / Reverb / Noise          |
| File size            | 615 KB                             |
| Real-time factor     | 12x                                |
| Sample rate          | 48 kHz                             |
| Buffer latency       | 10.67 ms                           |
| Algorithmic latency  | 0.00 ms                            |

---

### Speech Denoise Model – 96 ms
A high-fidelity noise reduction model that preserves natural speech tone while removing complex background noise. Ideal for recordings or post-processing where latency is not a concern.

| Model Specifications |                                 |
|----------------------|---------------------------------|
| File name            | speech-denoise-96ms.v26.1.hance |
| Output busses        | Dialogue / Noise                |
| File size            | 925 KB                          |
| Real-time factor     | 29x                             |
| Sample rate          | 48 kHz                          |
| Buffer latency       | 21.33 ms                        |
| Algorithmic latency  | 74.66 ms                        |

---

### Speech Denoise Model – 32 ms
A versatile model offering an excellent balance between speed and quality. Reduces noise effectively while maintaining the natural timbre of the voice, suitable for real-time use and production workflows.

| Model Specifications |                                 |
|----------------------|---------------------------------|
| File name            | speech-denoise-32ms.v26.1.hance |
| Output busses        | Dialogue / Noise                |
| File size            | 854 KB                          |
| Real-time factor     | 27x                             |
| Sample rate          | 48 kHz                          |
| Buffer latency       | 21.33 ms                        |
| Algorithmic latency  | 10.67 ms                        |

### Speech Denoise Model – 32 ms Tiny
A compact 241 KB version of the 32 ms model. Delivers nearly identical audio quality while achieving a much higher real-time factor, making it significantly more CPU-efficient. This means you can run more simultaneous audio streams on a single CPU without compromising clarity or responsiveness.

| Model Specifications |                                 |
|----------------------|---------------------------------|
| File name            | speech-denoise-32ms-tiny.v26.1.hance |
| Output busses        | Dialogue / Noise                |
| File size            | 241 KB                          |
| Real-time factor     | 125x                             |
| Sample rate          | 48 kHz                          |
| Buffer latency       | 21.33 ms                        |
| Algorithmic latency  | 10.67 ms                        |

---

### Speech Denoise Model – 21 ms
Compact and efficient, offering clean noise reduction with minimal delay. Performs well in moderate noise environments while keeping speech natural and intelligible.

| Model Specifications |                                 |
|----------------------|---------------------------------|
| File name            | speech-denoise-21ms.v26.1.hance |
| Output busses        | Dialogue / Noise                |
| File size            | 851 KB                          |
| Real-time factor     | 13x                             |
| Sample rate          | 48 kHz                          |
| Buffer latency       | 21.33 ms                        |
| Algorithmic latency  | 0.00 ms                         |

---

### Speech Denoise Model – 11 ms
Ultra-low latency model suitable for live speech and conferencing. Retains more of the room’s natural reverb, making the output sound open and less processed, though noise reduction is less aggressive.

| Model Specifications |                                 |
|----------------------|---------------------------------|
| File name            | speech-denoise-11ms.v26.1.hance |
| Output busses        | Dialogue / Noise                |
| File size            | 615 KB                          |
| Real-time factor     | 13x                             |
| Sample rate          | 48 kHz                          |
| Buffer latency       | 10.67 ms                        |
| Algorithmic latency  | 0.00 ms                         |
