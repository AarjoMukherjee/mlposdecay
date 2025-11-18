#include "Stepping.hh"
#include "G4RunManager.hh"
#include "Construction.hh"

PosSteppingAction::PosSteppingAction(PosEventAction *eventAction)
{
	fEventAction = eventAction;
}

PosSteppingAction::~PosSteppingAction()
{}


void PosSteppingAction::UserSteppingAction(const G4Step* step)
{
	G4Track *track = step->GetTrack();
	G4StepPoint *postStepPoint = step->GetPostStepPoint();
	G4StepPoint *preStepPoint = step->GetPreStepPoint();

	G4int trackID = track->GetTrackID();
	G4String particleType = track->GetParticleDefinition()->GetParticleName();

	G4double edep = step->GetTotalEnergyDeposit();

	const G4VProcess* process = postStepPoint->GetProcessDefinedStep();
	G4String procName = process->GetProcessName();

	const G4VPhysicalVolume* preVolume = preStepPoint->GetTouchableHandle()->GetVolume();
	auto postTouchable = postStepPoint->GetTouchableHandle();
	const G4VPhysicalVolume* postVolume = postTouchable->GetVolume();

	auto c = static_cast<PosDetectorConstruction*>(
					const_cast<G4VUserDetectorConstruction*>(
						G4RunManager::GetRunManager()->GetUserDetectorConstruction()
					)
				);

	if(trackID==1 && particleType=="gamma"){
		if(postVolume && postVolume->GetLogicalVolume() == c->logicCrystal && fEventAction->InitHit_flag && preVolume!=postVolume)
			{
				fEventAction->fHit = postStepPoint->GetPosition();
				fEventAction->fCrystalID = postTouchable->GetCopyNumber();
				fEventAction->InitHit_flag = false;
			}
		if(preVolume->GetLogicalVolume() == c->logicCrystal){
			G4int copyNo = preStepPoint->GetTouchableHandle()->GetCopyNumber();
			
			if(edep>0.){
				auto& data = fEventAction->crystalData[copyNo];
				if (procName == "compt") {
					data.EcomptLoss += edep/keV;
				}
				else if (procName == "phot"){
					data.P_BindingEnergy += edep/keV;
				}
			}
		}
		else if(preVolume->GetLogicalVolume() == c->logicArray){
			if(edep>0.) fEventAction->fAbsorb += edep / keV;
		}
	}
	if(!postVolume) {
		if(preVolume->GetLogicalVolume() == c->logicWorld) {
			G4ThreeVector leak = preStepPoint->GetPosition();
			G4double x = leak.x()/mm;
			G4double y = leak.y()/mm;
			G4double z = leak.z()/mm;
			G4double Xbound = (c->Nx*(c->crystX + c->gap)/2)/mm;
			G4double Ybound = (c->Ny*(c->crystY + c->gap)/2)/mm;
			G4double Zbound = ((c->crystZ+c->gap)/2)/mm;
			G4double leakedEnergy = postStepPoint->GetKineticEnergy();
			
			if (std::abs(std::abs(x) - Xbound) < 1e-4) {
				if (x > 0)
					fEventAction->fELeak[1] += leakedEnergy/keV; // Left
				else
					fEventAction->fELeak[3] += leakedEnergy/keV; // Right
			}
			else if (std::abs(std::abs(y) - Ybound) < 1e-4) {
				if (y > 0)
					fEventAction->fELeak[0] += leakedEnergy/keV; // Top
				else
					fEventAction->fELeak[2] += leakedEnergy/keV; // Bottom
			}
			else if (std::abs(std::abs(z) - Zbound) < 1e-4) {
				if (z > 0)
					fEventAction->fELeak[5] += leakedEnergy/keV; // Back
				else
					fEventAction->fELeak[4] += leakedEnergy/keV; // Front
			}
		}
	}
}