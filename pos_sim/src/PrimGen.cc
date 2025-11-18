#include "PrimGen.hh"

PosPrimaryGenerator::PosPrimaryGenerator()
{	
	fParticleGun = new G4ParticleGun(1);
}

PosPrimaryGenerator::~PosPrimaryGenerator()
{
	delete fParticleGun;
}

void PosPrimaryGenerator::GeneratePrimaries(G4Event *pEvent)
{
	G4ParticleTable *pTable = G4ParticleTable::GetParticleTable();
	G4ParticleDefinition *particle = pTable->FindParticle("gamma");
	G4double maxAngle = 2.0 * CLHEP::deg;

    G4double cosTheta = 1.0 - G4UniformRand() * (1.0 - std::cos(maxAngle));
    G4double sinTheta = std::sqrt(1.0 - cosTheta*cosTheta);
    G4double phi = 2.0 * CLHEP::pi * G4UniformRand();

    G4double dx = sinTheta * std::cos(phi);
    G4double dy = sinTheta * std::sin(phi);
    G4double dz = cosTheta;

    G4ThreeVector dir(dx, dy, dz);
    dir = dir.unit();

    fParticleGun->SetParticleMomentumDirection(dir);

    fParticleGun->SetParticlePosition(G4ThreeVector(0., 0., -55.025*mm));

    G4double eMin = 0.0;
    G4double eMax = 1000.*keV;
    G4double energy = eMin + (eMax - eMin) * G4UniformRand();
    fParticleGun->SetParticleEnergy(energy);
	fParticleGun->SetParticleDefinition(particle);
	fParticleGun->GeneratePrimaryVertex(pEvent);
}

