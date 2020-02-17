> For v2.0.0 of `justkeydding`, I am planning to move entirely to Python. The main advantage is that I will be able to use the deepchroma features from `madmom`, which seem to deliver better results than NNLS-Chroma. Another feature in mind (WIP) is the annotation of a `MusicXML`|`krn`|`MIDI` file in symbolic. The code should be able to deliver an aligned-and-annotated file back (local keys as lyrics, global key annotated somewhere in the metadata) as well as the individual features. This will be released as a python package in PyPI (pip). Expected release is May 2020. Development of that new direction is happening in the `pythononly` branch, if you are interested in checking that code.

# justkeydding
Audio and Symbolic Key Detection algorithm based on [nnls-chroma](http://www.isophonics.net/nnls-chroma) and a Hidden Markov Model.

## About the project
This project is the extension of a previous [symbolic key detection algorithm](https://github.com/napulen/keytracker). The original algorithm required a sequence of pitch-classes that can be easily obtained from a symbolic music file. The old project has now been integrated in this repository to deal with both, symbolic and audio files. In the audio version, the pitch-classes are obtained from chromagrams. At this moment, the algorithm supports `midi`, `wav`, and `csv` input files. The supported `csv` consist of a chromagram file as given by the [Sonic Annotator](https://www.vamp-plugins.org/sonic-annotator/) program using the `nnls-chroma` Vamp plugin developed by Matthias Mauch and Chris Cannam.

During 2019, a few experiments showed that different key profiles worked for different types of music and different datasets, therefore, I started using the predictions from different key profiles with a **meta-classifier** algorithm that figured out keys better than individual key profiles did. This was also a way to *train* the model, as the original HMM model did not require any training from the data (and nevertheless had a good performance in MIREX ;), it was assumed that the key profiles provided the training aspect of pitch-class distributions in all keys. The current version of the model uses the ensemble method, it is slower, but hopefully, way more accurate.

## Building from source
### Vagrantfile
So far, this project has only been compiled and run in a virtualized Linux environment, more specifically, a Vagrant box of Ubuntu-18.04 x64. Compilation and execution in a native environment of that OS should be straighforward. I provide a simple `Vagrantfile` that launches the corresponding Vagrant box and installs the necessary packages to the system. We assume that Vagrant will be used to compile the project.

### Cloning the repo
The project has submodules, so **cloning recursively (adding the `--recursive` flag) is recommended**:
```
git clone https://github.com/napulen/justkeydding.git --recursive
cd justkeydding
```
### Launching the Vagrant box
Inside the repository, we can launch the Vagrant box
```
vagrant up
```
This takes a while, it will download the Ubuntu image (if you don't have it), spawn the vm, and compile the code. Once it finishes, the virtual machine should be running, we can then log to it
```
vagrant ssh
```
### Running full algorithm
Inside the virtual machine, the repository's folder has been mounted as a shared folder in `/vagrant`. Go there.
```
cd /vagrant
```
Run the full algorithm (including the meta-classifier) with
```
./justkeydding <input_file> <output_file>
```
The `output_file` is where the key is going to be stored.

#### Running individual HMM
If you are interested in running the individual HMM, that is the binary located at `bin/justkeydding`. This program supports a few  optional arguments, but in its most basic usage, it only requires an input midi|audio|chromagram_csv file as a command line argument.
```
bin/justkeydding <audio_filename>.wav
```
## Acknowledgments
Thanks to [Claire Arthur](https://github.com/musicnerd) for providing a dataset for testing the performance of this algorithm.
