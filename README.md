# Freqy
Freqy is (or will be) a realtime audio visualizer. This project is in its infancy, many changes coming soon, assuming my schedule allows it.
## Goals
1. Work with system audio ([Pulse audio](https://www.freedesktop.org/wiki/Software/PulseAudio/) buffer, accessed by using [pipewire](https://pipewire.org/) to bridge to [JACK audio](https://jackaudio.org/)) to test realtime audio capabilities
2. Visualize system audio to test RT visualizations (runtime efficiancy, frame rate, memory and CPU usage, etc.)
3. Implement various audio filters/digital signal processing pipelines for realtime audio ([DFT](https://en.wikipedia.org/wiki/Discrete_Fourier_transform), [FFT](https://en.wikipedia.org/wiki/Fast_Fourier_transform), etc.) 
4. Generalize audio/visual components in order to work with other audio sources/interfaces
5. Use software as a GUI/interface for Software Defined Radio
## Progress
Not much at the moment, still in the "getting things to work" phase of the project.
## TODO
- [ ] Build high-level interfaces for JACK and OPENGL (in progress)
- [ ] Implement relevant DSP algorithms (DFT with FFT)
- [ ] Create high-level interfaces for common visualizations (Waveforms, Spectrograms/sonograph, etc.)
- [ ] Cross platform??
