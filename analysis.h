#ifndef ANALYSIS_H
#define ANALYSIS_H

#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sys/stat.h>
#include "TFile.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TStyle.h"

class DamsaAnalysis
{
public:
    static DamsaAnalysis* Instance();
    
    void RecordParticle(const G4String& particleName, G4double energy, 
                       const G4String& location, G4double angle);
    void PrintSummary();
    void SaveToFile(const G4String& filename);
    void WriteROOTHistograms(const G4String& filename);
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
    
    // Neutron histogram storage
    static const int kMaxNeutrons = 1000;
    G4double fDetectorNeutronEnergies[kMaxNeutrons];
    G4double fDetectorNeutronAngles[kMaxNeutrons];
    G4int fDetectorNeutronCount;
    
    // Electron histogram storage
    static const int kMaxElectrons = 1000;
    G4double fDetectorElectronEnergies[kMaxElectrons];
    G4double fDetectorElectronAngles[kMaxElectrons];
    G4int fDetectorElectronCount;
    
    // Positron histogram storage
    static const int kMaxPositrons = 1000;
    G4double fDetectorPositronEnergies[kMaxPositrons];
    G4double fDetectorPositronAngles[kMaxPositrons];
    G4int fDetectorPositronCount;
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
  fDetectorEnergy(0), fDetectorPhotonEnergyCount(0),
  fDetectorNeutronCount(0), fDetectorElectronCount(0),
  fDetectorPositronCount(0)
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
        if(particleName == "neutron") {
            fDetectorNeutrons++;
            if(fDetectorNeutronCount < kMaxNeutrons) {
                fDetectorNeutronEnergies[fDetectorNeutronCount] = energy;
                fDetectorNeutronAngles[fDetectorNeutronCount] = angle;
                fDetectorNeutronCount++;
            }
        }
        else if(particleName == "gamma") {
            fDetectorPhotons++;
            if(fDetectorPhotonEnergyCount < kMaxPhotons) {
                fDetectorPhotonEnergies[fDetectorPhotonEnergyCount] = energy;
                fDetectorPhotonAngles[fDetectorPhotonEnergyCount] = angle;
                fDetectorPhotonEnergyCount++;
            }
        }
        else if(particleName == "e-") {
            fDetectorElectrons++;
            if(fDetectorElectronCount < kMaxElectrons) {
                fDetectorElectronEnergies[fDetectorElectronCount] = energy;
                fDetectorElectronAngles[fDetectorElectronCount] = angle;
                fDetectorElectronCount++;
            }
        }
        else if(particleName == "e+") {
            fDetectorPositrons++;
            if(fDetectorPositronCount < kMaxPositrons) {
                fDetectorPositronEnergies[fDetectorPositronCount] = energy;
                fDetectorPositronAngles[fDetectorPositronCount] = angle;
                fDetectorPositronCount++;
            }
        }
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
    
    // Neutron energy and angular distribution
    if(fDetectorNeutronCount > 0) {
        G4cout << "\n=== Neutron Energy Spectrum at Detector Entrance ===" << G4endl;
        G4double neutronEnergyBins[] = {0.0, 0.1, 0.5, 1.0, 2.0, 5.0, 10.0, 50.0, 100.0};
        G4int nNeutronEnergyBins = 8;
        G4int neutronEnergyCounts[8] = {0, 0, 0, 0, 0, 0, 0, 0};
        
        for(G4int i = 0; i < fDetectorNeutronCount; i++) {
            G4double E = fDetectorNeutronEnergies[i] / GeV;
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
        
        G4cout << "\n=== Neutron Angular Distribution at Detector Entrance ===" << G4endl;
        G4double neutronAngleBins[] = {0.0, 5.0, 10.0, 20.0, 30.0, 45.0, 60.0, 90.0};
        G4int nNeutronAngleBins = 7;
        G4int neutronAngleCounts[7] = {0, 0, 0, 0, 0, 0, 0};
        
        for(G4int i = 0; i < fDetectorNeutronCount; i++) {
            G4double angleDeg = fDetectorNeutronAngles[i] * 180.0 / 3.14159265;
            for(G4int j = 0; j < nNeutronAngleBins; j++) {
                if(angleDeg >= neutronAngleBins[j] && angleDeg < neutronAngleBins[j+1]) {
                    neutronAngleCounts[j]++;
                    break;
                }
            }
        }
        
        G4cout << "  (Angle relative to beam axis)" << G4endl;
        for(G4int i = 0; i < nNeutronAngleBins; i++) {
            G4cout << "  [" << neutronAngleBins[i] << " - " << neutronAngleBins[i+1] 
                   << " degrees]: " << neutronAngleCounts[i] << " neutrons" << G4endl;
        }
    }
    
    // Electron energy and angular distribution
    if(fDetectorElectronCount > 0) {
        G4cout << "\n=== Electron Energy Spectrum at Detector Entrance ===" << G4endl;
        G4double electronEnergyBins[] = {0.0, 0.1, 0.2, 0.5, 1.0, 2.0, 4.0, 8.0};
        G4int nElectronEnergyBins = 7;
        G4int electronEnergyCounts[7] = {0, 0, 0, 0, 0, 0, 0};
        
        for(G4int i = 0; i < fDetectorElectronCount; i++) {
            G4double E = fDetectorElectronEnergies[i] / GeV;
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
        
        G4cout << "\n=== Electron Angular Distribution at Detector Entrance ===" << G4endl;
        G4double electronAngleBins[] = {0.0, 5.0, 10.0, 20.0, 30.0, 45.0, 60.0, 90.0};
        G4int nElectronAngleBins = 7;
        G4int electronAngleCounts[7] = {0, 0, 0, 0, 0, 0, 0};
        
        for(G4int i = 0; i < fDetectorElectronCount; i++) {
            G4double angleDeg = fDetectorElectronAngles[i] * 180.0 / 3.14159265;
            for(G4int j = 0; j < nElectronAngleBins; j++) {
                if(angleDeg >= electronAngleBins[j] && angleDeg < electronAngleBins[j+1]) {
                    electronAngleCounts[j]++;
                    break;
                }
            }
        }
        
        G4cout << "  (Angle relative to beam axis)" << G4endl;
        for(G4int i = 0; i < nElectronAngleBins; i++) {
            G4cout << "  [" << electronAngleBins[i] << " - " << electronAngleBins[i+1] 
                   << " degrees]: " << electronAngleCounts[i] << " electrons" << G4endl;
        }
    }
    
    // Positron energy and angular distribution
    if(fDetectorPositronCount > 0) {
        G4cout << "\n=== Positron Energy Spectrum at Detector Entrance ===" << G4endl;
        G4double positronEnergyBins[] = {0.0, 0.1, 0.2, 0.5, 1.0, 2.0, 4.0, 8.0};
        G4int nPositronEnergyBins = 7;
        G4int positronEnergyCounts[7] = {0, 0, 0, 0, 0, 0, 0};
        
        for(G4int i = 0; i < fDetectorPositronCount; i++) {
            G4double E = fDetectorPositronEnergies[i] / GeV;
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
        
        G4cout << "\n=== Positron Angular Distribution at Detector Entrance ===" << G4endl;
        G4double positronAngleBins[] = {0.0, 5.0, 10.0, 20.0, 30.0, 45.0, 60.0, 90.0};
        G4int nPositronAngleBins = 7;
        G4int positronAngleCounts[7] = {0, 0, 0, 0, 0, 0, 0};
        
        for(G4int i = 0; i < fDetectorPositronCount; i++) {
            G4double angleDeg = fDetectorPositronAngles[i] * 180.0 / 3.14159265;
            for(G4int j = 0; j < nPositronAngleBins; j++) {
                if(angleDeg >= positronAngleBins[j] && angleDeg < positronAngleBins[j+1]) {
                    positronAngleCounts[j]++;
                    break;
                }
            }
        }
        
        G4cout << "  (Angle relative to beam axis)" << G4endl;
        for(G4int i = 0; i < nPositronAngleBins; i++) {
            G4cout << "  [" << positronAngleBins[i] << " - " << positronAngleBins[i+1] 
                   << " degrees]: " << positronAngleCounts[i] << " positrons" << G4endl;
        }
    }
    
    G4cout << "=============================================\n" << G4endl;
}

void DamsaAnalysis::SaveToFile(const G4String& filename)
{
    // Create output directory if it doesn't exist
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
    
    outFile << "=== Particles EXITING Target ===" << std::endl;
    outFile << "Neutrons:     " << fTargetExitNeutrons << std::endl;
    outFile << "Photons:      " << fTargetExitPhotons << std::endl;
    outFile << "Electrons:    " << fTargetExitElectrons << std::endl;
    outFile << "Positrons:    " << fTargetExitPositrons << std::endl;
    outFile << "Total Energy: " << std::fixed << std::setprecision(6) 
            << fTargetExitEnergy/GeV << " GeV" << std::endl << std::endl;
    
    outFile << "=== Particles ENTERING Detector ===" << std::endl;
    outFile << "Neutrons:     " << fDetectorNeutrons << std::endl;
    outFile << "Photons:      " << fDetectorPhotons << std::endl;
    outFile << "Electrons:    " << fDetectorElectrons << std::endl;
    outFile << "Positrons:    " << fDetectorPositrons << std::endl;
    outFile << "Total Energy: " << std::fixed << std::setprecision(6) 
            << fDetectorEnergy/GeV << " GeV" << std::endl << std::endl;
    
    // Photon energy spectrum
    if(fDetectorPhotonEnergyCount > 0) {
        outFile << "=== Photon Energy Spectrum at Detector Entrance ===" << std::endl;
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
            outFile << "  [" << bins[i] << " - " << bins[i+1] << " GeV]: "
                    << counts[i] << " photons" << std::endl;
        }
        outFile << "  Total photons recorded: " << fDetectorPhotonEnergyCount << std::endl << std::endl;
        
        // Photon angular distribution
        outFile << "=== Photon Angular Distribution at Detector Entrance ===" << std::endl;
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
        
        outFile << "  (Angle relative to beam axis, 0° = straight forward)" << std::endl;
        for(G4int i = 0; i < nAngleBins; i++) {
            outFile << "  [" << angleBins[i] << " - " << angleBins[i+1] << " degrees]: " 
                    << angleCounts[i] << " photons" << std::endl;
        }
        outFile << std::endl;
    }
    
    // Neutron spectra
    if(fDetectorNeutronCount > 0) {
        outFile << "=== Neutron Energy Spectrum at Detector Entrance ===" << std::endl;
        G4double neutronEnergyBins[] = {0.0, 0.1, 0.5, 1.0, 2.0, 5.0, 10.0, 50.0, 100.0};
        G4int nNeutronEnergyBins = 8;
        G4int neutronEnergyCounts[8] = {0, 0, 0, 0, 0, 0, 0, 0};
        
        for(G4int i = 0; i < fDetectorNeutronCount; i++) {
            G4double E = fDetectorNeutronEnergies[i] / GeV;
            for(G4int j = 0; j < nNeutronEnergyBins; j++) {
                if(E >= neutronEnergyBins[j] && E < neutronEnergyBins[j+1]) {
                    neutronEnergyCounts[j]++;
                    break;
                }
            }
        }
        
        for(G4int i = 0; i < nNeutronEnergyBins; i++) {
            outFile << "  [" << neutronEnergyBins[i] << " - " << neutronEnergyBins[i+1] 
                    << " GeV]: " << neutronEnergyCounts[i] << " neutrons" << std::endl;
        }
        outFile << std::endl;
        
        outFile << "=== Neutron Angular Distribution at Detector Entrance ===" << std::endl;
        G4double neutronAngleBins[] = {0.0, 5.0, 10.0, 20.0, 30.0, 45.0, 60.0, 90.0};
        G4int nNeutronAngleBins = 7;
        G4int neutronAngleCounts[7] = {0, 0, 0, 0, 0, 0, 0};
        
        for(G4int i = 0; i < fDetectorNeutronCount; i++) {
            G4double angleDeg = fDetectorNeutronAngles[i] * 180.0 / 3.14159265;
            for(G4int j = 0; j < nNeutronAngleBins; j++) {
                if(angleDeg >= neutronAngleBins[j] && angleDeg < neutronAngleBins[j+1]) {
                    neutronAngleCounts[j]++;
                    break;
                }
            }
        }
        
        outFile << "  (Angle relative to beam axis)" << std::endl;
        for(G4int i = 0; i < nNeutronAngleBins; i++) {
            outFile << "  [" << neutronAngleBins[i] << " - " << neutronAngleBins[i+1] 
                    << " degrees]: " << neutronAngleCounts[i] << " neutrons" << std::endl;
        }
        outFile << std::endl;
    }
    
    // Electron spectra
    if(fDetectorElectronCount > 0) {
        outFile << "=== Electron Energy Spectrum at Detector Entrance ===" << std::endl;
        G4double electronEnergyBins[] = {0.0, 0.1, 0.2, 0.5, 1.0, 2.0, 4.0, 8.0};
        G4int nElectronEnergyBins = 7;
        G4int electronEnergyCounts[7] = {0, 0, 0, 0, 0, 0, 0};
        
        for(G4int i = 0; i < fDetectorElectronCount; i++) {
            G4double E = fDetectorElectronEnergies[i] / GeV;
            for(G4int j = 0; j < nElectronEnergyBins; j++) {
                if(E >= electronEnergyBins[j] && E < electronEnergyBins[j+1]) {
                    electronEnergyCounts[j]++;
                    break;
                }
            }
        }
        
        for(G4int i = 0; i < nElectronEnergyBins; i++) {
            outFile << "  [" << electronEnergyBins[i] << " - " << electronEnergyBins[i+1] 
                    << " GeV]: " << electronEnergyCounts[i] << " electrons" << std::endl;
        }
        outFile << std::endl;
        
        outFile << "=== Electron Angular Distribution at Detector Entrance ===" << std::endl;
        G4double electronAngleBins[] = {0.0, 5.0, 10.0, 20.0, 30.0, 45.0, 60.0, 90.0};
        G4int nElectronAngleBins = 7;
        G4int electronAngleCounts[7] = {0, 0, 0, 0, 0, 0, 0};
        
        for(G4int i = 0; i < fDetectorElectronCount; i++) {
            G4double angleDeg = fDetectorElectronAngles[i] * 180.0 / 3.14159265;
            for(G4int j = 0; j < nElectronAngleBins; j++) {
                if(angleDeg >= electronAngleBins[j] && angleDeg < electronAngleBins[j+1]) {
                    electronAngleCounts[j]++;
                    break;
                }
            }
        }
        
        outFile << "  (Angle relative to beam axis)" << std::endl;
        for(G4int i = 0; i < nElectronAngleBins; i++) {
            outFile << "  [" << electronAngleBins[i] << " - " << electronAngleBins[i+1] 
                    << " degrees]: " << electronAngleCounts[i] << " electrons" << std::endl;
        }
        outFile << std::endl;
    }
    
    // Positron spectra
    if(fDetectorPositronCount > 0) {
        outFile << "=== Positron Energy Spectrum at Detector Entrance ===" << std::endl;
        G4double positronEnergyBins[] = {0.0, 0.1, 0.2, 0.5, 1.0, 2.0, 4.0, 8.0};
        G4int nPositronEnergyBins = 7;
        G4int positronEnergyCounts[7] = {0, 0, 0, 0, 0, 0, 0};
        
        for(G4int i = 0; i < fDetectorPositronCount; i++) {
            G4double E = fDetectorPositronEnergies[i] / GeV;
            for(G4int j = 0; j < nPositronEnergyBins; j++) {
                if(E >= positronEnergyBins[j] && E < positronEnergyBins[j+1]) {
                    positronEnergyCounts[j]++;
                    break;
                }
            }
        }
        
        for(G4int i = 0; i < nPositronEnergyBins; i++) {
            outFile << "  [" << positronEnergyBins[i] << " - " << positronEnergyBins[i+1] 
                    << " GeV]: " << positronEnergyCounts[i] << " positrons" << std::endl;
        }
        outFile << std::endl;
        
        outFile << "=== Positron Angular Distribution at Detector Entrance ===" << std::endl;
        G4double positronAngleBins[] = {0.0, 5.0, 10.0, 20.0, 30.0, 45.0, 60.0, 90.0};
        G4int nPositronAngleBins = 7;
        G4int positronAngleCounts[7] = {0, 0, 0, 0, 0, 0, 0};
        
        for(G4int i = 0; i < fDetectorPositronCount; i++) {
            G4double angleDeg = fDetectorPositronAngles[i] * 180.0 / 3.14159265;
            for(G4int j = 0; j < nPositronAngleBins; j++) {
                if(angleDeg >= positronAngleBins[j] && angleDeg < positronAngleBins[j+1]) {
                    positronAngleCounts[j]++;
                    break;
                }
            }
        }
        
        outFile << "  (Angle relative to beam axis)" << std::endl;
        for(G4int i = 0; i < nPositronAngleBins; i++) {
            outFile << "  [" << positronAngleBins[i] << " - " << positronAngleBins[i+1] 
                    << " degrees]: " << positronAngleCounts[i] << " positrons" << std::endl;
        }
        outFile << std::endl;
    }
    
    outFile << "=============================================" << std::endl;
    outFile.close();
    
    G4cout << "Analysis results saved to: " << fullPath << G4endl;
}

void DamsaAnalysis::WriteROOTHistograms(const G4String& filename)
{
    // Create plots directory if it doesn't exist
    mkdir("plots", 0755);
    
    std::string rootPath = "plots/" + filename;
    TFile* rootFile = new TFile(rootPath.c_str(), "RECREATE");
    
    if(!rootFile || rootFile->IsZombie()) {
        G4cout << "ERROR: Could not create ROOT file " << rootPath << G4endl;
        return;
    }
    
    gStyle->SetOptStat(111111);
    gStyle->SetPalette(kRainBow);
    
    // Photon histograms
    if(fDetectorPhotonEnergyCount > 0) {
        TH1D* hPhotonEnergy = new TH1D("hPhotonEnergy", "Photon Energy at Detector;Energy [GeV];Counts", 100, 0, 8);
        TH1D* hPhotonAngle = new TH1D("hPhotonAngle", "Photon Angle at Detector;Angle [degrees];Counts", 90, 0, 90);
        
        for(G4int i = 0; i < fDetectorPhotonEnergyCount; i++) {
            hPhotonEnergy->Fill(fDetectorPhotonEnergies[i] / GeV);
            hPhotonAngle->Fill(fDetectorPhotonAngles[i] * 180.0 / 3.14159265);
        }
        
        TCanvas* c1 = new TCanvas("c1", "Photon Energy", 800, 600);
        hPhotonEnergy->Draw();
        c1->SaveAs(("plots/photon_energy_" + filename.substr(0, filename.find(".root")) + ".png").c_str());
        delete c1;
        
        TCanvas* c2 = new TCanvas("c2", "Photon Angle", 800, 600);
        hPhotonAngle->Draw();
        c2->SaveAs(("plots/photon_angle_" + filename.substr(0, filename.find(".root")) + ".png").c_str());
        delete c2;
        
        rootFile->cd();
        hPhotonEnergy->Write();
        hPhotonAngle->Write();
        delete hPhotonEnergy;
        delete hPhotonAngle;
    }
    
    // Neutron histograms
    if(fDetectorNeutronCount > 0) {
        TH1D* hNeutronEnergy = new TH1D("hNeutronEnergy", "Neutron Energy at Detector;Energy [GeV];Counts", 100, 0, 100);
        TH1D* hNeutronAngle = new TH1D("hNeutronAngle", "Neutron Angle at Detector;Angle [degrees];Counts", 90, 0, 90);
        
        for(G4int i = 0; i < fDetectorNeutronCount; i++) {
            hNeutronEnergy->Fill(fDetectorNeutronEnergies[i] / GeV);
            hNeutronAngle->Fill(fDetectorNeutronAngles[i] * 180.0 / 3.14159265);
        }
        
        TCanvas* c3 = new TCanvas("c3", "Neutron Energy", 800, 600);
        c3->SetLogy();
        hNeutronEnergy->Draw();
        c3->SaveAs(("plots/neutron_energy_" + filename.substr(0, filename.find(".root")) + ".png").c_str());
        delete c3;
        
        TCanvas* c4 = new TCanvas("c4", "Neutron Angle", 800, 600);
        hNeutronAngle->Draw();
        c4->SaveAs(("plots/neutron_angle_" + filename.substr(0, filename.find(".root")) + ".png").c_str());
        delete c4;
        
        rootFile->cd();
        hNeutronEnergy->Write();
        hNeutronAngle->Write();
        delete hNeutronEnergy;
        delete hNeutronAngle;
    }
    
    // Electron histograms
    if(fDetectorElectronCount > 0) {
        TH1D* hElectronEnergy = new TH1D("hElectronEnergy", "Electron Energy at Detector;Energy [GeV];Counts", 100, 0, 8);
        TH1D* hElectronAngle = new TH1D("hElectronAngle", "Electron Angle at Detector;Angle [degrees];Counts", 90, 0, 90);
        
        for(G4int i = 0; i < fDetectorElectronCount; i++) {
            hElectronEnergy->Fill(fDetectorElectronEnergies[i] / GeV);
            hElectronAngle->Fill(fDetectorElectronAngles[i] * 180.0 / 3.14159265);
        }
        
        TCanvas* c5 = new TCanvas("c5", "Electron Energy", 800, 600);
        hElectronEnergy->Draw();
        c5->SaveAs(("plots/electron_energy_" + filename.substr(0, filename.find(".root")) + ".png").c_str());
        delete c5;
        
        TCanvas* c6 = new TCanvas("c6", "Electron Angle", 800, 600);
        hElectronAngle->Draw();
        c6->SaveAs(("plots/electron_angle_" + filename.substr(0, filename.find(".root")) + ".png").c_str());
        delete c6;
        
        rootFile->cd();
        hElectronEnergy->Write();
        hElectronAngle->Write();
        delete hElectronEnergy;
        delete hElectronAngle;
    }
    
    // Positron histograms
    if(fDetectorPositronCount > 0) {
        TH1D* hPositronEnergy = new TH1D("hPositronEnergy", "Positron Energy at Detector;Energy [GeV];Counts", 100, 0, 8);
        TH1D* hPositronAngle = new TH1D("hPositronAngle", "Positron Angle at Detector;Angle [degrees];Counts", 90, 0, 90);
        
        for(G4int i = 0; i < fDetectorPositronCount; i++) {
            hPositronEnergy->Fill(fDetectorPositronEnergies[i] / GeV);
            hPositronAngle->Fill(fDetectorPositronAngles[i] * 180.0 / 3.14159265);
        }
        
        TCanvas* c7 = new TCanvas("c7", "Positron Energy", 800, 600);
        hPositronEnergy->Draw();
        c7->SaveAs(("plots/positron_energy_" + filename.substr(0, filename.find(".root")) + ".png").c_str());
        delete c7;
        
        TCanvas* c8 = new TCanvas("c8", "Positron Angle", 800, 600);
        hPositronAngle->Draw();
        c8->SaveAs(("plots/positron_angle_" + filename.substr(0, filename.find(".root")) + ".png").c_str());
        delete c8;
        
        rootFile->cd();
        hPositronEnergy->Write();
        hPositronAngle->Write();
        delete hPositronEnergy;
        delete hPositronAngle;
    }
    
    rootFile->Close();
    delete rootFile;
    
    G4cout << "ROOT histograms and plots saved to plots/ directory" << G4endl;
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
    fDetectorNeutronCount = 0;
    fDetectorElectronCount = 0;
    fDetectorPositronCount = 0;
}

#endif
