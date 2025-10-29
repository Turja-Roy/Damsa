#ifndef GENERATOR_H
#define GENERATOR_H

#include "G4VUserPrimaryGeneratorAction.hh"

#include "G4ParticleGun.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"

class DamsaPrimaryGenerator : public G4VUserPrimaryGeneratorAction {
public:
    DamsaPrimaryGenerator();
    virtual ~DamsaPrimaryGenerator();

    virtual void GeneratePrimaries(G4Event*);

private:
    G4ParticleGun* fParticleGun;
};

DamsaPrimaryGenerator::DamsaPrimaryGenerator () {
    fParticleGun = new G4ParticleGun(1);

    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    // G4String particleName = "proton";
    G4String particleName = "e-";
    G4ParticleDefinition* particle = particleTable->FindParticle(particleName);

    G4ThreeVector pos(0., 0., -60.*cm);
    G4ThreeVector mom(0., 0., 1.);

    fParticleGun->SetParticlePosition(pos);
    fParticleGun->SetParticleMomentumDirection(mom);
    fParticleGun->SetParticleMomentum(100.*MeV);
    fParticleGun->SetParticleDefinition(particle);
}
DamsaPrimaryGenerator::~DamsaPrimaryGenerator () {
    delete fParticleGun;
}

void DamsaPrimaryGenerator::GeneratePrimaries(G4Event* anEvent) {
    fParticleGun->GeneratePrimaryVertex(anEvent);
}

#endif
