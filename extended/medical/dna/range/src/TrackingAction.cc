//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// This example is provided by the Geant4-DNA collaboration
// Any report or published results obtained using the Geant4-DNA software
// shall cite the following Geant4-DNA collaboration publications:
// Med. Phys. 45 (2018) e722-e739
// Phys. Med. 31 (2015) 861-874
// Med. Phys. 37 (2010) 4692-4708
// Int. J. Model. Simul. Sci. Comput. 1 (2010) 157–178
//
// The Geant4-DNA web site is available at http://geant4-dna.org
//
/// \file medical/dna/range/src/TrackingAction.cc
/// \brief Implementation of the TrackingAction class

#include "TrackingAction.hh"

#include "HistoManager.hh"
#include "PrimaryGeneratorAction.hh"
#include "Run.hh"

#include "G4Positron.hh"
#include "G4RunManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TrackingAction::TrackingAction(PrimaryGeneratorAction* prim)
  : G4UserTrackingAction(), fPrimary(prim)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TrackingAction::PreUserTrackingAction(const G4Track* track)
{
  G4int trackID = track->GetTrackID();
  if (trackID == 1) fTrackLength = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TrackingAction::PostUserTrackingAction(const G4Track* track)
{
  // Histograms
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  G4int trackID = track->GetTrackID();

  Run* run = static_cast<Run*>(G4RunManager::GetRunManager()->GetNonConstCurrentRun());

  //

  G4double tracklen = track->GetTrackLength();

  // *** FOR ELECTRONS

  // Track length of primary particle or charged secondaries
  if (trackID == 1 && track->GetDefinition()->GetPDGCharge() == -1) {
    run->AddTrackLength(tracklen);
    analysisManager->FillH1(1, tracklen);
  };

  // Extract projected range of primary particle
  if (trackID == 1 && track->GetDefinition()->GetPDGCharge() == -1) {
    G4double pr =
      (track->GetPosition()) * (fPrimary->GetParticleGun()->GetParticleMomentumDirection());
    run->AddProjRange(pr);
    analysisManager->FillH1(2, pr);
  }

  // Extract penetration range of primary particle
  if (trackID == 1 && track->GetDefinition()->GetPDGCharge() == -1) {
    G4double pene = std::sqrt((track->GetPosition()) * (track->GetPosition()));
    run->AddPenetration(pene);
    analysisManager->FillH1(3, pene);
  };

  // *** FOR Geant4-DNA PROTONS, ALPHA AND CHARGED STATES AND IONS
  // track->GetDefinition()->GetPDGMass() > 0 avoids gammas

  // Track length of primary particle or charged secondaries
  if (track->GetDefinition()->GetPDGCharge() >= 0 && track->GetDefinition()->GetPDGMass() > 0
      && track->GetDefinition() != G4Positron::PositronDefinition())
  {
    // Local cumulation is required before scoring once
    fTrackLength = fTrackLength + tracklen;

    if (track->GetKineticEnergy() == 0) {
      run->AddTrackLength(fTrackLength);
      analysisManager->FillH1(1, fTrackLength);
    }
  }

  // Extract projected range of primary particle
  if (track->GetDefinition()->GetPDGCharge() >= 0 && track->GetDefinition()->GetPDGMass() > 0
      && track->GetDefinition() != G4Positron::PositronDefinition())
  {
    G4double pr =
      (track->GetPosition()) * (fPrimary->GetParticleGun()->GetParticleMomentumDirection());

    if (track->GetKineticEnergy() == 0) {
      run->AddProjRange(pr);
      analysisManager->FillH1(2, pr);
    }
  }

  // Extract penetration range of primary particle
  if (track->GetDefinition()->GetPDGCharge() >= 0 && track->GetDefinition()->GetPDGMass() > 0
      && track->GetDefinition() != G4Positron::PositronDefinition())
  {
    G4double pene = std::sqrt((track->GetPosition()) * (track->GetPosition()));

    if (track->GetKineticEnergy() == 0) {
      run->AddPenetration(pene);
      analysisManager->FillH1(3, pene);
    }
  };
}
