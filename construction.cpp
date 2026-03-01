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

    // Target exit scoring volume (placed inside vacuum chamber)
    // Target rear face is at zPos (current value), scoring plane 0.1mm after
    G4double targetExitZ_absolute = zPos + 0.1*mm;
    G4double chamberCenterZ_absolute = zPos + fChamberLength/2.;  // Chamber will be centered here
    G4double targetExitZ_local = targetExitZ_absolute - chamberCenterZ_absolute;  // Local Z in chamber frame
    
    auto* solidScoringTarget = new G4Box("solidScoringTarget", fTargetX/2.0, fTargetY/2.0, 0.1*mm);
    auto* logicScoringTarget = new G4LogicalVolume(solidScoringTarget, 
                                                   fMatVacuum, 
                                                   "logicScoringTarget");
    auto* scoringVis = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0, 1.0));
    scoringVis->SetForceSolid(true);
    logicScoringTarget->SetVisAttributes(scoringVis);
    // logicScoringTarget->SetVisAttributes(G4VisAttributes::GetInvisible());
    
    // Place scoring plane inside vacuum chamber volume
    new G4PVPlacement(0, G4ThreeVector(0., 0., targetExitZ_local), 
                      logicScoringTarget, "physScoringVolumeTarget", logicChamberVacuum, false, 0, true);

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

    // Magnet entrance scoring volume
    G4double hollowHalfZ = fMagnetOuterSize/2.;

    G4double scoringHalfThickness = 0.05*mm;  // Match reference implementation (0.1mm total thickness)
    auto* solidScoringMagnetEntrance = new G4Box("solidScoringMagnetEntrance", 4.45*cm, 4.45*cm, scoringHalfThickness);
    fLogicScoringMagnetEntrance = new G4LogicalVolume(solidScoringMagnetEntrance,
                                                      fMatVacuum,
                                                      "logicScoringMagnetEntrance");
    auto* scoringVis = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0, 1.0));
    scoringVis->SetForceSolid(true);
    fLogicScoringMagnetEntrance->SetVisAttributes(scoringVis);
    // fLogicScoringMagnetEntrance->SetVisAttributes(G4VisAttributes::GetInvisible());

    G4double scoringZ = -hollowHalfZ + scoringHalfThickness + 0.1*mm;  // 0.1mm clearance
    new G4PVPlacement(0, G4ThreeVector(0., 0., scoringZ),
                      fLogicScoringMagnetEntrance, "physScoringMagnetEntrance", 
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
    G4double scoringHalfThickness = 0.05*mm;  // Match reference implementation (0.1mm total thickness)
    
    // Scoring plane at calorimeter entrance (BEFORE ECAL, in worldLV)
    auto* solidScoringCaloEntrance = new G4Box("solidScoringCaloEntrance", fCaloSizeXY/2., fCaloSizeXY/2., scoringHalfThickness);
    fLogicScoringCaloEntrance = new G4LogicalVolume(solidScoringCaloEntrance,
                                                    fMatVacuum,
                                                    "logicScoringCaloEntrance");
    auto* scoringEntranceVis = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0, 1.0));
    scoringEntranceVis->SetForceSolid(true);
    fLogicScoringCaloEntrance->SetVisAttributes(scoringEntranceVis);
    // fLogicScoringCaloEntrance->SetVisAttributes(G4VisAttributes::GetInvisible());
    
    // Place entrance scoring plane just BEFORE ECAL front face (in worldLV, with clearance from magnet)
    // Magnet ends at zPos, so place scoring BEFORE zPos with extra clearance
    G4double caloEntranceZ = zPos - 1.0*mm - scoringHalfThickness;
    new G4PVPlacement(0, G4ThreeVector(0., 0., caloEntranceZ),
                      fLogicScoringCaloEntrance, "physScoringCaloEntrance",
                      worldLV, false, 0, true);
    
    G4cout << "\n=== CALORIMETER ENTRANCE SCORING GEOMETRY ===" << G4endl;
    G4cout << "ECAL depth: " << ecalDepth/cm << " cm" << G4endl;
    G4cout << "ECAL will be centered at absolute Z: " << (zPos + ecalDepth/2.)/cm << " cm" << G4endl;
    G4cout << "ECAL front face absolute Z: " << zPos/cm << " cm" << G4endl;
    G4cout << "Calo entrance scoring absolute Z: " << caloEntranceZ/cm << " cm" << G4endl;

    // Create ECAL container
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

    // Scoring plane at calorimeter exit (AFTER ECAL, in worldLV)
    auto* solidScoringCaloExit = new G4Box("solidScoringCaloExit", fCaloSizeXY/2., fCaloSizeXY/2., scoringHalfThickness);
    fLogicScoringCaloExit = new G4LogicalVolume(solidScoringCaloExit,
                                                fMatVacuum,
                                                "logicScoringCaloExit");
    auto* scoringExitVis = new G4VisAttributes(G4Colour(1.0, 0.0, 0.0, 1.0));
    scoringExitVis->SetForceSolid(true);
    fLogicScoringCaloExit->SetVisAttributes(scoringExitVis);
    // fLogicScoringCaloExit->SetVisAttributes(G4VisAttributes::GetInvisible());
    
    // Place ECAL in world AFTER magnet region
    zPos += ecalDepth/2.;  // Move to center of ECAL
    new G4PVPlacement(0, G4ThreeVector(0., 0., zPos), logicECAL, "physECAL", worldLV, false, 0, true);
    zPos += ecalDepth/2.;  // Move to end of ECAL
    
    // Place exit scoring plane just AFTER ECAL back face (in worldLV)
    G4double caloExitZ = zPos + 0.1*mm + scoringHalfThickness;
    new G4PVPlacement(0, G4ThreeVector(0., 0., caloExitZ),
                      fLogicScoringCaloExit, "physScoringCaloExit",
                      worldLV, false, 0, true);
    
    G4cout << "ECAL back face absolute Z: " << zPos/cm << " cm" << G4endl;
    G4cout << "Calo exit scoring absolute Z: " << caloExitZ/cm << " cm" << G4endl;
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

    // Scoring volume sensitive detectors
    auto* scoringMagnetEntranceSD = new G4MultiFunctionalDetector("ScoringMagnetEntranceSD");
    sdManager->AddNewDetector(scoringMagnetEntranceSD);
    
    auto* scoringMagnetEntranceEnergyDep = new G4PSEnergyDeposit("EnergyDeposit");
    scoringMagnetEntranceSD->RegisterPrimitive(scoringMagnetEntranceEnergyDep);
    
    auto* scoringMagnetEntranceNofSecondary = new G4PSNofSecondary("NofSecondary");
    scoringMagnetEntranceSD->RegisterPrimitive(scoringMagnetEntranceNofSecondary);
    
    fLogicScoringMagnetEntrance->SetSensitiveDetector(scoringMagnetEntranceSD);

    auto* scoringCaloEntranceSD = new G4MultiFunctionalDetector("ScoringCaloEntranceSD");
    sdManager->AddNewDetector(scoringCaloEntranceSD);
    
    auto* scoringCaloEntranceEnergyDep = new G4PSEnergyDeposit("EnergyDeposit");
    scoringCaloEntranceSD->RegisterPrimitive(scoringCaloEntranceEnergyDep);
    
    auto* scoringCaloEntranceNofSecondary = new G4PSNofSecondary("NofSecondary");
    scoringCaloEntranceSD->RegisterPrimitive(scoringCaloEntranceNofSecondary);
    
    fLogicScoringCaloEntrance->SetSensitiveDetector(scoringCaloEntranceSD);

    auto* scoringCaloExitSD = new G4MultiFunctionalDetector("ScoringCaloExitSD");
    sdManager->AddNewDetector(scoringCaloExitSD);
    
    auto* scoringCaloExitEnergyDep = new G4PSEnergyDeposit("EnergyDeposit");
    scoringCaloExitSD->RegisterPrimitive(scoringCaloExitEnergyDep);
    
    auto* scoringCaloExitNofSecondary = new G4PSNofSecondary("NofSecondary");
    scoringCaloExitSD->RegisterPrimitive(scoringCaloExitNofSecondary);
    
    fLogicScoringCaloExit->SetSensitiveDetector(scoringCaloExitSD);

    // fMagField = new MagneticField();
    // auto* fieldMgr = new G4FieldManager();
    // fieldMgr->SetDetectorField(fMagField);
    // fieldMgr->CreateChordFinder(fMagField);
    // fLogicMagnetHollow->SetFieldManager(fieldMgr, true);
}
