#include "HEROActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"

namespace HERO
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HEROActionInitialization::HEROActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HEROActionInitialization::~HEROActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HEROActionInitialization::BuildForMaster() const
{
  EventAction* eventAction = new EventAction;
  SetUserAction(new RunAction(eventAction));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HEROActionInitialization::Build() const
{
  SetUserAction(new PrimaryGeneratorAction);

  auto eventAction = new EventAction;
  SetUserAction(eventAction);

  SetUserAction(new RunAction(eventAction));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}
