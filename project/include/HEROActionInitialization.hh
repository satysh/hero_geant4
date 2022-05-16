#ifndef HEROACTIONINITIALIZATION_HH
#define HEROACTIONINITIALIZATION_HH

#include "G4VUserActionInitialization.hh"
#include "HERORunAction.hh"
#include "HEROPrimaryGenerator.hh"

class HEROActionInitialization : public G4VUserActionInitialization
{
public:
    HEROActionInitialization();
    ~HEROActionInitialization();

    void SetRunAction(HERORunAction *runAction) { fRunAction=runAction; }
    void SetPrimaryGenerator(HEROPrimaryGenerator *primeGen) { fPrimaryGenerator=primeGen; }
    void BuildForMaster() const;
    virtual void Build() const;
private:
    HERORunAction *fRunAction;
    HEROPrimaryGenerator *fPrimaryGenerator;
};

#endif
