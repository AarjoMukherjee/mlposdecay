#ifndef GENERATOR_HH
#define GENERATOR_HH

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "G4Gamma.hh"
#include "G4RandomDirection.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"

class PosPrimaryGenerator : public G4VUserPrimaryGeneratorAction
{
	public:
		PosPrimaryGenerator();
		~PosPrimaryGenerator();
		
 		virtual void GeneratePrimaries(G4Event*);
		G4double GetGunEnergy() const { return fParticleGun->GetParticleEnergy(); }
 	private:
 		G4ParticleGun *fParticleGun;
};

#endif
