void processing()
{
	TFile *ipf = new TFile("tracking.root");
	TTree *tree = (TTree*)ipf->Get("tree");

	TCut ct1="beamTrig"; 
	TCut ctar="abs(targetX)<20 && abs(targetY)<12.5";
	TCut ctChi="c2nx <=1 && c2ny <=1";

	TCanvas *c1=new TCanvas("c1");
	tree->Draw("targetX:targetY>>htx0(240,-60,60,240,-60,60)",ct1 && ctChi);
	TH2D *htx0=(TH2D*)gROOT->FindObject("htx0");
	htx0->Draw("colz");
	c1->Draw();
	
	TCanvas *c2=new TCanvas("c2");
	tree->Draw("targetX:targetY>>htx1(240,-60,60,240,-60,60",ct1 && ctChi &&ctar);
	TH2D *htx1=(TH2D*)gROOT->FindObject("htx1");
	htx1->Draw("colz");
	c2->Draw();


	Long64_t Ntar_all,Ntar_in;

	Ntar_all=tree->GetEntries(ct1 && ctChi);
	Ntar_in=tree->GetEntries(ct1 && ctChi && ctar);
	Double_t e_target;
	e_target=Double_t(Ntar_in)/Ntar_all;

	TString result;
	result.Form("target:\n e_target=%.2f%% ",e_target*100);
	cout<<result.Data()<<endl;



}