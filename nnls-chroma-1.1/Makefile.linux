
PLUGIN_LIBRARY_NAME = nnls-chroma

PLUGIN_CODE_OBJECTS = chromamethods.o NNLSBase.o NNLSChroma.o Chordino.o Tuning.o plugins.o nnls.o viterbi.o

VAMP_SDK_DIR = ../vamp-plugin-sdk

ARCHFLAGS = -O3 -ftree-vectorize -ffast-math
#ARCHFLAGS = -g

CFLAGS += $(ARCHFLAGS) -I$(VAMP_SDK_DIR) -Wall -fPIC
CXXFLAGS += $(ARCHFLAGS) -I$(VAMP_SDK_DIR) -Wall -fPIC
PLUGIN_EXT = .so
PLUGIN = $(PLUGIN_LIBRARY_NAME)$(PLUGIN_EXT)
LDFLAGS += -shared -Wl,-soname=$(PLUGIN) -L$(VAMP_SDK_DIR) -lvamp-sdk -Wl,--version-script=vamp-plugin.map 


$(PLUGIN): $(PLUGIN_CODE_OBJECTS)
	   $(CXX) -o $@ $^ $(LDFLAGS)

nnls.o:	nnls.c		# not nnls.f

clean:
	rm -f *.o

# DO NOT DELETE

Chordino.o: Chordino.h NNLSBase.h chromamethods.h nnls.h viterbi.h
chromamethods.o: chromamethods.h nnls.h
NNLSBase.o: NNLSBase.h chromamethods.h nnls.h
NNLSChroma.o: NNLSChroma.h NNLSBase.h chromamethods.h nnls.h
plugins.o: NNLSChroma.h NNLSBase.h Chordino.h Tuning.h
Tuning.o: Tuning.h NNLSBase.h chromamethods.h nnls.h
viterbi.o: viterbi.h
