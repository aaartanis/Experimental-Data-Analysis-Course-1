void treeADC(){
	const Double_t D=500;
	const Double_t L=100;
	const Double_t dD=5;
	const Double_t TRes=1;
	const Double_t Lambda=380;
	const Double_t QRes=0.1;
	const Double_t Vsc=7.5;
	const Double_t En0=100;
	const Double_t EnRes=50;
	const Double_t Eg0=10;
	const Double_t Rn=0.75;
	const Double_t Rg=0.05;
	const Double_t Rc=0.1;
	const Double_t Ec0=50;
	const Double_t EcRes=50;

	const Double_t ADCgain=60;
	const Double_t ADCuPed=100;
	const Double_t ADCdPed=120;
	const Double_t ADCnoise=10; //sigma of pedestal
	const Int_t ADCoverflow=4095;

	const Double_t TDCch2ns=40; //sigma of pedestal
	const Int_t TDCoverflow=4095;

	TFile *opf=new TFile("treeADC.root","recreate");
	TTree *opt=new TTree("tree","tree");

	//decare variable of tree structure
	Double_t x;
	Double_t e;
	Double_t tof,ctof;
	Double_t tu,td;
	Double_t qu1,qd1; //
	Double_t qu,qd;
	Double_t tu_off=5.5;
	Double_t td_off=20.4;

	int pid;
	int itu,itd;
	int iqu1,iqd1;//
	int iqu,iqd;//channel
	
	opt->Branch("x",&x,"x/D");
	opt->Branch("e",&e,"e/D");
	opt->Branch("tof",&tof,"tof/D");
	opt->Branch("ctof",&ctof,"ctof/D");
	opt->Branch("pid",&pid,"pid/I");

	opt->Branch("tu",&tu,"tu/D");
	opt->Branch("td",&td,"td/D");
	opt->Branch("qu1",&qu1,"qu1/D");//
	opt->Branch("qd1",&qd1,"qd1/D");//
	opt->Branch("qu",&qu,"qu/D");
	opt->Branch("qd",&qd,"qd/D");

	opt->Branch("itu",&itu,"itu/I");
	opt->Branch("iqu",&iqu,"iqu/I");
	opt->Branch("iqu1",&iqu1,"iqu1/I");//
	opt->Branch("iqd1",&iqd1,"iqd1/I");//
	opt->Branch("itd",&itd,"itd/I");
	opt->Branch("iqd",&iqd,"iqd/I");

	TRandom3 *gr=new TRandom3(0);
	for(int i=0;i<1000000;i++){
		x=gr->Uniform(-L,L);
		Double_t Dr=D+gr->Uniform(-0.5,0.5)*dD;
		Double_t d=TMath::Sqrt(Dr*Dr+x*x);
		Double_t ratio=gr->Uniform();
		if(ratio<Rg)
			pid=0;
		if(ratio>=Rg&&ratio<Rg+Rn)
			pid=1;
		if(ratio>=Rg+Rn&&ratio<Rg+Rn+Rc)
			pid=2;
		if(ratio>=Rg+Rn+Rc)
			pid=3;

		if(pid==0){
			e=Eg0;
			tof=10/3.*(d*0.01);
			}
		if(pid==1){
			e=gr->Gaus(En0,EnRes/2.35);
			tof=72./TMath::Sqrt(e)*(d*0.01);
			}
		if(pid==2){
			e=gr->Gaus(Ec0,EcRes/2.35);
			tof=72./TMath::Sqrt(e)*(d*0.01);
			}
		if(pid==3){
			e=-1;
			tof=-1;
			}
		//tu,qu,td,qd
		if(pid==3){
			tu=TDCoverflow;
			td=TDCoverflow;
			qu=ADCuPed+gr->Gaus(0,ADCnoise);
			qu1=qu;/////
			qd=ADCdPed+gr->Gaus(0,ADCnoise);
			qd1=qd;/////
			}
		else{
			tu=tof+(L-x)/Vsc+gr->Gaus(0,TRes/2.35)+tu_off;
			td=tof+(L+x)/Vsc+gr->Gaus(0,TRes/2.35)+td_off;
			tu*=TDCch2ns;
			td*=TDCch2ns;

			Double_t q0,q01;
			if(pid!=2)
				{q0=e*ADCgain*gr->Uniform();
				q01=q0;} ///////////
			else
				{q0=e*ADCgain;
				q01=q0;}////////////
			q0=gr->Gaus(q0,q0*QRes/2.35);

			qu=q0*TMath::Exp(-(L-x)/Lambda);
			qd=q0*TMath::Exp(-(L+x)/Lambda);

			qu1=q01*TMath::Exp(-(L-x)/Lambda);//////////
			qd1=q01*TMath::Exp(-(L+x)/Lambda);//////////
			qu1=gr->Gaus(qu1,qu1*QRes/2.35);////////
			qd1=gr->Gaus(qd1,qd1*QRes/2.35);/////////

			if(qu<100)
				tu=TDCoverflow;
			if(qd<100)
				td=TDCoverflow;

			qu+=gr->Gaus(ADCuPed,ADCnoise);
			qd+=gr->Gaus(ADCdPed,ADCnoise);
			if(qu<0) qu=0;
			if(qd<0) qd=0;
			///////////
			qu1+=gr->Gaus(ADCuPed,ADCnoise);
			qd1+=gr->Gaus(ADCdPed,ADCnoise);
			if(qu1<0) qu1=0;
			if(qd1<0) qd1=0;
			//////////////
			}
			if(tu>TDCoverflow) tu=TDCoverflow;
			if(td>TDCoverflow) td=TDCoverflow;
			if(qu>TDCoverflow) qu=ADCoverflow;
			if(qd>TDCoverflow) qd=ADCoverflow;
			if(qu1>TDCoverflow) qu1=ADCoverflow;////////
			if(qd1>TDCoverflow) qd1=ADCoverflow;//////

			itu=Int_t(tu);
			iqu=Int_t(qu);
			itd=Int_t(td);
			iqd=Int_t(qd);
			iqu1=Int_t(qu1);
			iqd1=Int_t(qd1);

			opt->Fill();

			itu=Int_t(tu);
			if(i%100000==0) cout<<"processing "<<i<<" of 1000000"<<endl;
			}
			opt->Write();
			opf->Close();
		
	}
