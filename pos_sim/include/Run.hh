#ifndef RUN_HH
#define RUN_HH

#include "G4UserRunAction.hh"
#include "globals.hh"
#include "TFile.h"
#include "TTree.h"


class PosRunAction : public G4UserRunAction
{
public:
    PosRunAction();
    ~PosRunAction() override;

    void BeginOfRunAction(const G4Run*) override;
    void EndOfRunAction(const G4Run*) override;

    TTree* GetTree() const { return tree; }

    // Make accessible to EventAction
    TFile* file = nullptr;
    TTree* tree = nullptr;
};


#endif