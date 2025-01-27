#include "Pythia8/Pythia.h"
using namespace Pythia8;
#include <cmath>
#include "TH1F.h"
#include "TFile.h"

template <typename T>
double calculateDeltaPhi(const T& event, int iParticle1, int iParticle2) {
    double deltaPhi = event[iParticle1].phi() - event[iParticle2].phi();
    if (deltaPhi > 3 * M_PI / 2) {
        deltaPhi -= 2 * M_PI;
    } else if (deltaPhi < -M_PI / 2) {
        deltaPhi += 2 * M_PI;
    }
    return deltaPhi;
}

int main() {

    Pythia pythia;
    pythia.readString("HardQCD:gg2ccbar = on");
    pythia.readString("Beams:eCM = 13600.");

    pythia.init();

    TH1F* hDeltaPhi = new TH1F("hDeltaPhi", "DeltaPhi between electron and muon;#Delta#phi (rad);Entries", 600,  -0.5 * M_PI, 1.5 * M_PI);
    TH1F* hElectronPt = new TH1F("hElectronPt", "Electron pT;p_{T} (GeV);Entries", 100, 0., 30.);
    TH1F* hMuonPt = new TH1F("hMuonPt", "Muon pT;p_{T} (GeV);Entries", 100, 0., 30.);
    TH1F* hElectronEta = new TH1F("hElectronEta", "Electron eta;#eta;Entries", 100, -5., 5.);
    TH1F* hMuonEta = new TH1F("hMuonEta", "Muon eta;#eta;Entries", 100, -5., 5.);

    const int D0ID = 421;       // D0
    const int DplusID = 411;    // D+
    const int DstarID = 413;    // D*+
    const int electronID = 11;  // Electron
    const int positronID = -11; // Positron
    const int muonID = 13;      // Muon
    const int antimuonID = -13; // Antimuon

    for (int iEvent = 0; iEvent < 100000000; ++iEvent) {
        pythia.next();

        // Vectors to store electrons, positrons, muons, and antimuons from D meson decays
        std::vector<int> electronIndices;
        std::vector<int> positronIndices;
        std::vector<int> muonIndices;
        std::vector<int> antimuonIndices;

        // Loop through the event record once
        for (int i = 0; i < pythia.event.size(); ++i) {
            int id = pythia.event[i].id();

            // Check for D mesons
            if (id == D0ID || id == DplusID || id == DstarID || id == -D0ID || id == -DplusID || id == -DstarID) {
                // Loop through decay products of the D meson
                for (int j = i + 1; j < pythia.event.size(); ++j) {
                    // Check if the particle is a decay product of the D meson
                    if (pythia.event[j].mother1() == i || pythia.event[j].mother2() == i) {
                        int daughterID = pythia.event[j].id();

                        // Check if the decay product is an electron, positron, muon, or antimuon
                        if (daughterID == electronID) {
                            electronIndices.push_back(j);
                        } else if (daughterID == positronID) {
                            positronIndices.push_back(j);
                        } else if (daughterID == muonID) {
                            muonIndices.push_back(j);
                        } else if (daughterID == antimuonID) {
                            antimuonIndices.push_back(j);
                        }
                    }
                }
            }
        }

        // Pair electrons with antimuons and positrons with muons
        for (size_t iElec = 0; iElec < electronIndices.size(); ++iElec) {
            double etaElec = pythia.event[electronIndices[iElec]].eta();
            if (etaElec < -0.8 || etaElec > 0.8) continue;
            for (size_t iAntiMu = 0; iAntiMu < antimuonIndices.size(); ++iAntiMu) {
                double etaAntiMu = pythia.event[antimuonIndices[iAntiMu]].eta();
                if (etaAntiMu < -4.0 || etaAntiMu > -2.5) continue;
                double deltaPhiPair = calculateDeltaPhi(pythia.event, electronIndices[iElec], antimuonIndices[iAntiMu]);

                hDeltaPhi->Fill(deltaPhiPair);
                hElectronPt->Fill(pythia.event[electronIndices[iElec]].pT());
                hMuonPt->Fill(pythia.event[antimuonIndices[iAntiMu]].pT());
                hElectronEta->Fill(pythia.event[electronIndices[iElec]].eta());
                hMuonEta->Fill(pythia.event[antimuonIndices[iAntiMu]].eta());
            }
        }

        for (size_t iPos = 0; iPos < positronIndices.size(); ++iPos) {
            double etaPos = pythia.event[positronIndices[iPos]].eta();
            if (etaPos < -0.8 || etaPos > 0.8) continue;
            for (size_t iMu = 0; iMu < muonIndices.size(); ++iMu) {
                double etaMu = pythia.event[muonIndices[iMu]].eta();
                if (etaMu < -4.0 || etaMu > -2.5) continue;
                double deltaPhiPair = calculateDeltaPhi(pythia.event, positronIndices[iPos], muonIndices[iMu]);

                hDeltaPhi->Fill(deltaPhiPair);
                hElectronPt->Fill(pythia.event[positronIndices[iPos]].pT());
                hMuonPt->Fill(pythia.event[muonIndices[iMu]].pT());
                hElectronEta->Fill(pythia.event[positronIndices[iPos]].eta());
                hMuonEta->Fill(pythia.event[muonIndices[iMu]].eta());
            }
        }
    }

    TFile* outFileRoot = new TFile("emu_etaCuts.root", "RECREATE");
    hDeltaPhi->Write();
    hElectronPt->Write();
    hMuonPt->Write();
    hElectronEta->Write();
    hMuonEta->Write();
    outFileRoot->Close();

    pythia.stat();

    return 0;
}