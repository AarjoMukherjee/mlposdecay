#ifndef ACTION_HH
#define ACTION_HH

#include "G4VUserActionInitialization.hh"
#include "PrimGen.hh"
#include "Run.hh"
#include "Event.hh"
#include "Stepping.hh"

class PosActionInitialization : public G4VUserActionInitialization
{
	public:
		PosActionInitialization();
		~PosActionInitialization();
		
		virtual void Build() const;
		virtual void BuildForMaster() const;
};

#endif
