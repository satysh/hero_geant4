#ifndef HERODETECTOR_HH
#define HERODETECTOR_HH

#include "G4VSensitiveDetector.hh"

class HEROSensitiveDetector : public G4VSensitiveDetector
{
public:
	HEROSensitiveDetector(G4String);
	~HEROSensitiveDetector();

	void SetStartEventId(G4int id) { fStartEventId=id; }
	void SetPrimaryStartTime(G4double time) { fPrimaryStartTime=time; }
private:
	virtual G4bool ProcessHits(G4Step *, G4TouchableHistory *);

private:
	G4int fStartEventId=0;
	G4double fPrimaryStartTime=0.;
};
#endif
