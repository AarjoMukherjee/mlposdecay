#include "Event.hh"
#include "Run.hh"
#include "PrimGen.hh"
#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"

PosEventAction::PosEventAction(PosRunAction*)
{
}

PosEventAction::~PosEventAction()
{}

void PosEventAction::BeginOfEventAction(const G4Event*)
{
    crystalData.clear();
    fAbsorb = 0.;
    fCrystalID = -1;
    fHit = G4ThreeVector(-9999., -9999., -9999.);
    fHitArray = G4ThreeVector(-9999., -9999., -9999.);
    for(int i = 0; i < 6; i++) fELeak[i] = 0.;
    InitHit_flag = true;
    eventID = 0;
    copyNo = 0;
    CrystID = -1;
    totalEdep = 0., Ecompt = 0., Ephoto = 0., P_BindingEnergy = 0., EComptLoss = 0. ,Eabsorb = 0., xHit = 0., yHit = 0., ETrue = 0.;
}

void PosEventAction::EndOfEventAction(const G4Event* event)
{
    auto analysis = G4AnalysisManager::Instance();
    G4double ETrueChk = 0., EdepChk = 0., EabsChk = 0., ELeakChk = 0., EBeChk = 0., ECLossChk = 0.;
    for(auto const &kv : crystalData) {
        eventID = event->GetEventID();
        totalEdep = kv.second.totalEdep;
        Ecompt = kv.second.Ecompt;
        Ephoto = kv.second.Ephoto;
        P_BindingEnergy = kv.second.P_BindingEnergy;
        EComptLoss = kv.second.EcomptLoss;
        copyNo = kv.first;

        xHit = fHit.x()/mm;
        yHit = fHit.y()/mm;
        Eabsorb = fAbsorb;

        ETrue =
            static_cast<PosPrimaryGenerator*>(
            const_cast<G4VUserPrimaryGeneratorAction*>(
                G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction()
            )
        )->GetGunEnergy()/keV;

        analysis->FillNtupleIColumn(0, eventID);
        analysis->FillNtupleIColumn(1, copyNo);
        analysis->FillNtupleDColumn(2, totalEdep);
        analysis->FillNtupleDColumn(3, Ecompt);
        analysis->FillNtupleDColumn(4, Ephoto);
        analysis->FillNtupleDColumn(5, P_BindingEnergy);
        analysis->FillNtupleDColumn(6, EComptLoss);
        analysis->FillNtupleDColumn(7, Eabsorb);
        analysis->FillNtupleDColumn(8, xHit);
        analysis->FillNtupleDColumn(9, yHit);
        analysis->FillNtupleIColumn(10, fCrystalID);
        for (int i = 0; i < 6; i++)
        {
            analysis->FillNtupleDColumn(11 + i, fELeak[i]);
        }
        analysis->FillNtupleDColumn(17, ETrue);

        analysis->AddNtupleRow();
        ETrueChk = ETrue;
        ECLossChk += kv.second.EcomptLoss;
        EdepChk += kv.second.totalEdep;
        EabsChk = fAbsorb; 
        ELeakChk = fELeak[0] + fELeak[1] + fELeak[2] + fELeak[3] + fELeak[4] + fELeak[5];
        EBeChk += kv.second.P_BindingEnergy;
    }

     if (crystalData.empty()) {
        eventID = event->GetEventID();
        totalEdep = 0.;
        Ecompt = 0.;
        Ephoto = 0.;
        P_BindingEnergy = 0.;
        EComptLoss = 0.0;
        copyNo = -1;

        xHit = fHit.x()/mm;
        yHit = fHit.y()/mm;
        Eabsorb = fAbsorb;

        ETrue =
            static_cast<PosPrimaryGenerator*>(
            const_cast<G4VUserPrimaryGeneratorAction*>(
                G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction()
            )
        )->GetGunEnergy()/keV;

        analysis->FillNtupleIColumn(0, eventID);
        analysis->FillNtupleIColumn(1, copyNo);
        analysis->FillNtupleDColumn(2, totalEdep);
        analysis->FillNtupleDColumn(3, Ecompt);
        analysis->FillNtupleDColumn(4, Ephoto);
        analysis->FillNtupleDColumn(5, P_BindingEnergy);
        analysis->FillNtupleDColumn(6, EComptLoss);
        analysis->FillNtupleDColumn(7, Eabsorb);
        analysis->FillNtupleDColumn(8, xHit);
        analysis->FillNtupleDColumn(9, yHit);
        analysis->FillNtupleIColumn(10, fCrystalID);
        for (int i = 0; i < 6; i++)
        {
            analysis->FillNtupleDColumn(11 + i, fELeak[i]);
        }
        analysis->FillNtupleDColumn(17, ETrue);

        analysis->AddNtupleRow();
        ELeakChk = fELeak[0] + fELeak[1] + fELeak[2] + fELeak[3] + fELeak[4] + fELeak[5];
        // G4cout<<"EventID: "<<event->GetEventID()<<" ETrue: "<<ETrue<<" Total: "
        //       <<    ELeakChk+EabsChk<<G4endl;
    }
// if(ETrueChk-(EdepChk + EabsChk + ECLossChk + ELeakChk + EBeChk) < -0.0001){
//     G4cout<<"EventID: "<<event->GetEventID()<<" ETrue: "<<ETrueChk<<" Total: "
//           <<EdepChk + EabsChk + ECLossChk + ELeakChk + EBeChk<<G4endl;
// }

    // Fill Ntuple
}
