#ifndef PLOTTING_H
#define PLOTTING_H

// Plotting utilities for DAMSA Analysis
// Provides publication-quality visualization of particle spectra

#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TLine.h"
#include "TPad.h"
#include "TGaxis.h"
#include "TColor.h"
#include "TROOT.h"
#include <vector>
#include <string>
#include <cmath>
#include <sys/stat.h>

// Color scheme for particle types (consistent across all plots)
const Color_t kPhotonColor = kRed;
const Color_t kNeutronColor = kBlue;
const Color_t kElectronColor = kGreen+2;
const Color_t kPositronColor = kMagenta;

// Location colors for evolution plots
const Color_t kTargetExitColor = kRed;
const Color_t kMagnetEntranceColor = kBlue;
const Color_t kCaloEntranceColor = kGreen+2;
const Color_t kCaloExitColor = kMagenta;

// Set publication-quality style
inline void SetPublicationStyle() {
    // Stat box settings - positioned in the extended right margin
    gStyle->SetOptStat("emruo"); // entries, mean, rms, underflow, overflow
    gStyle->SetStatX(0.99);      // Right edge of stat box (in NDC)
    gStyle->SetStatY(0.92);      // Top edge of stat box
    gStyle->SetStatW(0.18);      // Width of stat box
    gStyle->SetStatH(0.18);      // Height of stat box
    gStyle->SetStatFont(42);
    gStyle->SetStatFontSize(0.025);
    gStyle->SetStatBorderSize(1);
    
    gStyle->SetPalette(kBird);
    gStyle->SetLabelSize(0.045, "xyz");
    gStyle->SetTitleSize(0.050, "xyz");
    gStyle->SetTitleOffset(1.0, "x");
    gStyle->SetTitleOffset(1.2, "y");
    gStyle->SetTitleFont(42, "xyz");
    gStyle->SetLabelFont(42, "xyz");
    
    gStyle->SetLineWidth(2);
    gStyle->SetHistLineWidth(2);
    gStyle->SetFrameLineWidth(2);
    
    // Increased right margin to accommodate stat box outside plot area
    gStyle->SetPadLeftMargin(0.12);
    gStyle->SetPadRightMargin(0.20);  // Extended for stat box
    gStyle->SetPadBottomMargin(0.12);
    gStyle->SetPadTopMargin(0.08);
    
    gStyle->SetPadTickX(1);
    gStyle->SetPadTickY(1);
    
    gStyle->SetCanvasDefW(900);  // Slightly wider to accommodate margin
    gStyle->SetCanvasDefH(600);
}

// Apply consistent style to a 1D histogram
inline void StyleHist1D(TH1D* h, Color_t color, const char* xtitle, const char* ytitle) {
    h->SetLineColor(color);
    h->SetLineWidth(2);
    h->SetMarkerColor(color);
    h->SetMarkerStyle(20);
    h->SetMarkerSize(0.8);
    h->GetXaxis()->SetTitle(xtitle);
    h->GetYaxis()->SetTitle(ytitle);
    h->GetXaxis()->SetTitleFont(42);
    h->GetYaxis()->SetTitleFont(42);
    h->GetXaxis()->SetLabelFont(42);
    h->GetYaxis()->SetLabelFont(42);
}

// Apply consistent style to a 2D histogram
inline void StyleHist2D(TH2D* h, const char* xtitle, const char* ytitle, const char* ztitle) {
    h->GetXaxis()->SetTitle(xtitle);
    h->GetYaxis()->SetTitle(ytitle);
    h->GetZaxis()->SetTitle(ztitle);
    h->GetXaxis()->SetTitleFont(42);
    h->GetYaxis()->SetTitleFont(42);
    h->GetZaxis()->SetTitleFont(42);
    h->GetXaxis()->SetLabelFont(42);
    h->GetYaxis()->SetLabelFont(42);
    h->GetZaxis()->SetLabelFont(42);
    h->GetZaxis()->SetTitleOffset(0.8);
    h->SetStats(0);  // Disable stat box for 2D plots (clutters the display)
}

// Create directory structure for output
inline void CreatePlotDirectories() {
    mkdir("plots", 0755);
    mkdir("plots/png", 0755);
    mkdir("plots/png/targetexit", 0755);
    mkdir("plots/png/magnetentrance", 0755);
    mkdir("plots/png/caloentrance", 0755);
    mkdir("plots/png/caloexit", 0755);
    mkdir("plots/png/comparison", 0755);
    mkdir("plots/png/statistics", 0755);
    mkdir("plots/summary", 0755);
}

// Save canvas in PNG format
inline void SaveCanvas(TCanvas* c, const std::string& path) {
    c->SaveAs((path + ".png").c_str());
}

// Create a normalized copy of a histogram
inline TH1D* CreateNormalizedCopy(TH1D* h, const char* suffix) {
    std::string newName = std::string(h->GetName()) + suffix;
    std::string newTitle = std::string(h->GetTitle()) + " (Normalized)";
    TH1D* hNorm = (TH1D*)h->Clone(newName.c_str());
    hNorm->SetTitle(newTitle.c_str());
    if(hNorm->Integral() > 0) {
        hNorm->Scale(1.0/hNorm->Integral());
    }
    hNorm->GetYaxis()->SetTitle("Normalized Counts");
    return hNorm;
}

// Create 1D energy histogram from vector data
inline TH1D* CreateEnergyHist(const std::vector<double>& energies, 
                              const char* name, const char* title,
                              Color_t color, double maxE = 8.0, int nbins = 80) {
    TH1D* h = new TH1D(name, title, nbins, 0, maxE);
    for(size_t i = 0; i < energies.size(); i++) {
        h->Fill(energies[i]);
    }
    StyleHist1D(h, color, "Energy [GeV]", "Counts");
    return h;
}

// Create 1D angular histogram from vector data (angles in radians, converted to degrees)
inline TH1D* CreateAngleHist(const std::vector<double>& angles,
                             const char* name, const char* title,
                             Color_t color) {
    TH1D* h = new TH1D(name, title, 90, 0, 90);
    for(size_t i = 0; i < angles.size(); i++) {
        h->Fill(angles[i] * 180.0 / 3.14159265);
    }
    StyleHist1D(h, color, "Angle [degrees]", "Counts");
    return h;
}

// Create 2D Angle vs Energy heatmap with auto-ranging (Angle on X, Energy on Y)
inline TH2D* CreateEnergyAngleHist(const std::vector<double>& energies,
                                   const std::vector<double>& angles,
                                   const char* name, const char* title,
                                   double maxE = -1.0) {
    // Auto-detect energy range if not specified (maxE < 0)
    double actualMaxE = maxE;
    if(maxE < 0 && energies.size() > 0) {
        actualMaxE = 0;
        for(size_t i = 0; i < energies.size(); i++) {
            if(energies[i] > actualMaxE) actualMaxE = energies[i];
        }
        // Add 20% headroom and round up to nice values
        actualMaxE *= 1.2;
        if(actualMaxE < 0.1) actualMaxE = 0.1;
        else if(actualMaxE < 0.5) actualMaxE = 0.5;
        else if(actualMaxE < 1.0) actualMaxE = 1.0;
        else if(actualMaxE < 2.0) actualMaxE = 2.0;
        else if(actualMaxE < 5.0) actualMaxE = 5.0;
        else if(actualMaxE < 10.0) actualMaxE = 10.0;
        else actualMaxE = 8.0;  // Cap at beam energy
    } else if(maxE < 0) {
        actualMaxE = 8.0;  // Default if no data
    }
    
    // X-axis: Angle (0-90 deg), Y-axis: Energy (0-maxE GeV)
    TH2D* h = new TH2D(name, title, 45, 0, 90, 50, 0, actualMaxE);
    for(size_t i = 0; i < energies.size() && i < angles.size(); i++) {
        h->Fill(angles[i] * 180.0 / 3.14159265, energies[i]);
    }
    StyleHist2D(h, "Angle [degrees]", "Energy [GeV]", "Counts");
    return h;
}

// Create overlay of 4 particle histograms on one canvas
inline TCanvas* CreateOverlayCanvas(TH1D* hPhoton, TH1D* hNeutron, 
                                    TH1D* hElectron, TH1D* hPositron,
                                    const char* canvasName, const char* title,
                                    bool logY = true) {
    TCanvas* c = new TCanvas(canvasName, title, 900, 600);
    c->SetRightMargin(0.12);  // Smaller margin for overlay (no stat box)
    
    // Disable stat boxes for overlay plots (legend provides identification)
    gStyle->SetOptStat(0);
    
    // Find maximum for y-axis range
    double maxY = 0;
    if(hPhoton && hPhoton->GetMaximum() > maxY) maxY = hPhoton->GetMaximum();
    if(hNeutron && hNeutron->GetMaximum() > maxY) maxY = hNeutron->GetMaximum();
    if(hElectron && hElectron->GetMaximum() > maxY) maxY = hElectron->GetMaximum();
    if(hPositron && hPositron->GetMaximum() > maxY) maxY = hPositron->GetMaximum();
    
    if(logY) {
        c->SetLogy();
        maxY *= 5;
    } else {
        maxY *= 1.3;
    }
    
    // Draw histograms (stat boxes disabled, override title with overlay title)
    bool first = true;
    if(hPhoton && hPhoton->GetEntries() > 0) {
        hPhoton->SetStats(0);
        hPhoton->SetTitle(title);  // Override with overlay title
        hPhoton->SetMaximum(maxY);
        hPhoton->SetMinimum(logY ? 0.5 : 0);
        hPhoton->Draw("HIST");
        first = false;
    }
    if(hNeutron && hNeutron->GetEntries() > 0) {
        hNeutron->SetStats(0);
        if(first) {
            hNeutron->SetTitle(title);  // Override with overlay title
            hNeutron->SetMaximum(maxY);
            hNeutron->SetMinimum(logY ? 0.5 : 0);
            hNeutron->Draw("HIST");
            first = false;
        } else {
            hNeutron->Draw("HIST SAME");
        }
    }
    if(hElectron && hElectron->GetEntries() > 0) {
        hElectron->SetStats(0);
        if(first) {
            hElectron->SetTitle(title);  // Override with overlay title
            hElectron->SetMaximum(maxY);
            hElectron->SetMinimum(logY ? 0.5 : 0);
            hElectron->Draw("HIST");
            first = false;
        } else {
            hElectron->Draw("HIST SAME");
        }
    }
    if(hPositron && hPositron->GetEntries() > 0) {
        hPositron->SetStats(0);
        if(first) {
            hPositron->SetTitle(title);  // Override with overlay title
            hPositron->SetMaximum(maxY);
            hPositron->SetMinimum(logY ? 0.5 : 0);
            hPositron->Draw("HIST");
            first = false;
        } else {
            hPositron->Draw("HIST SAME");
        }
    }
    
    // Re-enable stat boxes for subsequent plots
    gStyle->SetOptStat("emruo");
    
    // Add legend
    TLegend* leg = new TLegend(0.70, 0.70, 0.88, 0.91);
    leg->SetBorderSize(0);
    leg->SetFillStyle(0);
    leg->SetTextFont(42);
    leg->SetTextSize(0.035);
    if(hPhoton && hPhoton->GetEntries() > 0) leg->AddEntry(hPhoton, "Photons", "l");
    if(hNeutron && hNeutron->GetEntries() > 0) leg->AddEntry(hNeutron, "Neutrons", "l");
    if(hElectron && hElectron->GetEntries() > 0) leg->AddEntry(hElectron, "Electrons", "l");
    if(hPositron && hPositron->GetEntries() > 0) leg->AddEntry(hPositron, "Positrons", "l");
    leg->Draw();
    
    return c;
}

// Create particle count bar chart
inline TH1D* CreateParticleCountHist(int nPhoton, int nNeutron, int nElectron, int nPositron,
                                     const char* name, const char* title) {
    TH1D* h = new TH1D(name, title, 4, 0, 4);
    h->SetBinContent(1, nPhoton);
    h->SetBinContent(2, nNeutron);
    h->SetBinContent(3, nElectron);
    h->SetBinContent(4, nPositron);
    
    h->GetXaxis()->SetBinLabel(1, "Photons");
    h->GetXaxis()->SetBinLabel(2, "Neutrons");
    h->GetXaxis()->SetBinLabel(3, "Electrons");
    h->GetXaxis()->SetBinLabel(4, "Positrons");
    
    h->SetFillColor(kAzure+1);
    h->SetLineColor(kAzure+2);
    h->SetLineWidth(2);
    h->GetYaxis()->SetTitle("Count");
    h->GetXaxis()->SetLabelSize(0.06);
    h->SetStats(0);
    
    return h;
}

// Create energy budget histogram (primary vs secondary)
inline TH1D* CreateEnergyBudgetHist(double primaryE, double secondaryE,
                                    const char* name, const char* title) {
    TH1D* h = new TH1D(name, title, 2, 0, 2);
    h->SetBinContent(1, primaryE);
    h->SetBinContent(2, secondaryE);
    
    h->GetXaxis()->SetBinLabel(1, "Primary");
    h->GetXaxis()->SetBinLabel(2, "Secondary");
    
    h->SetFillColor(kOrange+1);
    h->SetLineColor(kOrange+2);
    h->SetLineWidth(2);
    h->GetYaxis()->SetTitle("Energy [GeV]");
    h->GetXaxis()->SetLabelSize(0.08);
    h->SetStats(0);
    
    return h;
}

// Calculate mean and RMS of a vector
inline void CalculateStats(const std::vector<double>& data, double& mean, double& rms) {
    mean = 0;
    rms = 0;
    if(data.size() == 0) return;
    
    for(size_t i = 0; i < data.size(); i++) {
        mean += data[i];
    }
    mean /= data.size();
    
    for(size_t i = 0; i < data.size(); i++) {
        rms += (data[i] - mean) * (data[i] - mean);
    }
    rms = sqrt(rms / data.size());
}

#endif
