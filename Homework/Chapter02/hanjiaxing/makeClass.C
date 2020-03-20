#define makeClass_cxx
#include "makeClass.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TRandom3.h> 
#include <TMath.h> 
#include <iostream> 

#if !defined(__CLING__) || defined(__ROOTCLING__) 
ClassImp(makeClass);
#endif

void makeClass::Loop()
{
  //   In a ROOT session, you can do:
  //      root> .L makeClass.C
  //      root> makeClass t
  //      root> t.GetEntry(12); // Fill t data members with entry number 12
  //      root> t.Show();       // Show values of entry 12
  //      root> t.Show(16);     // Read and show values of entry 16
  //      root> t.Loop();       // Loop on all entries
  //

  //     This is the loop skeleton where:
  //    jentry is the global entry number in the chain
  //    ientry is the entry number in the current Tree
  //  Note that the argument to GetEntry must be:
  //    jentry for TChain::GetEntry
  //    ientry for TTree::GetEntry and TBranch::GetEntry
  //
  //       To read only selected branches, Insert statements like:
  // METHOD1:
  //    fChain->SetBranchStatus("*",0);  // disable all branches
  //    fChain->SetBranchStatus("branchname",1);  // activate branchname
  // METHOD2: replace line
  //    fChain->GetEntry(jentry);       //read all branches
  //by  b_branchname->GetEntry(ientry); //read only this branch


  const Double_t L=100.;      //cm, half length of the scin.
  const Double_t TRes=1.;     //ns, time resolution(FWHM) of the scintillator.
  const Double_t Vsc=7.5;     //ns/cm, speed of light in the scin.
  const Double_t Lambda=380.; //cm, attenuation lenght of the scin.
  const Double_t QRes=0.1;    //relative energy resolution(FWHM) of the scin. 

  //ADC
  const Double_t ADCgain=60;   //1MeV=600ch.
  const Double_t ADCuPed=100;  //baseline of ADC for upper side
  const Double_t ADCdPed=120;  //                for bottom side
  const Double_t ADCgainv=600;   //1MeV=600ch.
  const Double_t ADCuPedv=100;  //baseline of ADC for upper side
  const Double_t ADCdPedv=120;  //                for bottom side
  const Double_t ADCnoise=10;  //sigma of noise
  const Int_t    ADCoverflow=4095;

  //TDC
  const Double_t TDCch2ns=40.; //1ns=40ch.
  const Int_t TDCoverflow=4095;

  Double_t tu_off = 5.5;  //time offset
  Double_t td_off = 20.4; //time offset
  Double_t tu_offv = 5.5;  //time offset
  Double_t td_offv = 20.4; //time offset

  TRandom3 *gr = new TRandom3(0);

  TFile *opf = new TFile("veto.root","recreate");
  //TTree *opt = new TTree("tree","veto");
  TTree *opt = (TTree*)fChain->Clone(0);

  Double_t xv;
  Double_t ev;
  Double_t tofv;
  Double_t tuv;
  Double_t tdv;
  Double_t quv;
  Double_t qdv;
  Int_t ituv;
  Int_t itdv;
  Int_t iquv;
  Int_t iqdv;
  //opt->Branch("xv",&xv,"xv/D");
  //opt->Branch("ev",&ev,"ev/D");
  //opt->Branch("tofv",&tofv,"tofv/D");
  //opt->Branch("tuv",&tuv,"tuv/D");
  //opt->Branch("tdv",&tdv,"tdv/D");
  //opt->Branch("quv",&quv,"quv/D");
  //opt->Branch("qdv",&qdv,"qdv/D");
  //opt->Branch("ituv",&ituv,"ituv/I");
  //opt->Branch("itdv",&itdv,"itdv/I");
  //opt->Branch("iquv",&iquv,"iquv/I");
  //opt->Branch("iqdv",&iqdv,"iqdv/I");

  if (fChain == 0) return;
  Long64_t nentries = fChain->GetEntriesFast();
  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    // if (Cut(ientry) < 0) continue;


    xv = x;

    if(jentry%100000==0 || jentry==999999) std::cout << jentry << "/" << nentries << "    " << x << "    " << xv << std::endl;

    //if(pid==2) {
    //  ev = e*0.1;
    //  e = e*0.9;
    //  tofv = tof;

    //  tu = tof+(L-x)/Vsc+gr->Gaus(0,TRes/2.35)+tu_off;
    //  td = tof+(L+x)/Vsc+gr->Gaus(0,TRes/2.35)+td_off;
    //  tu *= TDCch2ns;
    //  td *= TDCch2ns;

    //  tuv = tofv+(L-x)/Vsc+gr->Gaus(0,TRes/2.35)+tu_offv;
    //  tdv = tofv+(L+x)/Vsc+gr->Gaus(0,TRes/2.35)+td_offv;
    //  tuv *= TDCch2ns;
    //  tdv *= TDCch2ns;

    //  Double_t q0 = e*ADCgain; //charged particle
    //  //q0 = gr->Gaus(q0,q0*QRes/2.35);
    //  qu = q0*TMath::Exp(-(L-x)/Lambda);
    //  qd = q0*TMath::Exp(-(L+x)/Lambda);
    //  qu = gr->Gaus(qu,qu*QRes/2.35);
    //  qd = gr->Gaus(qd,qd*QRes/2.35);
    //  
    //  Double_t q0v = ev*ADCgainv;
    //  //q0v = gr->Gaus(q0v,q0v*QRes/2.35);
    //  quv = q0v*TMath::Exp(-(L-x)/Lambda);
    //  qdv = q0v*TMath::Exp(-(L+x)/Lambda);
    //  quv = gr->Gaus(quv,quv*QRes/2.35);
    //  qdv = gr->Gaus(qdv,qdv*QRes/2.35);

    //  if(qu<100) tu = TDCoverflow;
    //  if(qd<100) td = TDCoverflow;
    //  if(quv<100) tuv = TDCoverflow;
    //  if(qdv<100) tdv = TDCoverflow;

    //  qu += gr->Gaus(ADCuPed,ADCnoise);
    //  qd += gr->Gaus(ADCdPed,ADCnoise);
    //  if(qu<0) qu=0;
    //  if(qd<0) qd=0;

    //  quv += gr->Gaus(ADCuPedv,ADCnoise);
    //  qdv += gr->Gaus(ADCdPedv,ADCnoise);
    //  if(quv<0) quv=0;
    //  if(qdv<0) qdv=0;
    //}
    //else {
    //  ev = 0;
    //  tofv = -1;
    //  tuv = TDCoverflow;
    //  tdv = TDCoverflow;
    //  quv = ADCuPedv + gr->Gaus(0,ADCnoise);
    //  qdv = ADCdPedv + gr->Gaus(0,ADCnoise);
    //}

    ////overflow check
    //if(tu>TDCoverflow) tu=TDCoverflow;
    //if(td>TDCoverflow) td=TDCoverflow;
    //if(qu>ADCoverflow) qu=ADCoverflow;
    //if(qd>ADCoverflow) qd=ADCoverflow;

    //if(tuv>TDCoverflow) tuv=TDCoverflow;
    //if(tdv>TDCoverflow) tdv=TDCoverflow;
    //if(quv>ADCoverflow) quv=ADCoverflow;
    //if(qdv>ADCoverflow) qdv=ADCoverflow;

    ////digitization
    //itu = Int_t(tu);
    //itd = Int_t(td);
    //iqu = Int_t(qu);
    //iqd = Int_t(qd);

    //ituv = Int_t(tuv);
    //itdv = Int_t(tdv);
    //iquv = Int_t(quv);
    //iqdv = Int_t(qdv);

    opt->Fill();
  }

  opf->Write();
  opf->Close();
}
