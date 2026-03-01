#include <iostream>
#include <sstream>

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4SystemOfUnits.hh"

#include "construction.h"
#include "physics.h"
#include "action.h"
#include "analysis.h"

int main (int argc, char *argv[])
{
    G4RunManager *runManager = new G4RunManager();

    runManager->SetUserInitialization(new DamsaDetectorConstruction());
    runManager->SetUserInitialization(new DamsaPhysicsList());
    runManager->SetUserInitialization(new DamsaActionInitialization());

    runManager->Initialize();

    G4UImanager *UIManager = G4UImanager::GetUIpointer();
    
    G4cout << "\n=== DAMSA Energy-Dependent Background Study ===" << G4endl;
    G4cout << "Scanning beam energies from 1 MeV to 1 GeV" << G4endl;
    G4cout << "Measuring backgrounds vs beam energy\n" << G4endl;
    
    G4double energies[] = {1*MeV, 10*MeV, 100*MeV, 500*MeV, 800*MeV, 1*GeV};
    G4int nEnergies = 6;
    G4int eventsPerEnergy = 100;
    
    const G4VUserPrimaryGeneratorAction* genAction = 
        runManager->GetUserPrimaryGeneratorAction();
    DamsaPrimaryGenerator* primaryGenerator = 
        (DamsaPrimaryGenerator*)genAction;
    
    for(G4int i = 0; i < nEnergies; i++) {
        G4cout << "\n--- Energy Point " << i+1 << "/" << nEnergies << " ---" << G4endl;
        G4cout << "Beam Energy: " << energies[i]/MeV << " MeV" << G4endl;
        
        primaryGenerator->SetBeamEnergy(energies[i]);
        
        G4String command = "/run/beamOn " + std::to_string(eventsPerEnergy);
        UIManager->ApplyCommand(command);
        
        DamsaAnalysis::Instance()->PrintSummary();
        
        std::stringstream filename;
        filename << "analysis_" << energies[i]/MeV << "MeV.txt";
        DamsaAnalysis::Instance()->SaveToFile(filename.str());
        
        std::stringstream rootfile;
        rootfile << "particles_" << energies[i]/MeV << "MeV.root";
        DamsaAnalysis::Instance()->WriteROOTHistograms(rootfile.str());
        
        DamsaAnalysis::Instance()->Reset();
        
        G4cout << "Completed " << eventsPerEnergy << " events at " 
               << energies[i]/MeV << " MeV" << G4endl;
    }
    
    G4cout << "\n=== Energy Scan Complete ===" << G4endl;
    G4cout << "Results show energy-dependent backgrounds for DAMSA optimization" << G4endl;

    delete runManager;

    return 0;
}
