void neutron_tof(){
		const Double_t D=500; //cm, target->center of the scintillator
		const Double_t L=100; //cm
		const Double_t dD=5; //cm,thickness
		const Double_t TRes=1; //ns, time resolution(FWHM)
		const Double_t Lambda=380; //attenuation length
		const Double_t QRes=0.1; //relative energy resolution(FWHM)
		const Double_t Vsc=7.5; // cm/ns, speed of light in scintillator
		const Double_t En0=100; // MeV, average neutron energy
		const Double_t EnRes=50; // MeV, energy spred of neutron(FWHM)
		const Double_t Eg0=1; // MeV, gamma energy
		const Double_t Rg=0.3; // gamma:Rg; neutron:1-Rg;

		TFile *opf=new TFile("tof.root","recreate");
		TTree *opt=new TTree("tree","tree"); //

		Double_t x;
		Double_t e;
		Double_t tof,ctof;
		Double_t tu,td;
		Double_t qu,qd;

		Double_t tu_off=5.5;
		Double_t td_off=20.4; //offset(ns)

		int pid; //gamma:pid==0, neutron:pid==1

		opt->Branch("x",&x,"x/D");
		opt->Branch("e",&e,"e/D");
		opt->Branch("tof",&tof,"tof/D");
		opt->Branch("ctof",&ctof,"ctof/D");
		opt->Branch("tu",&tu,"tu/D");
		opt->Branch("td",&td,"td/D");
		opt->Branch("qu",&qu,"qu/D");
		opt->Branch("qd",&qd,"qd/D");
		opt->Branch("pid",&pid,"pid/I");

		TH1D *hctof=new TH1D("hctof","histogram:neutron_tof",1000,0,100);
		TRandom3 *gr=new TRandom3(0);

		for(int i=0;i<100000;i++){
				x=gr->Uniform(-L,L);
				Double_t Dr=D+gr->Uniform(-0.5,0.5)*dD;
				Double_t d=TMath::Sqrt(Dr*Dr+x*x);
				//gamma:
				if(gr->Uniform()<Rg){
						pid=0;
						e=Eg0;
						tof=3.333*(d*0.01);
				}
				//neutron:
				else{
						pid=1;
						e=gr->Gaus(En0,EnRes/2.35);
						tof=72/TMath::Sqrt(e)*(d*0.01);
				}

				tu=tof+(L-x)/Vsc+gr->Gaus(0,TRes/2.35)+tu_off;
				td=tof+(L+x)/Vsc+gr->Gaus(0,TRes/2.35)+td_off;
				ctof=(tu+td)/2;
				hctof->Fill(ctof);
				Double_t q0=e*gr->Uniform();
				qu=q0*TMath::Exp(-(L-x)/Lambda);
				qu=gr->Gaus(qu,qu*QRes/2.35);
				qd=q0*TMath::Exp(-(L+x)/Lambda);
				qd=gr->Gaus(qd,qd*QRes/2.35);
				opt->Fill();
				}
		hctof->Write();
		opt->Write();
		opf->Close();
		}
