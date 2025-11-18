#ifndef DETECTOR_HH
#define DETECTOR_HH

#include "G4VSensitiveDetector.hh"
#include "G4AnalysisManager.hh"
#include "G4RunManager.hh"

class PosSensitiveDetector : public G4VSensitiveDetector
{
	public:
		PosSensitiveDetector(G4String);
		~PosSensitiveDetector();
	
	private:
		virtual G4bool ProcessHits(G4Step *, G4TouchableHistory *);
};

#endif 
