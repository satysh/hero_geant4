#ifndef HEROACTIONINITIALIZATION_HH
#define HEROACTIONINITIALIZATION_HH

#include "G4VUserActionInitialization.hh"
#include "HEROPrimaryGenerator.hh"
#include "G4String.hh"

class HEROActionInitialization : public G4VUserActionInitialization
{
public:
    HEROActionInitialization();
    ~HEROActionInitialization();
    
    void SetPrimaryGenerator(HEROPrimaryGenerator *primeGen) { fPrimaryGenerator=primeGen; }
    void SetOutFileName(G4String name) { fOutFileName=name; }

    void BuildForMaster() const;
    virtual void Build() const;
private:
    HEROPrimaryGenerator *fPrimaryGenerator;
    G4String fOutFileName="output.root";
};

#endif
