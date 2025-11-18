#include "Action.hh"

PosActionInitialization::PosActionInitialization()
{}

PosActionInitialization::~PosActionInitialization()
{}

void PosActionInitialization::BuildForMaster() const
{
	PosRunAction *runAction = new PosRunAction();
	SetUserAction(runAction);
}

void PosActionInitialization::Build() const
{
	PosPrimaryGenerator *generator = new PosPrimaryGenerator();
	SetUserAction(generator);
	
	PosRunAction *runAction = new PosRunAction();
	SetUserAction(runAction);
	
	PosEventAction *eventAction = new PosEventAction(runAction);
	SetUserAction(eventAction);
	
	PosSteppingAction *steppingAction = new PosSteppingAction(eventAction);
	SetUserAction(steppingAction);
}