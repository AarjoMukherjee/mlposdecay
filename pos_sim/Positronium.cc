#include "G4RunManagerFactory.hh"
#include "G4UImanager.hh"
#include "G4UIExecutive.hh"
#include "G4VisExecutive.hh"
#include "G4VisManager.hh"

#include "Construction.hh"
#include "PhysList.hh"
#include "Action.hh"

int main(int argc, char** argv)
{
	#ifdef G4MULTITHREADED
  		auto* runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::MT);
  		runManager->SetNumberOfThreads(10);  // or however many threads you want
	#else
  		auto* runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::Serial);
	#endif

		
	runManager->SetUserInitialization(new PosDetectorConstruction());
	runManager->SetUserInitialization(new PosPhysicsList());
	runManager->SetUserInitialization(new PosActionInitialization());
	
	runManager->Initialize();
	
	G4UIExecutive* ui = 0;
  
  if(argc == 1)
  {
  	ui = new G4UIExecutive(argc, argv);
  }
  
  G4VisManager *visManager = new G4VisExecutive();
  visManager -> Initialize();
  G4UImanager *UImanager = G4UImanager::GetUIpointer();
  if(ui)
  {
		UImanager->ApplyCommand("/control/execute vis.mac");
		ui->SessionStart();
	}
	else
	{
		G4String command = "/control/execute ";
		G4String fileName = argv[1];
		UImanager->ApplyCommand(command+fileName);
	}
	
	delete runManager;
	delete visManager;
	return 0;
}
