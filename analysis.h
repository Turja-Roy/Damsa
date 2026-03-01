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
#include "TH2D.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TPad.h"
#include "plotting.h"

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
    G4cout << "\n=== Generating Publication-Quality Plots ===" << G4endl;
    
    // Create directory structure
    CreatePlotDirectories();
    
    // Set publication style
    SetPublicationStyle();
    
    // Convert G4double vectors to double vectors for plotting functions
    // Target Exit
    std::vector<double> tePhotonE, tePhotonA, teNeutronE, teNeutronA;
    std::vector<double> teElectronE, teElectronA, tePositronE, tePositronA;
    for(size_t i = 0; i < fTargetExitPhotonEnergies.size(); i++) {
        tePhotonE.push_back(fTargetExitPhotonEnergies[i]/GeV);
        tePhotonA.push_back(fTargetExitPhotonAngles[i]);
    }
    for(size_t i = 0; i < fTargetExitNeutronEnergies.size(); i++) {
        teNeutronE.push_back(fTargetExitNeutronEnergies[i]/GeV);
        teNeutronA.push_back(fTargetExitNeutronAngles[i]);
    }
    for(size_t i = 0; i < fTargetExitElectronEnergies.size(); i++) {
        teElectronE.push_back(fTargetExitElectronEnergies[i]/GeV);
        teElectronA.push_back(fTargetExitElectronAngles[i]);
    }
    for(size_t i = 0; i < fTargetExitPositronEnergies.size(); i++) {
        tePositronE.push_back(fTargetExitPositronEnergies[i]/GeV);
        tePositronA.push_back(fTargetExitPositronAngles[i]);
    }
    
    // Magnet Entrance
    std::vector<double> mePhotonE, mePhotonA, meNeutronE, meNeutronA;
    std::vector<double> meElectronE, meElectronA, mePositronE, mePositronA;
    for(size_t i = 0; i < fMagnetEntrancePhotonEnergies.size(); i++) {
        mePhotonE.push_back(fMagnetEntrancePhotonEnergies[i]/GeV);
        mePhotonA.push_back(fMagnetEntrancePhotonAngles[i]);
    }
    for(size_t i = 0; i < fMagnetEntranceNeutronEnergies.size(); i++) {
        meNeutronE.push_back(fMagnetEntranceNeutronEnergies[i]/GeV);
        meNeutronA.push_back(fMagnetEntranceNeutronAngles[i]);
    }
    for(size_t i = 0; i < fMagnetEntranceElectronEnergies.size(); i++) {
        meElectronE.push_back(fMagnetEntranceElectronEnergies[i]/GeV);
        meElectronA.push_back(fMagnetEntranceElectronAngles[i]);
    }
    for(size_t i = 0; i < fMagnetEntrancePositronEnergies.size(); i++) {
        mePositronE.push_back(fMagnetEntrancePositronEnergies[i]/GeV);
        mePositronA.push_back(fMagnetEntrancePositronAngles[i]);
    }
    
    // Calo Entrance
    std::vector<double> cePhotonE, cePhotonA, ceNeutronE, ceNeutronA;
    std::vector<double> ceElectronE, ceElectronA, cePositronE, cePositronA;
    for(size_t i = 0; i < fCaloEntrancePhotonEnergies.size(); i++) {
        cePhotonE.push_back(fCaloEntrancePhotonEnergies[i]/GeV);
        cePhotonA.push_back(fCaloEntrancePhotonAngles[i]);
    }
    for(size_t i = 0; i < fCaloEntranceNeutronEnergies.size(); i++) {
        ceNeutronE.push_back(fCaloEntranceNeutronEnergies[i]/GeV);
        ceNeutronA.push_back(fCaloEntranceNeutronAngles[i]);
    }
    for(size_t i = 0; i < fCaloEntranceElectronEnergies.size(); i++) {
        ceElectronE.push_back(fCaloEntranceElectronEnergies[i]/GeV);
        ceElectronA.push_back(fCaloEntranceElectronAngles[i]);
    }
    for(size_t i = 0; i < fCaloEntrancePositronEnergies.size(); i++) {
        cePositronE.push_back(fCaloEntrancePositronEnergies[i]/GeV);
        cePositronA.push_back(fCaloEntrancePositronAngles[i]);
    }
    
    // Calo Exit
    std::vector<double> cxPhotonE, cxPhotonA, cxNeutronE, cxNeutronA;
    std::vector<double> cxElectronE, cxElectronA, cxPositronE, cxPositronA;
    for(size_t i = 0; i < fCaloExitPhotonEnergies.size(); i++) {
        cxPhotonE.push_back(fCaloExitPhotonEnergies[i]/GeV);
        cxPhotonA.push_back(fCaloExitPhotonAngles[i]);
    }
    for(size_t i = 0; i < fCaloExitNeutronEnergies.size(); i++) {
        cxNeutronE.push_back(fCaloExitNeutronEnergies[i]/GeV);
        cxNeutronA.push_back(fCaloExitNeutronAngles[i]);
    }
    for(size_t i = 0; i < fCaloExitElectronEnergies.size(); i++) {
        cxElectronE.push_back(fCaloExitElectronEnergies[i]/GeV);
        cxElectronA.push_back(fCaloExitElectronAngles[i]);
    }
    for(size_t i = 0; i < fCaloExitPositronEnergies.size(); i++) {
        cxPositronE.push_back(fCaloExitPositronEnergies[i]/GeV);
        cxPositronA.push_back(fCaloExitPositronAngles[i]);
    }
    
    // ========================================================
    // Create single ROOT file with organized directory structure
    // ========================================================
    std::string rootPath = "plots/damsa_analysis.root";
    TFile* rootFile = new TFile(rootPath.c_str(), "RECREATE");
    
    if(!rootFile || rootFile->IsZombie()) {
        G4cout << "ERROR: Could not create ROOT file " << rootPath << G4endl;
        return;
    }
    
    G4cout << "Creating histograms in organized ROOT file..." << G4endl;
    
    // ========================================================
    // TARGET EXIT HISTOGRAMS
    // ========================================================
    rootFile->mkdir("TargetExit");
    rootFile->mkdir("TargetExit/Energy");
    rootFile->mkdir("TargetExit/Angular");
    rootFile->mkdir("TargetExit/Correlation");
    rootFile->mkdir("TargetExit/Summary");
    
    // Energy histograms
    rootFile->cd("TargetExit/Energy");
    TH1D* te_hPhotonE = CreateEnergyHist(tePhotonE, "hPhotonEnergy", "Photon Energy at Target Exit", kPhotonColor, 8.0);
    TH1D* te_hNeutronE = CreateEnergyHist(teNeutronE, "hNeutronEnergy", "Neutron Energy at Target Exit", kNeutronColor, 100.0);
    TH1D* te_hElectronE = CreateEnergyHist(teElectronE, "hElectronEnergy", "Electron Energy at Target Exit", kElectronColor, 8.0);
    TH1D* te_hPositronE = CreateEnergyHist(tePositronE, "hPositronEnergy", "Positron Energy at Target Exit", kPositronColor, 8.0);
    te_hPhotonE->Write();
    te_hNeutronE->Write();
    te_hElectronE->Write();
    te_hPositronE->Write();
    
    // Normalized energy histograms
    TH1D* te_hPhotonE_norm = CreateNormalizedCopy(te_hPhotonE, "_norm");
    TH1D* te_hNeutronE_norm = CreateNormalizedCopy(te_hNeutronE, "_norm");
    TH1D* te_hElectronE_norm = CreateNormalizedCopy(te_hElectronE, "_norm");
    TH1D* te_hPositronE_norm = CreateNormalizedCopy(te_hPositronE, "_norm");
    te_hPhotonE_norm->Write();
    te_hNeutronE_norm->Write();
    te_hElectronE_norm->Write();
    te_hPositronE_norm->Write();
    
    // Angular histograms
    rootFile->cd("TargetExit/Angular");
    TH1D* te_hPhotonA = CreateAngleHist(tePhotonA, "hPhotonAngle", "Photon Angle at Target Exit", kPhotonColor);
    TH1D* te_hNeutronA = CreateAngleHist(teNeutronA, "hNeutronAngle", "Neutron Angle at Target Exit", kNeutronColor);
    TH1D* te_hElectronA = CreateAngleHist(teElectronA, "hElectronAngle", "Electron Angle at Target Exit", kElectronColor);
    TH1D* te_hPositronA = CreateAngleHist(tePositronA, "hPositronAngle", "Positron Angle at Target Exit", kPositronColor);
    te_hPhotonA->Write();
    te_hNeutronA->Write();
    te_hElectronA->Write();
    te_hPositronA->Write();
    
    // Normalized angular histograms
    TH1D* te_hPhotonA_norm = CreateNormalizedCopy(te_hPhotonA, "_norm");
    TH1D* te_hNeutronA_norm = CreateNormalizedCopy(te_hNeutronA, "_norm");
    TH1D* te_hElectronA_norm = CreateNormalizedCopy(te_hElectronA, "_norm");
    TH1D* te_hPositronA_norm = CreateNormalizedCopy(te_hPositronA, "_norm");
    te_hPhotonA_norm->Write();
    te_hNeutronA_norm->Write();
    te_hElectronA_norm->Write();
    te_hPositronA_norm->Write();
    
    // 2D Correlation histograms (Energy vs Angle)
    rootFile->cd("TargetExit/Correlation");
    TH2D* te_h2Photon = CreateEnergyAngleHist(tePhotonE, tePhotonA, "h2PhotonEvsA", "Photon E vs Angle at Target Exit");
    TH2D* te_h2Neutron = CreateEnergyAngleHist(teNeutronE, teNeutronA, "h2NeutronEvsA", "Neutron E vs Angle at Target Exit");
    TH2D* te_h2Electron = CreateEnergyAngleHist(teElectronE, teElectronA, "h2ElectronEvsA", "Electron E vs Angle at Target Exit");
    TH2D* te_h2Positron = CreateEnergyAngleHist(tePositronE, tePositronA, "h2PositronEvsA", "Positron E vs Angle at Target Exit");
    te_h2Photon->Write();
    te_h2Neutron->Write();
    te_h2Electron->Write();
    te_h2Positron->Write();
    
    // Summary histograms
    rootFile->cd("TargetExit/Summary");
    TH1D* te_hCounts = CreateParticleCountHist(fTargetExitPhotons, fTargetExitNeutrons, 
                                                fTargetExitElectrons, fTargetExitPositrons,
                                                "hParticleCounts", "Particle Counts at Target Exit");
    TH1D* te_hEnergyBudget = CreateEnergyBudgetHist(fTargetExitPrimaryEnergy/GeV, 
                                                    (fTargetExitEnergy - fTargetExitPrimaryEnergy)/GeV,
                                                    "hEnergyBudget", "Energy Budget at Target Exit");
    te_hCounts->Write();
    te_hEnergyBudget->Write();
    
    // ========================================================
    // MAGNET ENTRANCE HISTOGRAMS
    // ========================================================
    rootFile->mkdir("MagnetEntrance");
    rootFile->mkdir("MagnetEntrance/Energy");
    rootFile->mkdir("MagnetEntrance/Angular");
    rootFile->mkdir("MagnetEntrance/Correlation");
    rootFile->mkdir("MagnetEntrance/Summary");
    
    rootFile->cd("MagnetEntrance/Energy");
    TH1D* me_hPhotonE = CreateEnergyHist(mePhotonE, "hPhotonEnergy", "Photon Energy at Magnet Entrance", kPhotonColor, 8.0);
    TH1D* me_hNeutronE = CreateEnergyHist(meNeutronE, "hNeutronEnergy", "Neutron Energy at Magnet Entrance", kNeutronColor, 100.0);
    TH1D* me_hElectronE = CreateEnergyHist(meElectronE, "hElectronEnergy", "Electron Energy at Magnet Entrance", kElectronColor, 8.0);
    TH1D* me_hPositronE = CreateEnergyHist(mePositronE, "hPositronEnergy", "Positron Energy at Magnet Entrance", kPositronColor, 8.0);
    me_hPhotonE->Write();
    me_hNeutronE->Write();
    me_hElectronE->Write();
    me_hPositronE->Write();
    
    TH1D* me_hPhotonE_norm = CreateNormalizedCopy(me_hPhotonE, "_norm");
    TH1D* me_hNeutronE_norm = CreateNormalizedCopy(me_hNeutronE, "_norm");
    TH1D* me_hElectronE_norm = CreateNormalizedCopy(me_hElectronE, "_norm");
    TH1D* me_hPositronE_norm = CreateNormalizedCopy(me_hPositronE, "_norm");
    me_hPhotonE_norm->Write();
    me_hNeutronE_norm->Write();
    me_hElectronE_norm->Write();
    me_hPositronE_norm->Write();
    
    rootFile->cd("MagnetEntrance/Angular");
    TH1D* me_hPhotonA = CreateAngleHist(mePhotonA, "hPhotonAngle", "Photon Angle at Magnet Entrance", kPhotonColor);
    TH1D* me_hNeutronA = CreateAngleHist(meNeutronA, "hNeutronAngle", "Neutron Angle at Magnet Entrance", kNeutronColor);
    TH1D* me_hElectronA = CreateAngleHist(meElectronA, "hElectronAngle", "Electron Angle at Magnet Entrance", kElectronColor);
    TH1D* me_hPositronA = CreateAngleHist(mePositronA, "hPositronAngle", "Positron Angle at Magnet Entrance", kPositronColor);
    me_hPhotonA->Write();
    me_hNeutronA->Write();
    me_hElectronA->Write();
    me_hPositronA->Write();
    
    TH1D* me_hPhotonA_norm = CreateNormalizedCopy(me_hPhotonA, "_norm");
    TH1D* me_hNeutronA_norm = CreateNormalizedCopy(me_hNeutronA, "_norm");
    TH1D* me_hElectronA_norm = CreateNormalizedCopy(me_hElectronA, "_norm");
    TH1D* me_hPositronA_norm = CreateNormalizedCopy(me_hPositronA, "_norm");
    me_hPhotonA_norm->Write();
    me_hNeutronA_norm->Write();
    me_hElectronA_norm->Write();
    me_hPositronA_norm->Write();
    
    rootFile->cd("MagnetEntrance/Correlation");
    TH2D* me_h2Photon = CreateEnergyAngleHist(mePhotonE, mePhotonA, "h2PhotonEvsA", "Photon E vs Angle at Magnet Entrance");
    TH2D* me_h2Neutron = CreateEnergyAngleHist(meNeutronE, meNeutronA, "h2NeutronEvsA", "Neutron E vs Angle at Magnet Entrance");
    TH2D* me_h2Electron = CreateEnergyAngleHist(meElectronE, meElectronA, "h2ElectronEvsA", "Electron E vs Angle at Magnet Entrance");
    TH2D* me_h2Positron = CreateEnergyAngleHist(mePositronE, mePositronA, "h2PositronEvsA", "Positron E vs Angle at Magnet Entrance");
    me_h2Photon->Write();
    me_h2Neutron->Write();
    me_h2Electron->Write();
    me_h2Positron->Write();
    
    rootFile->cd("MagnetEntrance/Summary");
    TH1D* me_hCounts = CreateParticleCountHist(fMagnetEntrancePhotons, fMagnetEntranceNeutrons,
                                                fMagnetEntranceElectrons, fMagnetEntrancePositrons,
                                                "hParticleCounts", "Particle Counts at Magnet Entrance");
    TH1D* me_hEnergyBudget = CreateEnergyBudgetHist(fMagnetEntrancePrimaryEnergy/GeV,
                                                    (fMagnetEntranceEnergy - fMagnetEntrancePrimaryEnergy)/GeV,
                                                    "hEnergyBudget", "Energy Budget at Magnet Entrance");
    me_hCounts->Write();
    me_hEnergyBudget->Write();
    
    // ========================================================
    // CALORIMETER ENTRANCE HISTOGRAMS
    // ========================================================
    rootFile->mkdir("CaloEntrance");
    rootFile->mkdir("CaloEntrance/Energy");
    rootFile->mkdir("CaloEntrance/Angular");
    rootFile->mkdir("CaloEntrance/Correlation");
    rootFile->mkdir("CaloEntrance/Summary");
    
    rootFile->cd("CaloEntrance/Energy");
    TH1D* ce_hPhotonE = CreateEnergyHist(cePhotonE, "hPhotonEnergy", "Photon Energy at Calo Entrance", kPhotonColor, 8.0);
    TH1D* ce_hNeutronE = CreateEnergyHist(ceNeutronE, "hNeutronEnergy", "Neutron Energy at Calo Entrance", kNeutronColor, 100.0);
    TH1D* ce_hElectronE = CreateEnergyHist(ceElectronE, "hElectronEnergy", "Electron Energy at Calo Entrance", kElectronColor, 8.0);
    TH1D* ce_hPositronE = CreateEnergyHist(cePositronE, "hPositronEnergy", "Positron Energy at Calo Entrance", kPositronColor, 8.0);
    ce_hPhotonE->Write();
    ce_hNeutronE->Write();
    ce_hElectronE->Write();
    ce_hPositronE->Write();
    
    TH1D* ce_hPhotonE_norm = CreateNormalizedCopy(ce_hPhotonE, "_norm");
    TH1D* ce_hNeutronE_norm = CreateNormalizedCopy(ce_hNeutronE, "_norm");
    TH1D* ce_hElectronE_norm = CreateNormalizedCopy(ce_hElectronE, "_norm");
    TH1D* ce_hPositronE_norm = CreateNormalizedCopy(ce_hPositronE, "_norm");
    ce_hPhotonE_norm->Write();
    ce_hNeutronE_norm->Write();
    ce_hElectronE_norm->Write();
    ce_hPositronE_norm->Write();
    
    rootFile->cd("CaloEntrance/Angular");
    TH1D* ce_hPhotonA = CreateAngleHist(cePhotonA, "hPhotonAngle", "Photon Angle at Calo Entrance", kPhotonColor);
    TH1D* ce_hNeutronA = CreateAngleHist(ceNeutronA, "hNeutronAngle", "Neutron Angle at Calo Entrance", kNeutronColor);
    TH1D* ce_hElectronA = CreateAngleHist(ceElectronA, "hElectronAngle", "Electron Angle at Calo Entrance", kElectronColor);
    TH1D* ce_hPositronA = CreateAngleHist(cePositronA, "hPositronAngle", "Positron Angle at Calo Entrance", kPositronColor);
    ce_hPhotonA->Write();
    ce_hNeutronA->Write();
    ce_hElectronA->Write();
    ce_hPositronA->Write();
    
    TH1D* ce_hPhotonA_norm = CreateNormalizedCopy(ce_hPhotonA, "_norm");
    TH1D* ce_hNeutronA_norm = CreateNormalizedCopy(ce_hNeutronA, "_norm");
    TH1D* ce_hElectronA_norm = CreateNormalizedCopy(ce_hElectronA, "_norm");
    TH1D* ce_hPositronA_norm = CreateNormalizedCopy(ce_hPositronA, "_norm");
    ce_hPhotonA_norm->Write();
    ce_hNeutronA_norm->Write();
    ce_hElectronA_norm->Write();
    ce_hPositronA_norm->Write();
    
    rootFile->cd("CaloEntrance/Correlation");
    TH2D* ce_h2Photon = CreateEnergyAngleHist(cePhotonE, cePhotonA, "h2PhotonEvsA", "Photon E vs Angle at Calo Entrance");
    TH2D* ce_h2Neutron = CreateEnergyAngleHist(ceNeutronE, ceNeutronA, "h2NeutronEvsA", "Neutron E vs Angle at Calo Entrance");
    TH2D* ce_h2Electron = CreateEnergyAngleHist(ceElectronE, ceElectronA, "h2ElectronEvsA", "Electron E vs Angle at Calo Entrance");
    TH2D* ce_h2Positron = CreateEnergyAngleHist(cePositronE, cePositronA, "h2PositronEvsA", "Positron E vs Angle at Calo Entrance");
    ce_h2Photon->Write();
    ce_h2Neutron->Write();
    ce_h2Electron->Write();
    ce_h2Positron->Write();
    
    rootFile->cd("CaloEntrance/Summary");
    TH1D* ce_hCounts = CreateParticleCountHist(fCaloEntrancePhotons, fCaloEntranceNeutrons,
                                                fCaloEntranceElectrons, fCaloEntrancePositrons,
                                                "hParticleCounts", "Particle Counts at Calo Entrance");
    TH1D* ce_hEnergyBudget = CreateEnergyBudgetHist(fCaloEntrancePrimaryEnergy/GeV,
                                                    (fCaloEntranceEnergy - fCaloEntrancePrimaryEnergy)/GeV,
                                                    "hEnergyBudget", "Energy Budget at Calo Entrance");
    ce_hCounts->Write();
    ce_hEnergyBudget->Write();
    
    // ========================================================
    // CALORIMETER EXIT HISTOGRAMS
    // ========================================================
    rootFile->mkdir("CaloExit");
    rootFile->mkdir("CaloExit/Energy");
    rootFile->mkdir("CaloExit/Angular");
    rootFile->mkdir("CaloExit/Correlation");
    rootFile->mkdir("CaloExit/Summary");
    
    rootFile->cd("CaloExit/Energy");
    TH1D* cx_hPhotonE = CreateEnergyHist(cxPhotonE, "hPhotonEnergy", "Photon Energy at Calo Exit", kPhotonColor, 8.0);
    TH1D* cx_hNeutronE = CreateEnergyHist(cxNeutronE, "hNeutronEnergy", "Neutron Energy at Calo Exit", kNeutronColor, 100.0);
    TH1D* cx_hElectronE = CreateEnergyHist(cxElectronE, "hElectronEnergy", "Electron Energy at Calo Exit", kElectronColor, 8.0);
    TH1D* cx_hPositronE = CreateEnergyHist(cxPositronE, "hPositronEnergy", "Positron Energy at Calo Exit", kPositronColor, 8.0);
    cx_hPhotonE->Write();
    cx_hNeutronE->Write();
    cx_hElectronE->Write();
    cx_hPositronE->Write();
    
    TH1D* cx_hPhotonE_norm = CreateNormalizedCopy(cx_hPhotonE, "_norm");
    TH1D* cx_hNeutronE_norm = CreateNormalizedCopy(cx_hNeutronE, "_norm");
    TH1D* cx_hElectronE_norm = CreateNormalizedCopy(cx_hElectronE, "_norm");
    TH1D* cx_hPositronE_norm = CreateNormalizedCopy(cx_hPositronE, "_norm");
    cx_hPhotonE_norm->Write();
    cx_hNeutronE_norm->Write();
    cx_hElectronE_norm->Write();
    cx_hPositronE_norm->Write();
    
    rootFile->cd("CaloExit/Angular");
    TH1D* cx_hPhotonA = CreateAngleHist(cxPhotonA, "hPhotonAngle", "Photon Angle at Calo Exit", kPhotonColor);
    TH1D* cx_hNeutronA = CreateAngleHist(cxNeutronA, "hNeutronAngle", "Neutron Angle at Calo Exit", kNeutronColor);
    TH1D* cx_hElectronA = CreateAngleHist(cxElectronA, "hElectronAngle", "Electron Angle at Calo Exit", kElectronColor);
    TH1D* cx_hPositronA = CreateAngleHist(cxPositronA, "hPositronAngle", "Positron Angle at Calo Exit", kPositronColor);
    cx_hPhotonA->Write();
    cx_hNeutronA->Write();
    cx_hElectronA->Write();
    cx_hPositronA->Write();
    
    TH1D* cx_hPhotonA_norm = CreateNormalizedCopy(cx_hPhotonA, "_norm");
    TH1D* cx_hNeutronA_norm = CreateNormalizedCopy(cx_hNeutronA, "_norm");
    TH1D* cx_hElectronA_norm = CreateNormalizedCopy(cx_hElectronA, "_norm");
    TH1D* cx_hPositronA_norm = CreateNormalizedCopy(cx_hPositronA, "_norm");
    cx_hPhotonA_norm->Write();
    cx_hNeutronA_norm->Write();
    cx_hElectronA_norm->Write();
    cx_hPositronA_norm->Write();
    
    rootFile->cd("CaloExit/Correlation");
    TH2D* cx_h2Photon = CreateEnergyAngleHist(cxPhotonE, cxPhotonA, "h2PhotonEvsA", "Photon E vs Angle at Calo Exit");
    TH2D* cx_h2Neutron = CreateEnergyAngleHist(cxNeutronE, cxNeutronA, "h2NeutronEvsA", "Neutron E vs Angle at Calo Exit");
    TH2D* cx_h2Electron = CreateEnergyAngleHist(cxElectronE, cxElectronA, "h2ElectronEvsA", "Electron E vs Angle at Calo Exit");
    TH2D* cx_h2Positron = CreateEnergyAngleHist(cxPositronE, cxPositronA, "h2PositronEvsA", "Positron E vs Angle at Calo Exit");
    cx_h2Photon->Write();
    cx_h2Neutron->Write();
    cx_h2Electron->Write();
    cx_h2Positron->Write();
    
    rootFile->cd("CaloExit/Summary");
    TH1D* cx_hCounts = CreateParticleCountHist(fCaloExitPhotons, fCaloExitNeutrons,
                                                fCaloExitElectrons, fCaloExitPositrons,
                                                "hParticleCounts", "Particle Counts at Calo Exit");
    TH1D* cx_hEnergyBudget = CreateEnergyBudgetHist(fCaloExitPrimaryEnergy/GeV,
                                                    (fCaloExitEnergy - fCaloExitPrimaryEnergy)/GeV,
                                                    "hEnergyBudget", "Energy Budget at Calo Exit");
    cx_hCounts->Write();
    cx_hEnergyBudget->Write();
    
    // ========================================================
    // COMPARISON / EVOLUTION HISTOGRAMS
    // ========================================================
    rootFile->mkdir("Comparison");
    rootFile->mkdir("Comparison/Evolution");
    rootFile->mkdir("Comparison/Spectra");
    rootFile->mkdir("Statistics");
    
    rootFile->cd("Comparison/Evolution");
    
    // Particle count evolution
    TH1D* hPhotonEvolution = new TH1D("hPhotonEvolution", "Photon Count Evolution;Location;Count", 4, 0, 4);
    hPhotonEvolution->SetBinContent(1, fTargetExitPhotons);
    hPhotonEvolution->SetBinContent(2, fMagnetEntrancePhotons);
    hPhotonEvolution->SetBinContent(3, fCaloEntrancePhotons);
    hPhotonEvolution->SetBinContent(4, fCaloExitPhotons);
    hPhotonEvolution->GetXaxis()->SetBinLabel(1, "Target Exit");
    hPhotonEvolution->GetXaxis()->SetBinLabel(2, "Magnet Ent.");
    hPhotonEvolution->GetXaxis()->SetBinLabel(3, "Calo Ent.");
    hPhotonEvolution->GetXaxis()->SetBinLabel(4, "Calo Exit");
    StyleHist1D(hPhotonEvolution, kPhotonColor, "", "Count");
    hPhotonEvolution->SetStats(0);
    hPhotonEvolution->Write();
    
    TH1D* hNeutronEvolution = new TH1D("hNeutronEvolution", "Neutron Count Evolution;Location;Count", 4, 0, 4);
    hNeutronEvolution->SetBinContent(1, fTargetExitNeutrons);
    hNeutronEvolution->SetBinContent(2, fMagnetEntranceNeutrons);
    hNeutronEvolution->SetBinContent(3, fCaloEntranceNeutrons);
    hNeutronEvolution->SetBinContent(4, fCaloExitNeutrons);
    hNeutronEvolution->GetXaxis()->SetBinLabel(1, "Target Exit");
    hNeutronEvolution->GetXaxis()->SetBinLabel(2, "Magnet Ent.");
    hNeutronEvolution->GetXaxis()->SetBinLabel(3, "Calo Ent.");
    hNeutronEvolution->GetXaxis()->SetBinLabel(4, "Calo Exit");
    StyleHist1D(hNeutronEvolution, kNeutronColor, "", "Count");
    hNeutronEvolution->SetStats(0);
    hNeutronEvolution->Write();
    
    TH1D* hElectronEvolution = new TH1D("hElectronEvolution", "Electron Count Evolution;Location;Count", 4, 0, 4);
    hElectronEvolution->SetBinContent(1, fTargetExitElectrons);
    hElectronEvolution->SetBinContent(2, fMagnetEntranceElectrons);
    hElectronEvolution->SetBinContent(3, fCaloEntranceElectrons);
    hElectronEvolution->SetBinContent(4, fCaloExitElectrons);
    hElectronEvolution->GetXaxis()->SetBinLabel(1, "Target Exit");
    hElectronEvolution->GetXaxis()->SetBinLabel(2, "Magnet Ent.");
    hElectronEvolution->GetXaxis()->SetBinLabel(3, "Calo Ent.");
    hElectronEvolution->GetXaxis()->SetBinLabel(4, "Calo Exit");
    StyleHist1D(hElectronEvolution, kElectronColor, "", "Count");
    hElectronEvolution->SetStats(0);
    hElectronEvolution->Write();
    
    TH1D* hPositronEvolution = new TH1D("hPositronEvolution", "Positron Count Evolution;Location;Count", 4, 0, 4);
    hPositronEvolution->SetBinContent(1, fTargetExitPositrons);
    hPositronEvolution->SetBinContent(2, fMagnetEntrancePositrons);
    hPositronEvolution->SetBinContent(3, fCaloEntrancePositrons);
    hPositronEvolution->SetBinContent(4, fCaloExitPositrons);
    hPositronEvolution->GetXaxis()->SetBinLabel(1, "Target Exit");
    hPositronEvolution->GetXaxis()->SetBinLabel(2, "Magnet Ent.");
    hPositronEvolution->GetXaxis()->SetBinLabel(3, "Calo Ent.");
    hPositronEvolution->GetXaxis()->SetBinLabel(4, "Calo Exit");
    StyleHist1D(hPositronEvolution, kPositronColor, "", "Count");
    hPositronEvolution->SetStats(0);
    hPositronEvolution->Write();
    
    // Energy evolution
    TH1D* hTotalEnergyEvolution = new TH1D("hTotalEnergyEvolution", "Total Energy Evolution;Location;Energy [GeV]", 4, 0, 4);
    hTotalEnergyEvolution->SetBinContent(1, fTargetExitEnergy/GeV);
    hTotalEnergyEvolution->SetBinContent(2, fMagnetEntranceEnergy/GeV);
    hTotalEnergyEvolution->SetBinContent(3, fCaloEntranceEnergy/GeV);
    hTotalEnergyEvolution->SetBinContent(4, fCaloExitEnergy/GeV);
    hTotalEnergyEvolution->GetXaxis()->SetBinLabel(1, "Target Exit");
    hTotalEnergyEvolution->GetXaxis()->SetBinLabel(2, "Magnet Ent.");
    hTotalEnergyEvolution->GetXaxis()->SetBinLabel(3, "Calo Ent.");
    hTotalEnergyEvolution->GetXaxis()->SetBinLabel(4, "Calo Exit");
    hTotalEnergyEvolution->SetFillColor(kOrange);
    hTotalEnergyEvolution->SetLineColor(kOrange+2);
    hTotalEnergyEvolution->SetLineWidth(2);
    hTotalEnergyEvolution->SetStats(0);
    hTotalEnergyEvolution->Write();
    
    TH1D* hPrimaryEnergyEvolution = new TH1D("hPrimaryEnergyEvolution", "Primary Energy Evolution;Location;Energy [GeV]", 4, 0, 4);
    hPrimaryEnergyEvolution->SetBinContent(1, fTargetExitPrimaryEnergy/GeV);
    hPrimaryEnergyEvolution->SetBinContent(2, fMagnetEntrancePrimaryEnergy/GeV);
    hPrimaryEnergyEvolution->SetBinContent(3, fCaloEntrancePrimaryEnergy/GeV);
    hPrimaryEnergyEvolution->SetBinContent(4, fCaloExitPrimaryEnergy/GeV);
    hPrimaryEnergyEvolution->GetXaxis()->SetBinLabel(1, "Target Exit");
    hPrimaryEnergyEvolution->GetXaxis()->SetBinLabel(2, "Magnet Ent.");
    hPrimaryEnergyEvolution->GetXaxis()->SetBinLabel(3, "Calo Ent.");
    hPrimaryEnergyEvolution->GetXaxis()->SetBinLabel(4, "Calo Exit");
    hPrimaryEnergyEvolution->SetFillColor(kAzure+1);
    hPrimaryEnergyEvolution->SetLineColor(kAzure+2);
    hPrimaryEnergyEvolution->SetLineWidth(2);
    hPrimaryEnergyEvolution->SetStats(0);
    hPrimaryEnergyEvolution->Write();
    
    // Statistics - Mean energy evolution with error bars
    rootFile->cd("Statistics");
    
    double meanPhotonE[4], rmsPhotonE[4];
    double meanNeutronE[4], rmsNeutronE[4];
    double meanElectronE[4], rmsElectronE[4];
    double meanPositronE[4], rmsPositronE[4];
    double xPos[4] = {1, 2, 3, 4};
    double xErr[4] = {0, 0, 0, 0};
    
    CalculateStats(tePhotonE, meanPhotonE[0], rmsPhotonE[0]);
    CalculateStats(mePhotonE, meanPhotonE[1], rmsPhotonE[1]);
    CalculateStats(cePhotonE, meanPhotonE[2], rmsPhotonE[2]);
    CalculateStats(cxPhotonE, meanPhotonE[3], rmsPhotonE[3]);
    
    CalculateStats(teNeutronE, meanNeutronE[0], rmsNeutronE[0]);
    CalculateStats(meNeutronE, meanNeutronE[1], rmsNeutronE[1]);
    CalculateStats(ceNeutronE, meanNeutronE[2], rmsNeutronE[2]);
    CalculateStats(cxNeutronE, meanNeutronE[3], rmsNeutronE[3]);
    
    CalculateStats(teElectronE, meanElectronE[0], rmsElectronE[0]);
    CalculateStats(meElectronE, meanElectronE[1], rmsElectronE[1]);
    CalculateStats(ceElectronE, meanElectronE[2], rmsElectronE[2]);
    CalculateStats(cxElectronE, meanElectronE[3], rmsElectronE[3]);
    
    CalculateStats(tePositronE, meanPositronE[0], rmsPositronE[0]);
    CalculateStats(mePositronE, meanPositronE[1], rmsPositronE[1]);
    CalculateStats(cePositronE, meanPositronE[2], rmsPositronE[2]);
    CalculateStats(cxPositronE, meanPositronE[3], rmsPositronE[3]);
    
    TGraphErrors* grPhotonMean = new TGraphErrors(4, xPos, meanPhotonE, xErr, rmsPhotonE);
    grPhotonMean->SetName("grPhotonMeanEnergy");
    grPhotonMean->SetTitle("Photon Mean Energy Evolution;Location;Mean Energy [GeV]");
    grPhotonMean->SetMarkerStyle(20);
    grPhotonMean->SetMarkerColor(kPhotonColor);
    grPhotonMean->SetLineColor(kPhotonColor);
    grPhotonMean->SetLineWidth(2);
    grPhotonMean->Write();
    
    TGraphErrors* grNeutronMean = new TGraphErrors(4, xPos, meanNeutronE, xErr, rmsNeutronE);
    grNeutronMean->SetName("grNeutronMeanEnergy");
    grNeutronMean->SetTitle("Neutron Mean Energy Evolution;Location;Mean Energy [GeV]");
    grNeutronMean->SetMarkerStyle(21);
    grNeutronMean->SetMarkerColor(kNeutronColor);
    grNeutronMean->SetLineColor(kNeutronColor);
    grNeutronMean->SetLineWidth(2);
    grNeutronMean->Write();
    
    TGraphErrors* grElectronMean = new TGraphErrors(4, xPos, meanElectronE, xErr, rmsElectronE);
    grElectronMean->SetName("grElectronMeanEnergy");
    grElectronMean->SetTitle("Electron Mean Energy Evolution;Location;Mean Energy [GeV]");
    grElectronMean->SetMarkerStyle(22);
    grElectronMean->SetMarkerColor(kElectronColor);
    grElectronMean->SetLineColor(kElectronColor);
    grElectronMean->SetLineWidth(2);
    grElectronMean->Write();
    
    TGraphErrors* grPositronMean = new TGraphErrors(4, xPos, meanPositronE, xErr, rmsPositronE);
    grPositronMean->SetName("grPositronMeanEnergy");
    grPositronMean->SetTitle("Positron Mean Energy Evolution;Location;Mean Energy [GeV]");
    grPositronMean->SetMarkerStyle(23);
    grPositronMean->SetMarkerColor(kPositronColor);
    grPositronMean->SetLineColor(kPositronColor);
    grPositronMean->SetLineWidth(2);
    grPositronMean->Write();
    
    G4cout << "Histograms written to ROOT file" << G4endl;
    
    // ========================================================
    // GENERATE PNG PLOTS
    // ========================================================
    G4cout << "Generating individual PNG plots..." << G4endl;
    
    // Target Exit PNGs
    TCanvas* c1 = new TCanvas("c1", "", 800, 600);
    
    // Energy overlay - Target Exit
    TCanvas* cTeEnergy = CreateOverlayCanvas(te_hPhotonE, te_hNeutronE, te_hElectronE, te_hPositronE,
                                             "cTeEnergy", "Energy Spectra at Target Exit", true);
    SaveCanvas(cTeEnergy, "plots/png/targetexit/energy_overlay");
    delete cTeEnergy;
    
    // Angular overlay - Target Exit
    TCanvas* cTeAngle = CreateOverlayCanvas(te_hPhotonA, te_hNeutronA, te_hElectronA, te_hPositronA,
                                            "cTeAngle", "Angular Distributions at Target Exit", false);
    SaveCanvas(cTeAngle, "plots/png/targetexit/angle_overlay");
    delete cTeAngle;
    
    // Individual energy plots - Target Exit
    c1->SetLogy();
    te_hPhotonE->Draw("HIST"); SaveCanvas(c1, "plots/png/targetexit/energy_photon");
    te_hNeutronE->Draw("HIST"); SaveCanvas(c1, "plots/png/targetexit/energy_neutron");
    te_hElectronE->Draw("HIST"); SaveCanvas(c1, "plots/png/targetexit/energy_electron");
    te_hPositronE->Draw("HIST"); SaveCanvas(c1, "plots/png/targetexit/energy_positron");
    
    // Individual angle plots - Target Exit
    c1->SetLogy(0);
    te_hPhotonA->Draw("HIST"); SaveCanvas(c1, "plots/png/targetexit/angle_photon");
    te_hNeutronA->Draw("HIST"); SaveCanvas(c1, "plots/png/targetexit/angle_neutron");
    te_hElectronA->Draw("HIST"); SaveCanvas(c1, "plots/png/targetexit/angle_electron");
    te_hPositronA->Draw("HIST"); SaveCanvas(c1, "plots/png/targetexit/angle_positron");
    
    // 2D correlation plots - Target Exit
    c1->SetRightMargin(0.15);
    te_h2Photon->Draw("COLZ"); SaveCanvas(c1, "plots/png/targetexit/corr_photon");
    te_h2Neutron->Draw("COLZ"); SaveCanvas(c1, "plots/png/targetexit/corr_neutron");
    te_h2Electron->Draw("COLZ"); SaveCanvas(c1, "plots/png/targetexit/corr_electron");
    te_h2Positron->Draw("COLZ"); SaveCanvas(c1, "plots/png/targetexit/corr_positron");
    c1->SetRightMargin(0.05);
    
    // Summary plots - Target Exit
    te_hCounts->Draw("BAR"); SaveCanvas(c1, "plots/png/targetexit/particle_counts");
    te_hEnergyBudget->Draw("BAR"); SaveCanvas(c1, "plots/png/targetexit/energy_budget");
    
    // Magnet Entrance PNGs
    TCanvas* cMeEnergy = CreateOverlayCanvas(me_hPhotonE, me_hNeutronE, me_hElectronE, me_hPositronE,
                                             "cMeEnergy", "Energy Spectra at Magnet Entrance", true);
    SaveCanvas(cMeEnergy, "plots/png/magnetentrance/energy_overlay");
    delete cMeEnergy;
    
    TCanvas* cMeAngle = CreateOverlayCanvas(me_hPhotonA, me_hNeutronA, me_hElectronA, me_hPositronA,
                                            "cMeAngle", "Angular Distributions at Magnet Entrance", false);
    SaveCanvas(cMeAngle, "plots/png/magnetentrance/angle_overlay");
    delete cMeAngle;
    
    c1->SetLogy();
    me_hPhotonE->Draw("HIST"); SaveCanvas(c1, "plots/png/magnetentrance/energy_photon");
    me_hNeutronE->Draw("HIST"); SaveCanvas(c1, "plots/png/magnetentrance/energy_neutron");
    me_hElectronE->Draw("HIST"); SaveCanvas(c1, "plots/png/magnetentrance/energy_electron");
    me_hPositronE->Draw("HIST"); SaveCanvas(c1, "plots/png/magnetentrance/energy_positron");
    
    c1->SetLogy(0);
    me_hPhotonA->Draw("HIST"); SaveCanvas(c1, "plots/png/magnetentrance/angle_photon");
    me_hNeutronA->Draw("HIST"); SaveCanvas(c1, "plots/png/magnetentrance/angle_neutron");
    me_hElectronA->Draw("HIST"); SaveCanvas(c1, "plots/png/magnetentrance/angle_electron");
    me_hPositronA->Draw("HIST"); SaveCanvas(c1, "plots/png/magnetentrance/angle_positron");
    
    c1->SetRightMargin(0.15);
    me_h2Photon->Draw("COLZ"); SaveCanvas(c1, "plots/png/magnetentrance/corr_photon");
    me_h2Neutron->Draw("COLZ"); SaveCanvas(c1, "plots/png/magnetentrance/corr_neutron");
    me_h2Electron->Draw("COLZ"); SaveCanvas(c1, "plots/png/magnetentrance/corr_electron");
    me_h2Positron->Draw("COLZ"); SaveCanvas(c1, "plots/png/magnetentrance/corr_positron");
    c1->SetRightMargin(0.05);
    
    me_hCounts->Draw("BAR"); SaveCanvas(c1, "plots/png/magnetentrance/particle_counts");
    me_hEnergyBudget->Draw("BAR"); SaveCanvas(c1, "plots/png/magnetentrance/energy_budget");
    
    // Calo Entrance PNGs
    TCanvas* cCeEnergy = CreateOverlayCanvas(ce_hPhotonE, ce_hNeutronE, ce_hElectronE, ce_hPositronE,
                                             "cCeEnergy", "Energy Spectra at Calo Entrance", true);
    SaveCanvas(cCeEnergy, "plots/png/caloentrance/energy_overlay");
    delete cCeEnergy;
    
    TCanvas* cCeAngle = CreateOverlayCanvas(ce_hPhotonA, ce_hNeutronA, ce_hElectronA, ce_hPositronA,
                                            "cCeAngle", "Angular Distributions at Calo Entrance", false);
    SaveCanvas(cCeAngle, "plots/png/caloentrance/angle_overlay");
    delete cCeAngle;
    
    c1->SetLogy();
    ce_hPhotonE->Draw("HIST"); SaveCanvas(c1, "plots/png/caloentrance/energy_photon");
    ce_hNeutronE->Draw("HIST"); SaveCanvas(c1, "plots/png/caloentrance/energy_neutron");
    ce_hElectronE->Draw("HIST"); SaveCanvas(c1, "plots/png/caloentrance/energy_electron");
    ce_hPositronE->Draw("HIST"); SaveCanvas(c1, "plots/png/caloentrance/energy_positron");
    
    c1->SetLogy(0);
    ce_hPhotonA->Draw("HIST"); SaveCanvas(c1, "plots/png/caloentrance/angle_photon");
    ce_hNeutronA->Draw("HIST"); SaveCanvas(c1, "plots/png/caloentrance/angle_neutron");
    ce_hElectronA->Draw("HIST"); SaveCanvas(c1, "plots/png/caloentrance/angle_electron");
    ce_hPositronA->Draw("HIST"); SaveCanvas(c1, "plots/png/caloentrance/angle_positron");
    
    c1->SetRightMargin(0.15);
    ce_h2Photon->Draw("COLZ"); SaveCanvas(c1, "plots/png/caloentrance/corr_photon");
    ce_h2Neutron->Draw("COLZ"); SaveCanvas(c1, "plots/png/caloentrance/corr_neutron");
    ce_h2Electron->Draw("COLZ"); SaveCanvas(c1, "plots/png/caloentrance/corr_electron");
    ce_h2Positron->Draw("COLZ"); SaveCanvas(c1, "plots/png/caloentrance/corr_positron");
    c1->SetRightMargin(0.05);
    
    ce_hCounts->Draw("BAR"); SaveCanvas(c1, "plots/png/caloentrance/particle_counts");
    ce_hEnergyBudget->Draw("BAR"); SaveCanvas(c1, "plots/png/caloentrance/energy_budget");
    
    // Calo Exit PNGs
    TCanvas* cCxEnergy = CreateOverlayCanvas(cx_hPhotonE, cx_hNeutronE, cx_hElectronE, cx_hPositronE,
                                             "cCxEnergy", "Energy Spectra at Calo Exit", true);
    SaveCanvas(cCxEnergy, "plots/png/caloexit/energy_overlay");
    delete cCxEnergy;
    
    TCanvas* cCxAngle = CreateOverlayCanvas(cx_hPhotonA, cx_hNeutronA, cx_hElectronA, cx_hPositronA,
                                            "cCxAngle", "Angular Distributions at Calo Exit", false);
    SaveCanvas(cCxAngle, "plots/png/caloexit/angle_overlay");
    delete cCxAngle;
    
    c1->SetLogy();
    cx_hPhotonE->Draw("HIST"); SaveCanvas(c1, "plots/png/caloexit/energy_photon");
    cx_hNeutronE->Draw("HIST"); SaveCanvas(c1, "plots/png/caloexit/energy_neutron");
    cx_hElectronE->Draw("HIST"); SaveCanvas(c1, "plots/png/caloexit/energy_electron");
    cx_hPositronE->Draw("HIST"); SaveCanvas(c1, "plots/png/caloexit/energy_positron");
    
    c1->SetLogy(0);
    cx_hPhotonA->Draw("HIST"); SaveCanvas(c1, "plots/png/caloexit/angle_photon");
    cx_hNeutronA->Draw("HIST"); SaveCanvas(c1, "plots/png/caloexit/angle_neutron");
    cx_hElectronA->Draw("HIST"); SaveCanvas(c1, "plots/png/caloexit/angle_electron");
    cx_hPositronA->Draw("HIST"); SaveCanvas(c1, "plots/png/caloexit/angle_positron");
    
    c1->SetRightMargin(0.15);
    cx_h2Photon->Draw("COLZ"); SaveCanvas(c1, "plots/png/caloexit/corr_photon");
    cx_h2Neutron->Draw("COLZ"); SaveCanvas(c1, "plots/png/caloexit/corr_neutron");
    cx_h2Electron->Draw("COLZ"); SaveCanvas(c1, "plots/png/caloexit/corr_electron");
    cx_h2Positron->Draw("COLZ"); SaveCanvas(c1, "plots/png/caloexit/corr_positron");
    c1->SetRightMargin(0.05);
    
    cx_hCounts->Draw("BAR"); SaveCanvas(c1, "plots/png/caloexit/particle_counts");
    cx_hEnergyBudget->Draw("BAR"); SaveCanvas(c1, "plots/png/caloexit/energy_budget");
    
    // Comparison/Evolution PNGs
    hPhotonEvolution->Draw("HIST"); SaveCanvas(c1, "plots/png/comparison/photon_evolution");
    hNeutronEvolution->Draw("HIST"); SaveCanvas(c1, "plots/png/comparison/neutron_evolution");
    hElectronEvolution->Draw("HIST"); SaveCanvas(c1, "plots/png/comparison/electron_evolution");
    hPositronEvolution->Draw("HIST"); SaveCanvas(c1, "plots/png/comparison/positron_evolution");
    hTotalEnergyEvolution->Draw("HIST"); SaveCanvas(c1, "plots/png/comparison/total_energy_evolution");
    hPrimaryEnergyEvolution->Draw("HIST"); SaveCanvas(c1, "plots/png/comparison/primary_energy_evolution");
    
    // Mean energy evolution
    grPhotonMean->Draw("APE"); SaveCanvas(c1, "plots/png/statistics/photon_mean_energy");
    grNeutronMean->Draw("APE"); SaveCanvas(c1, "plots/png/statistics/neutron_mean_energy");
    grElectronMean->Draw("APE"); SaveCanvas(c1, "plots/png/statistics/electron_mean_energy");
    grPositronMean->Draw("APE"); SaveCanvas(c1, "plots/png/statistics/positron_mean_energy");
    
    delete c1;
    
    G4cout << "Individual PNG plots saved to plots/png/" << G4endl;
    
    // ========================================================
    // MULTI-PANEL SUMMARY FIGURES
    // ========================================================
    G4cout << "Generating multi-panel summary figures..." << G4endl;
    
    // Full 4x4 Grid Summary
    TCanvas* cGrid = new TCanvas("cGrid", "DAMSA Analysis Summary", 2400, 2400);
    cGrid->Divide(4, 4, 0.002, 0.002);
    
    // Row 1: Target Exit
    cGrid->cd(1); gPad->SetLogy();
    te_hPhotonE->SetTitle("Target Exit - Energy");
    te_hPhotonE->Draw("HIST");
    te_hNeutronE->Draw("HIST SAME");
    te_hElectronE->Draw("HIST SAME");
    te_hPositronE->Draw("HIST SAME");
    TLegend* leg1 = new TLegend(0.65, 0.70, 0.94, 0.91);
    leg1->SetBorderSize(0); leg1->SetFillStyle(0); leg1->SetTextSize(0.035);
    leg1->AddEntry(te_hPhotonE, "Photons", "l");
    leg1->AddEntry(te_hNeutronE, "Neutrons", "l");
    leg1->AddEntry(te_hElectronE, "Electrons", "l");
    leg1->AddEntry(te_hPositronE, "Positrons", "l");
    leg1->Draw();
    
    cGrid->cd(2);
    te_hPhotonA->SetTitle("Target Exit - Angular");
    te_hPhotonA->Draw("HIST");
    te_hNeutronA->Draw("HIST SAME");
    te_hElectronA->Draw("HIST SAME");
    te_hPositronA->Draw("HIST SAME");
    
    cGrid->cd(3); gPad->SetRightMargin(0.15);
    te_h2Photon->SetTitle("Target Exit - E vs Angle (Photons)");
    te_h2Photon->Draw("COLZ");
    
    cGrid->cd(4);
    te_hCounts->SetTitle("Target Exit - Counts");
    te_hCounts->Draw("BAR");
    
    // Row 2: Magnet Entrance
    cGrid->cd(5); gPad->SetLogy();
    me_hPhotonE->SetTitle("Magnet Entrance - Energy");
    me_hPhotonE->Draw("HIST");
    me_hNeutronE->Draw("HIST SAME");
    me_hElectronE->Draw("HIST SAME");
    me_hPositronE->Draw("HIST SAME");
    
    cGrid->cd(6);
    me_hPhotonA->SetTitle("Magnet Entrance - Angular");
    me_hPhotonA->Draw("HIST");
    me_hNeutronA->Draw("HIST SAME");
    me_hElectronA->Draw("HIST SAME");
    me_hPositronA->Draw("HIST SAME");
    
    cGrid->cd(7); gPad->SetRightMargin(0.15);
    me_h2Photon->SetTitle("Magnet Entrance - E vs Angle (Photons)");
    me_h2Photon->Draw("COLZ");
    
    cGrid->cd(8);
    me_hCounts->SetTitle("Magnet Entrance - Counts");
    me_hCounts->Draw("BAR");
    
    // Row 3: Calo Entrance
    cGrid->cd(9); gPad->SetLogy();
    ce_hPhotonE->SetTitle("Calo Entrance - Energy");
    ce_hPhotonE->Draw("HIST");
    ce_hNeutronE->Draw("HIST SAME");
    ce_hElectronE->Draw("HIST SAME");
    ce_hPositronE->Draw("HIST SAME");
    
    cGrid->cd(10);
    ce_hPhotonA->SetTitle("Calo Entrance - Angular");
    ce_hPhotonA->Draw("HIST");
    ce_hNeutronA->Draw("HIST SAME");
    ce_hElectronA->Draw("HIST SAME");
    ce_hPositronA->Draw("HIST SAME");
    
    cGrid->cd(11); gPad->SetRightMargin(0.15);
    ce_h2Photon->SetTitle("Calo Entrance - E vs Angle (Photons)");
    ce_h2Photon->Draw("COLZ");
    
    cGrid->cd(12);
    ce_hCounts->SetTitle("Calo Entrance - Counts");
    ce_hCounts->Draw("BAR");
    
    // Row 4: Calo Exit
    cGrid->cd(13); gPad->SetLogy();
    cx_hPhotonE->SetTitle("Calo Exit - Energy");
    cx_hPhotonE->Draw("HIST");
    cx_hNeutronE->Draw("HIST SAME");
    cx_hElectronE->Draw("HIST SAME");
    cx_hPositronE->Draw("HIST SAME");
    
    cGrid->cd(14);
    cx_hPhotonA->SetTitle("Calo Exit - Angular");
    cx_hPhotonA->Draw("HIST");
    cx_hNeutronA->Draw("HIST SAME");
    cx_hElectronA->Draw("HIST SAME");
    cx_hPositronA->Draw("HIST SAME");
    
    cGrid->cd(15); gPad->SetRightMargin(0.15);
    cx_h2Photon->SetTitle("Calo Exit - E vs Angle (Photons)");
    cx_h2Photon->Draw("COLZ");
    
    cGrid->cd(16);
    cx_hCounts->SetTitle("Calo Exit - Counts");
    cx_hCounts->Draw("BAR");
    
    SaveCanvas(cGrid, "plots/summary/full_analysis_grid");
    rootFile->cd();
    cGrid->Write("cFullAnalysisGrid");
    delete cGrid;
    
    // 2x2 Energy Comparison Panel
    TCanvas* cEnergyComp = new TCanvas("cEnergyComp", "Energy Spectra Comparison", 1600, 1200);
    cEnergyComp->Divide(2, 2, 0.01, 0.01);
    
    cEnergyComp->cd(1); gPad->SetLogy();
    te_hPhotonE->SetTitle("Target Exit");
    te_hPhotonE->Draw("HIST");
    te_hNeutronE->Draw("HIST SAME");
    te_hElectronE->Draw("HIST SAME");
    te_hPositronE->Draw("HIST SAME");
    TLegend* legE = new TLegend(0.65, 0.70, 0.94, 0.91);
    legE->SetBorderSize(0); legE->SetFillStyle(0);
    legE->AddEntry(te_hPhotonE, "Photons", "l");
    legE->AddEntry(te_hNeutronE, "Neutrons", "l");
    legE->AddEntry(te_hElectronE, "Electrons", "l");
    legE->AddEntry(te_hPositronE, "Positrons", "l");
    legE->Draw();
    
    cEnergyComp->cd(2); gPad->SetLogy();
    me_hPhotonE->SetTitle("Magnet Entrance");
    me_hPhotonE->Draw("HIST");
    me_hNeutronE->Draw("HIST SAME");
    me_hElectronE->Draw("HIST SAME");
    me_hPositronE->Draw("HIST SAME");
    
    cEnergyComp->cd(3); gPad->SetLogy();
    ce_hPhotonE->SetTitle("Calo Entrance");
    ce_hPhotonE->Draw("HIST");
    ce_hNeutronE->Draw("HIST SAME");
    ce_hElectronE->Draw("HIST SAME");
    ce_hPositronE->Draw("HIST SAME");
    
    cEnergyComp->cd(4); gPad->SetLogy();
    cx_hPhotonE->SetTitle("Calo Exit");
    cx_hPhotonE->Draw("HIST");
    cx_hNeutronE->Draw("HIST SAME");
    cx_hElectronE->Draw("HIST SAME");
    cx_hPositronE->Draw("HIST SAME");
    
    SaveCanvas(cEnergyComp, "plots/summary/energy_comparison_4panel");
    rootFile->cd();
    cEnergyComp->Write("cEnergyComparison");
    delete cEnergyComp;
    
    // 2x2 Angular Comparison Panel
    TCanvas* cAngleComp = new TCanvas("cAngleComp", "Angular Distributions Comparison", 1600, 1200);
    cAngleComp->Divide(2, 2, 0.01, 0.01);
    
    cAngleComp->cd(1);
    te_hPhotonA->SetTitle("Target Exit");
    te_hPhotonA->Draw("HIST");
    te_hNeutronA->Draw("HIST SAME");
    te_hElectronA->Draw("HIST SAME");
    te_hPositronA->Draw("HIST SAME");
    TLegend* legA = new TLegend(0.65, 0.70, 0.94, 0.91);
    legA->SetBorderSize(0); legA->SetFillStyle(0);
    legA->AddEntry(te_hPhotonA, "Photons", "l");
    legA->AddEntry(te_hNeutronA, "Neutrons", "l");
    legA->AddEntry(te_hElectronA, "Electrons", "l");
    legA->AddEntry(te_hPositronA, "Positrons", "l");
    legA->Draw();
    
    cAngleComp->cd(2);
    me_hPhotonA->SetTitle("Magnet Entrance");
    me_hPhotonA->Draw("HIST");
    me_hNeutronA->Draw("HIST SAME");
    me_hElectronA->Draw("HIST SAME");
    me_hPositronA->Draw("HIST SAME");
    
    cAngleComp->cd(3);
    ce_hPhotonA->SetTitle("Calo Entrance");
    ce_hPhotonA->Draw("HIST");
    ce_hNeutronA->Draw("HIST SAME");
    ce_hElectronA->Draw("HIST SAME");
    ce_hPositronA->Draw("HIST SAME");
    
    cAngleComp->cd(4);
    cx_hPhotonA->SetTitle("Calo Exit");
    cx_hPhotonA->Draw("HIST");
    cx_hNeutronA->Draw("HIST SAME");
    cx_hElectronA->Draw("HIST SAME");
    cx_hPositronA->Draw("HIST SAME");
    
    SaveCanvas(cAngleComp, "plots/summary/angular_comparison_4panel");
    rootFile->cd();
    cAngleComp->Write("cAngularComparison");
    delete cAngleComp;
    
    // Evolution Summary (3 panels stacked)
    TCanvas* cEvolution = new TCanvas("cEvolution", "Particle Evolution Analysis", 800, 1800);
    cEvolution->Divide(1, 3, 0.01, 0.01);
    
    cEvolution->cd(1);
    double maxCount = 0;
    if(hPhotonEvolution->GetMaximum() > maxCount) maxCount = hPhotonEvolution->GetMaximum();
    if(hNeutronEvolution->GetMaximum() > maxCount) maxCount = hNeutronEvolution->GetMaximum();
    if(hElectronEvolution->GetMaximum() > maxCount) maxCount = hElectronEvolution->GetMaximum();
    if(hPositronEvolution->GetMaximum() > maxCount) maxCount = hPositronEvolution->GetMaximum();
    hPhotonEvolution->SetMaximum(maxCount * 1.2);
    hPhotonEvolution->SetTitle("Particle Count Evolution Through Detector");
    hPhotonEvolution->Draw("HIST");
    hNeutronEvolution->Draw("HIST SAME");
    hElectronEvolution->Draw("HIST SAME");
    hPositronEvolution->Draw("HIST SAME");
    TLegend* legEvol = new TLegend(0.70, 0.70, 0.94, 0.91);
    legEvol->SetBorderSize(0); legEvol->SetFillStyle(0);
    legEvol->AddEntry(hPhotonEvolution, "Photons", "l");
    legEvol->AddEntry(hNeutronEvolution, "Neutrons", "l");
    legEvol->AddEntry(hElectronEvolution, "Electrons", "l");
    legEvol->AddEntry(hPositronEvolution, "Positrons", "l");
    legEvol->Draw();
    
    cEvolution->cd(2);
    hTotalEnergyEvolution->SetTitle("Energy Evolution Through Detector");
    hTotalEnergyEvolution->Draw("HIST");
    hPrimaryEnergyEvolution->Draw("HIST SAME");
    TLegend* legEnergy = new TLegend(0.70, 0.75, 0.94, 0.91);
    legEnergy->SetBorderSize(0); legEnergy->SetFillStyle(0);
    legEnergy->AddEntry(hTotalEnergyEvolution, "Total Energy", "f");
    legEnergy->AddEntry(hPrimaryEnergyEvolution, "Primary Energy", "f");
    legEnergy->Draw();
    
    cEvolution->cd(3);
    // Combined mean energy evolution
    double maxMean = 0;
    for(int i = 0; i < 4; i++) {
        if(meanPhotonE[i] + rmsPhotonE[i] > maxMean) maxMean = meanPhotonE[i] + rmsPhotonE[i];
        if(meanNeutronE[i] + rmsNeutronE[i] > maxMean) maxMean = meanNeutronE[i] + rmsNeutronE[i];
    }
    grPhotonMean->SetTitle("Mean Energy Evolution (with RMS)");
    grPhotonMean->GetYaxis()->SetRangeUser(0, maxMean * 1.3);
    grPhotonMean->GetXaxis()->SetLimits(0.5, 4.5);
    grPhotonMean->Draw("APE");
    grNeutronMean->Draw("PE SAME");
    grElectronMean->Draw("PE SAME");
    grPositronMean->Draw("PE SAME");
    TLegend* legMean = new TLegend(0.70, 0.70, 0.94, 0.91);
    legMean->SetBorderSize(0); legMean->SetFillStyle(0);
    legMean->AddEntry(grPhotonMean, "Photons", "lep");
    legMean->AddEntry(grNeutronMean, "Neutrons", "lep");
    legMean->AddEntry(grElectronMean, "Electrons", "lep");
    legMean->AddEntry(grPositronMean, "Positrons", "lep");
    legMean->Draw();
    
    SaveCanvas(cEvolution, "plots/summary/evolution_analysis");
    rootFile->cd();
    cEvolution->Write("cEvolutionAnalysis");
    delete cEvolution;
    
    // Clean up all histograms
    delete te_hPhotonE; delete te_hNeutronE; delete te_hElectronE; delete te_hPositronE;
    delete te_hPhotonA; delete te_hNeutronA; delete te_hElectronA; delete te_hPositronA;
    delete te_hPhotonE_norm; delete te_hNeutronE_norm; delete te_hElectronE_norm; delete te_hPositronE_norm;
    delete te_hPhotonA_norm; delete te_hNeutronA_norm; delete te_hElectronA_norm; delete te_hPositronA_norm;
    delete te_h2Photon; delete te_h2Neutron; delete te_h2Electron; delete te_h2Positron;
    delete te_hCounts; delete te_hEnergyBudget;
    
    delete me_hPhotonE; delete me_hNeutronE; delete me_hElectronE; delete me_hPositronE;
    delete me_hPhotonA; delete me_hNeutronA; delete me_hElectronA; delete me_hPositronA;
    delete me_hPhotonE_norm; delete me_hNeutronE_norm; delete me_hElectronE_norm; delete me_hPositronE_norm;
    delete me_hPhotonA_norm; delete me_hNeutronA_norm; delete me_hElectronA_norm; delete me_hPositronA_norm;
    delete me_h2Photon; delete me_h2Neutron; delete me_h2Electron; delete me_h2Positron;
    delete me_hCounts; delete me_hEnergyBudget;
    
    delete ce_hPhotonE; delete ce_hNeutronE; delete ce_hElectronE; delete ce_hPositronE;
    delete ce_hPhotonA; delete ce_hNeutronA; delete ce_hElectronA; delete ce_hPositronA;
    delete ce_hPhotonE_norm; delete ce_hNeutronE_norm; delete ce_hElectronE_norm; delete ce_hPositronE_norm;
    delete ce_hPhotonA_norm; delete ce_hNeutronA_norm; delete ce_hElectronA_norm; delete ce_hPositronA_norm;
    delete ce_h2Photon; delete ce_h2Neutron; delete ce_h2Electron; delete ce_h2Positron;
    delete ce_hCounts; delete ce_hEnergyBudget;
    
    delete cx_hPhotonE; delete cx_hNeutronE; delete cx_hElectronE; delete cx_hPositronE;
    delete cx_hPhotonA; delete cx_hNeutronA; delete cx_hElectronA; delete cx_hPositronA;
    delete cx_hPhotonE_norm; delete cx_hNeutronE_norm; delete cx_hElectronE_norm; delete cx_hPositronE_norm;
    delete cx_hPhotonA_norm; delete cx_hNeutronA_norm; delete cx_hElectronA_norm; delete cx_hPositronA_norm;
    delete cx_h2Photon; delete cx_h2Neutron; delete cx_h2Electron; delete cx_h2Positron;
    delete cx_hCounts; delete cx_hEnergyBudget;
    
    delete hPhotonEvolution; delete hNeutronEvolution; delete hElectronEvolution; delete hPositronEvolution;
    delete hTotalEnergyEvolution; delete hPrimaryEnergyEvolution;
    delete grPhotonMean; delete grNeutronMean; delete grElectronMean; delete grPositronMean;
    
    rootFile->Close();
    delete rootFile;
    
    G4cout << "\n=== Plot Generation Complete ===" << G4endl;
    G4cout << "ROOT file: plots/damsa_analysis.root" << G4endl;
    G4cout << "PNG plots: plots/png/ (subdirectories by location)" << G4endl;
    G4cout << "Summary figures: plots/summary/" << G4endl;
    G4cout << "========================================\n" << G4endl;
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
