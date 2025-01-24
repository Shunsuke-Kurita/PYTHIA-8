#include "Pythia8/Pythia.h"
using namespace Pythia8;
#include <cmath>
#include "TH1F.h"
#include "TFile.h"

// Charm and bottom pair creations by gluon fusion

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

    for (int iEvent = 0; iEvent < 10000; ++iEvent) {
        pythia.next();

        int iCharm = 0, iAntiCharm = 0;
        for (int i = 0; i < pythia.event.size(); ++i) {
            if (pythia.event[i].id() == 4) iCharm = i;
            if (pythia.event[i].id() == -4) iAntiCharm = i;
        }

        double kDeltaPhiPair = 0.;
        double kDeltaPhi = pythia.event[iCharm].phi() - pythia.event[iAntiCharm].phi();
        if (kDeltaPhi  > 3 * M_PI / 2) {
            kDeltaPhiPair = kDeltaPhi  - 2 * M_PI;
        } else if (kDeltaPhi  < -M_PI / 2) {
            kDeltaPhiPair = kDeltaPhi  + 2 * M_PI;
        } else {
            kDeltaPhiPair = kDeltaPhi;
        }

        pT->Fill(pythia.event[iCharm].pT());
        eta->Fill(pythia.event[iCharm].eta());
        phi->Fill(pythia.event[iCharm].phi());
        hDeltaPhi->Fill(kDeltaPhiPair);
        
    }

    // cout << pT << eta << phi;
    TFile* outFileRoot = new TFile("output.root", "RECREATE");
    pT->Write();
    eta->Write();
    phi->Write();
    hDeltaPhi->Write();
    outFileRoot->Close();

    pythia.stat();

    return 0;
}