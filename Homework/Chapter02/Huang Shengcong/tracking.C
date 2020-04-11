#define tracking_cxx
#include "tracking.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TFitResult.h>

void tracking::SetBranch(TTree *tree)
{
   //measured position 
   tree->Branch("xx",&xx,"xx[5][2]/F");//0-measured,1-fitted
   tree->Branch("xz",&xz,"xz[5]/F");
   tree->Branch("yy",&yy,"yy[5][2]/F");
   tree->Branch("yz",&yz,"yz[5]/F");

   //flag
   tree->Branch("flag",&flag,"flag/I");

   //ta 
   tree->Branch("ta",&ta,"ta[5]/F");
   
   //difference between measured and calculated for position resolution
   tree->Branch("dx",&dx,"dx[5]/F");
   tree->Branch("dy",&dy,"dy[5]/F");

   //target x-y
   tree->Branch("tx",&tx,"tx/D");
   tree->Branch("ty",&ty,"ty/D");

   //ch2/ndf for linear fitting.
   tree->Branch("c2nx",&c2nx,"c2nx/F");
   tree->Branch("c2ny",&c2ny,"c2ny/F");

   tree->Branch("beamTrig",&beamTrig,"beamTrig/I");
   tree->Branch("must2Trig",&must2Trig,"must2Trig/I");
   
   tree->Branch("targetX",&targetX,"targetX/F");
   tree->Branch("targetY",&targetY,"targetY/F");
   
}

void tracking::TrackInit()
{
   tx=-999;
   ty=-999;
   
   for(int i=0;i<5;i++)
   {
      xx[i][0]=PPACF8[i][0];
      yy[i][0]=PPACF8[i][1];
      xz[i]=PPACF8[i][2];
      yz[i]=PPACF8[i][3];

      xx[i][1]=-999;
      yy[i][1]=-999;

      ta[i]=PPACF8[i][4];
   }
}

void tracking::SetTrace(TH2D *h,Double_t k,Double_t b,Int_t min,Int_t max)
{
   if(h==0) return;
   if(min>=max) return;
   
   for(int i=min;i<max;i++)
   {
      h->Fill(i,(Int_t)(i*k+b));
   }
}

void tracking::Loop()
{
   TH2D *htf8xz= new TH2D("htf8xz","xz trace by ppac",2200,-2000,200,300,-150,150);
   TH2D *htf8yz= new TH2D("htf8yz","yz trace by ppac",2200,-2000,200,300,-150,150);

   TH2D *htf111= new TH2D("htf111","xz trace by ppac test",2200,-2000,200,300,-150,150);

   TFile *opf=new TFile("tracking.root","recreate");
   TTree *tree= new TTree("tree","ppac tracking");

   SetBranch(tree);
   
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;

      TrackInit();

      Int_t bppac[5];
      int fh=0;
      for(int i=0;i<4;i++)
      {
         bppac[i]= abs(xx[i][0])<120 && abs(yy[i][0])<75;
         fh = fh + bppac[i];
      }
      bppac[4] = abs(xx[4][0])<50 && abs(yy[4][0])<50;
      fh = fh + bppac[4];

      // no signal and 1 signal
      if(fh<2) continue;
      
      // 1A1B and 2A2B
      int fh1 = bppac[0] + bppac[2];
      int fh2 = bppac[1] + bppac[3];
      bool bt = (fh==2) && ((fh1==2) || (fh2==2)); 
      if(bt) continue;

      //pick up 
      for(int i;i<5;i++)
      {
         if(bppac[i]==0)
         {
            xx[i][0]=-1000;
            yy[i][0]=-1000;
         }
      }

      // make the Graph array and flag
      int n=0;
      Float_t gxx[5],gyy[5],gxz[5],gyz[5];
      flag=100000;//the last six number means different PPAC layer

      for(int i=0;i<5;i++)
      {
         if(bppac[i]==1)
         {
            flag += pow(10,(4-i));

            gxx[n]=xx[i][0];
            gyy[n]=yy[i][0];
            gxz[n]=xz[i];
            gyz[n]=yz[i];
            n++;
         }
      }

    
      //fit x-z trajectory
      TFitResultPtr r;
      TGraph *grx=new TGraph(n,gxz,gxx);// npoint ,gxz=x,gxx=y
      TF1 *fx=new TF1("fx","pol1",-2000,0);
      r=grx->Fit(fx,"SQ"); // the result of fit return to TFitResultPtr ;quiet mode 
      //tx=fx->Eval(0);
      Double_t kx,bx;
      kx= fx->GetParameter(1);
      bx= fx->GetParameter(0);
      SetTrace(htf8xz,kx,bx,-1800,0);

      Double_t x=bx/(1+kx);
      targetX = x;

      for(int i=0;i<5;i++) 
      {
         xx[i][1]= Float_t(fx->Eval(xz[i]));//need type change
         dx[i]=xx[i][0]-xx[i][1];
      }
      c2nx=r->Chi2()/r->Ndf();
      
      //test
      if(c2nx<=5)
      { 
         SetTrace(htf111,kx,bx,-1800,0);
         //if(jentry%10000==0) cout<<"Yes We test"<<endl;
      }

      delete grx;
      delete fx;

      //fit y-z trajectory
      TGraph *gry=new TGraph(n,gyz,gyy);
      TF1 *fy=new TF1("fy","pol1",-2000,0);
      r=gry->Fit(fy,"SQ");
      ty=fy->Eval(0);
      Double_t ky,by;
      ky= fy->GetParameter(1);
      by= fy->GetParameter(0);
      SetTrace(htf8yz,ky,bx,-1800,0);
      
      Double_t y=(-bx*ky)/(1+kx) + by;
      Double_t z= -x;
      targetY = TMath::Sqrt(TMath::Sq(y)+TMath::Sq(z));

      for (int i=0;i<5;i++) 
      {
         yy[i][1]=Float_t(fy->Eval(yz[i]));
         dy[i]=yy[i][0]-yy[i][1];
      }
      c2ny=r->Chi2()/r->Ndf();//output fit error 
      delete gry;
      delete fy;

      tree->Fill();
      if(jentry%10000==0) cout<<"processing "<<jentry<<endl;
      
   }
   htf8xz->Write();
   htf8yz->Write();
   htf111->Write();
   tree->Write();
   opf->Close();

}
