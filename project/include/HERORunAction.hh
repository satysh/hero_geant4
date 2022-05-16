#ifndef HERORUNACTION_HH
#define HERORUNACTION_HH

#include "G4UserRunAction.hh"
#include "G4Run.hh"

class HERORunAction : public G4UserRunAction
{
public:
	HERORunAction();
	~HERORunAction();

	virtual void BeginOfRunAction(const G4Run*);
	virtual void EndOfRunAction(const G4Run*);
};
#endif
