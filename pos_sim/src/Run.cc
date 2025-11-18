#include "Run.hh"
#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"
#include "G4Threading.hh"

PosRunAction::PosRunAction(){
    auto analysis = G4AnalysisManager::Instance();
    analysis->SetVerboseLevel(1);
    analysis->SetFileName("../Output/data_20m_0000_1000_keV.root");
    analysis->CreateNtuple("T", "Data");
    analysis->CreateNtupleIColumn("eventID");
    analysis->CreateNtupleIColumn("copyNo");
    analysis->CreateNtupleDColumn("totalEdep");
    analysis->CreateNtupleDColumn("Ecompt");
    analysis->CreateNtupleDColumn("Ephoto");
    analysis->CreateNtupleDColumn("P_BindingEnergy");
    analysis->CreateNtupleDColumn("EComptLoss");
    analysis->CreateNtupleDColumn("absorbEdep");
    analysis->CreateNtupleDColumn("xHit");
    analysis->CreateNtupleDColumn("yHit");
    analysis->CreateNtupleIColumn("InitCrystalID");
    analysis->CreateNtupleDColumn("ELeak_Top");
    analysis->CreateNtupleDColumn("ELeak_Left");
    analysis->CreateNtupleDColumn("ELeak_Bottom");
    analysis->CreateNtupleDColumn("ELeak_Right");
    analysis->CreateNtupleDColumn("ELeak_Front");
    analysis->CreateNtupleDColumn("ELeak_Back");
    analysis->CreateNtupleDColumn("ETrue");
    analysis->FinishNtuple();

}

PosRunAction::~PosRunAction()
{}

void PosRunAction::BeginOfRunAction(const G4Run*)
{
    auto analysis = G4AnalysisManager::Instance();
    analysis->OpenFile(); // each worker thread opens its own file
}

void PosRunAction::EndOfRunAction(const G4Run*)
{
    auto analysis = G4AnalysisManager::Instance();
    analysis->Write();
    analysis->CloseFile(); // merging handled by AnalysisManager backend
}
