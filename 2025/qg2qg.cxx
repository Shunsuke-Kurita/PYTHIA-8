#include "Pythia8/Pythia.h"
using namespace Pythia8;


int main() {
    Pythia pythia;
    pythia.readString("HardQCD:all = on");
    pythia.readString("HardQCD:gg2ccbar = off");
    pythia.readString("HardQCD:qqbar2ccbar = off");
    pythia.readString("HardQCD:gg2bbbar = off");
    pythia.readString("HardQCD:qqbar2bbbar = off");
    pythia.readString("Vincia:nGluonToQuark = 4");
    pythia.readString("Vincia:ThresholdMC = 1.5");
    pythia.readString("Beams:eCM = 13600.");
    pythia.readString("Random:setSeed = on");
    pythia.readString("Random:seed = 42");
    pythia.readString("Next:numberShowEvent = 47");

    pythia.init();

    for (int iEvent = 0; iEvent < 47; ++iEvent) {
        if (!pythia.next()) continue;

        for (int i = 0; i < pythia.event.size(); ++i) {
            if (pythia.event[i].status() == -21 && abs(pythia.event[i].id()) == 4) {
                std::cout << "Event: " << iEvent + 1 << std::endl;
                std::cout << "Incoming charm quark detected!" << std::endl;
                std::cout << "  Particle ID: " << pythia.event[i].id()
                          << ", pT: " << pythia.event[i].pT()
                          << ", Status: " << pythia.event[i].status() << std::endl;

                std::cout << "Subprocess code: " << pythia.info.code() << std::endl;
                std::cout << "Subprocess name: " << pythia.info.name() << std::endl;

                break;
            }
        }
    }

    
    return 0;
}