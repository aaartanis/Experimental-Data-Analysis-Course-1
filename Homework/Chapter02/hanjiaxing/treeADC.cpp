#include <TROOT.h> 
#include <TFile.h> 
#include <TTree.h> 
#include <TRandom3.h> 
#include <TMath.h> 

void treeADC() {
  const Double_t D=500.;      //cm, distance between target and the scin.(Center)
  const Double_t L=100.;      //cm, half length of the scin.
  const Double_t dD=5.;       //cm, thickness of the scin.
  const Double_t TRes=1.;     //ns, time resolution(FWHM) of the scintillator.
  const Double_t Lambda=380.; //cm, attenuation lenght of the scin.
  const Double_t QRes=0.1;    //relative energy resolution(FWHM) of the scin. 
  const Double_t Vsc=7.5;     //ns/cm, speed of light in the scin.

  // neutron and gamma
  const Double_t En0=100;     //MeV, average neutron energy
  const Double_t EnRes=50.;   //MeV, energy spread of neutron(FWHM)
  const Double_t Eg0=10;      //MeV, gamma energy  
  const Double_t Rn=0.75;     //ratio of neutron 
  const Double_t Rg=0.05;     //ratio of gamma; 

  // charged particle hited on Neutron Detector, 
  const Double_t Rc=0.1;       //ratio of proton
  const Double_t Ec0=50;       //MeV
  const Double_t EcRes=50;     //MeV energy spread of proton(FWHM)

  //ADC
  const Double_t ADCgain=60;   //1MeV=60ch.
  const Double_t ADCuPed=100;  //baseline of ADC for upper side
  const Double_t ADCdPed=120;  //                for bottom side
  const Double_t ADCnoise=10;  //sigma of noise
  const Int_t    ADCoverflow=4095;

  //TDC
  const Double_t TDCch2ns=40.; //1ns=40ch.
  const Int_t TDCoverflow=4095;


  TFile *opf = new TFile("treeADC.root","recreate");
  TTree *opt = new TTree("tree","tree structure");

  Double_t x;
  Double_t e;
  int pid;              //0/1/2/3: gamma/neutron/proton/No_particle
  Double_t tof,ctof;
  Double_t tu,td;
  Double_t qu,qd;
  Int_t itu,itd;        //TDC
  Int_t iqu,iqd;        //ADC
  Double_t tu_off=5.5;  //time offset
  Double_t td_off=20.4; //time offset

  // input
  opt->Branch("x", &x, "x/D");        //position
  opt->Branch("e", &e, "e/D");        //energy
  opt->Branch("tof", &tof, "tof/D");  //time of flight
  opt->Branch("ctof",&ctof,"ctof/D"); //TOF from exp. data
  opt->Branch("pid", &pid, "pid/I");
  // raw time and energy
  opt->Branch("tu", &tu, "tu/D");
  opt->Branch("td", &td, "td/D");
  opt->Branch("qu", &qu, "qu/D");
  opt->Branch("qd", &qd, "qd/D");
  // energy in ADC, time in TDC 
  opt->Branch("itu", &itu, "itu/I");
  opt->Branch("itd", &itd, "itd/I");
  opt->Branch("iqu", &iqu, "iqu/I");
  opt->Branch("iqd", &iqd, "iqd/I");


  TRandom3 *gr=new TRandom3(0);
  for(int i=0;i<1000000;i++){
    x = gr->Uniform(-L, L);
    Double_t Dr = D+gr->Uniform(-0.5,0.5)*dD;
    Double_t d = TMath::Sqrt(Dr*Dr+x*x);    //cm, flight path

    //pid
    Double_t ratio = gr->Uniform();
    if(ratio<Rg) pid = 0;
    if(ratio>=Rg && ratio<Rg+Rn) pid = 1;
    if(ratio>=Rg+Rn && ratio<Rg+Rn+Rc) pid = 2;
    if(ratio>=Rg+Rn+Rc) pid = 3;

    // energy & tof    
    if(pid==0) { //gamma
      e = Eg0;
      tof = 10/3.*(d*0.01);
    }
    if(pid==1) { //neutron
      e = gr->Gaus(En0,EnRes/2.35); // neutron
      tof = 72./TMath::Sqrt(e)*(d*0.01);//ns
    }
    if(pid==2) { //proton
      e = gr->Gaus(Ec0,EcRes/2.35); // proton
      tof = 72./TMath::Sqrt(e)*(d*0.01);
    }
    if(pid==3) {
      e = -1;
      tof = -1;
    }

    // tu,td,qu,qd
    if(pid==3) {//
      tu = TDCoverflow;
      td = TDCoverflow;
      qu = ADCuPed + gr->Gaus(0,ADCnoise);
      qd = ADCdPed + gr->Gaus(0,ADCnoise);
    }
    else {
      // time
      tu = tof+(L-x)/Vsc+gr->Gaus(0,TRes/2.35)+tu_off;
      td = tof+(L+x)/Vsc+gr->Gaus(0,TRes/2.35)+td_off;
      tu *= TDCch2ns;
      td *= TDCch2ns;

      // energy depositon in the plastic
      Double_t q0;
      if(pid!=2) q0 = e*ADCgain*gr->Uniform();  //neutron &gamma,
      else q0 = e*ADCgain; //charged particle

      //resolution
      q0 = gr->Gaus(q0,q0*QRes/2.35);

      //light transmission within the plastic
      qu = q0*TMath::Exp(-(L-x)/Lambda);
      qd = q0*TMath::Exp(-(L+x)/Lambda);

      //threshold of time of tu and td
      if(qu<100) tu = TDCoverflow;
      if(qd<100) td = TDCoverflow;

      //ADC
      qu += gr->Gaus(ADCuPed,ADCnoise);
      qd += gr->Gaus(ADCdPed,ADCnoise);
      if(qu<0) qu=0;//no negative values
      if(qd<0) qd=0;
    }

    //overflow check
    if(tu>TDCoverflow) tu=TDCoverflow;
    if(td>TDCoverflow) td=TDCoverflow;
    if(qu>ADCoverflow) qu=ADCoverflow;
    if(qd>ADCoverflow) qd=ADCoverflow;

    //digitization
    itu = Int_t(tu);
    itd = Int_t(td);
    iqu = Int_t(qu);
    iqd = Int_t(qd);

    opt->Fill();

  }
  // 将数据写入root文件中
  opt->Write();
  opf->Close();
}
