#include "construction.h"

#include "G4Material.hh"
#include "G4Element.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4RotationMatrix.hh"

DamsaDetectorConstruction::DamsaDetectorConstruction()
: fLogicSiTracker(nullptr), fLogicCrystal(nullptr)
{
    fWorldSize = 0.4*m;
    
    fTargetX = 5.0*cm;
    fTargetY = 5.0*cm;
    fTargetZ = 10.0*cm;
    
    fChamberInnerRadius = 10.0*cm;
    fChamberWallThickness = 0.5*cm;
    fChamberLength = 30.0*cm;
    
    fMagnetOuterSize = 20.0*cm;
    fMagnetHollowSize = 10.0*cm;
    
    fTrackerSizeXY = 10.0*cm;
    fTrackerThickness = 1.0*cm;
    fTrackerHoleSize = 2.0*cm;
    fNumTrackers = 6;
    
    fCaloSizeXY = 12.0*cm;
    fLayerThickness = 1.0*cm;
    fNumCaloLayers = 44;
    fNumCrystalsPerLayer = 12;
}

DamsaDetectorConstruction::~DamsaDetectorConstruction(){}

void DamsaDetectorConstruction::DefineMaterials()
{
    G4NistManager* nist = G4NistManager::Instance();
    
    nist->FindOrBuildMaterial("G4_AIR");
    nist->FindOrBuildMaterial("G4_W");
    nist->FindOrBuildMaterial("G4_Galactic");
    nist->FindOrBuildMaterial("G4_STAINLESS-STEEL");
    nist->FindOrBuildMaterial("G4_Si");
    nist->FindOrBuildMaterial("G4_CESIUM_IODIDE");
    
    G4Material* neodymium = new G4Material("Neodymium", 7.01*g/cm3, 1);
    neodymium->AddElement(nist->FindOrBuildElement("Nd"), 1);
    
    (void)neodymium;
}

void DamsaDetectorConstruction::BuildTarget(G4LogicalVolume* worldLV, G4double& zPos)
{
    G4NistManager* nist = G4NistManager::Instance();
    
    G4double tungstenHalfLength = fTargetZ/2.0;
    G4Box* solidTungsten = new G4Box("solidTungsten", fTargetX/2.0, fTargetY/2.0, tungstenHalfLength);
    G4LogicalVolume* logicTungsten = new G4LogicalVolume(solidTungsten, nist->FindOrBuildMaterial("G4_W"), "logicTungsten");
    
    G4VisAttributes* tungstenVis = new G4VisAttributes(G4Colour(0.3, 0.3, 0.3, 1.0));
    tungstenVis->SetForceSolid(true);
    logicTungsten->SetVisAttributes(tungstenVis);
    
    zPos += tungstenHalfLength;
    new G4PVPlacement(0, G4ThreeVector(0., 0., zPos), logicTungsten, "physTungsten", worldLV, false, 0, true);
    zPos += tungstenHalfLength;
}

void DamsaDetectorConstruction::BuildVacuumChamber(G4LogicalVolume* worldLV, G4double& zPos)
{
    G4NistManager* nist = G4NistManager::Instance();
    
    G4double chamberOuterRadius = fChamberInnerRadius + fChamberWallThickness;
    
    G4Tubs* solidChamberOuter = new G4Tubs("solidChamberOuter", 0., chamberOuterRadius, fChamberLength/2., 0., 360.*deg);
    G4Tubs* solidChamberInner = new G4Tubs("solidChamberInner", 0., fChamberInnerRadius, fChamberLength/2., 0., 360.*deg);
    
    G4SubtractionSolid* solidChamberWall = new G4SubtractionSolid("solidChamberWall", solidChamberOuter, solidChamberInner);
    G4LogicalVolume* logicChamberWall = new G4LogicalVolume(solidChamberWall, nist->FindOrBuildMaterial("G4_STAINLESS-STEEL"), "logicChamberWall");
    
    G4VisAttributes* chamberWallVis = new G4VisAttributes(G4Colour(1.0, 1.0, 0.0, 0.7));
    chamberWallVis->SetForceSolid(true);
    logicChamberWall->SetVisAttributes(chamberWallVis);
    
    G4double tungstenSize = fTargetX;
    
    G4Tubs* solidEndCapFull = new G4Tubs("solidEndCapFull", 0., chamberOuterRadius, fChamberWallThickness/2., 0., 360.*deg);
    G4Box* solidTungstenOpening = new G4Box("solidTungstenOpening", tungstenSize/2., tungstenSize/2., fChamberWallThickness/2.);
    G4Box* solidMagnetOpening = new G4Box("solidMagnetOpening", fMagnetHollowSize/2., fMagnetHollowSize/2., fChamberWallThickness/2.);
    
    G4SubtractionSolid* solidEndCapFront = new G4SubtractionSolid("solidEndCapFront", solidEndCapFull, solidTungstenOpening);
    G4LogicalVolume* logicEndCapFront = new G4LogicalVolume(solidEndCapFront, nist->FindOrBuildMaterial("G4_STAINLESS-STEEL"), "logicEndCapFront");
    logicEndCapFront->SetVisAttributes(chamberWallVis);
    
    G4SubtractionSolid* solidEndCapBack = new G4SubtractionSolid("solidEndCapBack", solidEndCapFull, solidMagnetOpening);
    G4LogicalVolume* logicEndCapBack = new G4LogicalVolume(solidEndCapBack, nist->FindOrBuildMaterial("G4_STAINLESS-STEEL"), "logicEndCapBack");
    logicEndCapBack->SetVisAttributes(chamberWallVis);
    
    G4LogicalVolume* logicChamberVacuum = new G4LogicalVolume(solidChamberInner, nist->FindOrBuildMaterial("G4_Galactic"), "logicChamberVacuum");
    logicChamberVacuum->SetVisAttributes(G4VisAttributes::GetInvisible());
    
    zPos += fChamberLength/2.;
    new G4PVPlacement(0, G4ThreeVector(0., 0., zPos), logicChamberWall, "physChamberWall", worldLV, false, 0, true);
    new G4PVPlacement(0, G4ThreeVector(0., 0., zPos), logicChamberVacuum, "physChamberVacuum", worldLV, false, 0, true);
    
    G4double endCapFrontZ = zPos - fChamberLength/2. + fChamberWallThickness/2.;
    G4double endCapBackZ = zPos + fChamberLength/2. - fChamberWallThickness/2.;
    new G4PVPlacement(0, G4ThreeVector(0., 0., endCapFrontZ), logicEndCapFront, "physEndCapFront", worldLV, false, 0, true);
    new G4PVPlacement(0, G4ThreeVector(0., 0., endCapBackZ), logicEndCapBack, "physEndCapBack", worldLV, false, 1, true);
    
    zPos += fChamberLength/2.;
}

void DamsaDetectorConstruction::BuildMagnet(G4LogicalVolume* worldLV, G4double& zPos)
{
    G4Box* solidMagnetOuter = new G4Box("solidMagnetOuter", fMagnetOuterSize/2., fMagnetOuterSize/2., fMagnetOuterSize/2.);
    G4Box* solidMagnetHollow = new G4Box("solidMagnetHollow", fMagnetHollowSize/2., fMagnetHollowSize/2., fMagnetOuterSize/2.);
    G4SubtractionSolid* solidMagnet = new G4SubtractionSolid("solidMagnet", solidMagnetOuter, solidMagnetHollow);
    G4LogicalVolume* logicMagnet = new G4LogicalVolume(solidMagnet, G4Material::GetMaterial("Neodymium"), "logicMagnet");
    
    G4VisAttributes* magnetVis = new G4VisAttributes(G4Colour(0.5, 0.5, 0.5, 0.3));
    magnetVis->SetForceSolid(true);
    logicMagnet->SetVisAttributes(magnetVis);
    
    zPos += fMagnetOuterSize/2.;
    new G4PVPlacement(0, G4ThreeVector(0., 0., zPos), logicMagnet, "physMagnet", worldLV, false, 0, true);
}

void DamsaDetectorConstruction::BuildTrackerRegion(G4LogicalVolume* worldLV, G4double& zPos)
{
    G4NistManager* nist = G4NistManager::Instance();
    
    G4double magnetStartZ = zPos - fMagnetOuterSize/2.;
    G4double availableSpace = fMagnetOuterSize - fNumTrackers * fTrackerThickness;
    G4double trackerSpacing = availableSpace / (fNumTrackers - 1);
    
    G4Box* solidSiTrackerFull = new G4Box("solidSiTrackerFull", fTrackerSizeXY/2., fTrackerSizeXY/2., fTrackerThickness/2.);
    G4Box* solidTrackerHole = new G4Box("solidTrackerHole", fTrackerHoleSize/2., fTrackerHoleSize/2., fTrackerThickness/2.);
    G4SubtractionSolid* solidSiTracker = new G4SubtractionSolid("solidSiTracker", solidSiTrackerFull, solidTrackerHole);
    fLogicSiTracker = new G4LogicalVolume(solidSiTracker, nist->FindOrBuildMaterial("G4_Si"), "logicSiTracker");
    
    G4VisAttributes* trackerVis = new G4VisAttributes(G4Colour(0.0, 1.0, 1.0, 0.8));
    trackerVis->SetForceSolid(true);
    fLogicSiTracker->SetVisAttributes(trackerVis);
    
    G4double trackerStartZ = magnetStartZ + fTrackerThickness/2.;
    for (G4int i = 0; i < fNumTrackers; i++) {
        G4double trackerZ = trackerStartZ + i * (fTrackerThickness + trackerSpacing);
        new G4PVPlacement(0, G4ThreeVector(0., 0., trackerZ), fLogicSiTracker, "physSiTracker", worldLV, false, i, true);
    }
    
    zPos += fMagnetOuterSize/2.;
}

void DamsaDetectorConstruction::BuildCalorimeter(G4LogicalVolume* worldLV, G4double& zPos)
{
    G4Material* csi = G4Material::GetMaterial("G4_CESIUM_IODIDE");
    
    G4Box* solidCrystal = new G4Box("solidCrystal", 6.0*cm, 0.5*cm, 0.5*cm);
    
    fLogicCrystal = new G4LogicalVolume(solidCrystal, csi, "logicCrystal");
    
    G4VisAttributes* caloVis = new G4VisAttributes(G4Colour(1.0, 0.0, 1.0, 0.5));
    caloVis->SetForceSolid(true);
    fLogicCrystal->SetVisAttributes(caloVis);
    
    G4double caloStartZ = zPos + fLayerThickness/2.;
    
    for(G4int layer = 0; layer < fNumCaloLayers; layer++) {
        G4double layerZ = caloStartZ + layer * fLayerThickness;
        G4bool isXOriented = (layer % 2 == 0);
        
        G4RotationMatrix* rot = nullptr;
        if(!isXOriented) {
            rot = new G4RotationMatrix();
            rot->rotateZ(90.*deg);
        }
        
        for(G4int crystal = 0; crystal < fNumCrystalsPerLayer; crystal++) {
            G4double offset = -fCaloSizeXY/2. + 0.5*cm + crystal * 1.0*cm;
            G4ThreeVector position;
            
            if(isXOriented) {
                position = G4ThreeVector(0., offset, layerZ);
            } else {
                position = G4ThreeVector(offset, 0., layerZ);
            }
            
            G4int copyNo = layer * fNumCrystalsPerLayer + crystal;
            new G4PVPlacement(rot, position, fLogicCrystal, "physCalorimeter", worldLV, false, copyNo, true);
        }
    }
    
    zPos = caloStartZ + fNumCaloLayers * fLayerThickness - fLayerThickness/2.;
}

G4VPhysicalVolume* DamsaDetectorConstruction::Construct()
{
    DefineMaterials();
    
    G4NistManager* nist = G4NistManager::Instance();
    
    G4Material* worldMat = nist->FindOrBuildMaterial("G4_AIR");
    G4Box* solidWorld = new G4Box("solidWorld", 0.2*m, 0.2*m, 0.6*m);
    G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
    G4VPhysicalVolume* physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, true);
    
    G4double zPos = -50.*cm;
    
    BuildTarget(logicWorld, zPos);
    BuildVacuumChamber(logicWorld, zPos);
    BuildMagnet(logicWorld, zPos);
    BuildTrackerRegion(logicWorld, zPos);
    BuildCalorimeter(logicWorld, zPos);
    
    return physWorld;
}

void DamsaDetectorConstruction::ConstructSDandField()
{
    DamsaSensitiveDetector* sensDet = new DamsaSensitiveDetector("SensitiveDetector");
    fLogicSiTracker->SetSensitiveDetector(sensDet);
}
