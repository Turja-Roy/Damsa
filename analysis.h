#ifndef ANALYSIS_H
#define ANALYSIS_H

#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sys/stat.h>
#include <vector>
#include <set>
#include "TFile.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TStyle.h"

class DamsaAnalysis
{
public:
    static DamsaAnalysis* Instance();
    
    void RecordParticle(const G4String& particleName, G4double energy, 
                       const G4String& location, G4double angle, G4int trackID, G4bool isPrimary);
    void PrintSummary();
    void SaveToFile(const G4String& filename);
    void WriteROOTHistograms(const G4String& filename);
    void Reset();
    void ResetEventTracking();  // Reset per-event track ID sets
    
    // Check if a track was already recorded at a given location
    G4bool WasTrackRecorded(G4int trackID, const G4String& location);
    
private:
    DamsaAnalysis();
    ~DamsaAnalysis();
    static DamsaAnalysis* fInstance;
    
    // Per-event track ID sets to prevent double counting
    std::set<G4int> fTargetExitTracks;
    std::set<G4int> fMagnetEntranceTracks;
    std::set<G4int> fCaloEntranceTracks;
    std::set<G4int> fCaloExitTracks;
    
    // Counters at target exit
    G4int fTargetExitNeutrons;
    G4int fTargetExitPhotons;
    G4int fTargetExitElectrons;
    G4int fTargetExitPositrons;
    G4double fTargetExitEnergy;
    G4double fTargetExitPrimaryEnergy;
    
    // Target exit particle storage
    std::vector<G4double> fTargetExitPhotonEnergies;
    std::vector<G4double> fTargetExitPhotonAngles;
    std::vector<G4double> fTargetExitNeutronEnergies;
    std::vector<G4double> fTargetExitNeutronAngles;
    std::vector<G4double> fTargetExitElectronEnergies;
    std::vector<G4double> fTargetExitElectronAngles;
    std::vector<G4double> fTargetExitPositronEnergies;
    std::vector<G4double> fTargetExitPositronAngles;
    
    // Counters at magnet entrance
    G4int fMagnetEntranceNeutrons;
    G4int fMagnetEntrancePhotons;
    G4int fMagnetEntranceElectrons;
    G4int fMagnetEntrancePositrons;
    G4double fMagnetEntranceEnergy;
    G4double fMagnetEntrancePrimaryEnergy;
    
    // Counters at calorimeter entrance
    G4int fCaloEntranceNeutrons;
    G4int fCaloEntrancePhotons;
    G4int fCaloEntranceElectrons;
    G4int fCaloEntrancePositrons;
    G4double fCaloEntranceEnergy;
    G4double fCaloEntrancePrimaryEnergy;
    
    // Counters at calorimeter exit
    G4int fCaloExitNeutrons;
    G4int fCaloExitPhotons;
    G4int fCaloExitElectrons;
    G4int fCaloExitPositrons;
    G4double fCaloExitEnergy;
    G4double fCaloExitPrimaryEnergy;
    
    // Magnet entrance particle storage (using vectors for dynamic sizing)
    std::vector<G4double> fMagnetEntrancePhotonEnergies;
    std::vector<G4double> fMagnetEntrancePhotonAngles;
    std::vector<G4double> fMagnetEntranceNeutronEnergies;
    std::vector<G4double> fMagnetEntranceNeutronAngles;
    std::vector<G4double> fMagnetEntranceElectronEnergies;
    std::vector<G4double> fMagnetEntranceElectronAngles;
    std::vector<G4double> fMagnetEntrancePositronEnergies;
    std::vector<G4double> fMagnetEntrancePositronAngles;
    
    // Calorimeter entrance particle storage
    std::vector<G4double> fCaloEntrancePhotonEnergies;
    std::vector<G4double> fCaloEntrancePhotonAngles;
    std::vector<G4double> fCaloEntranceNeutronEnergies;
    std::vector<G4double> fCaloEntranceNeutronAngles;
    std::vector<G4double> fCaloEntranceElectronEnergies;
    std::vector<G4double> fCaloEntranceElectronAngles;
    std::vector<G4double> fCaloEntrancePositronEnergies;
    std::vector<G4double> fCaloEntrancePositronAngles;
    
    // Calorimeter exit particle storage
    std::vector<G4double> fCaloExitPhotonEnergies;
    std::vector<G4double> fCaloExitPhotonAngles;
    std::vector<G4double> fCaloExitNeutronEnergies;
    std::vector<G4double> fCaloExitNeutronAngles;
    std::vector<G4double> fCaloExitElectronEnergies;
    std::vector<G4double> fCaloExitElectronAngles;
    std::vector<G4double> fCaloExitPositronEnergies;
    std::vector<G4double> fCaloExitPositronAngles;
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
  fTargetExitEnergy(0), fTargetExitPrimaryEnergy(0),
  fMagnetEntranceNeutrons(0), fMagnetEntrancePhotons(0),
  fMagnetEntranceElectrons(0), fMagnetEntrancePositrons(0),
  fMagnetEntranceEnergy(0), fMagnetEntrancePrimaryEnergy(0),
  fCaloEntranceNeutrons(0), fCaloEntrancePhotons(0),
  fCaloEntranceElectrons(0), fCaloEntrancePositrons(0),
  fCaloEntranceEnergy(0), fCaloEntrancePrimaryEnergy(0),
  fCaloExitNeutrons(0), fCaloExitPhotons(0),
  fCaloExitElectrons(0), fCaloExitPositrons(0),
  fCaloExitEnergy(0), fCaloExitPrimaryEnergy(0)
{}

DamsaAnalysis::~DamsaAnalysis()
{}

G4bool DamsaAnalysis::WasTrackRecorded(G4int trackID, const G4String& location)
{
    if(location == "TargetExit") {
        return fTargetExitTracks.find(trackID) != fTargetExitTracks.end();
    }
    else if(location == "MagnetEntrance") {
        return fMagnetEntranceTracks.find(trackID) != fMagnetEntranceTracks.end();
    }
    else if(location == "CaloEntrance") {
        return fCaloEntranceTracks.find(trackID) != fCaloEntranceTracks.end();
    }
    else if(location == "CaloExit") {
        return fCaloExitTracks.find(trackID) != fCaloExitTracks.end();
    }
    return false;
}

void DamsaAnalysis::ResetEventTracking()
{
    fTargetExitTracks.clear();
    fMagnetEntranceTracks.clear();
    fCaloEntranceTracks.clear();
    fCaloExitTracks.clear();
}

void DamsaAnalysis::RecordParticle(const G4String& particleName, G4double energy,
                                   const G4String& location, G4double angle, 
                                   G4int trackID, G4bool isPrimary)
{
    // Mark this track as recorded at this location
    if(location == "TargetExit") {
        fTargetExitTracks.insert(trackID);
        if(particleName == "neutron") {
            fTargetExitNeutrons++;
            fTargetExitNeutronEnergies.push_back(energy);
            fTargetExitNeutronAngles.push_back(angle);
        }
        else if(particleName == "gamma") {
            fTargetExitPhotons++;
            fTargetExitPhotonEnergies.push_back(energy);
            fTargetExitPhotonAngles.push_back(angle);
        }
        else if(particleName == "e-") {
            fTargetExitElectrons++;
            fTargetExitElectronEnergies.push_back(energy);
            fTargetExitElectronAngles.push_back(angle);
        }
        else if(particleName == "e+") {
            fTargetExitPositrons++;
            fTargetExitPositronEnergies.push_back(energy);
            fTargetExitPositronAngles.push_back(angle);
        }
        // Count primary beam separately if needed
        if(isPrimary) {
            fTargetExitPrimaryEnergy += energy;
        }
        fTargetExitEnergy += energy;
    }
    else if(location == "MagnetEntrance") {
        fMagnetEntranceTracks.insert(trackID);
        if(particleName == "neutron") {
            fMagnetEntranceNeutrons++;
            fMagnetEntranceNeutronEnergies.push_back(energy);
            fMagnetEntranceNeutronAngles.push_back(angle);
        }
        else if(particleName == "gamma") {
            fMagnetEntrancePhotons++;
            fMagnetEntrancePhotonEnergies.push_back(energy);
            fMagnetEntrancePhotonAngles.push_back(angle);
        }
        else if(particleName == "e-") {
            fMagnetEntranceElectrons++;
            fMagnetEntranceElectronEnergies.push_back(energy);
            fMagnetEntranceElectronAngles.push_back(angle);
        }
        else if(particleName == "e+") {
            fMagnetEntrancePositrons++;
            fMagnetEntrancePositronEnergies.push_back(energy);
            fMagnetEntrancePositronAngles.push_back(angle);
        }
        if(isPrimary) {
            fMagnetEntrancePrimaryEnergy += energy;
        }
        fMagnetEntranceEnergy += energy;
    }
    else if(location == "CaloEntrance") {
        fCaloEntranceTracks.insert(trackID);
        if(particleName == "neutron") {
            fCaloEntranceNeutrons++;
            fCaloEntranceNeutronEnergies.push_back(energy);
            fCaloEntranceNeutronAngles.push_back(angle);
        }
        else if(particleName == "gamma") {
            fCaloEntrancePhotons++;
            fCaloEntrancePhotonEnergies.push_back(energy);
            fCaloEntrancePhotonAngles.push_back(angle);
        }
        else if(particleName == "e-") {
            fCaloEntranceElectrons++;
            fCaloEntranceElectronEnergies.push_back(energy);
            fCaloEntranceElectronAngles.push_back(angle);
        }
        else if(particleName == "e+") {
            fCaloEntrancePositrons++;
            fCaloEntrancePositronEnergies.push_back(energy);
            fCaloEntrancePositronAngles.push_back(angle);
        }
        if(isPrimary) {
            fCaloEntrancePrimaryEnergy += energy;
        }
        fCaloEntranceEnergy += energy;
    }
    else if(location == "CaloExit") {
        fCaloExitTracks.insert(trackID);
        if(particleName == "neutron") {
            fCaloExitNeutrons++;
            fCaloExitNeutronEnergies.push_back(energy);
            fCaloExitNeutronAngles.push_back(angle);
        }
        else if(particleName == "gamma") {
            fCaloExitPhotons++;
            fCaloExitPhotonEnergies.push_back(energy);
            fCaloExitPhotonAngles.push_back(angle);
        }
        else if(particleName == "e-") {
            fCaloExitElectrons++;
            fCaloExitElectronEnergies.push_back(energy);
            fCaloExitElectronAngles.push_back(angle);
        }
        else if(particleName == "e+") {
            fCaloExitPositrons++;
            fCaloExitPositronEnergies.push_back(energy);
            fCaloExitPositronAngles.push_back(angle);
        }
        if(isPrimary) {
            fCaloExitPrimaryEnergy += energy;
        }
        fCaloExitEnergy += energy;
    }
}

void DamsaAnalysis::PrintSummary()
{
    // Target Exit Summary
    G4cout << "\n=============================================" << G4endl;
    G4cout << "=== Particles EXITING Target ===" << G4endl;
    G4cout << "=============================================" << G4endl;
    G4cout << "Neutrons:   " << fTargetExitNeutrons << G4endl;
    G4cout << "Photons:    " << fTargetExitPhotons << G4endl;
    G4cout << "Electrons:  " << fTargetExitElectrons << G4endl;
    G4cout << "Positrons:  " << fTargetExitPositrons << G4endl;
    G4cout << "Total Energy: " << fTargetExitEnergy/GeV << " GeV" << G4endl;
    G4cout << "  (Primary beam: " << fTargetExitPrimaryEnergy/GeV << " GeV, Secondaries: " 
           << (fTargetExitEnergy - fTargetExitPrimaryEnergy)/GeV << " GeV)" << G4endl;
    
    // Target Exit Photon Spectrum
    if(fTargetExitPhotonEnergies.size() > 0) {
        G4cout << "\n=== Photon Energy Spectrum at Target Exit ===" << G4endl;
        G4double bins[] = {0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 1.0, 2.0, 4.0, 8.0};
        G4int nBins = 9;
        G4int counts[9] = {0};
        
        for(size_t i = 0; i < fTargetExitPhotonEnergies.size(); i++) {
            G4double E = fTargetExitPhotonEnergies[i] / GeV;
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
        G4cout << "  Total photons: " << fTargetExitPhotonEnergies.size() << G4endl;
    }
    
    // Target Exit Photon Angular Distribution
    if(fTargetExitPhotonAngles.size() > 0) {
        G4cout << "\n=== Photon Angular Distribution at Target Exit ===" << G4endl;
        G4double angleBins[] = {0.0, 5.0, 10.0, 20.0, 30.0, 45.0, 60.0, 90.0};
        G4int nAngleBins = 7;
        G4int angleCounts[7] = {0};
        
        for(size_t i = 0; i < fTargetExitPhotonAngles.size(); i++) {
            G4double angleDeg = fTargetExitPhotonAngles[i] * 180.0 / 3.14159265;
            for(G4int j = 0; j < nAngleBins; j++) {
                if(angleDeg >= angleBins[j] && angleDeg < angleBins[j+1]) {
                    angleCounts[j]++;
                    break;
                }
            }
        }
        
        G4cout << "  (Angle relative to beam axis, 0 deg = straight forward)" << G4endl;
        for(G4int i = 0; i < nAngleBins; i++) {
            G4cout << "  [" << angleBins[i] << " - " << angleBins[i+1] << " degrees]: " 
                   << angleCounts[i] << " photons" << G4endl;
        }
    }
    
    // Target Exit Neutron Spectrum
    if(fTargetExitNeutronEnergies.size() > 0) {
        G4cout << "\n=== Neutron Energy Spectrum at Target Exit ===" << G4endl;
        G4double neutronEnergyBins[] = {0.0, 0.1, 0.5, 1.0, 2.0, 5.0, 10.0, 50.0, 100.0};
        G4int nNeutronEnergyBins = 8;
        G4int neutronEnergyCounts[8] = {0};
        
        for(size_t i = 0; i < fTargetExitNeutronEnergies.size(); i++) {
            G4double E = fTargetExitNeutronEnergies[i] / GeV;
            for(G4int j = 0; j < nNeutronEnergyBins; j++) {
                if(E >= neutronEnergyBins[j] && E < neutronEnergyBins[j+1]) {
                    neutronEnergyCounts[j]++;
                    break;
                }
            }
        }
        
        for(G4int i = 0; i < nNeutronEnergyBins; i++) {
            G4cout << "  [" << neutronEnergyBins[i] << " - " << neutronEnergyBins[i+1] 
                   << " GeV]: " << neutronEnergyCounts[i] << " neutrons" << G4endl;
        }
    }
    
    // Target Exit Neutron Angular Distribution
    if(fTargetExitNeutronAngles.size() > 0) {
        G4cout << "\n=== Neutron Angular Distribution at Target Exit ===" << G4endl;
        G4double angleBins[] = {0.0, 5.0, 10.0, 20.0, 30.0, 45.0, 60.0, 90.0};
        G4int nAngleBins = 7;
        G4int angleCounts[7] = {0};
        
        for(size_t i = 0; i < fTargetExitNeutronAngles.size(); i++) {
            G4double angleDeg = fTargetExitNeutronAngles[i] * 180.0 / 3.14159265;
            for(G4int j = 0; j < nAngleBins; j++) {
                if(angleDeg >= angleBins[j] && angleDeg < angleBins[j+1]) {
                    angleCounts[j]++;
                    break;
                }
            }
        }
        
        G4cout << "  (Angle relative to beam axis, 0 deg = straight forward)" << G4endl;
        for(G4int i = 0; i < nAngleBins; i++) {
            G4cout << "  [" << angleBins[i] << " - " << angleBins[i+1] << " degrees]: " 
                   << angleCounts[i] << " neutrons" << G4endl;
        }
    }
    
    // Target Exit Electron Spectrum
    if(fTargetExitElectronEnergies.size() > 0) {
        G4cout << "\n=== Electron Energy Spectrum at Target Exit ===" << G4endl;
        G4double electronEnergyBins[] = {0.0, 0.1, 0.2, 0.5, 1.0, 2.0, 4.0, 8.0};
        G4int nElectronEnergyBins = 7;
        G4int electronEnergyCounts[7] = {0};
        
        for(size_t i = 0; i < fTargetExitElectronEnergies.size(); i++) {
            G4double E = fTargetExitElectronEnergies[i] / GeV;
            for(G4int j = 0; j < nElectronEnergyBins; j++) {
                if(E >= electronEnergyBins[j] && E < electronEnergyBins[j+1]) {
                    electronEnergyCounts[j]++;
                    break;
                }
            }
        }
        
        for(G4int i = 0; i < nElectronEnergyBins; i++) {
            G4cout << "  [" << electronEnergyBins[i] << " - " << electronEnergyBins[i+1] 
                   << " GeV]: " << electronEnergyCounts[i] << " electrons" << G4endl;
        }
    }
    
    // Target Exit Electron Angular Distribution
    if(fTargetExitElectronAngles.size() > 0) {
        G4cout << "\n=== Electron Angular Distribution at Target Exit ===" << G4endl;
        G4double angleBins[] = {0.0, 5.0, 10.0, 20.0, 30.0, 45.0, 60.0, 90.0};
        G4int nAngleBins = 7;
        G4int angleCounts[7] = {0};
        
        for(size_t i = 0; i < fTargetExitElectronAngles.size(); i++) {
            G4double angleDeg = fTargetExitElectronAngles[i] * 180.0 / 3.14159265;
            for(G4int j = 0; j < nAngleBins; j++) {
                if(angleDeg >= angleBins[j] && angleDeg < angleBins[j+1]) {
                    angleCounts[j]++;
                    break;
                }
            }
        }
        
        G4cout << "  (Angle relative to beam axis, 0 deg = straight forward)" << G4endl;
        for(G4int i = 0; i < nAngleBins; i++) {
            G4cout << "  [" << angleBins[i] << " - " << angleBins[i+1] << " degrees]: " 
                   << angleCounts[i] << " electrons" << G4endl;
        }
    }
    
    // Target Exit Positron Spectrum
    if(fTargetExitPositronEnergies.size() > 0) {
        G4cout << "\n=== Positron Energy Spectrum at Target Exit ===" << G4endl;
        G4double positronEnergyBins[] = {0.0, 0.1, 0.2, 0.5, 1.0, 2.0, 4.0, 8.0};
        G4int nPositronEnergyBins = 7;
        G4int positronEnergyCounts[7] = {0};
        
        for(size_t i = 0; i < fTargetExitPositronEnergies.size(); i++) {
            G4double E = fTargetExitPositronEnergies[i] / GeV;
            for(G4int j = 0; j < nPositronEnergyBins; j++) {
                if(E >= positronEnergyBins[j] && E < positronEnergyBins[j+1]) {
                    positronEnergyCounts[j]++;
                    break;
                }
            }
        }
        
        for(G4int i = 0; i < nPositronEnergyBins; i++) {
            G4cout << "  [" << positronEnergyBins[i] << " - " << positronEnergyBins[i+1] 
                   << " GeV]: " << positronEnergyCounts[i] << " positrons" << G4endl;
        }
    }
    
    // Target Exit Positron Angular Distribution
    if(fTargetExitPositronAngles.size() > 0) {
        G4cout << "\n=== Positron Angular Distribution at Target Exit ===" << G4endl;
        G4double angleBins[] = {0.0, 5.0, 10.0, 20.0, 30.0, 45.0, 60.0, 90.0};
        G4int nAngleBins = 7;
        G4int angleCounts[7] = {0};
        
        for(size_t i = 0; i < fTargetExitPositronAngles.size(); i++) {
            G4double angleDeg = fTargetExitPositronAngles[i] * 180.0 / 3.14159265;
            for(G4int j = 0; j < nAngleBins; j++) {
                if(angleDeg >= angleBins[j] && angleDeg < angleBins[j+1]) {
                    angleCounts[j]++;
                    break;
                }
            }
        }
        
        G4cout << "  (Angle relative to beam axis, 0 deg = straight forward)" << G4endl;
        for(G4int i = 0; i < nAngleBins; i++) {
            G4cout << "  [" << angleBins[i] << " - " << angleBins[i+1] << " degrees]: " 
                   << angleCounts[i] << " positrons" << G4endl;
        }
    }
    
    // Magnet Entrance Summary
    G4cout << "\n=============================================" << G4endl;
    G4cout << "=== Particles ENTERING Magnet Region ===" << G4endl;
    G4cout << "=============================================" << G4endl;
    G4cout << "Neutrons:   " << fMagnetEntranceNeutrons << G4endl;
    G4cout << "Photons:    " << fMagnetEntrancePhotons << G4endl;
    G4cout << "Electrons:  " << fMagnetEntranceElectrons << G4endl;
    G4cout << "Positrons:  " << fMagnetEntrancePositrons << G4endl;
    G4cout << "Total Energy: " << fMagnetEntranceEnergy/GeV << " GeV" << G4endl;
    G4cout << "  (Primary beam: " << fMagnetEntrancePrimaryEnergy/GeV << " GeV, Secondaries: " 
           << (fMagnetEntranceEnergy - fMagnetEntrancePrimaryEnergy)/GeV << " GeV)" << G4endl;
    
    // Magnet Entrance Photon Spectrum
    if(fMagnetEntrancePhotonEnergies.size() > 0) {
        G4cout << "\n=== Photon Energy Spectrum at Magnet Entrance ===" << G4endl;
        G4double bins[] = {0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 1.0, 2.0, 4.0, 8.0};
        G4int nBins = 9;
        G4int counts[9] = {0};
        
        for(size_t i = 0; i < fMagnetEntrancePhotonEnergies.size(); i++) {
            G4double E = fMagnetEntrancePhotonEnergies[i] / GeV;
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
        G4cout << "  Total photons: " << fMagnetEntrancePhotonEnergies.size() << G4endl;
    }
    
    // Magnet Entrance Photon Angular Distribution
    if(fMagnetEntrancePhotonAngles.size() > 0) {
        G4cout << "\n=== Photon Angular Distribution at Magnet Entrance ===" << G4endl;
        G4double angleBins[] = {0.0, 5.0, 10.0, 20.0, 30.0, 45.0, 60.0, 90.0};
        G4int nAngleBins = 7;
        G4int angleCounts[7] = {0};
        
        for(size_t i = 0; i < fMagnetEntrancePhotonAngles.size(); i++) {
            G4double angleDeg = fMagnetEntrancePhotonAngles[i] * 180.0 / 3.14159265;
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
    
    // Magnet Entrance Neutron Spectrum
    if(fMagnetEntranceNeutronEnergies.size() > 0) {
        G4cout << "\n=== Neutron Energy Spectrum at Magnet Entrance ===" << G4endl;
        G4double neutronEnergyBins[] = {0.0, 0.1, 0.5, 1.0, 2.0, 5.0, 10.0, 50.0, 100.0};
        G4int nNeutronEnergyBins = 8;
        G4int neutronEnergyCounts[8] = {0};
        
        for(size_t i = 0; i < fMagnetEntranceNeutronEnergies.size(); i++) {
            G4double E = fMagnetEntranceNeutronEnergies[i] / GeV;
            for(G4int j = 0; j < nNeutronEnergyBins; j++) {
                if(E >= neutronEnergyBins[j] && E < neutronEnergyBins[j+1]) {
                    neutronEnergyCounts[j]++;
                    break;
                }
            }
        }
        
        for(G4int i = 0; i < nNeutronEnergyBins; i++) {
            G4cout << "  [" << neutronEnergyBins[i] << " - " << neutronEnergyBins[i+1] 
                   << " GeV]: " << neutronEnergyCounts[i] << " neutrons" << G4endl;
        }
    }
    
    // Magnet Entrance Electron Spectrum
    if(fMagnetEntranceElectronEnergies.size() > 0) {
        G4cout << "\n=== Electron Energy Spectrum at Magnet Entrance ===" << G4endl;
        G4double electronEnergyBins[] = {0.0, 0.1, 0.2, 0.5, 1.0, 2.0, 4.0, 8.0};
        G4int nElectronEnergyBins = 7;
        G4int electronEnergyCounts[7] = {0};
        
        for(size_t i = 0; i < fMagnetEntranceElectronEnergies.size(); i++) {
            G4double E = fMagnetEntranceElectronEnergies[i] / GeV;
            for(G4int j = 0; j < nElectronEnergyBins; j++) {
                if(E >= electronEnergyBins[j] && E < electronEnergyBins[j+1]) {
                    electronEnergyCounts[j]++;
                    break;
                }
            }
        }
        
        for(G4int i = 0; i < nElectronEnergyBins; i++) {
            G4cout << "  [" << electronEnergyBins[i] << " - " << electronEnergyBins[i+1] 
                   << " GeV]: " << electronEnergyCounts[i] << " electrons" << G4endl;
        }
    }
    
    // Magnet Entrance Positron Spectrum
    if(fMagnetEntrancePositronEnergies.size() > 0) {
        G4cout << "\n=== Positron Energy Spectrum at Magnet Entrance ===" << G4endl;
        G4double positronEnergyBins[] = {0.0, 0.1, 0.2, 0.5, 1.0, 2.0, 4.0, 8.0};
        G4int nPositronEnergyBins = 7;
        G4int positronEnergyCounts[7] = {0};
        
        for(size_t i = 0; i < fMagnetEntrancePositronEnergies.size(); i++) {
            G4double E = fMagnetEntrancePositronEnergies[i] / GeV;
            for(G4int j = 0; j < nPositronEnergyBins; j++) {
                if(E >= positronEnergyBins[j] && E < positronEnergyBins[j+1]) {
                    positronEnergyCounts[j]++;
                    break;
                }
            }
        }
        
        for(G4int i = 0; i < nPositronEnergyBins; i++) {
            G4cout << "  [" << positronEnergyBins[i] << " - " << positronEnergyBins[i+1] 
                   << " GeV]: " << positronEnergyCounts[i] << " positrons" << G4endl;
        }
    }
    
    // Calorimeter Entrance Summary
    G4cout << "\n=============================================" << G4endl;
    G4cout << "=== Particles ENTERING Calorimeter ===" << G4endl;
    G4cout << "=============================================" << G4endl;
    G4cout << "Neutrons:   " << fCaloEntranceNeutrons << G4endl;
    G4cout << "Photons:    " << fCaloEntrancePhotons << G4endl;
    G4cout << "Electrons:  " << fCaloEntranceElectrons << G4endl;
    G4cout << "Positrons:  " << fCaloEntrancePositrons << G4endl;
    G4cout << "Total Energy: " << fCaloEntranceEnergy/GeV << " GeV" << G4endl;
    G4cout << "  (Primary beam: " << fCaloEntrancePrimaryEnergy/GeV << " GeV, Secondaries: " 
           << (fCaloEntranceEnergy - fCaloEntrancePrimaryEnergy)/GeV << " GeV)" << G4endl;
    
    // Calorimeter Entrance Photon Spectrum
    if(fCaloEntrancePhotonEnergies.size() > 0) {
        G4cout << "\n=== Photon Energy Spectrum at Calorimeter Entrance ===" << G4endl;
        G4double bins[] = {0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 1.0, 2.0, 4.0, 8.0};
        G4int nBins = 9;
        G4int counts[9] = {0};
        
        for(size_t i = 0; i < fCaloEntrancePhotonEnergies.size(); i++) {
            G4double E = fCaloEntrancePhotonEnergies[i] / GeV;
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
        G4cout << "  Total photons: " << fCaloEntrancePhotonEnergies.size() << G4endl;
    }
    
    // Calorimeter Entrance Neutron Spectrum
    if(fCaloEntranceNeutronEnergies.size() > 0) {
        G4cout << "\n=== Neutron Energy Spectrum at Calorimeter Entrance ===" << G4endl;
        G4double neutronEnergyBins[] = {0.0, 0.1, 0.5, 1.0, 2.0, 5.0, 10.0, 50.0, 100.0};
        G4int nNeutronEnergyBins = 8;
        G4int neutronEnergyCounts[8] = {0};
        
        for(size_t i = 0; i < fCaloEntranceNeutronEnergies.size(); i++) {
            G4double E = fCaloEntranceNeutronEnergies[i] / GeV;
            for(G4int j = 0; j < nNeutronEnergyBins; j++) {
                if(E >= neutronEnergyBins[j] && E < neutronEnergyBins[j+1]) {
                    neutronEnergyCounts[j]++;
                    break;
                }
            }
        }
        
        for(G4int i = 0; i < nNeutronEnergyBins; i++) {
            G4cout << "  [" << neutronEnergyBins[i] << " - " << neutronEnergyBins[i+1] 
                   << " GeV]: " << neutronEnergyCounts[i] << " neutrons" << G4endl;
        }
    }
    
    // Calorimeter Entrance Photon Angular Distribution
    if(fCaloEntrancePhotonAngles.size() > 0) {
        G4cout << "\n=== Photon Angular Distribution at Calorimeter Entrance ===" << G4endl;
        G4double angleBins[] = {0.0, 5.0, 10.0, 20.0, 30.0, 45.0, 60.0, 90.0};
        G4int nAngleBins = 7;
        G4int angleCounts[7] = {0};
        
        for(size_t i = 0; i < fCaloEntrancePhotonAngles.size(); i++) {
            G4double angleDeg = fCaloEntrancePhotonAngles[i] * 180.0 / 3.14159265;
            for(G4int j = 0; j < nAngleBins; j++) {
                if(angleDeg >= angleBins[j] && angleDeg < angleBins[j+1]) {
                    angleCounts[j]++;
                    break;
                }
            }
        }
        
        G4cout << "  (Angle relative to beam axis, 0 deg = straight forward)" << G4endl;
        for(G4int i = 0; i < nAngleBins; i++) {
            G4cout << "  [" << angleBins[i] << " - " << angleBins[i+1] << " degrees]: " 
                   << angleCounts[i] << " photons" << G4endl;
        }
    }
    
    // Calorimeter Entrance Neutron Angular Distribution
    if(fCaloEntranceNeutronAngles.size() > 0) {
        G4cout << "\n=== Neutron Angular Distribution at Calorimeter Entrance ===" << G4endl;
        G4double angleBins[] = {0.0, 5.0, 10.0, 20.0, 30.0, 45.0, 60.0, 90.0};
        G4int nAngleBins = 7;
        G4int angleCounts[7] = {0};
        
        for(size_t i = 0; i < fCaloEntranceNeutronAngles.size(); i++) {
            G4double angleDeg = fCaloEntranceNeutronAngles[i] * 180.0 / 3.14159265;
            for(G4int j = 0; j < nAngleBins; j++) {
                if(angleDeg >= angleBins[j] && angleDeg < angleBins[j+1]) {
                    angleCounts[j]++;
                    break;
                }
            }
        }
        
        G4cout << "  (Angle relative to beam axis, 0 deg = straight forward)" << G4endl;
        for(G4int i = 0; i < nAngleBins; i++) {
            G4cout << "  [" << angleBins[i] << " - " << angleBins[i+1] << " degrees]: " 
                   << angleCounts[i] << " neutrons" << G4endl;
        }
    }
    
    // Calorimeter Entrance Electron Spectrum
    if(fCaloEntranceElectronEnergies.size() > 0) {
        G4cout << "\n=== Electron Energy Spectrum at Calorimeter Entrance ===" << G4endl;
        G4double electronEnergyBins[] = {0.0, 0.1, 0.2, 0.5, 1.0, 2.0, 4.0, 8.0};
        G4int nElectronEnergyBins = 7;
        G4int electronEnergyCounts[7] = {0};
        
        for(size_t i = 0; i < fCaloEntranceElectronEnergies.size(); i++) {
            G4double E = fCaloEntranceElectronEnergies[i] / GeV;
            for(G4int j = 0; j < nElectronEnergyBins; j++) {
                if(E >= electronEnergyBins[j] && E < electronEnergyBins[j+1]) {
                    electronEnergyCounts[j]++;
                    break;
                }
            }
        }
        
        for(G4int i = 0; i < nElectronEnergyBins; i++) {
            G4cout << "  [" << electronEnergyBins[i] << " - " << electronEnergyBins[i+1] 
                   << " GeV]: " << electronEnergyCounts[i] << " electrons" << G4endl;
        }
    }
    
    // Calorimeter Entrance Electron Angular Distribution
    if(fCaloEntranceElectronAngles.size() > 0) {
        G4cout << "\n=== Electron Angular Distribution at Calorimeter Entrance ===" << G4endl;
        G4double angleBins[] = {0.0, 5.0, 10.0, 20.0, 30.0, 45.0, 60.0, 90.0};
        G4int nAngleBins = 7;
        G4int angleCounts[7] = {0};
        
        for(size_t i = 0; i < fCaloEntranceElectronAngles.size(); i++) {
            G4double angleDeg = fCaloEntranceElectronAngles[i] * 180.0 / 3.14159265;
            for(G4int j = 0; j < nAngleBins; j++) {
                if(angleDeg >= angleBins[j] && angleDeg < angleBins[j+1]) {
                    angleCounts[j]++;
                    break;
                }
            }
        }
        
        G4cout << "  (Angle relative to beam axis, 0 deg = straight forward)" << G4endl;
        for(G4int i = 0; i < nAngleBins; i++) {
            G4cout << "  [" << angleBins[i] << " - " << angleBins[i+1] << " degrees]: " 
                   << angleCounts[i] << " electrons" << G4endl;
        }
    }
    
    // Calorimeter Entrance Positron Spectrum
    if(fCaloEntrancePositronEnergies.size() > 0) {
        G4cout << "\n=== Positron Energy Spectrum at Calorimeter Entrance ===" << G4endl;
        G4double positronEnergyBins[] = {0.0, 0.1, 0.2, 0.5, 1.0, 2.0, 4.0, 8.0};
        G4int nPositronEnergyBins = 7;
        G4int positronEnergyCounts[7] = {0};
        
        for(size_t i = 0; i < fCaloEntrancePositronEnergies.size(); i++) {
            G4double E = fCaloEntrancePositronEnergies[i] / GeV;
            for(G4int j = 0; j < nPositronEnergyBins; j++) {
                if(E >= positronEnergyBins[j] && E < positronEnergyBins[j+1]) {
                    positronEnergyCounts[j]++;
                    break;
                }
            }
        }
        
        for(G4int i = 0; i < nPositronEnergyBins; i++) {
            G4cout << "  [" << positronEnergyBins[i] << " - " << positronEnergyBins[i+1] 
                   << " GeV]: " << positronEnergyCounts[i] << " positrons" << G4endl;
        }
    }
    
    // Calorimeter Entrance Positron Angular Distribution
    if(fCaloEntrancePositronAngles.size() > 0) {
        G4cout << "\n=== Positron Angular Distribution at Calorimeter Entrance ===" << G4endl;
        G4double angleBins[] = {0.0, 5.0, 10.0, 20.0, 30.0, 45.0, 60.0, 90.0};
        G4int nAngleBins = 7;
        G4int angleCounts[7] = {0};
        
        for(size_t i = 0; i < fCaloEntrancePositronAngles.size(); i++) {
            G4double angleDeg = fCaloEntrancePositronAngles[i] * 180.0 / 3.14159265;
            for(G4int j = 0; j < nAngleBins; j++) {
                if(angleDeg >= angleBins[j] && angleDeg < angleBins[j+1]) {
                    angleCounts[j]++;
                    break;
                }
            }
        }
        
        G4cout << "  (Angle relative to beam axis, 0 deg = straight forward)" << G4endl;
        for(G4int i = 0; i < nAngleBins; i++) {
            G4cout << "  [" << angleBins[i] << " - " << angleBins[i+1] << " degrees]: " 
                   << angleCounts[i] << " positrons" << G4endl;
        }
    }
    
    // Calorimeter Exit Summary
    G4cout << "\n=============================================" << G4endl;
    G4cout << "=== Particles EXITING Calorimeter ===" << G4endl;
    G4cout << "=============================================" << G4endl;
    G4cout << "Neutrons:   " << fCaloExitNeutrons << G4endl;
    G4cout << "Photons:    " << fCaloExitPhotons << G4endl;
    G4cout << "Electrons:  " << fCaloExitElectrons << G4endl;
    G4cout << "Positrons:  " << fCaloExitPositrons << G4endl;
    G4cout << "Total Energy: " << fCaloExitEnergy/GeV << " GeV" << G4endl;
    G4cout << "  (Primary beam: " << fCaloExitPrimaryEnergy/GeV << " GeV, Secondaries: " 
           << (fCaloExitEnergy - fCaloExitPrimaryEnergy)/GeV << " GeV)" << G4endl;
    
    // Calorimeter Exit Photon Spectrum
    if(fCaloExitPhotonEnergies.size() > 0) {
        G4cout << "\n=== Photon Energy Spectrum at Calorimeter Exit ===" << G4endl;
        G4double bins[] = {0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 1.0, 2.0, 4.0, 8.0};
        G4int nBins = 9;
        G4int counts[9] = {0};
        
        for(size_t i = 0; i < fCaloExitPhotonEnergies.size(); i++) {
            G4double E = fCaloExitPhotonEnergies[i] / GeV;
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
        G4cout << "  Total photons: " << fCaloExitPhotonEnergies.size() << G4endl;
    }
    
    // Calorimeter Exit Photon Angular Distribution
    if(fCaloExitPhotonAngles.size() > 0) {
        G4cout << "\n=== Photon Angular Distribution at Calorimeter Exit ===" << G4endl;
        G4double angleBins[] = {0.0, 5.0, 10.0, 20.0, 30.0, 45.0, 60.0, 90.0};
        G4int nAngleBins = 7;
        G4int angleCounts[7] = {0};
        
        for(size_t i = 0; i < fCaloExitPhotonAngles.size(); i++) {
            G4double angleDeg = fCaloExitPhotonAngles[i] * 180.0 / 3.14159265;
            for(G4int j = 0; j < nAngleBins; j++) {
                if(angleDeg >= angleBins[j] && angleDeg < angleBins[j+1]) {
                    angleCounts[j]++;
                    break;
                }
            }
        }
        
        G4cout << "  (Angle relative to beam axis, 0 deg = straight forward)" << G4endl;
        for(G4int i = 0; i < nAngleBins; i++) {
            G4cout << "  [" << angleBins[i] << " - " << angleBins[i+1] << " degrees]: " 
                   << angleCounts[i] << " photons" << G4endl;
        }
    }
    
    // Calorimeter Exit Neutron Spectrum
    if(fCaloExitNeutronEnergies.size() > 0) {
        G4cout << "\n=== Neutron Energy Spectrum at Calorimeter Exit ===" << G4endl;
        G4double neutronEnergyBins[] = {0.0, 0.1, 0.5, 1.0, 2.0, 5.0, 10.0, 50.0, 100.0};
        G4int nNeutronEnergyBins = 8;
        G4int neutronEnergyCounts[8] = {0};
        
        for(size_t i = 0; i < fCaloExitNeutronEnergies.size(); i++) {
            G4double E = fCaloExitNeutronEnergies[i] / GeV;
            for(G4int j = 0; j < nNeutronEnergyBins; j++) {
                if(E >= neutronEnergyBins[j] && E < neutronEnergyBins[j+1]) {
                    neutronEnergyCounts[j]++;
                    break;
                }
            }
        }
        
        for(G4int i = 0; i < nNeutronEnergyBins; i++) {
            G4cout << "  [" << neutronEnergyBins[i] << " - " << neutronEnergyBins[i+1] 
                   << " GeV]: " << neutronEnergyCounts[i] << " neutrons" << G4endl;
        }
    }
    
    // Calorimeter Exit Neutron Angular Distribution
    if(fCaloExitNeutronAngles.size() > 0) {
        G4cout << "\n=== Neutron Angular Distribution at Calorimeter Exit ===" << G4endl;
        G4double angleBins[] = {0.0, 5.0, 10.0, 20.0, 30.0, 45.0, 60.0, 90.0};
        G4int nAngleBins = 7;
        G4int angleCounts[7] = {0};
        
        for(size_t i = 0; i < fCaloExitNeutronAngles.size(); i++) {
            G4double angleDeg = fCaloExitNeutronAngles[i] * 180.0 / 3.14159265;
            for(G4int j = 0; j < nAngleBins; j++) {
                if(angleDeg >= angleBins[j] && angleDeg < angleBins[j+1]) {
                    angleCounts[j]++;
                    break;
                }
            }
        }
        
        G4cout << "  (Angle relative to beam axis, 0 deg = straight forward)" << G4endl;
        for(G4int i = 0; i < nAngleBins; i++) {
            G4cout << "  [" << angleBins[i] << " - " << angleBins[i+1] << " degrees]: " 
                   << angleCounts[i] << " neutrons" << G4endl;
        }
    }
    
    // Calorimeter Exit Electron Spectrum
    if(fCaloExitElectronEnergies.size() > 0) {
        G4cout << "\n=== Electron Energy Spectrum at Calorimeter Exit ===" << G4endl;
        G4double electronEnergyBins[] = {0.0, 0.1, 0.2, 0.5, 1.0, 2.0, 4.0, 8.0};
        G4int nElectronEnergyBins = 7;
        G4int electronEnergyCounts[7] = {0};
        
        for(size_t i = 0; i < fCaloExitElectronEnergies.size(); i++) {
            G4double E = fCaloExitElectronEnergies[i] / GeV;
            for(G4int j = 0; j < nElectronEnergyBins; j++) {
                if(E >= electronEnergyBins[j] && E < electronEnergyBins[j+1]) {
                    electronEnergyCounts[j]++;
                    break;
                }
            }
        }
        
        for(G4int i = 0; i < nElectronEnergyBins; i++) {
            G4cout << "  [" << electronEnergyBins[i] << " - " << electronEnergyBins[i+1] 
                   << " GeV]: " << electronEnergyCounts[i] << " electrons" << G4endl;
        }
    }
    
    // Calorimeter Exit Electron Angular Distribution
    if(fCaloExitElectronAngles.size() > 0) {
        G4cout << "\n=== Electron Angular Distribution at Calorimeter Exit ===" << G4endl;
        G4double angleBins[] = {0.0, 5.0, 10.0, 20.0, 30.0, 45.0, 60.0, 90.0};
        G4int nAngleBins = 7;
        G4int angleCounts[7] = {0};
        
        for(size_t i = 0; i < fCaloExitElectronAngles.size(); i++) {
            G4double angleDeg = fCaloExitElectronAngles[i] * 180.0 / 3.14159265;
            for(G4int j = 0; j < nAngleBins; j++) {
                if(angleDeg >= angleBins[j] && angleDeg < angleBins[j+1]) {
                    angleCounts[j]++;
                    break;
                }
            }
        }
        
        G4cout << "  (Angle relative to beam axis, 0 deg = straight forward)" << G4endl;
        for(G4int i = 0; i < nAngleBins; i++) {
            G4cout << "  [" << angleBins[i] << " - " << angleBins[i+1] << " degrees]: " 
                   << angleCounts[i] << " electrons" << G4endl;
        }
    }
    
    // Calorimeter Exit Positron Spectrum
    if(fCaloExitPositronEnergies.size() > 0) {
        G4cout << "\n=== Positron Energy Spectrum at Calorimeter Exit ===" << G4endl;
        G4double positronEnergyBins[] = {0.0, 0.1, 0.2, 0.5, 1.0, 2.0, 4.0, 8.0};
        G4int nPositronEnergyBins = 7;
        G4int positronEnergyCounts[7] = {0};
        
        for(size_t i = 0; i < fCaloExitPositronEnergies.size(); i++) {
            G4double E = fCaloExitPositronEnergies[i] / GeV;
            for(G4int j = 0; j < nPositronEnergyBins; j++) {
                if(E >= positronEnergyBins[j] && E < positronEnergyBins[j+1]) {
                    positronEnergyCounts[j]++;
                    break;
                }
            }
        }
        
        for(G4int i = 0; i < nPositronEnergyBins; i++) {
            G4cout << "  [" << positronEnergyBins[i] << " - " << positronEnergyBins[i+1] 
                   << " GeV]: " << positronEnergyCounts[i] << " positrons" << G4endl;
        }
    }
    
    // Calorimeter Exit Positron Angular Distribution
    if(fCaloExitPositronAngles.size() > 0) {
        G4cout << "\n=== Positron Angular Distribution at Calorimeter Exit ===" << G4endl;
        G4double angleBins[] = {0.0, 5.0, 10.0, 20.0, 30.0, 45.0, 60.0, 90.0};
        G4int nAngleBins = 7;
        G4int angleCounts[7] = {0};
        
        for(size_t i = 0; i < fCaloExitPositronAngles.size(); i++) {
            G4double angleDeg = fCaloExitPositronAngles[i] * 180.0 / 3.14159265;
            for(G4int j = 0; j < nAngleBins; j++) {
                if(angleDeg >= angleBins[j] && angleDeg < angleBins[j+1]) {
                    angleCounts[j]++;
                    break;
                }
            }
        }
        
        G4cout << "  (Angle relative to beam axis, 0 deg = straight forward)" << G4endl;
        for(G4int i = 0; i < nAngleBins; i++) {
            G4cout << "  [" << angleBins[i] << " - " << angleBins[i+1] << " degrees]: " 
                   << angleCounts[i] << " positrons" << G4endl;
        }
    }
    
    G4cout << "=============================================\n" << G4endl;
}
void DamsaAnalysis::SaveToFile(const G4String& filename)
{
    mkdir("output", 0755);
    
    std::string fullPath = "output/" + filename;
    std::ofstream outFile(fullPath);
    
    if(!outFile.is_open()) {
        G4cout << "ERROR: Could not open file " << fullPath << " for writing!" << G4endl;
        return;
    }
    
    outFile << "=============================================" << std::endl;
    outFile << "=== DAMSA Background Analysis Results ===" << std::endl;
    outFile << "=============================================" << std::endl << std::endl;
    
    // Target Exit data
    outFile << "=== Particles EXITING Target ===" << std::endl;
    outFile << "Neutrons:     " << fTargetExitNeutrons << std::endl;
    outFile << "Photons:      " << fTargetExitPhotons << std::endl;
    outFile << "Electrons:    " << fTargetExitElectrons << std::endl;
    outFile << "Positrons:    " << fTargetExitPositrons << std::endl;
    outFile << "Total Energy: " << std::fixed << std::setprecision(6) 
            << fTargetExitEnergy/GeV << " GeV" << std::endl << std::endl;
    
    // Magnet Entrance data
    outFile << "=== Particles ENTERING Magnet ===" << std::endl;
    outFile << "Neutrons:     " << fMagnetEntranceNeutrons << std::endl;
    outFile << "Photons:      " << fMagnetEntrancePhotons << std::endl;
    outFile << "Electrons:    " << fMagnetEntranceElectrons << std::endl;
    outFile << "Positrons:    " << fMagnetEntrancePositrons << std::endl;
    outFile << "Total Energy: " << std::fixed << std::setprecision(6) 
            << fMagnetEntranceEnergy/GeV << " GeV" << std::endl << std::endl;
    
    // Calorimeter Entrance data
    outFile << "=== Particles ENTERING Calorimeter ===" << std::endl;
    outFile << "Neutrons:     " << fCaloEntranceNeutrons << std::endl;
    outFile << "Photons:      " << fCaloEntrancePhotons << std::endl;
    outFile << "Electrons:    " << fCaloEntranceElectrons << std::endl;
    outFile << "Positrons:    " << fCaloEntrancePositrons << std::endl;
    outFile << "Total Energy: " << std::fixed << std::setprecision(6) 
            << fCaloEntranceEnergy/GeV << " GeV" << std::endl << std::endl;
    
    // Calorimeter Exit data
    outFile << "=== Particles EXITING Calorimeter ===" << std::endl;
    outFile << "Neutrons:     " << fCaloExitNeutrons << std::endl;
    outFile << "Photons:      " << fCaloExitPhotons << std::endl;
    outFile << "Electrons:    " << fCaloExitElectrons << std::endl;
    outFile << "Positrons:    " << fCaloExitPositrons << std::endl;
    outFile << "Total Energy: " << std::fixed << std::setprecision(6) 
            << fCaloExitEnergy/GeV << " GeV" << std::endl << std::endl;
    
    outFile << "=============================================" << std::endl;
    outFile.close();
    
    G4cout << "Analysis results saved to: " << fullPath << G4endl;
}

void DamsaAnalysis::WriteROOTHistograms(const G4String& filename)
{
    mkdir("plots", 0755);
    
    gStyle->SetOptStat(111111);
    gStyle->SetPalette(kRainBow);
    
    // Create separate ROOT file for Target Exit
    std::string targetExitPath = "plots/targetexit_histograms.root";
    TFile* targetExitFile = new TFile(targetExitPath.c_str(), "RECREATE");
    
    if(!targetExitFile || targetExitFile->IsZombie()) {
        G4cout << "ERROR: Could not create ROOT file " << targetExitPath << G4endl;
    } else {
        // Photon histograms - Target Exit
        if(fTargetExitPhotonEnergies.size() > 0) {
            TH1D* hPhotonEnergy = new TH1D("hPhotonEnergy", "Photon Energy at Target Exit;Energy [GeV];Counts", 100, 0, 8);
            TH1D* hPhotonAngle = new TH1D("hPhotonAngle", "Photon Angle at Target Exit;Angle [degrees];Counts", 90, 0, 90);
            
            for(size_t i = 0; i < fTargetExitPhotonEnergies.size(); i++) {
                hPhotonEnergy->Fill(fTargetExitPhotonEnergies[i] / GeV);
                hPhotonAngle->Fill(fTargetExitPhotonAngles[i] * 180.0 / 3.14159265);
            }
            
            hPhotonEnergy->Write();
            hPhotonAngle->Write();
            delete hPhotonEnergy;
            delete hPhotonAngle;
        }
        
        // Neutron histograms - Target Exit
        if(fTargetExitNeutronEnergies.size() > 0) {
            TH1D* hNeutronEnergy = new TH1D("hNeutronEnergy", "Neutron Energy at Target Exit;Energy [GeV];Counts", 100, 0, 100);
            TH1D* hNeutronAngle = new TH1D("hNeutronAngle", "Neutron Angle at Target Exit;Angle [degrees];Counts", 90, 0, 90);
            
            for(size_t i = 0; i < fTargetExitNeutronEnergies.size(); i++) {
                hNeutronEnergy->Fill(fTargetExitNeutronEnergies[i] / GeV);
                hNeutronAngle->Fill(fTargetExitNeutronAngles[i] * 180.0 / 3.14159265);
            }
            
            hNeutronEnergy->Write();
            hNeutronAngle->Write();
            delete hNeutronEnergy;
            delete hNeutronAngle;
        }
        
        // Electron histograms - Target Exit
        if(fTargetExitElectronEnergies.size() > 0) {
            TH1D* hElectronEnergy = new TH1D("hElectronEnergy", "Electron Energy at Target Exit;Energy [GeV];Counts", 100, 0, 8);
            TH1D* hElectronAngle = new TH1D("hElectronAngle", "Electron Angle at Target Exit;Angle [degrees];Counts", 90, 0, 90);
            
            for(size_t i = 0; i < fTargetExitElectronEnergies.size(); i++) {
                hElectronEnergy->Fill(fTargetExitElectronEnergies[i] / GeV);
                hElectronAngle->Fill(fTargetExitElectronAngles[i] * 180.0 / 3.14159265);
            }
            
            hElectronEnergy->Write();
            hElectronAngle->Write();
            delete hElectronEnergy;
            delete hElectronAngle;
        }
        
        // Positron histograms - Target Exit
        if(fTargetExitPositronEnergies.size() > 0) {
            TH1D* hPositronEnergy = new TH1D("hPositronEnergy", "Positron Energy at Target Exit;Energy [GeV];Counts", 100, 0, 8);
            TH1D* hPositronAngle = new TH1D("hPositronAngle", "Positron Angle at Target Exit;Angle [degrees];Counts", 90, 0, 90);
            
            for(size_t i = 0; i < fTargetExitPositronEnergies.size(); i++) {
                hPositronEnergy->Fill(fTargetExitPositronEnergies[i] / GeV);
                hPositronAngle->Fill(fTargetExitPositronAngles[i] * 180.0 / 3.14159265);
            }
            
            hPositronEnergy->Write();
            hPositronAngle->Write();
            delete hPositronEnergy;
            delete hPositronAngle;
        }
        
        targetExitFile->Close();
        delete targetExitFile;
        G4cout << "Target Exit histograms saved to: " << targetExitPath << G4endl;
    }
    
    // Create separate ROOT file for Magnet Entrance
    std::string magnetPath = "plots/magnetentrance_histograms.root";
    TFile* magnetFile = new TFile(magnetPath.c_str(), "RECREATE");
    
    if(!magnetFile || magnetFile->IsZombie()) {
        G4cout << "ERROR: Could not create ROOT file " << magnetPath << G4endl;
    } else {
        // Photon histograms - Magnet Entrance
        if(fMagnetEntrancePhotonEnergies.size() > 0) {
            TH1D* hPhotonEnergy = new TH1D("hPhotonEnergy", "Photon Energy at Magnet Entrance;Energy [GeV];Counts", 100, 0, 8);
            TH1D* hPhotonAngle = new TH1D("hPhotonAngle", "Photon Angle at Magnet Entrance;Angle [degrees];Counts", 90, 0, 90);
            
            for(size_t i = 0; i < fMagnetEntrancePhotonEnergies.size(); i++) {
                hPhotonEnergy->Fill(fMagnetEntrancePhotonEnergies[i] / GeV);
                hPhotonAngle->Fill(fMagnetEntrancePhotonAngles[i] * 180.0 / 3.14159265);
            }
            
            hPhotonEnergy->Write();
            hPhotonAngle->Write();
            delete hPhotonEnergy;
            delete hPhotonAngle;
        }
        
        // Neutron histograms - Magnet Entrance
        if(fMagnetEntranceNeutronEnergies.size() > 0) {
            TH1D* hNeutronEnergy = new TH1D("hNeutronEnergy", "Neutron Energy at Magnet Entrance;Energy [GeV];Counts", 100, 0, 100);
            TH1D* hNeutronAngle = new TH1D("hNeutronAngle", "Neutron Angle at Magnet Entrance;Angle [degrees];Counts", 90, 0, 90);
            
            for(size_t i = 0; i < fMagnetEntranceNeutronEnergies.size(); i++) {
                hNeutronEnergy->Fill(fMagnetEntranceNeutronEnergies[i] / GeV);
                hNeutronAngle->Fill(fMagnetEntranceNeutronAngles[i] * 180.0 / 3.14159265);
            }
            
            hNeutronEnergy->Write();
            hNeutronAngle->Write();
            delete hNeutronEnergy;
            delete hNeutronAngle;
        }
        
        // Electron histograms - Magnet Entrance
        if(fMagnetEntranceElectronEnergies.size() > 0) {
            TH1D* hElectronEnergy = new TH1D("hElectronEnergy", "Electron Energy at Magnet Entrance;Energy [GeV];Counts", 100, 0, 8);
            TH1D* hElectronAngle = new TH1D("hElectronAngle", "Electron Angle at Magnet Entrance;Angle [degrees];Counts", 90, 0, 90);
            
            for(size_t i = 0; i < fMagnetEntranceElectronEnergies.size(); i++) {
                hElectronEnergy->Fill(fMagnetEntranceElectronEnergies[i] / GeV);
                hElectronAngle->Fill(fMagnetEntranceElectronAngles[i] * 180.0 / 3.14159265);
            }
            
            hElectronEnergy->Write();
            hElectronAngle->Write();
            delete hElectronEnergy;
            delete hElectronAngle;
        }
        
        // Positron histograms - Magnet Entrance
        if(fMagnetEntrancePositronEnergies.size() > 0) {
            TH1D* hPositronEnergy = new TH1D("hPositronEnergy", "Positron Energy at Magnet Entrance;Energy [GeV];Counts", 100, 0, 8);
            TH1D* hPositronAngle = new TH1D("hPositronAngle", "Positron Angle at Magnet Entrance;Angle [degrees];Counts", 90, 0, 90);
            
            for(size_t i = 0; i < fMagnetEntrancePositronEnergies.size(); i++) {
                hPositronEnergy->Fill(fMagnetEntrancePositronEnergies[i] / GeV);
                hPositronAngle->Fill(fMagnetEntrancePositronAngles[i] * 180.0 / 3.14159265);
            }
            
            hPositronEnergy->Write();
            hPositronAngle->Write();
            delete hPositronEnergy;
            delete hPositronAngle;
        }
        
        magnetFile->Close();
        delete magnetFile;
        G4cout << "Magnet Entrance histograms saved to: " << magnetPath << G4endl;
    }
    
    // Create separate ROOT file for Calorimeter Entrance
    std::string caloEntrancePath = "plots/caloentrance_histograms.root";
    TFile* caloEntranceFile = new TFile(caloEntrancePath.c_str(), "RECREATE");
    
    if(!caloEntranceFile || caloEntranceFile->IsZombie()) {
        G4cout << "ERROR: Could not create ROOT file " << caloEntrancePath << G4endl;
    } else {
        // Photon histograms - Calo Entrance
        if(fCaloEntrancePhotonEnergies.size() > 0) {
            TH1D* hPhotonEnergy = new TH1D("hPhotonEnergy", "Photon Energy at Calorimeter Entrance;Energy [GeV];Counts", 100, 0, 8);
            TH1D* hPhotonAngle = new TH1D("hPhotonAngle", "Photon Angle at Calorimeter Entrance;Angle [degrees];Counts", 90, 0, 90);
            
            for(size_t i = 0; i < fCaloEntrancePhotonEnergies.size(); i++) {
                hPhotonEnergy->Fill(fCaloEntrancePhotonEnergies[i] / GeV);
                hPhotonAngle->Fill(fCaloEntrancePhotonAngles[i] * 180.0 / 3.14159265);
            }
            
            hPhotonEnergy->Write();
            hPhotonAngle->Write();
            delete hPhotonEnergy;
            delete hPhotonAngle;
        }
        
        // Neutron histograms - Calo Entrance
        if(fCaloEntranceNeutronEnergies.size() > 0) {
            TH1D* hNeutronEnergy = new TH1D("hNeutronEnergy", "Neutron Energy at Calorimeter Entrance;Energy [GeV];Counts", 100, 0, 100);
            TH1D* hNeutronAngle = new TH1D("hNeutronAngle", "Neutron Angle at Calorimeter Entrance;Angle [degrees];Counts", 90, 0, 90);
            
            for(size_t i = 0; i < fCaloEntranceNeutronEnergies.size(); i++) {
                hNeutronEnergy->Fill(fCaloEntranceNeutronEnergies[i] / GeV);
                hNeutronAngle->Fill(fCaloEntranceNeutronAngles[i] * 180.0 / 3.14159265);
            }
            
            hNeutronEnergy->Write();
            hNeutronAngle->Write();
            delete hNeutronEnergy;
            delete hNeutronAngle;
        }
        
        // Electron histograms - Calo Entrance
        if(fCaloEntranceElectronEnergies.size() > 0) {
            TH1D* hElectronEnergy = new TH1D("hElectronEnergy", "Electron Energy at Calorimeter Entrance;Energy [GeV];Counts", 100, 0, 8);
            TH1D* hElectronAngle = new TH1D("hElectronAngle", "Electron Angle at Calorimeter Entrance;Angle [degrees];Counts", 90, 0, 90);
            
            for(size_t i = 0; i < fCaloEntranceElectronEnergies.size(); i++) {
                hElectronEnergy->Fill(fCaloEntranceElectronEnergies[i] / GeV);
                hElectronAngle->Fill(fCaloEntranceElectronAngles[i] * 180.0 / 3.14159265);
            }
            
            hElectronEnergy->Write();
            hElectronAngle->Write();
            delete hElectronEnergy;
            delete hElectronAngle;
        }
        
        // Positron histograms - Calo Entrance
        if(fCaloEntrancePositronEnergies.size() > 0) {
            TH1D* hPositronEnergy = new TH1D("hPositronEnergy", "Positron Energy at Calorimeter Entrance;Energy [GeV];Counts", 100, 0, 8);
            TH1D* hPositronAngle = new TH1D("hPositronAngle", "Positron Angle at Calorimeter Entrance;Angle [degrees];Counts", 90, 0, 90);
            
            for(size_t i = 0; i < fCaloEntrancePositronEnergies.size(); i++) {
                hPositronEnergy->Fill(fCaloEntrancePositronEnergies[i] / GeV);
                hPositronAngle->Fill(fCaloEntrancePositronAngles[i] * 180.0 / 3.14159265);
            }
            
            hPositronEnergy->Write();
            hPositronAngle->Write();
            delete hPositronEnergy;
            delete hPositronAngle;
        }
        
        caloEntranceFile->Close();
        delete caloEntranceFile;
        G4cout << "Calorimeter Entrance histograms saved to: " << caloEntrancePath << G4endl;
    }
    
    // Create separate ROOT file for Calorimeter Exit
    std::string caloExitPath = "plots/caloexit_histograms.root";
    TFile* caloExitFile = new TFile(caloExitPath.c_str(), "RECREATE");
    
    if(!caloExitFile || caloExitFile->IsZombie()) {
        G4cout << "ERROR: Could not create ROOT file " << caloExitPath << G4endl;
    } else {
        // Photon histograms - Calo Exit
        if(fCaloExitPhotonEnergies.size() > 0) {
            TH1D* hPhotonEnergy = new TH1D("hPhotonEnergy", "Photon Energy at Calorimeter Exit;Energy [GeV];Counts", 100, 0, 8);
            TH1D* hPhotonAngle = new TH1D("hPhotonAngle", "Photon Angle at Calorimeter Exit;Angle [degrees];Counts", 90, 0, 90);
            
            for(size_t i = 0; i < fCaloExitPhotonEnergies.size(); i++) {
                hPhotonEnergy->Fill(fCaloExitPhotonEnergies[i] / GeV);
                hPhotonAngle->Fill(fCaloExitPhotonAngles[i] * 180.0 / 3.14159265);
            }
            
            hPhotonEnergy->Write();
            hPhotonAngle->Write();
            delete hPhotonEnergy;
            delete hPhotonAngle;
        }
        
        // Neutron histograms - Calo Exit
        if(fCaloExitNeutronEnergies.size() > 0) {
            TH1D* hNeutronEnergy = new TH1D("hNeutronEnergy", "Neutron Energy at Calorimeter Exit;Energy [GeV];Counts", 100, 0, 100);
            TH1D* hNeutronAngle = new TH1D("hNeutronAngle", "Neutron Angle at Calorimeter Exit;Angle [degrees];Counts", 90, 0, 90);
            
            for(size_t i = 0; i < fCaloExitNeutronEnergies.size(); i++) {
                hNeutronEnergy->Fill(fCaloExitNeutronEnergies[i] / GeV);
                hNeutronAngle->Fill(fCaloExitNeutronAngles[i] * 180.0 / 3.14159265);
            }
            
            hNeutronEnergy->Write();
            hNeutronAngle->Write();
            delete hNeutronEnergy;
            delete hNeutronAngle;
        }
        
        // Electron histograms - Calo Exit
        if(fCaloExitElectronEnergies.size() > 0) {
            TH1D* hElectronEnergy = new TH1D("hElectronEnergy", "Electron Energy at Calorimeter Exit;Energy [GeV];Counts", 100, 0, 8);
            TH1D* hElectronAngle = new TH1D("hElectronAngle", "Electron Angle at Calorimeter Exit;Angle [degrees];Counts", 90, 0, 90);
            
            for(size_t i = 0; i < fCaloExitElectronEnergies.size(); i++) {
                hElectronEnergy->Fill(fCaloExitElectronEnergies[i] / GeV);
                hElectronAngle->Fill(fCaloExitElectronAngles[i] * 180.0 / 3.14159265);
            }
            
            hElectronEnergy->Write();
            hElectronAngle->Write();
            delete hElectronEnergy;
            delete hElectronAngle;
        }
        
        // Positron histograms - Calo Exit
        if(fCaloExitPositronEnergies.size() > 0) {
            TH1D* hPositronEnergy = new TH1D("hPositronEnergy", "Positron Energy at Calorimeter Exit;Energy [GeV];Counts", 100, 0, 8);
            TH1D* hPositronAngle = new TH1D("hPositronAngle", "Positron Angle at Calorimeter Exit;Angle [degrees];Counts", 90, 0, 90);
            
            for(size_t i = 0; i < fCaloExitPositronEnergies.size(); i++) {
                hPositronEnergy->Fill(fCaloExitPositronEnergies[i] / GeV);
                hPositronAngle->Fill(fCaloExitPositronAngles[i] * 180.0 / 3.14159265);
            }
            
            hPositronEnergy->Write();
            hPositronAngle->Write();
            delete hPositronEnergy;
            delete hPositronAngle;
        }
        
        caloExitFile->Close();
        delete caloExitFile;
        G4cout << "Calorimeter Exit histograms saved to: " << caloExitPath << G4endl;
    }
    
    G4cout << "All ROOT histograms saved to plots/ directory" << G4endl;
}

void DamsaAnalysis::Reset()
{
    fTargetExitNeutrons = 0;
    fTargetExitPhotons = 0;
    fTargetExitElectrons = 0;
    fTargetExitPositrons = 0;
    fTargetExitEnergy = 0;
    fTargetExitPrimaryEnergy = 0;
    fTargetExitPhotonEnergies.clear();
    fTargetExitPhotonAngles.clear();
    fTargetExitNeutronEnergies.clear();
    fTargetExitNeutronAngles.clear();
    fTargetExitElectronEnergies.clear();
    fTargetExitElectronAngles.clear();
    fTargetExitPositronEnergies.clear();
    fTargetExitPositronAngles.clear();
    
    fMagnetEntranceNeutrons = 0;
    fMagnetEntrancePhotons = 0;
    fMagnetEntranceElectrons = 0;
    fMagnetEntrancePositrons = 0;
    fMagnetEntranceEnergy = 0;
    fMagnetEntrancePrimaryEnergy = 0;
    fMagnetEntrancePhotonEnergies.clear();
    fMagnetEntrancePhotonAngles.clear();
    fMagnetEntranceNeutronEnergies.clear();
    fMagnetEntranceNeutronAngles.clear();
    fMagnetEntranceElectronEnergies.clear();
    fMagnetEntranceElectronAngles.clear();
    fMagnetEntrancePositronEnergies.clear();
    fMagnetEntrancePositronAngles.clear();
    
    fCaloEntranceNeutrons = 0;
    fCaloEntrancePhotons = 0;
    fCaloEntranceElectrons = 0;
    fCaloEntrancePositrons = 0;
    fCaloEntranceEnergy = 0;
    fCaloEntrancePrimaryEnergy = 0;
    fCaloEntrancePhotonEnergies.clear();
    fCaloEntrancePhotonAngles.clear();
    fCaloEntranceNeutronEnergies.clear();
    fCaloEntranceNeutronAngles.clear();
    fCaloEntranceElectronEnergies.clear();
    fCaloEntranceElectronAngles.clear();
    fCaloEntrancePositronEnergies.clear();
    fCaloEntrancePositronAngles.clear();
    
    fCaloExitNeutrons = 0;
    fCaloExitPhotons = 0;
    fCaloExitElectrons = 0;
    fCaloExitPositrons = 0;
    fCaloExitEnergy = 0;
    fCaloExitPrimaryEnergy = 0;
    fCaloExitPhotonEnergies.clear();
    fCaloExitPhotonAngles.clear();
    fCaloExitNeutronEnergies.clear();
    fCaloExitNeutronAngles.clear();
    fCaloExitElectronEnergies.clear();
    fCaloExitElectronAngles.clear();
    fCaloExitPositronEnergies.clear();
    fCaloExitPositronAngles.clear();
    
    // Also clear per-event tracking
    ResetEventTracking();
}

#endif
