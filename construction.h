#ifndef CONSTRUCTION_H
#define CONSTRUCTION_H

#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "detector.h"

class DamsaDetectorConstruction : public G4VUserDetectorConstruction
{
public:
    DamsaDetectorConstruction();
    virtual ~DamsaDetectorConstruction();

    virtual G4VPhysicalVolume* Construct() override;

private:
    virtual void ConstructSDandField();
    
    void DefineMaterials();
    void BuildTarget(G4LogicalVolume* worldLV, G4double& zPos);
    void BuildVacuumChamber(G4LogicalVolume* worldLV, G4double& zPos);
    void BuildMagnet(G4LogicalVolume* worldLV, G4double& zPos);
    void BuildTrackerRegion(G4LogicalVolume* worldLV, G4double& zPos);
    void BuildCalorimeter(G4LogicalVolume* worldLV, G4double& zPos);

    G4double fWorldSize;
    G4double fTargetX, fTargetY, fTargetZ;
    G4double fChamberInnerRadius, fChamberWallThickness, fChamberLength;
    G4double fMagnetOuterSize, fMagnetHollowSize;
    G4double fTrackerSizeXY, fTrackerThickness, fTrackerHoleSize;
    G4int fNumTrackers;
    G4double fCaloSizeXY, fLayerThickness;
    G4int fNumCaloLayers, fNumCrystalsPerLayer;

    G4LogicalVolume* fLogicSiTracker;
    G4LogicalVolume* fLogicCrystal;
};

#endif

