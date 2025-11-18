#ifndef CONSTRUCTION_HH
#define CONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "G4Box.hh"
#include "G4VSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include <cmath>

#include "Detector.hh"

class PosDetectorConstruction : public G4VUserDetectorConstruction
{
	public:
		PosDetectorConstruction();
		~PosDetectorConstruction();
		
		virtual G4VPhysicalVolume *Construct();
		G4double crystX = 3.0*mm, crystY = 3.0*mm, crystZ = 10.0*mm;
    	G4double gap = 0.05*mm;
		G4int Nx = 8, Ny = 8;
		G4LogicalVolume *logicWorld, *logicCrystal, *logicArray;	
	private:
		virtual void ConstructSDandField();
};

#endif
