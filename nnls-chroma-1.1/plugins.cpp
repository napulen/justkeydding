/* -*- c-basic-offset: 4 indent-tabs-mode: nil -*-  vi:set ts=8 sts=4 sw=4: */

/*
  NNLS-Chroma / Chordino

  Audio feature extraction plugins for chromagram and chord
  estimation.

  Centre for Digital Music, Queen Mary University of London.
  This file copyright 2008-2010 Matthias Mauch and QMUL.
    
  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License as
  published by the Free Software Foundation; either version 2 of the
  License, or (at your option) any later version.  See the file
  COPYING included with this distribution for more information.
*/

#include <vamp/vamp.h>
#include <vamp-sdk/PluginAdapter.h>

#include "NNLSChroma.h"
#include "Chordino.h"
#include "Tuning.h"


static Vamp::PluginAdapter<NNLSChroma> chromaAdapter;
static Vamp::PluginAdapter<Chordino> chordinoAdapter;
static Vamp::PluginAdapter<Tuning> tuningAdapter;


const VampPluginDescriptor *
vampGetPluginDescriptor(unsigned int version, unsigned int index)
{
    if (version < 1) return 0;

    // Return a different plugin adaptor's descriptor for each index,
    // and return 0 for the first index after you run out of plugins.
    // (That's how the host finds out how many plugins are in this
    // library.)

    switch (index) {
    case  0: return chromaAdapter.getDescriptor();
    case  1: return chordinoAdapter.getDescriptor();
    case  2: return tuningAdapter.getDescriptor();
    default: return 0;
    }
}


