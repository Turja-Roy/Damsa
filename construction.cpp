#include "construction.h"

#include "G4Material.hh"
#include "G4Element.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4RotationMatrix.hh"
#include "MagneticField.h"

DamsaDetectorConstruction::DamsaDetectorConstruction()
    : fLogicSiTracker(nullptr), fLogicCrystal(nullptr), fLogicMagnetHollow(nullptr), fMagField(nullptr),
    fMatAir(nullptr), fMatVacuum(nullptr), fMatTungsten(nullptr), fMatStainlessSteel(nullptr),
    fMatSilicon(nullptr), fMatCsI(nullptr), fMatNeodymium(nullptr)
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

    fTrackerSizeXY = 9.8*cm;
    fTrackerThickness = 0.2*cm;
    fTrackerHoleSize = 2.0*cm;
    fNumTrackers = 6;

    fCaloSizeXY = 12.0*cm;
    fLayerThickness = 1.0*cm;
    fNumCaloLayers = 44;  // Alternating X/Y orientation
    fNumCrystalsPerLayer = 12;
}

DamsaDetectorConstruction::~DamsaDetectorConstruction(){}

G4VPhysicalVolume* DamsaDetectorConstruction::Construct()
{
    DefineMaterials();

    auto* solidWorld = new G4Box("solidWorld", 0.2*m, 0.2*m, 0.6*m);
    auto* logicWorld = new G4LogicalVolume(solidWorld, fMatAir, "logicWorld");
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

    fMatAir = nist->FindOrBuildMaterial("G4_AIR");
    fMatVacuum = nist->FindOrBuildMaterial("G4_Galactic");
    fMatTungsten = nist->FindOrBuildMaterial("G4_W");
    fMatStainlessSteel = nist->FindOrBuildMaterial("G4_STAINLESS-STEEL");
    fMatSilicon = nist->FindOrBuildMaterial("G4_Si");
    fMatCsI = nist->FindOrBuildMaterial("G4_CESIUM_IODIDE");

    // Neodymium
    fMatNeodymium = new G4Material("Neodymium", 7.01*g/cm3, 1);
    fMatNeodymium->AddElement(nist->FindOrBuildElement("Nd"), 1);
}

void DamsaDetectorConstruction::BuildTarget(G4LogicalVolume* worldLV, G4double& zPos)
{
    auto* solidTungsten = new G4Box("solidTungsten", fTargetX/2.0, fTargetY/2.0, fTargetZ/2.0);
    auto* logicTungsten = new G4LogicalVolume(solidTungsten, fMatTungsten, "logicTungsten");
    new G4PVPlacement(0, G4ThreeVector(0., 0., zPos+fTargetZ/2.0), logicTungsten, "physTungsten", worldLV, false, 0, true);

    zPos += fTargetZ;

    auto* tungstenVis = new G4VisAttributes(G4Colour(0.3, 0.3, 0.3, 1.0));
    tungstenVis->SetForceSolid(true);
    logicTungsten->SetVisAttributes(tungstenVis);

    // Target exit scoring volume
    auto* solidScoringTarget = new G4Box("solidScoringTarget", fTargetX/2.0, fTargetY/2.0, 0.1*mm);
    auto* logicScoringTarget = new G4LogicalVolume(solidScoringTarget, 
                                                   fMatVacuum, 
                                                   "logicScoringTarget");
    auto* scoringVis = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0, 1.0));
    scoringVis->SetForceSolid(true);
    logicScoringTarget->SetVisAttributes(scoringVis);
    // logicScoringTarget->SetVisAttributes(G4VisAttributes::GetInvisible());
    new G4PVPlacement(0, G4ThreeVector(0., 0., zPos + 0.1*mm), 
                      logicScoringTarget, "physScoringVolumeTarget", worldLV, false, 0, true);
}

void DamsaDetectorConstruction::BuildVacuumChamber(G4LogicalVolume* worldLV, G4double& zPos)
{
    G4double chamberOuterRadius = fChamberInnerRadius + fChamberWallThickness;

    auto* solidChamberOuter = new G4Tubs("solidChamberOuter", 0., chamberOuterRadius, fChamberLength/2., 0., 360.*deg);
    auto* solidChamberInner = new G4Tubs("solidChamberInner", 0., fChamberInnerRadius, fChamberLength/2., 0., 360.*deg);

    auto* solidChamberWall = new G4SubtractionSolid("solidChamberWall", solidChamberOuter, solidChamberInner);
    auto* logicChamberWall = new G4LogicalVolume(solidChamberWall, fMatStainlessSteel, "logicChamberWall");

    auto* chamberWallVis = new G4VisAttributes(G4Colour(1.0, 1.0, 0.0, 0.7));
    chamberWallVis->SetForceSolid(true);
    logicChamberWall->SetVisAttributes(chamberWallVis);

    G4double tungstenSize = fTargetX;

    auto* solidEndCapFull = new G4Tubs("solidEndCapFull", 0., chamberOuterRadius, fChamberWallThickness/2., 0., 360.*deg);
    auto* solidTungstenOpening = new G4Box("solidTungstenOpening", tungstenSize/2., tungstenSize/2., fChamberWallThickness/2.);
    auto* solidMagnetOpening = new G4Box("solidMagnetOpening", fMagnetHollowSize/2., fMagnetHollowSize/2., fChamberWallThickness/2.);

    auto* solidEndCapFront = new G4SubtractionSolid("solidEndCapFront", solidEndCapFull, solidTungstenOpening);
    auto* logicEndCapFront = new G4LogicalVolume(solidEndCapFront, fMatStainlessSteel, "logicEndCapFront");
    logicEndCapFront->SetVisAttributes(chamberWallVis);

    auto* solidEndCapBack = new G4SubtractionSolid("solidEndCapBack", solidEndCapFull, solidMagnetOpening);
    auto* logicEndCapBack = new G4LogicalVolume(solidEndCapBack, fMatStainlessSteel, "logicEndCapBack");
    logicEndCapBack->SetVisAttributes(chamberWallVis);

    auto* logicChamberVacuum = new G4LogicalVolume(solidChamberInner, fMatVacuum, "logicChamberVacuum");
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
    // Air-filled region placed directly in world to avoid subtraction solid navigation issues.
    // Neodymium magnet frame disabled (B=0).

    auto* solidMagnetHollow = new G4Box("solidMagnetHollow", fMagnetHollowSize/2., fMagnetHollowSize/2., fMagnetOuterSize/2.);
    fLogicMagnetHollow = new G4LogicalVolume(solidMagnetHollow, fMatAir, "logicMagnetHollow");
    fLogicMagnetHollow->SetVisAttributes(G4VisAttributes::GetInvisible());

    zPos += fMagnetOuterSize/2.;
    new G4PVPlacement(0, G4ThreeVector(0., 0., zPos), fLogicMagnetHollow, "physMagnetHollow", worldLV, false, 0, true);

    // Neodymium magnet (disabled)
    // auto* solidMagnetOuter = new G4Box("solidMagnetOuter", fMagnetOuterSize/2., fMagnetOuterSize/2., fMagnetOuterSize/2.);
    // auto* solidMagnet = new G4SubtractionSolid("solidMagnet", solidMagnetOuter, solidMagnetHollow);
    // auto* logicMagnet = new G4LogicalVolume(solidMagnet, fMatNeodymium, "logicMagnet");
    // auto* magnetVis = new G4VisAttributes(G4Colour(0.5, 0.5, 0.5, 0.3));
    // magnetVis->SetForceSolid(true);
    // logicMagnet->SetVisAttributes(magnetVis);
    // new G4PVPlacement(0, G4ThreeVector(0., 0., zPos), logicMagnet, "physMagnet", worldLV, false, 0, true);

    // Detector entrance scoring volume
    G4double hollowHalfZ = fMagnetOuterSize/2.;

    G4double scoringHalfThickness = 0.01*mm;  // Thin to minimize multiple steps
    auto* solidScoringDetector = new G4Box("solidScoringDetector", 4.45*cm, 4.45*cm, scoringHalfThickness);
    auto* logicScoringDetector = new G4LogicalVolume(solidScoringDetector,
                                                     fMatVacuum,
                                                     "logicScoringDetector");
    logicScoringDetector->SetVisAttributes(G4VisAttributes::GetInvisible());

    G4double scoringZ = -hollowHalfZ + scoringHalfThickness + 0.1*mm;  // 0.1mm clearance
    new G4PVPlacement(0, G4ThreeVector(0., 0., scoringZ),
                      logicScoringDetector, "physScoringVolumeDetector", 
                      fLogicMagnetHollow, false, 0, true);

    G4double zClearance = 0.7*mm;  // Clearance for scoring volume and boundary
    G4double availableSpace = 2.*hollowHalfZ - 2.*zClearance - fNumTrackers * fTrackerThickness;
    G4double trackerSpacing = availableSpace / (fNumTrackers - 1);

    G4double trackerFitSize = fTrackerSizeXY - 0.2*mm;
    auto* solidSiTrackerFull = new G4Box("solidSiTrackerFull", trackerFitSize/2., trackerFitSize/2., fTrackerThickness/2.);
    auto* solidTrackerHole = new G4Box("solidTrackerHole", fTrackerHoleSize/2., fTrackerHoleSize/2., fTrackerThickness/2.);
    auto* solidSiTracker = new G4SubtractionSolid("solidSiTracker", solidSiTrackerFull, solidTrackerHole);
    fLogicSiTracker = new G4LogicalVolume(solidSiTracker, fMatSilicon, "logicSiTracker");

    auto* trackerVis = new G4VisAttributes(G4Colour(0.0, 1.0, 1.0, 0.8));
    trackerVis->SetForceSolid(true);
    fLogicSiTracker->SetVisAttributes(trackerVis);

    for (G4int i = 0; i < fNumTrackers; i++) {
        G4double localZ = -hollowHalfZ + zClearance + fTrackerThickness/2. + i * (fTrackerThickness + trackerSpacing);
        new G4PVPlacement(0, G4ThreeVector(0., 0., localZ), fLogicSiTracker, "physSiTracker", fLogicMagnetHollow, false, i, true);
    }

    zPos += fMagnetOuterSize/2.;
}

void DamsaDetectorConstruction::BuildCalorimeter(G4LogicalVolume* worldLV, G4double& zPos)
{
    G4double ecalDepth = fNumCaloLayers * fLayerThickness;
    auto* solidECAL = new G4Box("solidECAL", fCaloSizeXY/2., fCaloSizeXY/2., ecalDepth/2.);
    auto* logicECAL = new G4LogicalVolume(solidECAL, fMatAir, "logicECAL");
    logicECAL->SetVisAttributes(G4VisAttributes::GetInvisible());

    auto* solidCrystal = new G4Box("solidCrystal", 6.0*cm, 0.5*cm, 0.5*cm);
    fLogicCrystal = new G4LogicalVolume(solidCrystal, fMatCsI, "logicCrystal");

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

void DamsaDetectorConstruction::ConstructSDandField()
{
    auto* sdManager = G4SDManager::GetSDMpointer();

    auto* trackerSD = new G4MultiFunctionalDetector("TrackerSD");
    sdManager->AddNewDetector(trackerSD);

    auto* energyDep = new G4PSEnergyDeposit("EnergyDeposit");
    trackerSD->RegisterPrimitive(energyDep);

    auto* nofSecondary = new G4PSNofSecondary("NofSecondary");
    trackerSD->RegisterPrimitive(nofSecondary);

    fLogicSiTracker->SetSensitiveDetector(trackerSD);

    auto* calorimeterSD = new G4MultiFunctionalDetector("CalorimeterSD");
    sdManager->AddNewDetector(calorimeterSD);

    auto* caloEnergyDep = new G4PSEnergyDeposit("EnergyDeposit");
    calorimeterSD->RegisterPrimitive(caloEnergyDep);

    auto* caloNofSecondary = new G4PSNofSecondary("NofSecondary");
    calorimeterSD->RegisterPrimitive(caloNofSecondary);

    fLogicCrystal->SetSensitiveDetector(calorimeterSD);

    // fMagField = new MagneticField();
    // auto* fieldMgr = new G4FieldManager();
    // fieldMgr->SetDetectorField(fMagField);
    // fieldMgr->CreateChordFinder(fMagField);
    // fLogicMagnetHollow->SetFieldManager(fieldMgr, true);
}
