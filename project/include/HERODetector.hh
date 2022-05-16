#ifndef HERODETECTOR_HH
#define HERODETECTOR_HH

#include "G4VSensitiveDetector.hh"

class HEROSensitiveDetector : public G4VSensitiveDetector
{
public:
	HEROSensitiveDetector(G4String);
	~HEROSensitiveDetector();
private:
	virtual G4bool ProcessHits(G4Step *, G4TouchableHistory *);

};
#endif
