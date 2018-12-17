CC=g++

BUILD=build
SRC=src
BIN=bin
INCLUDE=include
TEST=test
MKDIR_P=mkdir -p
NNLS_CHROMA=nnls-chroma-1.1
MIDIFILE=midifile
MIDIFILE_INC=$(MIDIFILE)/include
MIDIFILE_SRC=$(MIDIFILE)/src-library

TESTS = test_key test_pitchclass test_keyprofile \
		test_keytransition test_hiddenmarkovmodel test_chromagram

CFLAGS=-I$(INCLUDE) --std=c++11 -O3

LFLAGS=-O3

ADDITIONAL_LIBRARIES=-lsndfile -lvamp-hostsdk -ldl

.PHONY: directories clean

all: directories justkeydding

test: directories $(TESTS)

directories: $(BUILD) $(BIN)

$(BUILD):
	$(MKDIR_P) $(BUILD)

$(BIN):
	$(MKDIR_P) $(BIN)

clean:
	rm -R $(BUILD)
	rm -R $(BIN)

$(BUILD)/midi.o: $(SRC)/midi.cc
	$(CC) -c -o $(BUILD)/midi.o $(SRC)/midi.cc \
	$(CFLAGS) -I$(MIDIFILE_INC)

$(BUILD)/Binasc.o: $(MIDIFILE_SRC)/Binasc.cpp
	$(CC) -c -o $(BUILD)/Binasc.o $(MIDIFILE_SRC)/Binasc.cpp \
	$(CFLAGS) -I$(MIDIFILE_INC)

$(BUILD)/MidiEvent.o: $(MIDIFILE_SRC)/MidiEvent.cpp
	$(CC) -c -o $(BUILD)/MidiEvent.o $(MIDIFILE_SRC)/MidiEvent.cpp \
	$(CFLAGS) -I$(MIDIFILE_INC)

$(BUILD)/MidiEventList.o: $(MIDIFILE_SRC)/MidiEventList.cpp
	$(CC) -c -o $(BUILD)/MidiEventList.o $(MIDIFILE_SRC)/MidiEventList.cpp \
	$(CFLAGS) -I$(MIDIFILE_INC)

$(BUILD)/MidiFile.o: $(MIDIFILE_SRC)/MidiFile.cpp
	$(CC) -c -o $(BUILD)/MidiFile.o $(MIDIFILE_SRC)/MidiFile.cpp \
	$(CFLAGS) -I$(MIDIFILE_INC)

$(BUILD)/MidiMessage.o: $(MIDIFILE_SRC)/MidiMessage.cpp
	$(CC) -c -o $(BUILD)/MidiMessage.o $(MIDIFILE_SRC)/MidiMessage.cpp \
	$(CFLAGS) -I$(MIDIFILE_INC)

justkeydding: $(BUILD)/justkeydding.o \
		$(BUILD)/key.o $(BUILD)/pitchclass.o \
		$(BUILD)/keyprofile.o $(BUILD)/keytransition.o \
		$(BUILD)/chromagram.o $(BUILD)/hiddenmarkovmodel.o \
		$(BUILD)/NNLSChroma.o $(BUILD)/NNLSBase.o \
		$(BUILD)/chromamethods.o $(BUILD)/nnls.o \
		$(BUILD)/midi.o $(BUILD)/Binasc.o \
		$(BUILD)/MidiEvent.o $(BUILD)/MidiEventList.o \
		$(BUILD)/MidiFile.o $(BUILD)/MidiMessage.o
	$(CC) -o $(BIN)/justkeydding $(BUILD)/justkeydding.o \
	$(BUILD)/key.o $(BUILD)/pitchclass.o $(BUILD)/keyprofile.o \
	$(BUILD)/keytransition.o $(BUILD)/chromagram.o \
	$(BUILD)/hiddenmarkovmodel.o $(BUILD)/NNLSChroma.o \
	$(BUILD)/NNLSBase.o $(BUILD)/chromamethods.o $(BUILD)/nnls.o \
	$(BUILD)/midi.o $(BUILD)/Binasc.o $(BUILD)/MidiEvent.o \
	$(BUILD)/MidiEventList.o $(BUILD)/MidiFile.o $(BUILD)/MidiMessage.o \
	$(LFLAGS) $(ADDITIONAL_LIBRARIES)

$(BUILD)/justkeydding.o: $(SRC)/justkeydding.cc
	$(CC) -c -o$(BUILD)/justkeydding.o $(SRC)/justkeydding.cc \
	$(CFLAGS) -I$(NNLS_CHROMA) -I$(MIDIFILE_INC)

test_key: $(BUILD)/test_key.o $(BUILD)/key.o
	$(CC) -o $(BIN)/test_key $(BUILD)/test_key.o \
	$(BUILD)/key.o $(LFLAGS)

$(BUILD)/test_key.o: $(TEST)/test_key.cc
	$(CC) -c -o $(BUILD)/test_key.o $(TEST)/test_key.cc $(CFLAGS)

$(BUILD)/key.o: $(SRC)/key.cc
	$(CC) -c -o $(BUILD)/key.o $(SRC)/key.cc $(CFLAGS)


test_pitchclass: $(BUILD)/test_pitchclass.o $(BUILD)/pitchclass.o
	$(CC) -o $(BIN)/test_pitchclass $(BUILD)/test_pitchclass.o \
	$(BUILD)/pitchclass.o $(LFLAGS)

$(BUILD)/test_pitchclass.o: $(TEST)/test_pitchclass.cc
	$(CC) -c -o $(BUILD)/test_pitchclass.o \
	$(TEST)/test_pitchclass.cc $(CFLAGS)

$(BUILD)/pitchclass.o: $(SRC)/pitchclass.cc
	$(CC) -c -o $(BUILD)/pitchclass.o $(SRC)/pitchclass.cc $(CFLAGS)


test_keyprofile: $(BUILD)/test_keyprofile.o $(BUILD)/keyprofile.o \
		$(BUILD)/pitchclass.o $(BUILD)/key.o
	$(CC) -o $(BIN)/test_keyprofile $(BUILD)/test_keyprofile.o \
	$(BUILD)/keyprofile.o $(BUILD)/pitchclass.o $(BUILD)/key.o $(LFLAGS)

$(BUILD)/test_keyprofile.o: $(TEST)/test_keyprofile.cc
	$(CC) -c -o $(BUILD)/test_keyprofile.o \
	$(TEST)/test_keyprofile.cc $(CFLAGS)

$(BUILD)/keyprofile.o: $(SRC)/keyprofile.cc
	$(CC) -c -o $(BUILD)/keyprofile.o $(SRC)/keyprofile.cc $(CFLAGS)


test_keytransition: $(BUILD)/test_keytransition.o $(BUILD)/keytransition.o \
		$(BUILD)/key.o $(BUILD)/pitchclass.o
	$(CC) -o $(BIN)/test_keytransition $(BUILD)/test_keytransition.o \
	$(BUILD)/keytransition.o $(BUILD)/key.o $(BUILD)/pitchclass.o $(LFLAGS)

$(BUILD)/test_keytransition.o: $(TEST)/test_keytransition.cc
	$(CC) -c -o $(BUILD)/test_keytransition.o \
	$(TEST)/test_keytransition.cc $(CFLAGS)

$(BUILD)/keytransition.o: $(SRC)/keytransition.cc
	$(CC) -c -o $(BUILD)/keytransition.o $(SRC)/keytransition.cc $(CFLAGS)


test_hiddenmarkovmodel: $(BUILD)/test_hiddenmarkovmodel.o \
		$(BUILD)/pitchclass.o $(BUILD)/key.o $(BUILD)/keytransition.o \
		$(BUILD)/keyprofile.o $(BUILD)/hiddenmarkovmodel.o
	$(CC) -o $(BIN)/test_hiddenmarkovmodel $(BUILD)/test_hiddenmarkovmodel.o \
	$(BUILD)/pitchclass.o $(BUILD)/key.o $(BUILD)/keytransition.o \
	$(BUILD)/keyprofile.o $(BUILD)/hiddenmarkovmodel.o $(LFLAGS)

$(BUILD)/test_hiddenmarkovmodel.o: $(TEST)/test_hiddenmarkovmodel.cc
	$(CC) -c -o $(BUILD)/test_hiddenmarkovmodel.o \
	$(TEST)/test_hiddenmarkovmodel.cc $(CFLAGS)

$(BUILD)/hiddenmarkovmodel.o: $(SRC)/hiddenmarkovmodel.cc
	$(CC) -c -o $(BUILD)/hiddenmarkovmodel.o \
	$(SRC)/hiddenmarkovmodel.cc $(CFLAGS)


test_chromagram: $(BUILD)/test_chromagram.o $(BUILD)/chromagram.o \
		$(BUILD)/pitchclass.o $(BUILD)/key.o $(BUILD)/keytransition.o \
		$(BUILD)/keyprofile.o $(BUILD)/hiddenmarkovmodel.o \
		$(BUILD)/NNLSChroma.o $(BUILD)/NNLSBase.o \
		$(BUILD)/chromamethods.o $(BUILD)/nnls.o
	$(CC) -o $(BIN)/test_chromagram $(BUILD)/test_chromagram.o \
	$(BUILD)/chromagram.o $(BUILD)/pitchclass.o $(BUILD)/key.o \
	$(BUILD)/keytransition.o $(BUILD)/keyprofile.o \
	$(BUILD)/hiddenmarkovmodel.o $(BUILD)/NNLSChroma.o \
	$(BUILD)/NNLSBase.o $(BUILD)/chromamethods.o $(BUILD)/nnls.o \
	$(LFLAGS) $(ADDITIONAL_LIBRARIES)

$(BUILD)/test_chromagram.o: $(TEST)/test_chromagram.cc
	$(CC) -c -o $(BUILD)/test_chromagram.o \
	$(TEST)/test_chromagram.cc $(CFLAGS) -I$(NNLS_CHROMA)

$(BUILD)/NNLSChroma.o: $(NNLS_CHROMA)/NNLSChroma.cpp
	$(CC) -c -o $(BUILD)/NNLSChroma.o $(NNLS_CHROMA)/NNLSChroma.cpp

$(BUILD)/NNLSBase.o: $(NNLS_CHROMA)/NNLSBase.cpp
	$(CC) -c -o $(BUILD)/NNLSBase.o $(NNLS_CHROMA)/NNLSBase.cpp

$(BUILD)/chromamethods.o: $(NNLS_CHROMA)/chromamethods.cpp
	$(CC) -c -o $(BUILD)/chromamethods.o $(NNLS_CHROMA)/chromamethods.cpp

$(BUILD)/nnls.o: $(NNLS_CHROMA)/nnls.c
	$(CC) -c -o $(BUILD)/nnls.o $(NNLS_CHROMA)/nnls.c

$(BUILD)/chromagram.o: $(SRC)/chromagram.cc
	$(CC) -c -o $(BUILD)/chromagram.o $(SRC)/chromagram.cc \
	$(CFLAGS) -I$(NNLS_CHROMA) -D_VAMP_PLUGIN_IN_HOST_NAMESPACE