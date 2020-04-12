#define tracking_cxx
#include "tracking.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void tracking::TrackInit()
{
	tx=-1000;
	ty=-1000;
	for(Int_t i=0;i<5;i++)
	{
		xx[i][0]=PPACF8[i][0];
		yy[i][0]=PPACF8[i][1];
		xz[i]=PPACF8[i][2];
		yz[i]=PPACF8[i][3];
		xx[i][1]=-1000;
		yy[i][1]=-1000;
		sigmax[i]=1;
		sigmay[i]=1;

	}

	anode3=PPACF8[4][4];
	anode1a=PPACF8[0][4];

}

void tracking::SetBranch(TTree *tree)
{
	//measured pos 1A:[0];1B:[1];2A:[2];2B[3];3[4]
	tree->Branch("xx",&xx,"xx[5][2]/D");
	tree->Branch("xz",&xz,"xz[5]/D");
	tree->Branch("yy",&yy,"yy[5][2]/D");
	tree->Branch("yz",&yz,"yz[5]/D");

	//difference between measured and calculated - for pos resolution
	tree->Branch("dx",&dx,"dx[5]/D");
	tree->Branch("dy",&dy,"dy[5]/D");

	//target x-y
	tree->Branch("tx",&tx,"tx/D");
	tree->Branch("ty",&ty,"ty/D");
	tree->Branch("CIDx",&CIDx,"CIDx/I");
	tree->Branch("CIDy",&CIDy,"CIDy/I");
	tree->Branch("nx",&nx,"nx/I");
	tree->Branch("ny",&ny,"ny/I");

	//ch2/ndf for linear fitting
	tree->Branch("c2nx",&c2nx,"c2nx/D");
	tree->Branch("c2ny",&c2ny,"c2ny/D");

	tree->Branch("beamTrig",&beamTrig,"beamTrig/I");
	tree->Branch("must2Trig",&must2Trig,"must2Trig/I");

	tree->Branch("targetX",&targetX,"targetX");
	tree->Branch("targetY",&targetY,"targetY");
	//position resolution
	tree->Branch("xxs",&xxs,"xxs[5]/D");
	tree->Branch("yys",&yys,"yys[5]/D");
	tree->Branch("dxs",&dxs,"dxs[5]/D");
	tree->Branch("dys",&dys,"dys[5]/D");
	tree->Branch("txs",&txs,"txs[5]/D");
	tree->Branch("tys",&tys,"tys[5]/D");
	tree->Branch("xxss",&xxss,"xxss[5]/D");
	tree->Branch("yyss",&yyss,"yyss[5]/D");

}

void tracking::SetTrace(TH2D *h,Double_t k,Double_t b,Int_t min,Int_t max)
{
	if(h==0) return;
	if(min>max) return;

	for(Int_t i=min;i<max;i++){
		h->Fill(i,(Int_t)(i*k+b));
	}
}


void tracking::Loop()
{
	TH2D *htf8xz=new TH2D("htf8xz","xz trace by ppac",2200,-2000,200,300,-150,150);
	TH2D *htf8yz=new TH2D("htf8yz","yz trace by ppac",2200,-2000,200,300,-150,150);

	TFile *opf=new TFile("tracking.root","recreate");
	TTree *tree=new TTree("tree","ppac tracking");
	SetBranch(tree);
	TRandom3 *gr=new TRandom3(0);//position resolution
	for(Int_t i=0;i<3;i++)
	{
		for(Int_t j=0;j<6;j++)
		{
			m[i][j]=0,m0[i][j]=0;
		}
	}
	if (fChain == 0) return;
	Long64_t nentries = fChain->GetEntriesFast();
	Long64_t nbytes = 0, nb = 0;
	for (Long64_t jentry=0; jentry<nentries;jentry++) {
		Long64_t ientry = LoadTree(jentry);
		if (ientry < 0) break;
		nb = fChain->GetEntry(jentry);   nbytes += nb;
		// if (Cut(ientry) < 0) continue;
		TrackInit();
		//x
		nx=0;
		CIDx=0;
		for(Int_t i=0;i<5;i++)
		{
			if(abs(xx[i][0])<150)
			{
				gx[nx]=xx[i][0];
				gxz[nx]=xz[i];
				CIDx += pow(10,4-i);
				nx += 1;
			}

		}
		CIDx += pow(10,5);
		//y
		ny=0;
		CIDy=0;
		for(Int_t i=0;i<5;i++)
		{
			if(abs(yy[i][0])<150)
			{
				gy[ny]=yy[i][0];
				gyz[ny]=yz[i];
				CIDy += pow(10,4-i);
				ny += 1;
			}
		}
		CIDy += pow(10,5);
		if(nx<2 || ny<2) continue;

		//fit x-z trajectory
		TFitResultPtr r;
		TGraph *grx=new TGraph(nx,gxz,gx);
		TF1 *fx=new TF1("fx","pol1",-2000,0);
		r=grx->Fit(fx,"SQ");
		for(Int_t i=0;i<5;i++)
		{
			xx[i][1]=fx->Eval(xz[i]);
		}
		tx=fx->Eval(-fx->GetParameter(0)/(TMath::Sqrt(2.)*(1+fx->GetParameter(1))));
		SetTrace(htf8xz,fx->GetParameter(1),fx->GetParameter(0),-1800,0);
		for(Int_t i=0;i<5;i++) dx[i]=xx[i][0]-fx->Eval(xz[i]);
		c2nx=r->Chi2()/r->Ndf();

		//fit y-z trajectory
		TGraph *gry=new TGraph(ny,gyz,gy);
		TF1 *fy=new TF1("fy","pol1",-2000,0);
		r=gry->Fit(fy,"SQ");
		for(Int_t i=0;i<5;i++)
		{
			yy[i][1]=fy->Eval(yz[i]);
		}
		ty=fy->Eval(-fx->GetParameter(0)/(TMath::Sqrt(2.)*(1+fx->GetParameter(1))));
		SetTrace(htf8yz,fy->GetParameter(1),fy->GetParameter(0),-1800,0);
		for(int i=0;i<5;i++) dy[i]=yy[i][0]-fy->Eval(yz[i]);
		c2ny=r->Chi2()/r->Ndf();

		delete grx;
		delete fx;		
		delete gry;
		delete fy;
		//position resolution
		//x
		if(nx==5)
		{
			for(Int_t i=0;i<5;i++)
			{
				xxs[i]=gr->Gaus(xx[i][1],sigmax[i]);
			}
		}
		TFitResultPtr re;
		TGraph *grxs=new TGraph(5,xz,xxs);
		TF1 *fxs=new TF1("fxs","pol1",-2000,0);
		re=grxs->Fit(fxs,"SQ");
		for(Int_t i=0;i<5;i++)
		{
			xxss[i]=fxs->Eval(xz[i]);
		}
		txs=fxs->Eval(-fxs->GetParameter(0)/(TMath::Sqrt(2.)*(1+fxs->GetParameter(1))));
		for(Int_t i=0;i<5;i++) dxs[i]=xxs[i]-xxss[i];
		//y
		if(ny==5 && nx==5)
		{
			for(Int_t i=0;i<5;i++)
			{
				yys[i]=gr->Gaus(yy[i][1],sigmay[i]);
			}
		}
		TGraph *grys=new TGraph(5,yz,yys);
		TF1 *fys=new TF1("fys","pol1",-2000,0);
		re=grys->Fit(fys,"SQ");
		for(Int_t i=0;i<5;i++)
		{
			yyss[i]=fys->Eval(xz[i]);
		}
		tys=fys->Eval(-fxs->GetParameter(0)/(TMath::Sqrt(2.)*(1+fxs->GetParameter(1))));
		for(Int_t i=0;i<5;i++) dys[i]=yys[i]-yyss[i];

		delete grxs;
		delete fxs;	
		delete grys;
		delete fys;	

		//efficiency
		//m[i][j]i=method;j=0:1ax;1:1ay;2:1axy;33x;43y;53xy
		if(anode1a>0) m0[0][1] +=1;
		if(anode3>0) m0[0][4] +=1;
		if(abs(xx[0][0])<150) m[0][0] +=1;
		if(abs(yy[0][0])<150) m[0][1] +=1;
		if(abs(xx[0][0])<150 && yy[0][0]<150) m[0][2] +=1;
		if(abs(xx[4][0])<100) m[0][3] +=1;
		if(abs(yy[4][0])<100) m[0][4] +=1;
		if(abs(xx[4][0])<100 && yy[4][0]<100) m[0][5] +=1;


		//efficiency 1b&&2b 
		if(abs(xx[1][0])<150 && abs(xx[3][0])<150)
		{
			TFitResultPtr re;
			Double_t e1xz[2],e1xx[2];
			e1xx[0]=xx[1][0],e1xx[1]=xx[3][0];
			e1xz[0]=xz[1],e1xz[1]=xz[3];
			TGraph *grxs=new TGraph(2,e1xz,e1xx);
			TF1 *fxs=new TF1("fxs","pol1",-2000,0);
			re=grxs->Fit(fxs,"SQ");
			for(Int_t i=0;i<5;i++)
			{
				xxe[i]=fxs->Eval(xz[i]);
			}
			delete grxs;
			delete fxs;	
			
			if(abs(xxe[0])<150) m[1][0] +=1;
			if(abs(xx[0][0])<150) m0[1][0] +=1;
			if(abs(xxe[4])<100) m[1][3] +=1;
			if(abs(xx[4][0])<100) m0[1][3] +=1;
		}
		if(abs(yy[1][0])<150 && abs(yy[3][0])<150)
		{
			TFitResultPtr re;
			Double_t e1yz[2],e1yy[2];
			e1yy[0]=yy[1][0],e1yy[1]=yy[3][0];
			e1yz[0]=yz[1],e1yz[1]=yz[3];
			TGraph *grys=new TGraph(2,e1yz,e1yy);
			TF1 *fys=new TF1("fys","pol1",-2000,0);
			re=grys->Fit(fys,"SQ");
			for(Int_t i=0;i<5;i++)
			{
				yye[i]=fys->Eval(yz[i]);
			}
			delete grys;
			delete fys;	
			if(abs(yye[0])<150) m[1][1] +=1;
			if(abs(yy[0][0])<150) m0[1][1] +=1;
			if(abs(yye[0])<150 && xxe[0]<150) m[1][2] +=1;
			if(abs(yy[0][0])<150 && xx[0][0]<150) m0[1][2] +=1;
			if(abs(yye[4])<100) m[1][4] +=1;
			if(abs(yy[4][0])<100) m0[1][4] +=1;
			if(abs(yye[4])<100 && xxe[4]<100) m[1][5] +=1;
			if(abs(yy[4][0])<100 && xx[4][0]<100) m0[1][5] +=1;
		}
				
		//efficiency 1b&&2a 
		if(abs(xx[1][0])<150 && abs(xx[2][0])<150)
		{
			TFitResultPtr re;
			Double_t e1xz[2],e1xx[2];
			e1xx[0]=xx[1][0],e1xx[1]=xx[2][0];
			e1xz[0]=xz[1],e1xz[1]=xz[2];
			TGraph *grxs=new TGraph(2,e1xz,e1xx);
			TF1 *fxs=new TF1("fxs","pol1",-2000,0);
			re=grxs->Fit(fxs,"SQ");
			for(Int_t i=0;i<5;i++)
			{
				xxe[i]=fxs->Eval(xz[i]);
			}
			delete grxs;
			delete fxs;	
			if(abs(xxe[0])<150) m[2][0] +=1;
			if(abs(xx[0][0])<150) m0[2][0] +=1;
			if(abs(xxe[4])<100) m[2][3] +=1;
			if(abs(xx[4][0])<100) m0[2][3] +=1;
		}
		if(abs(yy[1][0])<150 && abs(yy[2][0])<150)
		{
			TFitResultPtr re;
			Double_t e1yz[2],e1yy[2];
			e1yy[0]=yy[1][0],e1yy[1]=yy[3][0];
			e1yz[0]=yz[1],e1yz[1]=yz[3];
			TGraph *grys=new TGraph(2,e1yz,e1yy);
			TF1 *fys=new TF1("fys","pol1",-2000,0);
			re=grys->Fit(fys,"SQ");
			for(Int_t i=0;i<5;i++)
			{
				yye[i]=fys->Eval(yz[i]);
			}
			delete grys;
			delete fys;	
			if(abs(yye[0])<150) m[2][1] +=1;
			if(abs(yy[0][0])<150) m0[2][1] +=1;
			if(abs(yye[0])<150 && xxe[0]<150) m[2][2] +=1;
			if(abs(yy[0][0])<150 && xx[0][0]<150) m0[2][2] +=1;
			if(abs(yye[4])<100) m[2][4] +=1;
			if(abs(yy[4][0])<100) m0[2][4] +=1;
			if(abs(yye[4])<100 && xxe[4]<100) m[2][5] +=1;
			if(abs(yy[4][0])<100 && xx[4][0]<100) m0[2][5] +=1;
		}


		
		tree->Fill();
		if(jentry%10000==0) cout<<"processing "<<jentry<<endl;
		
	}
	//efficiency
	//m[i][j]i=method;j=0:1ax;1:1ay;2:1axy;33x;43y;53xy

	for(Int_t i=0;i<3;i++)
	{
		eff[0][i]=m[0][i]/m0[0][1];
	}
	for(Int_t i=3;i<6;i++)
	{
		eff[0][i]=m[0][i]/m0[0][4];
	}
	for(Int_t i=1;i<3;i++)
	{
		for(Int_t j=0;j<6;j++)
		{
			eff[i][j]=m0[i][j]/m[i][j];
		}
	}
	for(Int_t i=0;i<3;i++)
	{
		for(Int_t j=0;j<6;j++)
		{
			cout<<"eff["<<i<<"]["<<j<<"]:"<<eff[i][j]<<"     ";			

		}
		cout<<endl;
		for(Int_t j=0;j<6;j++)
		{
			cout<<"m"<<m[i][j]<<m0[i][j]<<"     ";

		}
		cout<<endl;
	}


	htf8xz->Write();
	htf8yz->Write();
	tree->Write();
	opf->Close();
}
