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

//from this line //

//const define from tree.c
//maybe useful,just leave it here
    const Double_t D=500.;//cm, distance between target and the scin.(Center)
    const Double_t L=100.;//cm, half length of the scin.
    const Double_t dD=5.;//cm, thickness of the scin.
    const Double_t TRes=1.;//ns, time resolution(FWHM) of the scintillator.
    const Double_t Lambda=380.;//cm, attenuation lenght of the scin.
    const Double_t QRes=0.1;//relative energy resolution(FWHM) of the scin. 
    const Double_t Vsc=7.5;//ns/cm, speed of light in the scin.
//neutron & gamma
    const Double_t En0=50;//MeV, average neutron energy
    const Double_t EnRes=50.;//MeV, energy spread of neutron(FWHM)
    const Double_t Eg0=10;//MeV, gamma energy  

    const Double_t Rn=0.75;//ratio of neutron 
    const Double_t Rg=0.05;//ratio of gamma; 
//charged particle hited in Neutron Detector, 
    const Double_t Rc=0.1;//ratio of proton
    const Double_t Ec0=50;//MeV
    const Double_t EcRes=50;//MeV energy spread of proton(FWHM)
//ADC
    const Double_t ADCgain=60;//1MeV=60ch.
    const Double_t ADCuPed=140;//baseline of ADC for upper side
    const Double_t ADCdPed=130;//                for bottom side
    const Double_t ADCnoise=50;//sigma of noise
    const Int_t    ADCoverflow=4095;
//TDC
    const Double_t TriggerDelay=15;//ns, trigger延迟20ns,将感兴趣的时间信号放在TDC量程以内。
    const Double_t TDCch2ns=40.;//1ns=40ch.
    const Int_t TDCoverflow=4095;

    Double_t tu_off=5.5;//time offset
    Double_t td_off=20.4;//time offset

//veto detector's tree define
    TFile *opf=new TFile("ppac.root","recreate");//新文件ppac.root的指针 *opf
    TTree *tree=new TTree("tree","ppac");//新tree的指针 *opt

//Set tree veto branch
    Double_t veto_x;
    Double_t veto_e;
    Double_t veto_tof;
    Double_t veto_tu;
    Double_t veto_td;
    Double_t veto_qu;
    Double_t veto_qd;

    Int_t veto_pid;
    Int_t veto_itu;
    Int_t veto_itd;
    Int_t veto_iqd;
    Int_t veto_iqu;
//Set tree tof branch
    Double_t tof_x;
    Double_t tof_e;
    Double_t tof_tof;
    Double_t tof_tu;
    Double_t tof_td;
    Double_t tof_qu;
    Double_t tof_qd;

    Int_t tof_pid;
    Int_t tof_itu;
    Int_t tof_itd;
    Int_t tof_iqd;
    Int_t tof_iqu;

    tree->Branch("veto_x",&veto_x,"veto_x/D");
    tree->Branch("veto_e",&veto_e,"veto_e/D");
    tree->Branch("veto_tof",&veto_tof,"veto_tof/D");
    tree->Branch("veto_pid",&veto_pid,"veto_pid/I");
    tree->Branch("veto_tu",&veto_tu,"veto_tu/D");
    tree->Branch("veto_td",&veto_td,"veto_td/D");
    tree->Branch("veto_qu",&veto_qu,"veto_qu/D");
    tree->Branch("veto_qd",&veto_qd,"veto_qd/D");
    tree->Branch("veto_itu",&veto_itu,"veto_itu/I");
    tree->Branch("veto_itd",&veto_itd,"veto_itd/I");
    tree->Branch("veto_iqu",&veto_iqu,"veto_iqu/I");
    tree->Branch("veto_iqd",&veto_iqd,"veto_iqd/I");
    tree->Branch("tof_x",&tof_x,"tof_x/D");
    tree->Branch("tof_e",&tof_e,"tof_e/D");
    tree->Branch("tof_tof",&tof_tof,"tof_tof/D");
    tree->Branch("tof_pid",&tof_pid,"tof_pid/I");
    tree->Branch("tof_tu",&tof_tu,"tof_tu/D");
    tree->Branch("tof_td",&tof_td,"tof_td/D");
    tree->Branch("tof_qu",&tof_qu,"tof_qu/D");
    tree->Branch("tof_qd",&tof_qd,"tof_qd/D");
    tree->Branch("tof_itu",&tof_itu,"tof_itu/I");
    tree->Branch("tof_itd",&tof_itd,"tof_itd/I");
    tree->Branch("tof_iqu",&tof_iqu,"tof_iqu/I");
    tree->Branch("tof_iqd",&tof_iqd,"tof_iqd/I");

    TRandom3 *gr= new TRandom3(0); 
    if (fChain == 0) return;

    Long64_t nentries = fChain->GetEntriesFast();
    Long64_t nbytes = 0, nb = 0;
    for (Long64_t jentry=0; jentry<nentries;jentry++) {
        Long64_t ientry = LoadTree(jentry);
        if (ientry < 0) break;
        nb = fChain->GetEntry(jentry);   nbytes += nb;
        //if (Cut(ientry) < 0) continue;
        Double_t Dr = D+gr->Uniform(-0.5,0.5)*dD;
        Double_t d=TMath::Sqrt(Dr*Dr+x*x);//cm flight path
        //use pid to separate partical
        if(pid==2){//proton
        //put the web code about proton 
            veto_pid=pid;
            veto_x=x;
            veto_e=e*0.1;
            veto_tof=72./TMath::Sqrt(e)*(d*0.01);
            //TDC
            veto_tu=veto_tof+(L-x)/Vsc+gr->Gaus(0,TRes/2.35)+tu_off;
            veto_td=veto_tof+(L+x)/Vsc+gr->Gaus(0,TRes/2.35)+td_off;
            veto_tu*=TDCch2ns;
            veto_td*=TDCch2ns;

            Double_t q0;
            q0=e*ADCgain*0.1;
            q0=q0*gr->Gaus(q0,q0*QRes/2.35);
            veto_qu=q0*TMath::Exp(-(L-x)/Lambda);
            veto_qd=q0*TMath::Exp(-(L+x)/Lambda);
            //ADC
            if(veto_qu<0) veto_qu=0;//no nagatetive values
            if(veto_qd<0) veto_qd=0;
            veto_qu += gr->Gaus(ADCuPed,ADCnoise);
            veto_qd += gr->Gaus(ADCdPed,ADCnoise);

            if(veto_qu<100) veto_tu=TDCoverflow;//threshold of time of tu and td
            if(veto_qd<100) veto_td=TDCoverflow;
            //overflow check
            if(veto_tu>TDCoverflow) veto_tu=TDCoverflow;
            if(veto_td>TDCoverflow) veto_td=TDCoverflow;
            if(veto_qu>ADCoverflow) veto_qu=ADCoverflow;
            if(veto_qd>ADCoverflow) veto_qd=ADCoverflow;
            //digtization
            veto_itu=Int_t(veto_tu);
            veto_itd=Int_t(veto_td);
            veto_iqu=Int_t(veto_qu);
            veto_iqd=Int_t(veto_qd);
            //from this line wo define tof variate
            tof_x = x;
            tof_e = 0.9*e;
            tof_pid = pid;
            tof_tof = tof;
            tof_tu = tu;
            tof_td = td;
            tof_qu = qu;
            tof_qd = qd;
            //ADC
            if(tof_qu<0) tof_qu=0;//no nagatetive values
            if(tof_qd<0) tof_qd=0;
            tof_qu += gr->Gaus(ADCuPed,ADCnoise);
            tof_qd += gr->Gaus(ADCdPed,ADCnoise);

            if(tof_qu<100) tof_tu=TDCoverflow;//threshold of time of tu and td
            if(tof_qd<100) tof_td=TDCoverflow;
            //overflow check
            if(tof_tu>TDCoverflow) tof_tu=TDCoverflow;
            if(tof_td>TDCoverflow) tof_td=TDCoverflow;
            if(tof_qu>ADCoverflow) tof_qu=ADCoverflow;
            if(tof_qd>ADCoverflow) tof_qd=ADCoverflow;
            //digtization
            tof_itu=Int_t(tof_tu);
            tof_itd=Int_t(tof_td);
            tof_iqu=Int_t(tof_qu);
            tof_iqd=Int_t(tof_qd);
        }
        else{//gamma and neutron
            veto_x=-300;//make veto do not detect them
            veto_e=-100;
            veto_tof=-100;
            veto_pid=pid;
            //tdc
            veto_tu=TDCoverflow;
            veto_td=TDCoverflow;
            veto_itu=Int_t(veto_tu);
            veto_itd=Int_t(veto_td);
            //adc
            veto_qu=gr->Gaus(ADCdPed,ADCnoise);
            veto_qd=gr->Gaus(ADCdPed,ADCnoise);
            veto_iqu=Int_t(veto_qu);
            veto_iqd=Int_t(veto_qd);
            //from this line wo define tof variate
            tof_x=x;//make veto do not detect them
            tof_e=e;
            tof_tof=tof;
            tof_pid=pid;
            //tdc
            tof_tu=tu;
            tof_td=td;
            tof_itu=itu;
            tof_itd=itd;
            //adc
            tof_qu=qu;
            tof_qd=qd;
            tof_iqu=iqu;
            tof_iqd=iqd;
        }
        tree->Fill();
    }
    tree->Write();
    opf->Close();
}
