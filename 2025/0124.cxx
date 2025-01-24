#include "Pythia8/Pythia.h" // Include Pythia headers.
using namespace Pythia8; // Let Pythia8:: be implicit.
#include <cmath>

// Charm and bottom pair creations by gluon fusion

int main() {

    Pythia pythia;
    pythia.readString("HardQCD:gg2ccbar = on");
    pythia.readString("HardQCD:gg2bbbar = on");
    pythia.readString("Beams:eCM = 13600.");

    pythia.init();

    Hist pT("charm transverse momentum", 100, 0., 200.);
    Hist eta("charm pseudorapidity", 100, -5., 5.);
    Hist phi("charm azimuthal angle", 600, 0., 2 * M_PI);

    for (int iEvent = 0; iEvent < 1000; ++iEvent) {
        pythia.next();

        int iCharm = 0;
        for (int i = 0; i < pythia.event.size(); ++i) {
            if (pythia.event[i].id() == 4) iCharm = i;
        }

        pT.fill( pythia.event[iCharm].pT() );
        eta.fill( pythia.event[iCharm].eta() );
        phi.fill( pythia.event[iCharm].phi() );
    }

    cout << pT << eta << phi;

    // Obtain statistics on the number of events generated of the different kinds, and the estimated cross sections
    pythia.stat();

    return 0;
}