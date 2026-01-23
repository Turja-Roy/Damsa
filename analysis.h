#ifndef ANALYSIS_H
#define ANALYSIS_H

#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include <iostream>

class DamsaAnalysis
{
public:
    static DamsaAnalysis* Instance();
    
    void RecordParticle(const G4String& particleName, G4double energy, 
                       const G4String& location, G4double angle);
    void PrintSummary();
    void Reset();
    
private:
    DamsaAnalysis();
    ~DamsaAnalysis();
    static DamsaAnalysis* fInstance;
    
    // Counters at target exit
    G4int fTargetExitNeutrons;
    G4int fTargetExitPhotons;
    G4int fTargetExitElectrons;
    G4int fTargetExitPositrons;
    G4double fTargetExitEnergy;
    
    // Counters at detector entrance
    G4int fDetectorNeutrons;
    G4int fDetectorPhotons;
    G4int fDetectorElectrons;
    G4int fDetectorPositrons;
    G4double fDetectorEnergy;
    
    // Photon energy spectrum storage
    static const int kMaxPhotons = 10000;
    G4double fDetectorPhotonEnergies[kMaxPhotons];
    G4int fDetectorPhotonEnergyCount;
    G4double fDetectorPhotonAngles[kMaxPhotons];
};

DamsaAnalysis* DamsaAnalysis::fInstance = nullptr;

DamsaAnalysis* DamsaAnalysis::Instance()
{
    if(!fInstance) {
        fInstance = new DamsaAnalysis();
    }
    return fInstance;
}

DamsaAnalysis::DamsaAnalysis()
: fTargetExitNeutrons(0), fTargetExitPhotons(0),
  fTargetExitElectrons(0), fTargetExitPositrons(0),
  fTargetExitEnergy(0),
  fDetectorNeutrons(0), fDetectorPhotons(0),
  fDetectorElectrons(0), fDetectorPositrons(0),
  fDetectorEnergy(0), fDetectorPhotonEnergyCount(0)
{}

DamsaAnalysis::~DamsaAnalysis()
{}

void DamsaAnalysis::RecordParticle(const G4String& particleName, G4double energy,
                                   const G4String& location, G4double angle)
{
    if(location == "TargetExit") {
        if(particleName == "neutron") fTargetExitNeutrons++;
        else if(particleName == "gamma") fTargetExitPhotons++;
        else if(particleName == "e-") fTargetExitElectrons++;
        else if(particleName == "e+") fTargetExitPositrons++;
        fTargetExitEnergy += energy;
    }
    else if(location == "DetectorEntrance") {
        if(particleName == "neutron") fDetectorNeutrons++;
        else if(particleName == "gamma") {
            fDetectorPhotons++;
            
            if(fDetectorPhotonEnergyCount < kMaxPhotons) {
                fDetectorPhotonEnergies[fDetectorPhotonEnergyCount] = energy;
                fDetectorPhotonAngles[fDetectorPhotonEnergyCount] = angle;
                fDetectorPhotonEnergyCount++;
            }
        }
        else if(particleName == "e-") fDetectorElectrons++;
        else if(particleName == "e+") fDetectorPositrons++;
        fDetectorEnergy += energy;
    }
}

void DamsaAnalysis::PrintSummary()
{
    G4cout << "\n=============================================" << G4endl;
    G4cout << "=== Particles EXITING Target ===" << G4endl;
    G4cout << "=============================================" << G4endl;
    G4cout << "Neutrons:   " << fTargetExitNeutrons << G4endl;
    G4cout << "Photons:    " << fTargetExitPhotons << G4endl;
    G4cout << "Electrons:  " << fTargetExitElectrons << G4endl;
    G4cout << "Positrons:  " << fTargetExitPositrons << G4endl;
    G4cout << "Total Energy: " << fTargetExitEnergy/GeV << " GeV" << G4endl;
    
    G4cout << "\n=============================================" << G4endl;
    G4cout << "=== Particles ENTERING Detector ===" << G4endl;
    G4cout << "=============================================" << G4endl;
    G4cout << "Neutrons:   " << fDetectorNeutrons << G4endl;
    G4cout << "Photons:    " << fDetectorPhotons << G4endl;
    G4cout << "Electrons:  " << fDetectorElectrons << G4endl;
    G4cout << "Positrons:  " << fDetectorPositrons << G4endl;
    G4cout << "Total Energy: " << fDetectorEnergy/GeV << " GeV" << G4endl;
    G4cout << "=============================================\n" << G4endl;

    // Print photon energy histogram
    if(fDetectorPhotonEnergyCount > 0) {
        G4cout << "\n=== Photon Energy Spectrum at Detector Entrance ===" << G4endl;
        G4double bins[] = {0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 1.0, 2.0, 4.0, 8.0};
        G4int nBins = 9;
        G4int counts[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
        
        for(G4int i = 0; i < fDetectorPhotonEnergyCount; i++) {
            G4double E = fDetectorPhotonEnergies[i] / GeV;
            for(G4int j = 0; j < nBins; j++) {
                if(E >= bins[j] && E < bins[j+1]) {
                    counts[j]++;
                    break;
                }
            }
        }
        
        for(G4int i = 0; i < nBins; i++) {
            G4cout << "  [" << bins[i] << " - " << bins[i+1] << " GeV]: "
                   << counts[i] << " photons" << G4endl;
        }
        G4cout << "  Total photons recorded: " << fDetectorPhotonEnergyCount << G4endl;
    }
    
    // Print photon angular distribution
    if(fDetectorPhotonEnergyCount > 0) {
        G4cout << "\n=== Photon Angular Distribution at Detector Entrance ===" << G4endl;
        G4double angleBins[] = {0.0, 5.0, 10.0, 20.0, 30.0, 45.0, 60.0, 90.0};
        G4int nAngleBins = 7;
        G4int angleCounts[7] = {0, 0, 0, 0, 0, 0, 0};
        
        for(G4int i = 0; i < fDetectorPhotonEnergyCount; i++) {
            G4double angleDeg = fDetectorPhotonAngles[i] * 180.0 / 3.14159265;
            
            for(G4int j = 0; j < nAngleBins; j++) {
                if(angleDeg >= angleBins[j] && angleDeg < angleBins[j+1]) {
                    angleCounts[j]++;
                    break;
                }
            }
        }
        
        G4cout << "  (Angle relative to beam axis, 0° = straight forward)" << G4endl;
        for(G4int i = 0; i < nAngleBins; i++) {
            G4cout << "  [" << angleBins[i] << " - " << angleBins[i+1] << " degrees]: " 
                   << angleCounts[i] << " photons" << G4endl;
        }
    }
    
    G4cout << "=============================================\n" << G4endl;
}

void DamsaAnalysis::Reset()
{
    fTargetExitNeutrons = 0;
    fTargetExitPhotons = 0;
    fTargetExitElectrons = 0;
    fTargetExitPositrons = 0;
    fTargetExitEnergy = 0;
    
    fDetectorNeutrons = 0;
    fDetectorPhotons = 0;
    fDetectorElectrons = 0;
    fDetectorPositrons = 0;
    fDetectorEnergy = 0;
    fDetectorPhotonEnergyCount = 0;
}

#endif
