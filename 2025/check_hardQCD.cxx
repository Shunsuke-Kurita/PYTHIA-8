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
    // pythia.readString("HardQCD:gg2bbbar = on");
    pythia.readString("Beams:eCM = 13600.");

    pythia.init();

    TH1F* pT = new TH1F("Transverse momentum", ";p_{T} (GeV);Entries", 100, 0., 30.);
    TH1F* eta = new TH1F("Pseudorapidity", ";#eta;Entries", 100, -5., 5.);
    TH1F* phi = new TH1F("Azimuthal angle", ";#phi (rad);Entries", 600, - M_PI, M_PI);
    TH1F* hDeltaPhi = new TH1F("hDeltaPhi", "Charm - AntiCharm;#Delta#phi (rad);Entries", 600, -0.5 * M_PI, 1.5 * M_PI);
    // TH1F* hDeltaPhi = new TH1F("hDeltaPhi", "Bottom - AntiBottom;#Delta#phi (rad);Entries", 600, -0.5 * M_PI, 1.5 * M_PI);

    for (int iEvent = 0; iEvent < 10000; ++iEvent) {
        pythia.next();

        int iParticle = 0, iAntiParticle = 0;

        for (int i = 0; i < pythia.event.size(); ++i) {
            if (pythia.event[i].id() == 4) iParticle = i;
            if (pythia.event[i].id() == -4) iAntiParticle = i;
            // if (pythia.event[i].id() == 5) iParticle = i;
            // if (pythia.event[i].id() == -5) iAntiParticle = i;
        }

        if (iParticle != 0 && iAntiParticle != 0) {
            double deltaPhiPair = calculateDeltaPhi(pythia.event, iParticle, iAntiParticle);

            pT->Fill(pythia.event[iParticle].pT());
            eta->Fill(pythia.event[iParticle].eta());
            phi->Fill(pythia.event[iParticle].phi());
            hDeltaPhi->Fill(deltaPhiPair);
        }
        
    }

    TFile* outFileRoot = new TFile("output.root", "RECREATE");
    pT->Write();
    eta->Write();
    phi->Write();
    hDeltaPhi->Write();
    outFileRoot->Close();

    pythia.stat();

    return 0;
}