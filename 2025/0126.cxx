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

    TH1F* pT = new TH1F("Transverse momentum", ";p_{T} (GeV);Entries", 100, 0., 30.);
    TH1F* eta = new TH1F("Pseudorapidity", ";#eta;Entries", 100, -5., 5.);
    TH1F* phi = new TH1F("Azimuthal angle", ";#phi (rad);Entries", 600, - M_PI, M_PI);
    TH1F* hDeltaPhi = new TH1F("hDeltaPhi", "D - AntiD;#Delta#phi (rad);Entries", 600, -0.5 * M_PI, 1.5 * M_PI);

    // PDG IDs for D mesons
    const int D0ID = 421;       // D0
    const int DplusID = 411;    // D+
    const int DstarID = 413;    // D*+

    for (int iEvent = 0; iEvent < 1000000; ++iEvent) {
        pythia.next();

        int iDMeson = 0, iAntiDMeson = 0;

        for (int i = 0; i < pythia.event.size(); ++i) {
            int id = pythia.event[i].id();

            if (id == D0ID || id == DplusID || id == DstarID) {
                iDMeson = i;
            }
            else if (id == -D0ID || id == -DplusID || id == -DstarID) {
                iAntiDMeson = i;
            }
        }

        // If both D and anti-D mesons are found, fill histograms
        if (iDMeson != 0 && iAntiDMeson != 0) {
            double deltaPhiPair = calculateDeltaPhi(pythia.event, iDMeson, iAntiDMeson);

            pT->Fill(pythia.event[iDMeson].pT());
            eta->Fill(pythia.event[iDMeson].eta());
            phi->Fill(pythia.event[iDMeson].phi());
            hDeltaPhi->Fill(deltaPhiPair);
        }
    }

    TFile* outFileRoot = new TFile("0125.root", "RECREATE");
    pT->Write();
    eta->Write();
    phi->Write();
    hDeltaPhi->Write();
    outFileRoot->Close();

    pythia.stat();

    return 0;
}