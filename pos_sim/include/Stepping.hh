#ifndef STEPPING_HH
#define STEPPING_HH

#include "G4UserSteppingAction.hh"
#include "G4AnalysisManager.hh"
#include "G4Step.hh"
#include "G4VProcess.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"

#include "Event.hh"

class PosSteppingAction : public G4UserSteppingAction
{
	public:
		PosSteppingAction(PosEventAction* eventAction);
		~PosSteppingAction();
		
		virtual void UserSteppingAction(const G4Step*);
		
	private:
		PosEventAction *fEventAction;
};

#endif
