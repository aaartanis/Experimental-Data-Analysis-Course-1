#define AnalyzeData_cxx
#include "AnalyzeData.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void AnalyzeData::Loop()
{
//   In a ROOT session, you can do:
//      root> .L AnalyzeData.C
//      root> AnalyzeData t
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
    gStyle->SetPalette(55);

    TH1D *hitu=new TH1D("hitu","itu",420,0,4200);
    TH1D *hitd=new TH1D("hitd","itd",420,0,4200);
    TH2D *h2dit=new TH2D("h2dit","itu:itd",420,0,4200,420,0,4200);

    TH1D *hdit=new TH1D("hdit","itu-itd(with both it<4095)",2500,-2000,1000);
    TH1D *hdt=new TH1D("hdt","tu-td(with both t<4095)",1000,-2000,1000);
    TH1D *hditemp=new TH1D("hditemp","itu-itd",840,-4200,4200);
    TH1D *htu=new TH1D("htu","tu*0.3",1200,0,1200);
    TH1D *htiu=new TH1D("htiu","itu*0.3",1200,0,1200);
    THStack *hs = new THStack("hs","test stacked histograms");

    TH1D *hquall=new TH1D("hquall","iqu",420,0,4200);
    TH1D *hqu1=new TH1D("hqu1","qu pid=0||1",420,0,4200);
    TH1D *hqu2=new TH1D("hqu2","qu pid=2",420,0,4200);
    TH1D *hqu3=new TH1D("hqu3","qu pid=3",420,0,4200);
    THStack *hsqu = new THStack("hsqu","qu with different pid");

    TH1D *hqdall=new TH1D("hqdall","iqd",420,0,4200);
    TH1D *hqd1=new TH1D("hqd1","qd pid=0||1",420,0,4200);
    TH1D *hqd2=new TH1D("hqd2","qd pid=2",420,0,4200);
    TH1D *hqd3=new TH1D("hqd3","qd pid=3",420,0,4200);
    THStack *hsqd = new THStack("hsqd","qd with different pid");

    TH2D *h2diq=new TH2D("h2diq","iqu:iqd",420,0,4200,420,0,4200);
    TH2D *hsqud=new TH2D("hsqud","iqu:iqd",420,0,4200,420,0,4200);

    TH1D *hntof = new TH1D("hntof","ntof",1000,0,4000);
    TH1D *hvetotof = new TH1D("hvetotof","vetotof",1000,0,4200);

    if (fChain == 0) return;
    Long64_t nentries = fChain->GetEntriesFast();
    Long64_t nbytes = 0, nb = 0;
    for (Long64_t jentry=0; jentry<nentries;jentry++) {
        Long64_t ientry = LoadTree(jentry);
        if (ientry < 0) break;
        nb = fChain->GetEntry(jentry);   nbytes += nb;
        // if (Cut(ientry) < 0) continue;
        hditemp->Fill(tof_itu-tof_itd);
        hquall->Fill(tof_iqu);
        hqdall->Fill(tof_iqd);
        h2diq->Fill(tof_iqd,tof_iqu);
        hntof->Fill((tof_itu+tof_itd)/2);
        hvetotof->Fill((veto_itu+veto_itd)/2);
        if(tof_pid==0||tof_pid==1){
            hqu1->Fill(tof_iqu); 
            hqd1->Fill(tof_iqd); 
        }
        if(tof_pid==2){
            hqu2->Fill(tof_iqu); 
            hqd2->Fill(tof_iqd); 
        }
        if(tof_pid==3){
            hqu3->Fill(tof_iqu); 
            hqd3->Fill(tof_iqd); 
        }
        if(tof_itu<4095 && tof_itd<4095){
            hitu->Fill(tof_itu);
            hitd->Fill(tof_itd);
            h2dit->Fill(tof_itd,tof_itu);
            hdit->Fill(tof_itu-tof_itd);
            hdt->Fill(tof_tu-tof_td);
            htu->Fill(tof_tu*0.3);
            htiu->Fill(tof_itu*0.3);
        }
    }

    hs->Add(hitu);
    hs->Add(hitd);
    hsqu->Add(hquall);
    hsqu->Add(hqu1);
    hsqu->Add(hqu2);
    hsqu->Add(hqu3);
    hsqd->Add(hqdall);
    hsqd->Add(hqd1);
    hsqd->Add(hqd2);
    hsqd->Add(hqd3);

    hquall->Fit("gaus","","",0,170);
    hqdall->Fit("gaus","","",0,230);

    TF1 *fgaus[2];
    Double_t ped[2],sigma[2];
    TString sq[2]={"qu","qd"};
    fgaus[0]=hquall->GetFunction("gaus");
    fgaus[1]=hqdall->GetFunction("gaus");
    for(int i=0;i<2;i++){
        ped[i]=fgaus[i]->GetParameter(1);
        sigma[i]=fgaus[i]->GetParameter(2);
        TString ss;
        ss.Form("ped_%s=%.2f,sigma_%s=%.2f",sq[i].Data(),ped[i],sq[i].Data(),sigma[i]);
        cout<<ss<<endl;
    }

    TCanvas *c1=new TCanvas("c1","tdc1",700,700);
    c1->Divide(2,2);
    c1->cd(1)->SetLogy();
    hitu->Draw();
    hitd->Draw("same");
    hitu->SetLineColor(kGreen);
    hitd->SetLineColor(kBlue);
    c1->cd(2);
    c1->cd(2)->SetLogy();
    hs->Draw("nostack");
    //hs->Draw();
    c1->cd(3);
    c1->cd(3)->SetLogz();
    h2dit->Draw("colz");
    c1->cd(4);
    c1->cd(4)->SetLogy();
    hditemp->Draw();
//    c1->SaveAs("tdc1.pdf"); 

    TCanvas *c2=new TCanvas("c2","tdc2",700,700);
    c2->Divide(2,2);
    c2->cd(1);
    hdit->Draw();
    c2->cd(2);
    hdt->Draw();
    c2->cd(3);
    htu->Draw();
    c2->cd(4);
    htiu->Draw();
//    c2->SaveAs("tdc2.pdf"); 

    TCanvas *c3=new TCanvas("c3","qdc1",600,600);
    c3->Divide(3,1);
    c3->cd(1)->SetLogy();
    hquall->SetLineColor(kBlack);
    hquall->Draw();
    hqu1->SetLineColor(kRed);
    hqu1->Draw("same");
    hqu2->SetLineColor(kGreen);
    hqu2->Draw("same");
    hqu3->SetLineColor(kBlue);
    hqu3->Draw("same");
    c3->cd(2)->SetLogy();
    hqdall->Draw();
    hqd1->Draw("same");
    hqd2->Draw("same");
    hqd3->Draw("same");
    hqdall->SetLineColor(kBlack);
    hqd1->SetLineColor(kRed);
    hqd2->SetLineColor(kGreen);
    hqd3->SetLineColor(kBlue);
    c3->cd(3);
    h2diq->Draw("colz");

    TCanvas *c4=new TCanvas("c4","tof",700,700);
    c4->cd();
    hntof->Draw();
//    hvetotof->Draw("same");
//    hvetotof->SetLineColor(kBlack);

}
