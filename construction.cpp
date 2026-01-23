#include "construction.h"

#include "G4Material.hh"
#include "G4Element.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4RotationMatrix.hh"
#include "MagneticField.h"

DamsaDetectorConstruction::DamsaDetectorConstruction()
: fLogicSiTracker(nullptr), fLogicCrystal(nullptr), fLogicMagnetHollow(nullptr), fMagField(nullptr)
{
    fWorldSize = 0.4*m; // cubic air box containing entire detector setup
    
    fTargetX = 5.0*cm; // tungsten target width (perpendicular to beam)
    fTargetY = 5.0*cm; // tungsten target height
    fTargetZ = 10.0*cm; // tungsten target length (along beam axis)
    
    fChamberInnerRadius = 10.0*cm; // vacuum chamber inner radius
    fChamberWallThickness = 0.5*cm; // stainless steel wall thickness
    fChamberLength = 30.0*cm; // vacuum chamber total length
    
    fMagnetOuterSize = 20.0*cm; // neodymium magnet outer dimension (cube)
    fMagnetHollowSize = 10.0*cm; // square hollow through magnet center
    
    fTrackerSizeXY = 10.0*cm; // silicon tracker plane size
    fTrackerThickness = 0.2*cm; // silicon tracker plane thickness
    fTrackerHoleSize = 2.0*cm; // square hole for beam passage
    fNumTrackers = 6; // number of silicon tracker planes
    
    fCaloSizeXY = 12.0*cm; // calorimeter transverse dimension
    fLayerThickness = 1.0*cm; // spacing between crystal layers (z-direction)
    fNumCaloLayers = 44; // total number of crystal layers (alternating X/Y)
    fNumCrystalsPerLayer = 12; // number of CsI crystals per layer
}

DamsaDetectorConstruction::~DamsaDetectorConstruction(){}

G4VPhysicalVolume* DamsaDetectorConstruction::Construct()
{
    DefineMaterials();
    
    auto* nist = G4NistManager::Instance();
    
    auto* worldMat = nist->FindOrBuildMaterial("G4_AIR");
    auto* solidWorld = new G4Box("solidWorld", 0.2*m, 0.2*m, 0.6*m);
    auto* logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
    // logicWorld->SetVisAttributes(G4VisAttributes::GetInvisible());
    auto* physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, true);
    
    G4double zPos = -50.*cm;
    
    BuildTarget(logicWorld, zPos);
    BuildVacuumChamber(logicWorld, zPos);
    BuildMagnetAndTrackerRegion(logicWorld, zPos);
    BuildCalorimeter(logicWorld, zPos);
    
    return physWorld;
}

void DamsaDetectorConstruction::DefineMaterials()
{
    auto* nist = G4NistManager::Instance();
    
    nist->FindOrBuildMaterial("G4_AIR");
    nist->FindOrBuildMaterial("G4_W");
    nist->FindOrBuildMaterial("G4_Galactic");
    nist->FindOrBuildMaterial("G4_STAINLESS-STEEL");
    nist->FindOrBuildMaterial("G4_Si");
    nist->FindOrBuildMaterial("G4_CESIUM_IODIDE");
    
    auto* neodymium = new G4Material("Neodymium", 7.01*g/cm3, 1);
    neodymium->AddElement(nist->FindOrBuildElement("Nd"), 1);
    
    (void)neodymium;
}

void DamsaDetectorConstruction::BuildTarget(G4LogicalVolume* worldLV, G4double& zPos)
{
    auto* nist = G4NistManager::Instance();
    
    G4double tungstenHalfLength = fTargetZ/2.0;
    auto* solidTungsten = new G4Box("solidTungsten", fTargetX/2.0, fTargetY/2.0, tungstenHalfLength);
    auto* logicTungsten = new G4LogicalVolume(solidTungsten, nist->FindOrBuildMaterial("G4_W"), "logicTungsten");
    
    auto* tungstenVis = new G4VisAttributes(G4Colour(0.3, 0.3, 0.3, 1.0));
    tungstenVis->SetForceSolid(true);
    logicTungsten->SetVisAttributes(tungstenVis);
    
    zPos += tungstenHalfLength;
    new G4PVPlacement(0, G4ThreeVector(0., 0., zPos), logicTungsten, "physTungsten", worldLV, false, 0, true);
    zPos += tungstenHalfLength;
    
    // Target exit scoring volume
    auto* solidScoringTarget = new G4Box("solidScoringTarget", 15.*cm, 15.*cm, 0.1*mm);
    auto* logicScoringTarget = new G4LogicalVolume(solidScoringTarget, 
                                                    nist->FindOrBuildMaterial("G4_Galactic"), 
                                                    "logicScoringTarget");
    logicScoringTarget->SetVisAttributes(G4VisAttributes::GetInvisible());
    
    new G4PVPlacement(0, G4ThreeVector(0., 0., zPos + 0.1*mm), 
                     logicScoringTarget, "physScoringVolumeTarget", worldLV, false, 0, true);
}

void DamsaDetectorConstruction::BuildVacuumChamber(G4LogicalVolume* worldLV, G4double& zPos)
{
    auto* nist = G4NistManager::Instance();
    
    G4double chamberOuterRadius = fChamberInnerRadius + fChamberWallThickness;
    
    auto* solidChamberOuter = new G4Tubs("solidChamberOuter", 0., chamberOuterRadius, fChamberLength/2., 0., 360.*deg);
    auto* solidChamberInner = new G4Tubs("solidChamberInner", 0., fChamberInnerRadius, fChamberLength/2., 0., 360.*deg);
    
    auto* solidChamberWall = new G4SubtractionSolid("solidChamberWall", solidChamberOuter, solidChamberInner);
    auto* logicChamberWall = new G4LogicalVolume(solidChamberWall, nist->FindOrBuildMaterial("G4_STAINLESS-STEEL"), "logicChamberWall");
    
    auto* chamberWallVis = new G4VisAttributes(G4Colour(1.0, 1.0, 0.0, 0.7));
    chamberWallVis->SetForceSolid(true);
    logicChamberWall->SetVisAttributes(chamberWallVis);
    
    G4double tungstenSize = fTargetX;
    
    auto* solidEndCapFull = new G4Tubs("solidEndCapFull", 0., chamberOuterRadius, fChamberWallThickness/2., 0., 360.*deg);
    auto* solidTungstenOpening = new G4Box("solidTungstenOpening", tungstenSize/2., tungstenSize/2., fChamberWallThickness/2.);
    auto* solidMagnetOpening = new G4Box("solidMagnetOpening", fMagnetHollowSize/2., fMagnetHollowSize/2., fChamberWallThickness/2.);
    
    auto* solidEndCapFront = new G4SubtractionSolid("solidEndCapFront", solidEndCapFull, solidTungstenOpening);
    auto* logicEndCapFront = new G4LogicalVolume(solidEndCapFront, nist->FindOrBuildMaterial("G4_STAINLESS-STEEL"), "logicEndCapFront");
    logicEndCapFront->SetVisAttributes(chamberWallVis);
    
    auto* solidEndCapBack = new G4SubtractionSolid("solidEndCapBack", solidEndCapFull, solidMagnetOpening);
    auto* logicEndCapBack = new G4LogicalVolume(solidEndCapBack, nist->FindOrBuildMaterial("G4_STAINLESS-STEEL"), "logicEndCapBack");
    logicEndCapBack->SetVisAttributes(chamberWallVis);
    
    auto* logicChamberVacuum = new G4LogicalVolume(solidChamberInner, nist->FindOrBuildMaterial("G4_Galactic"), "logicChamberVacuum");
    logicChamberVacuum->SetVisAttributes(G4VisAttributes::GetInvisible());
    
    zPos += fChamberLength/2.;
    new G4PVPlacement(0, G4ThreeVector(0., 0., zPos), logicChamberWall, "physChamberWall", worldLV, false, 0, true);
    new G4PVPlacement(0, G4ThreeVector(0., 0., zPos), logicChamberVacuum, "physChamberVacuum", worldLV, false, 0, true);
    
    G4double endCapFrontZ = zPos - fChamberLength/2. - fChamberWallThickness/2.;
    G4double endCapBackZ = zPos + fChamberLength/2. + fChamberWallThickness/2.;
    new G4PVPlacement(0, G4ThreeVector(0., 0., endCapFrontZ), logicEndCapFront, "physEndCapFront", worldLV, false, 0, true);
    new G4PVPlacement(0, G4ThreeVector(0., 0., endCapBackZ), logicEndCapBack, "physEndCapBack", worldLV, false, 1, true);
    
    zPos += fChamberLength/2. + fChamberWallThickness;
}

void DamsaDetectorConstruction::BuildMagnetAndTrackerRegion(G4LogicalVolume* worldLV, G4double& zPos)
{
    auto* nist = G4NistManager::Instance();
    
    auto* solidMagnetOuter = new G4Box("solidMagnetOuter", fMagnetOuterSize/2., fMagnetOuterSize/2., fMagnetOuterSize/2.);
    auto* solidMagnetHollow = new G4Box("solidMagnetHollow", fMagnetHollowSize/2., fMagnetHollowSize/2., fMagnetOuterSize/2.);
    auto* solidMagnet = new G4SubtractionSolid("solidMagnet", solidMagnetOuter, solidMagnetHollow);
    auto* logicMagnet = new G4LogicalVolume(solidMagnet, G4Material::GetMaterial("Neodymium"), "logicMagnet");
    
    auto* solidMagnetHollowDaughter = new G4Box("solidMagnetHollowDaughter", fMagnetHollowSize/2. - 0.01*mm, fMagnetHollowSize/2. - 0.01*mm, fMagnetOuterSize/2. - 0.01*mm);
    
    auto* magnetVis = new G4VisAttributes(G4Colour(0.5, 0.5, 0.5, 0.3));
    magnetVis->SetForceSolid(true);
    logicMagnet->SetVisAttributes(magnetVis);
    
    fLogicMagnetHollow = new G4LogicalVolume(solidMagnetHollowDaughter, nist->FindOrBuildMaterial("G4_AIR"), "logicMagnetHollow");
    fLogicMagnetHollow->SetVisAttributes(G4VisAttributes::GetInvisible());
    
    zPos += fMagnetOuterSize/2.;
    new G4PVPlacement(0, G4ThreeVector(0., 0., zPos), logicMagnet, "physMagnet", worldLV, false, 0, true);
    new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), fLogicMagnetHollow, "physMagnetHollow", logicMagnet, false, 0, true);
    
    // Detector entrance scoring volume (inside magnet hollow, before first tracker)
    // auto* solidScoringDetector = new G4Box("solidScoringDetector", 15.*cm, 15.*cm, 0.1*mm);
    auto* solidScoringDetector = new G4Box("solidScoringDetector", 4.45*cm, 4.45*cm, 0.1*mm);
    auto* logicScoringDetector = new G4LogicalVolume(solidScoringDetector,
                                                      nist->FindOrBuildMaterial("G4_Galactic"),
                                                      "logicScoringDetector");
    logicScoringDetector->SetVisAttributes(G4VisAttributes::GetInvisible());
    
    G4double scoringZ = -fMagnetOuterSize/2. + 0.5*mm;
    new G4PVPlacement(0, G4ThreeVector(0., 0., scoringZ),
                     logicScoringDetector, "physScoringVolumeDetector", 
                     fLogicMagnetHollow, false, 0, true);
    
    G4double availableSpace = fMagnetOuterSize - fNumTrackers * fTrackerThickness;
    G4double trackerSpacing = availableSpace / (fNumTrackers - 1);
    
    G4double trackerFitSize = fTrackerSizeXY - 0.1*mm;
    auto* solidSiTrackerFull = new G4Box("solidSiTrackerFull", trackerFitSize/2., trackerFitSize/2., fTrackerThickness/2.);
    auto* solidTrackerHole = new G4Box("solidTrackerHole", fTrackerHoleSize/2., fTrackerHoleSize/2., fTrackerThickness/2.);
    auto* solidSiTracker = new G4SubtractionSolid("solidSiTracker", solidSiTrackerFull, solidTrackerHole);
    fLogicSiTracker = new G4LogicalVolume(solidSiTracker, nist->FindOrBuildMaterial("G4_Si"), "logicSiTracker");
    
    auto* trackerVis = new G4VisAttributes(G4Colour(0.0, 1.0, 1.0, 0.8));
    trackerVis->SetForceSolid(true);
    fLogicSiTracker->SetVisAttributes(trackerVis);
    
    for (G4int i = 0; i < fNumTrackers; i++) {
        G4double localZ = -fMagnetOuterSize/2. + fTrackerThickness/2. + i * (fTrackerThickness + trackerSpacing);
        new G4PVPlacement(0, G4ThreeVector(0., 0., localZ), fLogicSiTracker, "physSiTracker", fLogicMagnetHollow, false, i, true);
    }
    
    zPos += fMagnetOuterSize/2.;
}

void DamsaDetectorConstruction::BuildCalorimeter(G4LogicalVolume* worldLV, G4double& zPos)
{
    auto* csi = G4Material::GetMaterial("G4_CESIUM_IODIDE");
    auto* air = G4Material::GetMaterial("G4_AIR");
    
    G4double ecalDepth = fNumCaloLayers * fLayerThickness;
    auto* solidECAL = new G4Box("solidECAL", fCaloSizeXY/2., fCaloSizeXY/2., ecalDepth/2.);
    auto* logicECAL = new G4LogicalVolume(solidECAL, air, "logicECAL");
    logicECAL->SetVisAttributes(G4VisAttributes::GetInvisible());
    
    auto* solidCrystal = new G4Box("solidCrystal", 6.0*cm, 0.5*cm, 0.5*cm);
    fLogicCrystal = new G4LogicalVolume(solidCrystal, csi, "logicCrystal");
    
    auto* caloVis = new G4VisAttributes(G4Colour(1.0, 0.0, 1.0, 0.5));
    caloVis->SetForceSolid(true);
    fLogicCrystal->SetVisAttributes(caloVis);
    
    for(G4int layer = 0; layer < fNumCaloLayers; layer++) {
        G4double localZ = -ecalDepth/2. + fLayerThickness/2. + layer * fLayerThickness;
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
                position = G4ThreeVector(0., offset, localZ);
            } else {
                position = G4ThreeVector(offset, 0., localZ);
            }
            
            G4int copyNo = layer * fNumCrystalsPerLayer + crystal;
            new G4PVPlacement(rot, position, fLogicCrystal, "physCalorimeter", logicECAL, false, copyNo, true);
        }
    }
    
    zPos += ecalDepth/2.;
    new G4PVPlacement(0, G4ThreeVector(0., 0., zPos), logicECAL, "physECAL", worldLV, false, 0, true);
    zPos += ecalDepth/2.;
}

// void DamsaDetectorConstruction::ConstructSDandField()
// {
//     auto* sdManager = G4SDManager::GetSDMpointer();
//     
//     auto* trackerSD = new G4MultiFunctionalDetector("TrackerSD");
//     sdManager->AddNewDetector(trackerSD);
//     
//     auto* energyDep = new G4PSEnergyDeposit("EnergyDeposit");
//     trackerSD->RegisterPrimitive(energyDep);
//     
//     auto* nofSecondary = new G4PSNofSecondary("NofSecondary");
//     trackerSD->RegisterPrimitive(nofSecondary);
//     
//     fLogicSiTracker->SetSensitiveDetector(trackerSD);
//     
//     auto* calorimeterSD = new G4MultiFunctionalDetector("CalorimeterSD");
//     sdManager->AddNewDetector(calorimeterSD);
//     
//     auto* caloEnergyDep = new G4PSEnergyDeposit("EnergyDeposit");
//     calorimeterSD->RegisterPrimitive(caloEnergyDep);
//     
//     auto* caloNofSecondary = new G4PSNofSecondary("NofSecondary");
//     calorimeterSD->RegisterPrimitive(caloNofSecondary);
//     
//     fLogicCrystal->SetSensitiveDetector(calorimeterSD);
//     
//     fMagField = new MagneticField();
//     auto* fieldMgr = new G4FieldManager();
//     fieldMgr->SetDetectorField(fMagField);
//     fieldMgr->CreateChordFinder(fMagField);
//     fLogicMagnetHollow->SetFieldManager(fieldMgr, true);
// }
