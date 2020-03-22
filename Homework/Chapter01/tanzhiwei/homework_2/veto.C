#define veto_cxx
#include "veto.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void veto::Loop()
{
//   In a ROOT session, you can do:
//      root> .L veto.C
//      root> veto t
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
   TFile *opf=new TFile("veto.root","recreate");
   TTree *tree=new TTree("tree","tree");
   tree->Branch("qu_dp",&qu_dp,"qu_dp/D");
   tree->Branch("qd_dp",&qd_dp,"qd_dp/D");
   tree->Branch("tu_dp",&tu_dp,"tu_dp/D");
   tree->Branch("td_dp",&td_dp,"td_dp/D");
   tree->Branch("tof_dp",&tof_dp,"tof_dp/D");
   tree->Branch("iqu_dp",&iqu_dp,"iqu_dp/I");
   tree->Branch("iqd_dp",&iqd_dp,"iqd_dp/I");
   tree->Branch("itu_dp",&itu_dp,"itu_dp/I");
   tree->Branch("itd_dp",&itd_dp,"itd_dp/I");
   tree->Branch("flag_t",&flag_t,"flag_t/I");
   tree->Branch("flag_e",&flag_e,"flag_e/I");

   tree->Branch("x",&x,"x/D");
   tree->Branch("e",&e,"e/D");
   tree->Branch("tof",&tof,"tof/D");
   tree->Branch("ctof",&ctof,"ctof/D");
   tree->Branch("tu",&tu,"tu/D");
   tree->Branch("td",&td,"td/D");
   tree->Branch("qu",&qu,"qu/D");
   tree->Branch("qd",&qd,"qd/D");
   tree->Branch("itu",&itu,"itu/I");
   tree->Branch("itd",&itd,"itd/I");
   tree->Branch("iqu",&iqu,"iqu/I");
   tree->Branch("iqd",&iqd,"iqd/I");
   tree->Branch("pid",&pid,"pid/I");


   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;

   Double_t D=497;//distance from target to plastic scintillator detector
   Double_t L=100;
   Double_t dD=1;//thickness of plastic scintillator detector
   Double_t TRes=1;
   Double_t Lambda=380;
   Double_t QRes=0.1;
   Double_t Vsc=7.5;

   Double_t ADCgain=60;
   Double_t ADCuPed=100;
   Double_t ADCdPed=120;
   Double_t ADCnoise=10;
   Int_t ADCoverflow=4095;

   Double_t TDCch2ns=40;
   Int_t TDCoverflow=4095;
   Double_t tu_off=5.5;
   Double_t td_off=20.4;
   TRandom3 *gr=new TRandom3(0);
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
	  if(pid==2){
		  q0_dp=0.1*e*ADCgain;
		  //tof_dp,tu_dp,td_dp
		  Double_t Dr=D+gr->Uniform(-0.5,0.5)*dD;
		  Double_t d=TMath::Sqrt(Dr*Dr+x*x);
		  tof_dp=72./TMath::Sqrt(e)*(d*0.01);
		  tu_dp=tof_dp+(L-x)/Vsc+gr->Gaus(0,TRes/2.35)+tu_off;
		  td_dp=tof_dp+(L+x)/Vsc+gr->Gaus(0,TRes/2.35)+td_off;
		  tu_dp*=TDCch2ns;
		  td_dp*=TDCch2ns;
		  //qu_dp,qd_dp
		  q0_dp=gr->Gaus(q0_dp,q0_dp*QRes/2.35);
		  qu_dp=q0_dp*TMath::Exp(-(L-x)/Lambda);
		  qu_dp+=gr->Gaus(ADCuPed,ADCnoise);
		  qd_dp=q0_dp*TMath::Exp(-(L+x)/Lambda);
		  qd_dp+=gr->Gaus(ADCdPed,ADCnoise);
         //
		  //qu_dn,qd_dn
		  q0_dn=gr->Gaus(q0_dn,q0_dn*QRes/2.35);
		  qu=q0_dn*TMath::Exp(-(L-x)/Lambda);
		  qu+=gr->Gaus(ADCuPed,ADCnoise);
		  qd=q0_dn*TMath::Exp(-(L+x)/Lambda);
		  qd+=gr->Gaus(ADCdPed,ADCnoise);
		  if(qu<100) tu=TDCoverflow;
		  if(qd<100) td=TDCoverflow;
		  if(qu<0) qu=0;
		  if(qd<0) qd=0;
		  }
	  else{
		  ///qu_dp,qd_dp
		  qu_dp=gr->Gaus(ADCuPed,ADCnoise);
		  qd_dp=gr->Gaus(ADCuPed,ADCnoise);
		  //tu_dp,td_dp
		  tof_dp=0;
		  tu_dp=TDCoverflow;
		  td_dp=TDCoverflow;
		  }
	  if(qu_dp<100) tu_dp=TDCoverflow;
	  if(qd_dp<100) td_dp=TDCoverflow;
	  if(qu_dp<0) qu_dp=0;
	  if(qd_dp<0) qd_dp=0;
	  if(tu_dp>TDCoverflow) tu_dp=TDCoverflow;
	  if(td_dp>TDCoverflow) td_dp=TDCoverflow;
	  if(qu_dp>ADCoverflow) qu_dp=ADCoverflow;
	  if(qd_dp>ADCoverflow) qd_dp=ADCoverflow;
	  itu_dp=Int_t(tu_dp);
	  itd_dp=Int_t(td_dp);
	  iqu_dp=Int_t(qu_dp);
	  iqd_dp=Int_t(qd_dp);
	  flag_e=label_e(iqu_dp,iqd_dp);
	  flag_t=label_t(itu_dp,itd_dp);
	  tree->Fill();
   }
   tree->Write();
   opf->Close();
}

Int_t veto::label_e(Int_t iqu_dp,Int_t iqd_dp){
	if(iqu_dp>(100+3*10)||iqd_dp>(100+3*10)) return 2;
	else return 0;
	}
Int_t veto::label_t(Int_t itu_dp,Int_t itd_dp){
	if(itu_dp<4095||itd_dp<4095) return 2;
	else return 0;
    }
