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
//
/// \file medical/DICOM/src/DicomPrimaryGeneratorAction.cc
/// \brief Implementation of the DicomPrimaryGeneratorAction class
//
// The code was written by :
//      *Louis Archambault louis.archambault@phy.ulaval.ca,
//      *Luc Beaulieu beaulieu@phy.ulaval.ca
//      +Vincent Hubert-Tremblay at tigre.2@sympatico.ca
//
//
// *Centre Hospitalier Universitaire de Quebec (CHUQ),
// Hotel-Dieu de Quebec, departement de Radio-oncologie
// 11 cote du palais. Quebec, QC, Canada, G1R 2J6
// tel (418) 525-4444 #6720
// fax (418) 691 5268
//
// + Université Laval, Québec (QC) Canada
//*******************************************************
#include "DicomPrimaryGeneratorAction.hh"

#include "CLHEP/Random/RandFlat.h"
#include "DicomRegularDetectorConstruction.hh"

#include "G4Event.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
DicomPrimaryGeneratorAction::DicomPrimaryGeneratorAction()
  : G4VUserPrimaryGeneratorAction(), fParticleGun(0)
{
  G4int nParticle = 1;
  fParticleGun = new G4ParticleGun(nParticle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
DicomPrimaryGeneratorAction::~DicomPrimaryGeneratorAction()
{
  delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DicomPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  G4ParticleDefinition* particle = particleTable->FindParticle(particleName = "gamma");
  fParticleGun->SetParticleDefinition(particle);
  // 以Z轴正方向为中心，圆锥角30度内随机分布
  G4double cosThetaMin = std::cos(CLHEP::pi/6.); // 30度
  G4double cosTheta = (1.0 - cosThetaMin) * G4UniformRand() + cosThetaMin; // [cos(30°), 1]
  G4double sinTheta = std::sqrt(1.0 - cosTheta * cosTheta);
  G4double phi = 2.0 * CLHEP::pi * G4UniformRand(); // [0, 2π]
  G4ThreeVector dir(sinTheta * std::cos(phi), sinTheta * std::sin(phi), cosTheta);
  dir /= dir.mag();
  fParticleGun->SetParticleMomentumDirection(dir);
  fParticleGun->SetParticleEnergy(10. * MeV);
  // put it at SAD = 1 m on xy plane of central slice
  fParticleGun->SetParticlePosition(G4ThreeVector(0., 0., -120.));
  // fParticleGun->SetParticlePosition(G4ThreeVector(0.,0.,-22.));
  fParticleGun->GeneratePrimaryVertex(anEvent);
}
