# justkeydding
Audio Key Detection algorithm based on [nnls-chroma](http://www.isophonics.net/nnls-chroma) and a Hidden Markov Model.

## About the project
This project is the extension of a previous [symbolic key detection algorithm](https://github.com/napulen/keytracker). The original algorithm requires a sequence of pitch-classes that can easily be obtained from the symbolic music file. In the audio version, they have been replaced by a chromagram. This chromagram has been obtained from the nnls-chroma Vamp plugin developed by Matthias Mauch and Chris Cannam.

## Building from source
### Vagrantfile
So far, this project has only been compiled and run in a virtualized Linux environment, more specifically, a Vagrant box of Ubuntu-14.04 x64. Compilation and execution in a native environment should be straighforward but it has not been tested. We provide a simple `Vagrantfile` that launches the corresponding Vagrant box and installs the necessary packages to the system. We assume that Vagrant will be used to compile the project.

### Cloning the repo
The project has submodules, so cloning recursively is recommended:
```
git clone https://github.com/napulen/justkeydding.git --recursive
cd justkeydding
```
### Launching the Vagrant box
Inside the repository, we can launch the Vagrant box
```
vagrant up
```
After a while, the virtual machine should be running, we can then log to it
```
vagrant ssh
```
### Compiling
Inside the virtual machine, the repository's folder has been mounted as a shared folder in `/vagrant`. We can compile from there.
```
cd /vagrant
make
```
### Running
After compiling, the binary should be located in `bin/justkeydding`. The program supports a few optional arguments, but in its most basic usage, it only requires an input audio file as a command line argument.
```
bin/justkeydding <audio_filename>.wav
```
