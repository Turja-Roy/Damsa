#ifndef MAGNETICFIELD_H
#define MAGNETICFIELD_H

#include "G4MagneticField.hh"
#include "G4SystemOfUnits.hh"

class MagneticField : public G4MagneticField
{
public:
    MagneticField();
    virtual ~MagneticField();
    
    virtual void GetFieldValue(const G4double point[4], G4double* bField) const override;

private:
    G4double fFieldStrength;
};

#endif
