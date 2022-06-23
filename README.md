# .wav Stripper

This is a C++ implementation of a program to strip away the metadata from a .wav file.

In some devices, such as elevators, the hardware responsible for the playback of audio files cannot diferentiate the audio data
from the metadata that is typically stored at the end of wave files. This can generate a crackling sound at the end of the audio
playback similar to that of a microphone being suddenly disconnected.

This implementation was developed in order to mitigate this issue. The program works by parsing the whole content of a wave file into
a struct according to the information stored in the header of the .wav file and, then, writing a new file that does not contain the
spurious data at the end of it. It is important to understand that wave files are RIFF files (Resource Interchange File Format) which 
start out with a header followed by a sequence of data chunks.

For more information about the RIFF file chunks and specifications: [The Canonical WAVE File Format](https://ccrma.stanford.edu/courses/422-winter-2014/projects/WaveFormat/#:~:text=The%20canonical%20WAVE%20format%20starts,the%20chunk%20following%20this%20number.)
