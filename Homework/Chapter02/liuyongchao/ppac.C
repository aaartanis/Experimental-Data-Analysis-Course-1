#define ppac_cxx
#include "ppac.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void ppac::Loop()
{
//   In a ROOT session, you can do:
//      root> .L ppac.C
//      root> ppac t
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
 	const Double_t D=500;
	const Double_t L=100;
	const Double_t dD=1.;
	const Double_t TRes=1.;
	const Double_t Lambda=380.;
	const Double_t QRes=0.1;
	const Double_t Vsc=7.5;
	const Double_t En0=100.;
	const Double_t EnRes=50;
	const Double_t Eg0=10.;

	const Double_t Rn=0.75;
	const Double_t Rg=0.05;

	const Double_t Rc=0.1;
	const Double_t Ec0=50;
	const Double_t EcRes=50;

	const Double_t ADCgain=60;
	const Double_t ADCuPed=100;
	const Double_t ADCdPed=120;
	const Double_t ADCnoise=10;
	const Int_t ADCoverflow=4095;

	const Double_t TDCch2ns=40.;
	const Int_t TDCoverflow=4095;
	
	Double_t tu_off=5.5;
	Double_t td_off=20.4;

	TFile *opf = new TFile("ppac.root","recreate");
	TTree *tree = new TTree("tree","ppac");
	//tree of veto
	Double_t ppac_x,ppac_e,ppac_tof,ppac_tu,ppac_td,ppac_qu,ppac_qd;
	Int_t ppac_pid,ppac_itu,ppac_itd,ppac_iqu,ppac_iqd;
	tree->Branch("ppac_x",&ppac_x,"ppac_x/D");
	tree->Branch("ppac_e",&ppac_e,"ppac_e/D");
	tree->Branch("ppac_tof",&ppac_tof,"ppac_tof/D");
	tree->Branch("ppac_pid",&ppac_pid,"ppac_pid/I");

	tree->Branch("ppac_tu",&ppac_tu,"tu/D");
	tree->Branch("ppac_td",&ppac_td,"ppac_td/D");
	tree->Branch("ppac_qu",&ppac_qu,"ppac_qu/D");
	tree->Branch("ppac_qd",&ppac_qd,"ppac_qd/D");
	tree->Branch("ppac_itu",&ppac_itu,"ppac_itu/I");
	tree->Branch("ppac_itd",&ppac_itd,"ppac_itd/I");
	tree->Branch("ppac_iqu",&ppac_iqu,"ppac_iqu/I");
	tree->Branch("ppac_iqd",&ppac_iqd,"ppac_iqd/I");
	//tree of tof
	Double_t tof_x,tof_e,tof_tof,tof_tu,tof_td,tof_qu,tof_qd;
	Int_t tof_pid,tof_itu,tof_itd,tof_iqu,tof_iqd;
	tree->Branch("tof_x",&tof_x,"tof_x/D");
	tree->Branch("tof_e",&tof_e,"tof_e/D");
	tree->Branch("tof_tof",&tof_tof,"tof_tof/D");
	tree->Branch("tof_pid",&tof_pid,"tof_pid/I");

	tree->Branch("tof_tu",&tof_tu,"tu/D");
	tree->Branch("tof_td",&tof_td,"tof_td/D");
	tree->Branch("tof_qu",&tof_qu,"tof_qu/D");
	tree->Branch("tof_qd",&tof_qd,"tof_qd/D");
	tree->Branch("tof_itu",&tof_itu,"tof_itu/I");
	tree->Branch("tof_itd",&tof_itd,"tof_itd/I");
	tree->Branch("tof_iqu",&tof_iqu,"tof_iqu/I");
	tree->Branch("tof_iqd",&tof_iqd,"ppac_iqd/I");	
//Ttree *fchain - Pointer pointing to the tree
	TRandom3 *gr=new TRandom3(0);

	if (fChain == 0) return;

	Long64_t nentries = fChain->GetEntriesFast();

	Long64_t nbytes = 0, nb = 0;
	for (Long64_t jentry=0; jentry<nentries;jentry++) {
		Long64_t ientry = LoadTree(jentry);
		if (ientry < 0) break;
		nb = fChain->GetEntry(jentry);   nbytes += nb;
		// if (Cut(ientry) < 0) continue;
		Double_t Dr=D+gr->Uniform(-0.5,0.5)*dD;
		Double_t d=TMath::Sqrt(Dr*Dr+x*x);
		if (pid==2)
		{

			/////////////////////////////veto//////////////////////////////
			ppac_tof=72./TMath::Sqrt(e)*(d*0.01);
			ppac_tu=ppac_tof+(L-x)/Vsc+gr->Gaus(0,TRes/2.35)+tu_off;
			ppac_td=ppac_tof+(L+x)/Vsc+gr->Gaus(0,TRes/2.35)+td_off;
			ppac_tu *=TDCch2ns;
			ppac_td *=TDCch2ns;

			Double_t q0;
			q0=e*0.1*ADCgain;
			q0=gr->Gaus(q0,q0*QRes/2.35);

			//light transmission within the plastic
			ppac_qu=q0*TMath::Exp(-(L-x)/Lambda);
			ppac_qd=q0*TMath::Exp(-(L+x)/Lambda);

			//threshold of time of tu and td
			if(ppac_qu<100) ppac_tu=TDCoverflow;
			if(ppac_qd<100) ppac_td=TDCoverflow; 
			//ADC
			ppac_qu +=gr->Gaus(ADCuPed,ADCnoise);
			ppac_qd +=gr->Gaus(ADCdPed,ADCnoise);

			if(ppac_qu<0) ppac_qu=0;
			if(ppac_qd<0) ppac_qd=0;
			
			if(ppac_tu>TDCoverflow) ppac_tu=TDCoverflow;
			if(ppac_td>TDCoverflow) ppac_td=TDCoverflow;
			if(ppac_qu>ADCoverflow) ppac_qu=ADCoverflow;
			if(ppac_qd>ADCoverflow) ppac_qd=ADCoverflow;

			//digitization
			ppac_x=x;
			ppac_e=e*0.1;
			ppac_pid=2;
			ppac_itu=Int_t(ppac_tu);
			ppac_itd=Int_t(ppac_td);
			ppac_iqu=Int_t(ppac_qu);
			ppac_iqd=Int_t(ppac_qd);

			//////////////////////////tof/////////////////////////
			Double_t q1;
			q1=e*0.9*ADCgain;
			q1=gr->Gaus(q1,q1*QRes/2.35);

			//light transmission within the plastic
			tof_qu=q1*TMath::Exp(-(L-x)/Lambda);
			tof_qd=q1*TMath::Exp(-(L+x)/Lambda);
			tof_td=td;
			tof_tu=tu;
			//threshold of time of tu and td
			if(tof_qu<100) tof_tu=TDCoverflow;
			if(tof_qd<100) tof_td=TDCoverflow; 
			//ADC
			tof_qu +=gr->Gaus(ADCuPed,ADCnoise);
			tof_qd +=gr->Gaus(ADCdPed,ADCnoise);

			if(tof_qu<0) tof_qu=0;
			if(tof_qd<0) tof_qd=0;
			
			if(tof_tu>TDCoverflow) tof_tu=TDCoverflow;
			if(tof_td>TDCoverflow) tof_td=TDCoverflow;
			if(tof_qu>ADCoverflow) tof_qu=ADCoverflow;
			if(tof_qd>ADCoverflow) tof_qd=ADCoverflow;

			//digitization
			tof_pid=2;
			tof_x=x;
			tof_tof=tof;
			tof_e=e*0.9;

			tof_itu=Int_t(tof_tu);
			tof_itd=Int_t(tof_td);
			tof_iqu=Int_t(tof_qu);
			tof_iqd=Int_t(tof_qd);

		}
		else
		{
			//////////////tof//////////////////////
			ppac_pid=pid;
			ppac_td=TDCoverflow;
			ppac_tu=TDCoverflow;
			ppac_itd=Int_t(ppac_itd);
			ppac_itu=Int_t(ppac_tu);
			ppac_qd=gr->Gaus(ADCdPed,ADCnoise);
			ppac_qu=gr->Gaus(ADCuPed,ADCnoise);
			ppac_iqd=Int_t(ppac_qd);
			ppac_iqu=Int_t(ppac_qu);
			ppac_x=-150;
			ppac_e=-1;
			ppac_tof=-1;
			///////////////tof//////////////////////
			tof_pid=pid;
			tof_iqd=iqd;
			tof_iqu=iqu;
			tof_itd=itd;
			tof_itu=itu;
			tof_td=td;
			tof_tu=tu;
			tof_qd=qd;
			tof_qu=qu;
			tof_x=x;
			tof_e=e;
			tof_tof=tof;
		}
    	tree->Fill();

	}
	tree->Write();
	opf->Close();
}
