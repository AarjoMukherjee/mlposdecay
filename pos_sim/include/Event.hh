#ifndef EVENT_HH
#define EVENT_HH

#include "G4UserEventAction.hh"
#include "G4Event.hh"

#include "G4AnalysisManager.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "Run.hh"
#include <map>
#include "globals.hh"
struct EdepData {
    G4double totalEdep = 0.;
    G4double Ecompt = 0.;
    G4double Ephoto = 0.;
    G4double P_BindingEnergy = 0.;
    G4double EcomptLoss = 0.;
    G4double Eother = 0.;
    G4String process = "";
};

class PosRunAction; // forward

class PosEventAction : public G4UserEventAction {
public:
    PosEventAction(PosRunAction* runAction);
    ~PosEventAction() override;

    void BeginOfEventAction(const G4Event*) override;
    void EndOfEventAction(const G4Event*) override;

    std::map<G4int, EdepData> crystalData;
	G4double fAbsorb = 0.;
    G4int fCrystalID = -1;
    std::vector<double> fELeak = {0., 0., 0., 0., 0., 0.};
    G4ThreeVector fHit, fHitArray;
    G4bool InitHit_flag = true;

    bool fBranchesCreated = false;
    void ResetBranchFlag() { fBranchesCreated = false; }
    void CreateBranchesIfNeeded();

private:
    PosRunAction* fRunAction;

    // Branch variables
    Int_t eventID = 0;
    Int_t copyNo = 0;
    Int_t CrystID = -1;
    Double_t totalEdep = 0., Ecompt = 0., Ephoto = 0., P_BindingEnergy = 0., EComptLoss = 0.,Eabsorb = 0., xHit = 0., yHit = 0., ETrue = 0.;
};

#endif
