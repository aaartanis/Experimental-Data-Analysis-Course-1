#define Result_cxx
#include "Result.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void Result::Loop()
{
//   In a ROOT session, you can do:
//      root> .L Result.C
//      root> Result t
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
	TCanvas *c0=new TCanvas("c0","c0");
	TH1D *tof=new TH1D("tof","time of flight",100,0,100);//提取gamma tof范围，方便后续的pid
	TCanvas *c1=new TCanvas("c1","c1");
	TH1D *htu=new TH1D("htu","htu",420,0,4200);
	TH1D *htd=new TH1D("htd","htd",420,0,4200);
	TCanvas *c2=new TCanvas("c2","c2");
	TH2D *itu_itd=new TH2D("itu_itd","itu:itd",420,0,4200,420,0,4200);
	TCanvas *c3=new TCanvas("c3","c3");
	TH1D *hquall=new TH1D("hquall","hquall",420,0,4200); 
	TH1D *hqu1=new TH1D("hqu1","hqu1",420,0,4200); 
	TH1D *hqu2=new TH1D("hqu2","hqu2",420,0,4200); 
	TH1D *hqu3=new TH1D("hqu3","hqu3",420,0,4200); 
	TCanvas *c4=new TCanvas("c4","c4");
	TH1D *hqdall=new TH1D("hqdall","hqdall",420,0,4200); 
	TH1D *hqd1=new TH1D("hqd1","hqd1",420,0,4200); 
	TH1D *hqd2=new TH1D("hqd2","hqd2",420,0,4200); 
	TH1D *hqd3=new TH1D("hqd3","hqd3",420,0,4200); 
	TCanvas *c7=new TCanvas("c7","c7");
	TH1D *htdiff=new TH1D("htdiff","tu-td",400,-1800,600);
	if (fChain == 0) return;
	Long64_t nentries = fChain->GetEntriesFast();
	Long64_t nbytes = 0, nb = 0;
	for (Long64_t jentry=0; jentry<nentries;jentry++) {
		Long64_t ientry = LoadTree(jentry);
		if (ientry < 0) break;
		nb = fChain->GetEntry(jentry);   nbytes += nb;
		// if (Cut(ientry) < 0) continue;
		htu->Fill(tof_itd);
		htd->Fill(tof_itu);
		itu_itd->Fill(tof_itd,tof_itu);
		tof->Fill(tof_tof);
		hquall->Fill(tof_iqu);
		hqdall->Fill(tof_iqd);
		if (tof_pid==0||tof_pid==1) {hqu1->Fill(tof_iqu);hqd1->Fill(tof_iqd);}
		if (tof_pid==2) {hqu2->Fill(tof_iqu);hqd2->Fill(tof_iqd);}
		if (tof_pid==3) {hqu3->Fill(tof_iqu);hqd3->Fill(tof_iqd);}
		if (tof_itu<4000 && tof_itd<4000) {htdiff->Fill(tof_itu - tof_itd);}

	}
	c0->cd();
	tof->Fit("gaus","","",10,20);
	tof->Draw();
	c0->SetLogy();
	c0->Draw();
	c0->SaveAs("tof.pdf");
	c1->cd();
	htu->Draw();
	htd->SetLineColor(kGreen);
	htd->Draw("same");
	c1->SetLogy();
	c1->Draw();
	c1->SaveAs("htuhtd.pdf");
	c2->cd();
	itu_itd->Draw("colz");
	c2->Draw();
	c2->SaveAs("itu_itd.pdf");
	c3->cd();
	c3->SetLogy();
	hquall->SetLineColor(kBlack);
	hquall->Draw();
	hqu1->SetLineColor(kRed);
	hqu1->Draw("same");
	hqu2->SetLineColor(kGreen);
	hqu2->Draw("same");
	hqu3->SetLineColor(kBlue);
	hqu3->Draw("same");
	hquall->Fit("gaus","","",50,150);
	c3->Draw();
	c3->SaveAs("hquall.pdf");
	c4->cd();
	c4->SetLogy();
	hqdall->SetLineColor(kBlack);
	hqdall->Draw();
	hqd1->SetLineColor(kRed);
	hqd1->Draw("same");
	hqd2->SetLineColor(kGreen);
	hqd2->Draw("same");
	hqd3->SetLineColor(kBlue);
	hqd3->Draw("same");
	hqdall->Fit("gaus","","",0,250);
	c4->Draw();
	c4->SaveAs("hqdall.pdf");
	c7->cd();
	htdiff->Draw();
	c7->Draw();
	c7->SaveAs("htdiff.pdf");

	TF1 *f1;
	Double_t gamma_center,gamma_sigma;
	gamma_center=tof->GetFunction("gaus")->GetParameter(1);
	gamma_sigma=tof->GetFunction("gaus")->GetParameter(2);
	cout<<"the gamma center:"<<gamma_center<<endl;
	cout<<"the gamma sigma:"<<gamma_center<<endl;

	TF1 *fgaus[2];
	Double_t ped[2],sigma[2];
	TString sq[2]={"qu","qd"};
	fgaus[0]=hquall->GetFunction("gaus");
	fgaus[1]=hqdall->GetFunction("gaus");
	for (int i = 0; i < 2; i++)
	{
		ped[i]=fgaus[i]->GetParameter(1);
		sigma[i]=fgaus[i]->GetParameter(2);
		TString ss;
		ss.Form("ped_%s=%.2f,sigma_%s=%.2f",sq[i].Data(),ped[i],sq[i].Data(),sigma[i]);
		cout<<ss<<endl;

	}

	TCanvas *c5=new TCanvas("c5","c5");
	TH2D *iqu_iqd=new TH2D("iqu_iqd","iqu_iqd",200,0,4000,200,0,4000);
	TCanvas *c6=new TCanvas("c6","c6");
	TH2D *iqu_iqd_diff2=new TH2D("iqu_iqd_diff2","iqu_iqd_diff2",200,0,4000,200,0,4000);
	TCanvas *c8=new TCanvas("c8","c8");
	TH1D *logiquiqd=new TH1D("logiquiqd","log(iqu/iqd)",100,-1,1);



	for (Long64_t jentry=0; jentry<nentries;jentry++) {
		Long64_t ientry = LoadTree(jentry);
		if (ientry < 0) break;
		nb = fChain->GetEntry(jentry);   nbytes += nb;
		// if (Cut(ientry) < 0) continue;
		if (tof_iqd<4000 && tof_iqd>ped[1]+sigma[1] && tof_iqu<4000 && tof_iqu>ped[0]+sigma[0]){
			iqu_iqd->Fill(tof_iqd-ped[1],tof_iqu-ped[0]);}
		if (tof_iqd<4000 && tof_iqd>ped[1]+sigma[1] && tof_iqu<4000 && tof_iqu>ped[0]+sigma[0] &&
		 ((ppac_itu>0 && ppac_itu<4000)||(ppac_itd>0 && ppac_itd<4000)) && tof_tof>gamma_center+3*gamma_sigma){
			iqu_iqd_diff2->Fill(tof_iqd-ped[1],tof_iqu-ped[0]);
			logiquiqd->Fill(log((tof_iqu-ped[0])/(tof_iqd-ped[1])));}
	}
	c5->cd();
	iqu_iqd->Draw("colz");
	c5->Draw();
	c5->SaveAs("iqu_iqd.pdf");
	c6->cd();
	iqu_iqd_diff2->Draw("colz");
	c6->Draw();
	c6->SaveAs("iqu_iqd_diff2.pdf");
	c8->cd();
	logiquiqd->Draw();
	c8->Draw();
	c8->SaveAs("logiquiqd.pdf");

	//qx
	TCanvas *c9=new TCanvas("c9","c9");
	TH1D *dqd=new TH1D("qdq","qdq",100,-1,1);
	for(int i=1;i<logiquiqd->GetNbinsX();i++)
	{
		Double_t dq=logiquiqd->GetBinContent(i+1)-logiquiqd->GetBinContent(i);
		dqd->Fill(logiquiqd->GetBinCenter(i),dq);
	}
	c9->cd();
	dqd->Sumw2(0);
	dqd->Fit("gaus","","",-0.8,-0.3);
	dqd->Draw();
	TF1 *f2=new TF1("f2","[0]*TMath::Exp(-0.5*((x-[1])/[2])^2)",0.3,0.8);
	f2->SetParameter(0,-430);
	f2->SetParameter(1,0.5);
	f2->SetParameter(2,0.2);
	dqd->Fit("f2","R+");
	dqd->Draw();
	// c3->Draw();
	c9->SaveAs("dqd.pdf");	


	Double_t ql,qr,qx;
	Double_t L=100.;
	ql=dqd->GetFunction("gaus")->GetParameter(1);
	qr=dqd->GetFunction("f2")->GetParameter(1);
	cout<<ql<<endl;
	cout<<qr<<endl;
	TCanvas *c10=new TCanvas("c10","c10");
	TH2D *qxdx=new TH2D("qxdx","qx-x:x",1200,-120,120,1200,-120,120);
	TCanvas *c11=new TCanvas("c11","c11");
	TH1D *qx1=new TH1D("qx1","qx",120,-120,120);
	for (Long64_t jentry=0; jentry<nentries;jentry++) {
		Long64_t ientry = LoadTree(jentry);
		if (ientry < 0) break;
		nb = fChain->GetEntry(jentry);   nbytes += nb;
		// if (Cut(ientry) < 0) continue;
		if (tof_iqd<4000 && tof_iqd>ped[1]+sigma[1] && tof_iqu<4000 && tof_iqu>ped[0]+sigma[0] &&
		 ((ppac_itu>0 && ppac_itu<4000)||(ppac_itd>0 && ppac_itd<4000)) && tof_tof>gamma_center+3*gamma_sigma){
			qx=2.*L/(qr-ql)*(log((tof_iqu-ped[0])/(tof_iqd-ped[1]))-(qr+ql)/2);
			qxdx->Fill(qx-tof_x,tof_x);
			qx1->Fill(qx);
		}

		
	}
	c10->cd();
	qxdx->Draw("colz");
	c10->Draw();
	c10->SaveAs("qxdx.pdf");
	c11->cd();
	qx1->Draw();
	c11->Draw();
	c11->SaveAs("qx1.pdf");
}
