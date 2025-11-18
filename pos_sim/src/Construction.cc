#include "Construction.hh"

PosDetectorConstruction::PosDetectorConstruction()
{}

PosDetectorConstruction::~PosDetectorConstruction()
{}

G4VPhysicalVolume *PosDetectorConstruction::Construct()
{
  	auto nist = G4NistManager::Instance();

    // G4double density = CLHEP::universe_mean_density;
    // G4Material* spaceMat = new G4Material("spaceMat", density, 2);
    // spaceMat->AddElement(nist->FindOrBuildElement("N"), .7);
    // spaceMat->AddElement(nist->FindOrBuildElement("O"), .3); 

    G4Material* spaceMat = nist->FindOrBuildMaterial("G4_Galactic");

    // G4Material *lyso = new G4Material("G4_LYSO", 7.1*g/cm3, 4);
		// lyso->AddElement(nist->FindOrBuildElement("Lu"), 0.7145);
		// lyso->AddElement(nist->FindOrBuildElement("Y"), 0.0403);
		// lyso->AddElement(nist->FindOrBuildElement("Si"), 0.0637);
		// lyso->AddElement(nist->FindOrBuildElement("O"), 0.1815);

    G4Material *gagg = new G4Material("G4_GAGG", 6.6*g/cm3, 5);
		gagg->AddElement(nist->FindOrBuildElement("Gd"), 0.503);
		gagg->AddElement(nist->FindOrBuildElement("Al"), 0.05764);
		gagg->AddElement(nist->FindOrBuildElement("Ga"), 0.22352);
		gagg->AddElement(nist->FindOrBuildElement("O"), 0.20493);
    gagg->AddElement(nist->FindOrBuildElement("Ce"), 0.01); 

    G4Material* tio2 = nist->FindOrBuildMaterial("G4_TITANIUM_DIOXIDE");

    // ----- World -----
    G4double world_size_xy = 50*mm;
    G4double world_size_z  = 12*cm;
    auto solidWorld = new G4Box("World", world_size_xy/2, world_size_xy/2, world_size_z/2);
    logicWorld = new G4LogicalVolume(solidWorld, spaceMat, "World");
    auto physWorld  = new G4PVPlacement(0, {}, logicWorld, "World", 0, false, 0, true);

    // ----- Crystal dimensions -----

    G4double pitchX = crystX + gap;
    G4double pitchY = crystY + gap;
    G4double fullX = Nx*pitchX;
    G4double fullY = Ny*pitchY;
    G4double fullZ = crystZ+gap;

    // ----- Create array mother volume (TiO2) -----
    auto solidArray = new G4Box("Array", fullX/2, fullY/2, fullZ/2);
    logicArray = new G4LogicalVolume(solidArray, tio2, "Array");

    // ----- Visual attributes -----
    // Blue wireframe for LYSO crystals
    auto blueWire = new G4VisAttributes(G4Colour(0.0, 0.0, 1.0)); 
    blueWire->SetVisibility(true);
    blueWire->SetForceAuxEdgeVisible(true);  // show edges
    blueWire->SetForceSolid(false);          // no solid fill

    // Red wireframe for TiO2 gaps
    auto redWire = new G4VisAttributes(G4Colour(1.0, 0.0, 0.0));
    redWire->SetVisibility(true);
    redWire->SetForceAuxEdgeVisible(true);
    redWire->SetForceSolid(false);


    logicArray->SetVisAttributes(redWire);   // TiO2 gaps = red

    // ----- LYSO crystals -----
    auto solidCrystal = new G4Box("Crystal", crystX/2, crystY/2, crystZ/2);
    logicCrystal = new G4LogicalVolume(solidCrystal, gagg, "Crystal");
    logicCrystal->SetVisAttributes(blueWire); // LYSO = blue

    for (int ix = 0; ix < Nx; ix++) {
        for (int iy = 0; iy < Ny; iy++) {
            G4double xpos = fullX/2 - (ix+0.5)*pitchX;
            G4double ypos = fullY/2 - (iy+0.5)*pitchY;
            G4double zpos = 0.0 + gap/2;
            
            new G4PVPlacement(0,
                              G4ThreeVector(xpos, ypos, zpos),
                              logicCrystal,
                              "Crystal",
                              logicArray,
                              false,
                              iy*Ny + ix,
                              true);
        }
    }

    // ----- Place array in world -----
    new G4PVPlacement(0,
                      G4ThreeVector(),
                      logicArray,
                      "Array",
                      logicWorld,
                      false,
                      0,
                      true);
    return physWorld;
}

void PosDetectorConstruction::ConstructSDandField()
{
	PosSensitiveDetector *sensDet = new PosSensitiveDetector("sensDet");
	logicCrystal->SetSensitiveDetector(sensDet);
  logicArray->SetSensitiveDetector(sensDet);
}
