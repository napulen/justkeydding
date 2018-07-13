
#include "viterbi.h"
#include <iostream>

std::vector<int> ViterbiPath(std::vector<double> init, std::vector<vector<double> > trans, std::vector<vector<double> > obs, double *delta, vector<double> *scale) {
    
    int nState = init.size();
    int nFrame = obs.size();
    
    // check for consistency
    if ((int)trans[0].size() != nState || (int)trans.size() != nState || (int)obs[0].size() != nState) {
        cerr << "ERROR: matrix sizes inconsistent." << endl;
    }

    vector<vector<int> > psi; //  "matrix" of remembered indices of the best transitions
    vector<int> path = vector<int>(nFrame, nState-1); // the final output path (current assignment arbitrary, makes sense only for Chordino, where nChord-1 is the "no chord" label)
    
    double deltasum = 0;
    
    /* initialise first frame */
    for (int iState = 0; iState < nState; ++iState) {
        delta[iState] = init[iState] * obs[0][iState];
//	cerr << "init[" << iState << "] = " << init[iState] << ", obs[0][" << iState << "] = " << obs[0][iState] << endl;
        deltasum += delta[iState];
    }
    for (int iState = 0; iState < nState; ++iState) delta[iState] /= deltasum; // normalise (scale)
    scale->push_back(1.0/deltasum);
    psi.push_back(vector<int>(nState,0));

//    cerr << "nState = " << nState << ", deltasum = " << deltasum << endl;
    
    /* rest of the forward step */
    for (int iFrame = 1; iFrame < nFrame; ++iFrame) {
        deltasum = 0;
        psi.push_back(vector<int>(nState,0));
        /* every state wants to know which previous state suits it best */
        for (int jState = 0; jState < nState; ++jState) {            
            int bestState = nState - 1;
            double bestValue = 0;
            if (obs[iFrame][jState] > 0) {
                for (int iState = 0; iState < nState; ++iState) {
                    double currentValue = delta[(iFrame-1) * nState + iState] * trans[iState][jState];
                    if (currentValue > bestValue) {
                        bestValue = currentValue;
                        bestState = iState;
                    }
                }
            }

            delta[iFrame * nState + jState] = bestValue * obs[iFrame][jState];
            deltasum += delta[iFrame * nState + jState];
            psi[iFrame][jState] = bestState;
        }
        if (deltasum > 0) {
            for (int iState = 0; iState < nState; ++iState) {            
                delta[iFrame * nState + iState] /= deltasum; // normalise (scale)
            }
            scale->push_back(1.0/deltasum);
        } else {
            for (int iState = 0; iState < nState; ++iState) {            
                delta[iFrame * nState + iState] = 1.0/nState;
            }
            scale->push_back(1.0);
        }
        
    }
    
    /* initialise backward step */
    double bestValue = 0;
    for (int iState = 0; iState < nState; ++iState) {
        double currentValue = delta[(nFrame-1) * nState + iState];
        if (currentValue > bestValue) {
            bestValue = currentValue;            
            path[nFrame-1] = iState;
        }
    }

    /* rest of backward step */
    for (int iFrame = nFrame-2; iFrame > -1; --iFrame) {
        path[iFrame] = psi[iFrame+1][path[iFrame+1]];
    }    
    
    return path;
}
