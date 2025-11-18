#include "Detector.hh"
#include "Event.hh"
#include "Construction.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4VProcess.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"

PosSensitiveDetector::PosSensitiveDetector(G4String name)
: G4VSensitiveDetector(name)
{}

PosSensitiveDetector::~PosSensitiveDetector()
{}

G4bool PosSensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *)
{
    G4double edep = aStep->GetTotalEnergyDeposit();

    G4LogicalVolume* lv =
        aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();

    // Access detector geometry
    auto det = static_cast<const PosDetectorConstruction*>(
        const_cast<G4VUserDetectorConstruction*>(
            G4RunManager::GetRunManager()->GetUserDetectorConstruction()
        )
    );

    auto evt = static_cast<PosEventAction*>(
        const_cast<G4UserEventAction*>(
            G4RunManager::GetRunManager()->GetUserEventAction()
        )
    );

    if (edep <= 0) return false;

    G4Track* track = aStep->GetTrack();
    auto particle = track->GetParticleDefinition();

    // Only charged particles deposit measurable energy in real detectors
    if (track->GetParentID() == 0.0)
        return false;

    if(lv == det->logicCrystal)
    {
        // Creator process tells us whether electron came from Compton or Photoelectric
        const G4VProcess* creator = track->GetCreatorProcess();

        // If there's no creator process → this is the primary gamma or user injection
        // It normally does not deposit energy in a crystal anyway.
        if (!creator)
        {
            // You may want to ignore or log this:
            // (e.g. optical photons created by scintillation)
            return false;
        }

        G4String procName = creator->GetProcessName();

        // Identify crystal by copy number
        G4TouchableHandle touchable = aStep->GetPreStepPoint()->GetTouchableHandle();
        G4int copyNo = touchable->GetCopyNumber();

        // Access event-level storage (your PosEventAction)
        auto& hit = evt->crystalData[copyNo];

        // Classify by interaction type
        if (procName == "compt") {
            hit.Ecompt += edep / keV;
            hit.totalEdep += edep / keV;
        }
        else if (procName == "phot") {
            hit.Ephoto += edep / keV;
            hit.totalEdep += edep / keV;
        }
        else {
            // For delta-rays, bremsstrahlung electrons, etc.
            hit.Eother += edep / keV;
            hit.totalEdep += edep / keV;
        }
    }
    else if(lv == det->logicArray){
        evt->fAbsorb += edep / keV;
    }
    return true;
}
