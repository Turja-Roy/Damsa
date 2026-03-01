#ifndef RUN_H
#define RUN_H

#include "G4UserRunAction.hh"
#include "G4AnalysisManager.hh"
#include "G4Run.hh"
#include "TCanvas.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TStyle.h"
#include "TTree.h"
#include "TFile.h"
#include <sys/stat.h>
#include <sstream>
#include <string>

#include "analysis.h"

class DamsaRunAction : public G4UserRunAction {
public:
    DamsaRunAction();
    virtual ~DamsaRunAction();

    virtual void BeginOfRunAction(const G4Run*);
    virtual void EndOfRunAction(const G4Run*);
};

DamsaRunAction::DamsaRunAction() {
    G4AnalysisManager* man = G4AnalysisManager::Instance();

    man->CreateNtuple("Hits", "Hits");
    man->CreateNtupleIColumn("fEvent");
    man->CreateNtupleDColumn("fX");
    man->CreateNtupleDColumn("fY");
    man->CreateNtupleDColumn("fZ");
    man->FinishNtuple(0);
}
DamsaRunAction::~DamsaRunAction() {}

void DamsaRunAction::BeginOfRunAction(const G4Run* run) {
    G4AnalysisManager* man = G4AnalysisManager::Instance();

    G4int runID = run->GetRunID();

    std::stringstream strRunID;
    strRunID << runID;

    man->OpenFile("output" + strRunID.str() + ".root");
}

void DamsaRunAction::EndOfRunAction(const G4Run* run) {
    G4int runID = run->GetRunID();
    std::stringstream strRunID;
    strRunID << runID;
    
    std::string rootFileName = "output" + strRunID.str() + ".root";
    std::string plotDir = "plots";
    
    G4AnalysisManager* man = G4AnalysisManager::Instance();
    man->Write();
    man->CloseFile(false);
    
    struct stat buffer;
    if (stat(rootFileName.c_str(), &buffer) != 0) {
        G4cout << "ROOT file does not exist: " << rootFileName << G4endl;
        return;
    }
    
    mkdir(plotDir.c_str(), 0755);
    
    TFile* file = TFile::Open(rootFileName.c_str(), "READ");
    if (!file || file->IsZombie()) {
        G4cout << "Cannot open ROOT file for plotting: " << rootFileName << G4endl;
        return;
    }
    
    TTree* tree = (TTree*)file->Get("Hits");
    if (!tree) {
        G4cout << "Cannot find Hits tree in ROOT file" << G4endl;
        file->Close();
        return;
    }
    
    Long64_t nEntries = tree->GetEntries();
    G4cout << "Found " << nEntries << " entries in ROOT file" << G4endl;
    
    if (nEntries == 0) {
        G4cout << "No hits recorded, skipping plot generation" << G4endl;
        file->Close();
        return;
    }
    
    Int_t fEvent;
    Double_t fX, fY, fZ;
    tree->SetBranchAddress("fEvent", &fEvent);
    tree->SetBranchAddress("fX", &fX);
    tree->SetBranchAddress("fY", &fY);
    tree->SetBranchAddress("fZ", &fZ);
    
    TH1D* hX = new TH1D("hX", "X Position Distribution;X [mm];Counts", 100, -200, 200);
    TH1D* hY = new TH1D("hY", "Y Position Distribution;Y [mm];Counts", 100, -200, 200);
    TH1D* hZ = new TH1D("hZ", "Z Distribution;Z [mm];Counts", 100, -600, 600);
    TH2D* hXY = new TH2D("hXY", "Hit Position Y vs X (Color = Z);X [mm];Y [mm]", 100, -200, 200, 100, -200, 200);
    
    for (Long64_t i = 0; i < nEntries; i++) {
        tree->GetEntry(i);
        hX->Fill(fX);
        hY->Fill(fY);
        hZ->Fill(fZ);
        hXY->Fill(fX, fY, fZ);
    }
    
    gStyle->SetOptStat(111111);
    gStyle->SetPalette(kRainBow);
    
    TCanvas* c1 = new TCanvas("c1", "X Distribution", 800, 600);
    hX->Draw();
    c1->SaveAs((plotDir + "/x_distribution_run" + strRunID.str() + ".png").c_str());
    delete c1;
    
    TCanvas* c2 = new TCanvas("c2", "Y Distribution", 800, 600);
    hY->Draw();
    c2->SaveAs((plotDir + "/y_distribution_run" + strRunID.str() + ".png").c_str());
    delete c2;
    
    TCanvas* c3 = new TCanvas("c3", "Z Distribution", 800, 600);
    hZ->Draw();
    c3->SaveAs((plotDir + "/z_distribution_run" + strRunID.str() + ".png").c_str());
    delete c3;
    
    TCanvas* c4 = new TCanvas("c4", "Y vs X (Z color-coded)", 900, 700);
    c4->SetRightMargin(0.15);
    hXY->Draw("COLZ");
    c4->SaveAs((plotDir + "/xy_position_zcolor_run" + strRunID.str() + ".png").c_str());
    delete c4;
    
    delete hX;
    delete hY;
    delete hZ;
    delete hXY;
    
    file->Close();
    delete file;
    
    G4cout << "Plots saved to " << plotDir << "/ directory" << G4endl;
    
    // Write analysis histograms (particle spectra at scoring planes)
    DamsaAnalysis::Instance()->WriteROOTHistograms("particle_spectra.root");
}

#endif
