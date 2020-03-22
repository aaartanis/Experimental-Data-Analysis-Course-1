#include<math.h>

void treeADC()
{
	const Double_t D=500;
	const Double_t L=100;
	const Double_t dD=5.;
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

	TFile *opf=new TFile("treeADC.root","recreate");
	TTree *opt=new TTree("tree","tree structure");
//Branch 中的变量
	Double_t x;
	Double_t e;
	int pid; //	0/1/2/3   gamma/neutron/proton/No_particle
	Double_t tof,ctof;
	Double_t tu,td;
	Double_t qu,qd;
	Int_t itu,itd;
	Int_t iqu,iqd;

	Double_t tu_off=5.5;
	Double_t td_off=20.4;

	//将变量分支添加到tree结构中
	opt->Branch("x",&x,"x/D");
	opt->Branch("e",&e,"e/D");
	opt->Branch("tof",&tof,"tof/D");
	opt->Branch("ctof",&ctof,"ctof/D");
	opt->Branch("pid",&pid,"pid/I");

	opt->Branch("tu",&tu,"tu/D");
	opt->Branch("td",&td,"td/D");
	opt->Branch("qu",&qu,"qu/D");
	opt->Branch("qd",&qu,"qd/D");
	opt->Branch("itu",&itu,"itu/I");
	opt->Branch("itd",&itd,"itd/I");
	opt->Branch("iqu",&iqu,"iqu/I");
	opt->Branch("iqd",&iqd,"iqd/I");


	TRandom3 *gr=new TRandom3(0);
	for(int i=0;i<1000000;i++)
	{
		x=gr->Uniform(-L,L);
		Double_t Dr=D+gr->Uniform(-0.5,0.5)*dD;
		Double_t d=TMath::Sqrt(Dr*Dr+x*x);
		//pid
		Double_t ratio=gr->Uniform();
		if(ratio<Rg) pid=0;
		if(ratio>Rg && ratio<Rg+Rn) pid=1;
		if(ratio>=Rg+Rn && ratio<Rg+Rn+Rc) pid=2;
		if(ratio>=Rg+Rn+Rc) pid=3;

		//energy & tof
		if(pid==0)
		{
			e=Eg0;
			tof=3.33*(d*0.01);
		}
		if(pid==1)
		{
			e=gr->Gaus(En0, EnRes/2.35);
			tof=72./TMath::Sqrt(e)*(d*0.01);
		}
		if(pid==2)
		{
			e=gr->Gaus(Ec0, EcRes/2.35);
			tof=72./TMath::Sqrt(e)*(d*0.01);
		}
		if(pid==3)
		{
			e=-1;
			tof=-1;
		}
		//tu,td,qu,qd 
		if(pid==3)
		{
			tu=TDCoverflow;
			td=TDCoverflow;
			qu=ADCuPed+gr->Gaus(0,ADCnoise);
			qd=ADCdPed+gr->Gaus(0,ADCnoise);
		}
		else{
			tu=tof+(L-x)/Vsc+gr->Gaus(0,TRes/2.35)+tu_off;
			td=tof+(L+x)/Vsc+gr->Gaus(0,TRes/2.35)+td_off;
			tu *=TDCch2ns;
			td *=TDCch2ns;

			Double_t q0;
			if(pid!=2) q0=e*ADCgain*gr->Uniform();
			else q0=e*ADCgain;
			q0=gr->Gaus(q0,q0*QRes/2.35);

			//light transmission within the plastic
			qu=q0*TMath::Exp(-(L-x)/Lambda);
			qd=q0*TMath::Exp(-(L+x)/Lambda);

			//threshold of time of tu and td
			if(qu<100) tu=TDCoverflow;
			if(qd<100) td=TDCoverflow; 
			//ADC
			qu +=gr->Gaus(ADCuPed,ADCnoise);
			qd +=gr->Gaus(ADCdPed,ADCnoise);
			if(qu<0) qu=0;
			if(qd<0) qd=0;
		}
		if(tu>TDCoverflow) tu=TDCoverflow;
		if(td>TDCoverflow) td=TDCoverflow;
		if(qu>ADCoverflow) qu=ADCoverflow;
		if(qd>ADCoverflow) qd=ADCoverflow;

		//digitization
		itu=Int_t(tu);
		itd=Int_t(td);
		iqu=Int_t(qu);
		iqd=Int_t(qd);

		opt->Fill();

	}

	opt->Write();
	opf->Close();

}