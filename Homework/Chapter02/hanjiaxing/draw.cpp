#include <TFile.h> 
#include <TTree.h> 
#include <TH1D.h> 
#include <TROOT.h> 
#include <TPad.h> 
#include <THStack.h> 
#include <TF1.h> 
#include <TCut.h> 
#include <iostream>
using namespace std;

void draw() {
  TFile *ipf = new TFile("treeADC.root");
  TTree *ipt = (TTree*)ipf->Get("tree");

  //ipt->Draw("itu>>htu(420,0,4200)");
  //TH1D *htu = (TH1D*)gROOT->FindObject("htu");
  //ipt->Draw("itd>>htd(420,0,4200)");
  //TH1D *htd = (TH1D*)gROOT->FindObject("htd");
  //htd->SetLineColor(kGreen);
  //htu->Draw();
  //htd->Draw("same");
  //gPad->SetLogy(1);

  //ipt->Draw("itu>>htu(420,0,4200)");
  //ipt->Draw("itd>>htd(420,0,4200)");
  //TH1D *htu = (TH1D*)gROOT->FindObject("htu");
  //TH1D *htd = (TH1D*)gROOT->FindObject("htd");
  //htd->SetLineColor(kGreen);
  //THStack *hs = new THStack("hs","test stacked histograms");
  //hs->Add(htu);
  //hs->Add(htd);
  //hs->Draw("nostack");
  //gPad->SetLogy(1);

  //ipt->Draw("itu:itd>>(420,0,4200,420,0,4200)","","colz");
  //ipt->Draw("itu:itd>>(420,0,4200,420,0,4200)","pid==3","colz");
  //gPad->SetLogy(0);
  //gPad->SetLogz();

  //位置分布
  //ipt->Draw("itu-itd");
  //ipt->Draw("itu-itd>>hix(3000,-2000,1000)","itu<4095 && itd<4095");
  //gPad->SetLogy();


  // ======================= QDC ========================= //
  ipt->Draw("iqu>>hquall(420,0,4200)");
  TH1D *hquall=(TH1D*)gROOT->FindObject("hquall");
  hquall->SetLineColor(kBlack);

  ipt->Draw("iqu>>hqu1(420,0,4200)","pid==0 || pid==1");
  TH1D *hqu1=(TH1D*)gROOT->FindObject("hqu1");
  hqu1->SetLineColor(kRed);

  ipt->Draw("iqu>>hqu2(420,0,4200)","pid==2");
  TH1D *hqu2=(TH1D*)gROOT->FindObject("hqu2");
  hqu2->SetLineColor(kGreen);

  ipt->Draw("iqu>>hqu3(420,0,4200)","pid==3");
  TH1D *hqu3=(TH1D*)gROOT->FindObject("hqu3");
  hqu3->SetLineColor(kBlue);

  //THStack *hsqu=new THStack("hsqu","qu with different pid");
  //hsqu->Add(hquall);
  //hsqu->Add(hqu1);
  //hsqu->Add(hqu2);
  //hsqu->Add(hqu3);
  //hsqu->Draw("nostack");
  //gPad->SetLogy();

  ipt->Draw("iqd>>hqdall(420,0,4200)");
  TH1D *hqdall=(TH1D*)gROOT->FindObject("hqdall");
  hqdall->SetLineColor(kBlack);

  ipt->Draw("iqd>>hqd1(420,0,4200)","pid==0 || pid==1");
  TH1D *hqd1=(TH1D*)gROOT->FindObject("hqd1");
  hqd1->SetLineColor(kRed);

  ipt->Draw("iqd>>hqd2(420,0,4200)","pid==2");
  TH1D *hqd2=(TH1D*)gROOT->FindObject("hqd2");
  hqd2->SetLineColor(kGreen);

  ipt->Draw("iqd>>hqd3(420,0,4200)","pid==3");
  TH1D *hqd3=(TH1D*)gROOT->FindObject("hqd3");
  hqd3->SetLineColor(kBlue);

  //THStack *hsqd=new THStack("hsqd","qd with different pid");
  //hsqd->Add(hqdall);
  //hsqd->Add(hqd1);
  //hsqd->Add(hqd2);
  //hsqd->Add(hqd3);
  //hsqd->Draw("nostack");
  //gPad->SetLogy();

  TF1 *fgaus[2];
  Double_t ped[2],sigma[2];//u,d
  TString sq[2]={"qu","qd"};

  hquall->Fit("gaus","","",0,250);
  hqdall->Fit("gaus","","",0,250);
  fgaus[0]=hquall->GetFunction("gaus");
  fgaus[1]=hqdall->GetFunction("gaus");
  for(int i=0;i<2;i++) {
      ped[i]=fgaus[i]->GetParameter(1);
      sigma[i]=fgaus[i]->GetParameter(2);
      TString ss;
      ss.Form("ped_%s=%.2f, sigma_%s=%.2f",sq[i].Data(),ped[i],sq[i].Data(),sigma[i]); 
      cout<<ss<<endl;
  }

  //ipt->Draw("iqu:iqd>>(420,0,4200,420,0,4200)","","colz");
  //gPad->SetLogy(0);
  //gPad->SetLogz();

  //qu,qd减去ped，去掉噪声及超界值，选择合理的取值范围
  //TString stree,scut;
  //stree.Form("(iqu-%f):(iqd-%f)>>hsud(200,0,4000,200,0,4000)",ped[0],ped[1]);//在""内加入动态数据的方法
  //scut.Form("iqu>(%f+3*%f) && iqu<4000 && iqd>(%f+3*%f) && iqd<4000",ped[0],sigma[0],ped[1],sigma[1]);
  //ipt->Draw(stree.Data(),scut.Data(),"colz");
  //gPad->SetLogz();
  //cout<<"tree: "<<stree.Data()<<endl;
  //cout<<"cut: "<<scut.Data()<<endl;

  TString squa,sqda,scut;
  squa.Form("iqu-%f",ped[0]);
  sqda.Form("iqd-%f",ped[1]);
  scut.Form("iqu>(%f+3*%f) && iqu<4000 && iqd>(%f+3*%f) && iqd<4000",ped[0],sigma[0],ped[1],sigma[1]);
  ipt->SetAlias("qua",squa.Data());
  ipt->SetAlias("qda",sqda.Data());
  TCut cpid="pid=2";
  ipt->Draw("sqrt(qua*qda)",scut.Data());
  gPad->SetLogy();

  TString stcut="itu>0&&itu<4000&&itd>0&&itd<4000";
  //scut=scut+"&&"+stcut;
  scut=scut+"&&"+stcut+"&&pid==2";
  //ipt->Draw("itd-itu:log(qua/qda)",scut.Data(),"colz");
  ipt->Draw("sqrt(qua*qda):(itd+itu)/2",scut.Data(),"colz");
  gPad->SetLogy(0);
  gPad->SetLogz();
}
