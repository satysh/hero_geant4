#ifndef HERORUNACTION_HH
#define HERORUNACTION_HH

#include "G4UserRunAction.hh"
#include "G4Run.hh"
#include "G4String.hh"

class HERORunAction : public G4UserRunAction
{
public:
	HERORunAction();
	~HERORunAction();
    
    void SetOutFileName(G4String name) { fOutFileName=name; }

	virtual void BeginOfRunAction(const G4Run*);
	virtual void EndOfRunAction(const G4Run*);
private:
	G4String fOutFileName;
};
#endif
