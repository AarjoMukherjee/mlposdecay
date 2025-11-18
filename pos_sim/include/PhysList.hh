#ifndef PHYSLIST_HH
#define PHYSLIST_HH

#include "G4VModularPhysicsList.hh"
#include "G4EmLivermorePhysics.hh"

class PosPhysicsList : public G4VModularPhysicsList
{
	public:
		PosPhysicsList();
		~PosPhysicsList();
};

#endif
