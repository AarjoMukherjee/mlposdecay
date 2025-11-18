#include "PhysList.hh"

PosPhysicsList::PosPhysicsList()
{
	RegisterPhysics(new G4EmLivermorePhysics);
	G4cout << "Pos Physics List constructed!" << G4endl;
}

PosPhysicsList::~PosPhysicsList()
{}



