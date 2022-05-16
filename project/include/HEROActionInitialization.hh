#ifndef HEROACTIONINITIALIZATION_HH
#define HEROACTIONINITIALIZATION_HH

#include "G4VUserActionInitialization.hh"

class HEROActionInitialization : public G4VUserActionInitialization
{
public:
    HEROActionInitialization();
    ~HEROActionInitialization();


    void BuildForMaster() const;
    virtual void Build() const;
};

#endif
