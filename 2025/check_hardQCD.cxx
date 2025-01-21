#include "Pythia8/Pythia.h" // Include Pythia headers.
using namespace Pythia8; // Let Pythia8:: be implicit.


int main() {
    // Set up generation.
    Pythia pythia; // Declare a Pythia object
    pythia.readString("HardQCD:all = on");           // Switch on process.
    pythia.readString("Beams:eCM = 13600.");         // GeV

    pythia.init(); // Initialize; incoming pp beams is default.

    // Generate event(s).
    pythia.next(); // Generate an(other) event. Fill event record.

    return 0;
}