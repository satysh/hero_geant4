#include "OpticalPhysicsWithoutCherenkov.hh"

#include "G4Cerenkov.hh"
#include "G4EmSaturation.hh"
#include "G4LossTableManager.hh"
#include "G4OpAbsorption.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4OpRayleigh.hh"
#include "G4OpMieHG.hh"
#include "G4OpticalParameters.hh"
#include "G4OpWLS.hh"
#include "G4OpWLS2.hh"
#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"
#include "G4Scintillation.hh"

// factory
#include "G4PhysicsConstructorFactory.hh"
G4_DECLARE_PHYSCONSTR_FACTORY(G4OpticalPhysics);

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4OpticalPhysics::G4OpticalPhysics(G4int verbose, const G4String& name)
  : G4VPhysicsConstructor(name)
{
  verboseLevel = verbose;
  G4OpticalParameters::Instance()->SetVerboseLevel(verboseLevel);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4OpticalPhysics::~G4OpticalPhysics() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void G4OpticalPhysics::PrintStatistics() const
{
  G4OpticalParameters::Instance()->Dump();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void G4OpticalPhysics::ConstructParticle()
{
  G4OpticalPhoton::OpticalPhotonDefinition();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void G4OpticalPhysics::ConstructProcess()
{
  if(verboseLevel > 0)
    G4cout << "G4OpticalPhysics:: Add Optical Physics Processes" << G4endl;

  auto params = G4OpticalParameters::Instance();

  // Add Optical Processes

  G4ProcessManager* pManager =
    G4OpticalPhoton::OpticalPhoton()->GetProcessManager();
  if(!pManager)
  {
    G4ExceptionDescription ed;
    ed << "Optical Photon without a Process Manager";
    G4Exception("G4OpticalPhysics::ConstructProcess()", "", FatalException, ed);
    return;
  }

  G4OpAbsorption* absorption = new G4OpAbsorption();
  if(params->GetProcessActivation("OpAbsorption"))
    pManager->AddDiscreteProcess(absorption);

  G4OpRayleigh* rayleigh = new G4OpRayleigh();
  if(params->GetProcessActivation("OpRayleigh"))
    pManager->AddDiscreteProcess(rayleigh);

  G4OpMieHG* mie = new G4OpMieHG();
  if(params->GetProcessActivation("OpMieHG"))
    pManager->AddDiscreteProcess(mie);

  G4OpBoundaryProcess* boundary = new G4OpBoundaryProcess();
  if(params->GetProcessActivation("OpBoundary"))
    pManager->AddDiscreteProcess(boundary);

  G4OpWLS* wls = new G4OpWLS();
  if(params->GetProcessActivation("OpWLS"))
    pManager->AddDiscreteProcess(wls);

  G4OpWLS2* wls2 = new G4OpWLS2();
  if(params->GetProcessActivation("OpWLS2"))
    pManager->AddDiscreteProcess(wls2);

  G4Scintillation* scint       = new G4Scintillation();
  G4EmSaturation* emSaturation = G4LossTableManager::Instance()->EmSaturation();
  scint->AddSaturation(emSaturation);

  G4Cerenkov* cerenkov = new G4Cerenkov();

  auto myParticleIterator = GetParticleIterator();
  myParticleIterator->reset();

  while((*myParticleIterator)())
  {
    G4ParticleDefinition* particle = myParticleIterator->value();
    G4String particleName          = particle->GetParticleName();

    pManager = particle->GetProcessManager();
    if(!pManager)
    {
      G4ExceptionDescription ed;
      ed << "Particle " << particleName << "without a Process Manager";
      G4Exception("G4OpticalPhysics::ConstructProcess()", "", FatalException,
                  ed);
      return;  // else coverity complains for pManager use below
    }

    if(cerenkov->IsApplicable(*particle) &&
       params->GetProcessActivation("Cerenkov"))
    {
      //G4cout << G4endl << G4endl << G4endl << "CERENKOV WAS NOT ADDED !!!!!!" << G4endl << G4endl << G4endl;
      //pManager->AddProcess(cerenkov);
      //pManager->SetProcessOrdering(cerenkov, idxPostStep);
    }
    if(scint->IsApplicable(*particle) &&
       params->GetProcessActivation("Scintillation"))
    {
      pManager->AddProcess(scint);
      pManager->SetProcessOrderingToLast(scint, idxAtRest);
      pManager->SetProcessOrderingToLast(scint, idxPostStep);
    }
    if(boundary->IsApplicable(*particle) &&
       params->GetProcessActivation("OpBoundary"))
    {
      pManager->SetProcessOrderingToLast(boundary, idxPostStep);
    }
  }

  if(verboseLevel > 1)
    PrintStatistics();
  if(verboseLevel > 0)
    G4cout << "### " << namePhysics << " physics constructed." << G4endl;
}
