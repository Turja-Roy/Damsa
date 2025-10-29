#include "construction.h"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"

DamsaDetectorConstruction::DamsaDetectorConstruction(){}
DamsaDetectorConstruction::~DamsaDetectorConstruction(){}

G4VPhysicalVolume* DamsaDetectorConstruction::Construct() {
    G4NistManager* nist = G4NistManager::Instance();

    G4Material* worldMat = nist->FindOrBuildMaterial("G4_AIR");
    G4Material* tungsten = nist->FindOrBuildMaterial("G4_W");
    G4Material* vacuum = nist->FindOrBuildMaterial("G4_Galactic");
    G4Material* stainlessSteel = nist->FindOrBuildMaterial("G4_STAINLESS-STEEL");
    G4Material* silicon = nist->FindOrBuildMaterial("G4_Si");
    
    G4Material* neodymium = new G4Material("Neodymium", 7.01*g/cm3, 1);
    neodymium->AddElement(nist->FindOrBuildElement("Nd"), 1);

    G4Box* solidWorld = new G4Box("solidWorld", 0.2*m, 0.2*m, 0.6*m);
    G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
    G4VPhysicalVolume* physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, true);

    G4double zStart = -50.*cm;
    G4double zPos = zStart;

    G4double tungstenHalfLength = 5.0*cm;
    G4Box* solidTungsten = new G4Box("solidTungsten", 2.5*cm, 2.5*cm, tungstenHalfLength);
    G4LogicalVolume* logicTungsten = new G4LogicalVolume(solidTungsten, tungsten, "logicTungsten");
    G4VisAttributes* tungstenVis = new G4VisAttributes(G4Colour(0.3, 0.3, 0.3, 1.0));
    tungstenVis->SetForceSolid(true);
    logicTungsten->SetVisAttributes(tungstenVis);
    zPos += tungstenHalfLength;
    G4VPhysicalVolume* physTungsten = new G4PVPlacement(0, G4ThreeVector(0., 0., zPos), logicTungsten, "physTungsten", logicWorld, false, 0, true);
    zPos += tungstenHalfLength;

    G4double chamberInnerRadius = 10.0*cm;
    G4double chamberWallThickness = 0.5*cm;
    G4double chamberOuterRadius = chamberInnerRadius + chamberWallThickness;
    G4double chamberLength = 30.0*cm;
    
    G4Tubs* solidChamberOuter = new G4Tubs("solidChamberOuter", 0., chamberOuterRadius, chamberLength/2., 0., 360.*deg);
    G4Tubs* solidChamberInner = new G4Tubs("solidChamberInner", 0., chamberInnerRadius, chamberLength/2., 0., 360.*deg);
    
    G4SubtractionSolid* solidChamberWall = new G4SubtractionSolid("solidChamberWall", solidChamberOuter, solidChamberInner);
    G4LogicalVolume* logicChamberWall = new G4LogicalVolume(solidChamberWall, stainlessSteel, "logicChamberWall");
    G4VisAttributes* chamberWallVis = new G4VisAttributes(G4Colour(1.0, 1.0, 0.0, 0.7));
    chamberWallVis->SetForceSolid(true);
    logicChamberWall->SetVisAttributes(chamberWallVis);
    
    G4double tungstenSize = 5.0*cm;
    G4double magnetHollowSize = 10.0*cm;
    
    G4Tubs* solidEndCapFull = new G4Tubs("solidEndCapFull", 0., chamberOuterRadius, chamberWallThickness/2., 0., 360.*deg);
    G4Box* solidTungstenOpening = new G4Box("solidTungstenOpening", tungstenSize/2., tungstenSize/2., chamberWallThickness/2.);
    G4Box* solidMagnetOpening = new G4Box("solidMagnetOpening", magnetHollowSize/2., magnetHollowSize/2., chamberWallThickness/2.);
    
    G4SubtractionSolid* solidEndCapFront = new G4SubtractionSolid("solidEndCapFront", solidEndCapFull, solidTungstenOpening);
    G4LogicalVolume* logicEndCapFront = new G4LogicalVolume(solidEndCapFront, stainlessSteel, "logicEndCapFront");
    logicEndCapFront->SetVisAttributes(chamberWallVis);
    
    G4SubtractionSolid* solidEndCapBack = new G4SubtractionSolid("solidEndCapBack", solidEndCapFull, solidMagnetOpening);
    G4LogicalVolume* logicEndCapBack = new G4LogicalVolume(solidEndCapBack, stainlessSteel, "logicEndCapBack");
    logicEndCapBack->SetVisAttributes(chamberWallVis);
    
    G4LogicalVolume* logicChamberVacuum = new G4LogicalVolume(solidChamberInner, vacuum, "logicChamberVacuum");
    logicChamberVacuum->SetVisAttributes(G4VisAttributes::GetInvisible());
    
    zPos += chamberLength/2.;
    G4VPhysicalVolume* physChamberWall = new G4PVPlacement(0, G4ThreeVector(0., 0., zPos), logicChamberWall, "physChamberWall", logicWorld, false, 0, true);
    G4VPhysicalVolume* physChamberVacuum = new G4PVPlacement(0, G4ThreeVector(0., 0., zPos), logicChamberVacuum, "physChamberVacuum", logicWorld, false, 0, true);
    
    G4double endCapFrontZ = zPos - chamberLength/2. + chamberWallThickness/2.;
    G4double endCapBackZ = zPos + chamberLength/2. - chamberWallThickness/2.;
    G4VPhysicalVolume* physEndCapFront = new G4PVPlacement(0, G4ThreeVector(0., 0., endCapFrontZ), logicEndCapFront, "physEndCapFront", logicWorld, false, 0, true);
    G4VPhysicalVolume* physEndCapBack = new G4PVPlacement(0, G4ThreeVector(0., 0., endCapBackZ), logicEndCapBack, "physEndCapBack", logicWorld, false, 1, true);
    
    zPos += chamberLength/2.;

    G4double magnetOuterSize = 20.0*cm;
    
    G4Box* solidMagnetOuter = new G4Box("solidMagnetOuter", magnetOuterSize/2., magnetOuterSize/2., magnetOuterSize/2.);
    G4Box* solidMagnetHollow = new G4Box("solidMagnetHollow", magnetHollowSize/2., magnetHollowSize/2., magnetOuterSize/2.);
    G4SubtractionSolid* solidMagnet = new G4SubtractionSolid("solidMagnet", solidMagnetOuter, solidMagnetHollow);
    G4LogicalVolume* logicMagnet = new G4LogicalVolume(solidMagnet, neodymium, "logicMagnet");
    G4VisAttributes* magnetVis = new G4VisAttributes(G4Colour(0.5, 0.5, 0.5, 0.3));
    magnetVis->SetForceSolid(true);
    logicMagnet->SetVisAttributes(magnetVis);
    
    zPos += magnetOuterSize/2.;
    G4VPhysicalVolume* physMagnet = new G4PVPlacement(0, G4ThreeVector(0., 0., zPos), logicMagnet, "physMagnet", logicWorld, false, 0, true);

    G4double trackerSizeXY = 10.0*cm;
    G4double trackerThickness = 1.0*cm;
    G4double trackerHoleSize = 2.0*cm;
    G4int numTrackers = 6;
    G4double magnetStartZ = zPos - magnetOuterSize/2.;
    G4double magnetEndZ = zPos + magnetOuterSize/2.;
    G4double availableSpace = magnetOuterSize - numTrackers * trackerThickness;
    G4double trackerSpacing = availableSpace / (numTrackers - 1);
    
    G4Box* solidSiTrackerFull = new G4Box("solidSiTrackerFull", trackerSizeXY/2., trackerSizeXY/2., trackerThickness/2.);
    G4Box* solidTrackerHole = new G4Box("solidTrackerHole", trackerHoleSize/2., trackerHoleSize/2., trackerThickness/2.);
    G4SubtractionSolid* solidSiTracker = new G4SubtractionSolid("solidSiTracker", solidSiTrackerFull, solidTrackerHole);
    logicSiTracker = new G4LogicalVolume(solidSiTracker, silicon, "logicSiTracker");
    G4VisAttributes* trackerVis = new G4VisAttributes(G4Colour(0.0, 1.0, 1.0, 0.8));
    trackerVis->SetForceSolid(true);
    logicSiTracker->SetVisAttributes(trackerVis);
    
    G4double trackerStartZ = magnetStartZ + trackerThickness/2.;
    for (G4int i = 0; i < numTrackers; i++) {
        G4double trackerZ = trackerStartZ + i * (trackerThickness + trackerSpacing);
        G4VPhysicalVolume* physSiTracker = new G4PVPlacement(
            0,
            G4ThreeVector(0., 0., trackerZ),
            logicSiTracker,
            "physSiTracker",
            logicWorld,
            false,
            i,
            true
        );
    }

    zPos += magnetOuterSize/2.;

    G4Material* csi = nist->FindOrBuildMaterial("G4_CESIUM_IODIDE");

    G4double caloSizeXY = 12.0*cm;
    G4double layerThickness = 1.0*cm;
    G4int numLayers = 44;
    G4int numCrystalsPerLayer = 12;

    G4Box* solidCrystalX = new G4Box("solidCrystalX", 6.0*cm, 0.5*cm, 0.5*cm);
    G4Box* solidCrystalY = new G4Box("solidCrystalY", 0.5*cm, 6.0*cm, 0.5*cm);
    
    G4LogicalVolume* logicCrystalX = new G4LogicalVolume(solidCrystalX, csi, "logicCrystalX");
    G4LogicalVolume* logicCrystalY = new G4LogicalVolume(solidCrystalY, csi, "logicCrystalY");
    
    G4VisAttributes* caloVis = new G4VisAttributes(G4Colour(1.0, 0.0, 1.0, 0.5));
    caloVis->SetForceSolid(true);
    logicCrystalX->SetVisAttributes(caloVis);
    logicCrystalY->SetVisAttributes(caloVis);
    
    logicCalorimeter = logicCrystalX;

    G4double caloStartZ = zPos + layerThickness/2.;

    for(G4int layer = 0; layer < numLayers; layer++) {
        G4double layerZ = caloStartZ + layer * layerThickness;
        G4bool isXOriented = (layer % 2 == 0);
        
        for(G4int crystal = 0; crystal < numCrystalsPerLayer; crystal++) {
            G4double offset = -caloSizeXY/2. + 0.5*cm + crystal * 1.0*cm;
            G4ThreeVector position;
            G4LogicalVolume* crystalLogic;
            
            if(isXOriented) {
                crystalLogic = logicCrystalX;
                position = G4ThreeVector(0., offset, layerZ);
            } else {
                crystalLogic = logicCrystalY;
                position = G4ThreeVector(offset, 0., layerZ);
            }
            
            G4int copyNo = layer * numCrystalsPerLayer + crystal;
            G4VPhysicalVolume* physCrystal = new G4PVPlacement(
                0,
                position,
                crystalLogic,
                "physCalorimeter",
                logicWorld,
                false,
                copyNo,
                true
            );
        }
    }

    zPos = caloStartZ + numLayers * layerThickness - layerThickness/2.;

    return physWorld;
}

void DamsaDetectorConstruction::ConstructSDandField() {
    DamsaSensitiveDetector* sensDet = new DamsaSensitiveDetector("SensitiveDetector");
    logicSiTracker->SetSensitiveDetector(sensDet);
}
