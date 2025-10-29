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
    G4LogicalVolume* logicSiTracker;
    G4LogicalVolume* logicCalorimeter;
    virtual void ConstructSDandField();
};

#endif
