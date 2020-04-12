void Draw1(){
	TFile *ipf=new TFile("tracking.root");
	TTree *tree=(TTree*)ipf->Get("tree");
	tree->Print();
	tree->Show(100);
	tree->Show(200);
	tree->Show(1000);
	tree->Show(10000);
	TCanvas *c1=new TCanvas("c1","c1");
	TH2D *hxz=(TH2D*) ipf->Get("htf8xz");
	c1->cd();
	gPad->SetLogz();
	hxz->Draw("colz");
	c1->Draw();
	c1->SaveAs("hxz.pdf");

	TCanvas *c2=new TCanvas("c2","c2");
	TH2D *hyz=(TH2D*) ipf->Get("htf8yz");
	c2->cd();
	gPad->SetLogz();
	hyz->Draw("colz");
	c2->Draw();
	c2->SaveAs("hyz.pdf");

	TCanvas *c3=new TCanvas("c3","c3");
	c3->cd();
	gPad->SetLogz();
	tree->Draw("ty:tx>>(120,-60,60,120,-60,60)","must2Trig","colz");
	c3->Draw();
	c3->SaveAs("tytx_must2Trig.pdf");


	TCanvas *c4=new TCanvas("c4","c4");
	c4->cd();
	gPad->SetLogz();
	tree->Draw("ty:tx>>(120,-60,60,120,-60,60)","beamTrig","colz");
	c4->Draw();
	c4->SaveAs("tytx_beamTrig.pdf");

	TCanvas *c5=new TCanvas("c5","c5");
	c5->cd();
	gPad->SetLogz();
	tree->Draw("ty:tx>>(120,-60,60,120,-60,60)","","colz");
	c5->Draw();
	c5->SaveAs("tytx_10001.pdf");


	TCanvas *c6=new TCanvas("c6","c6");
	c6->cd();
	tree->Draw("dx[0]>>(200,-5,5)","","");
	c6->Draw();
	c6->SaveAs("dx0.pdf");

	TCanvas *c11=new TCanvas("c11","c11");
	c11->cd();
	tree->Draw("dx[0]>>h111(100,-5,5)","abs(xx[0][0])<150 && nx==5 && ny==5","");
	tree->Draw("dxs[0]>>h112(100,-5,5)","abs(xxs[0])<150","");
	TH1D *h111 = (TH1D*)gDirectory->Get("h111");
	TH1D *h112 = (TH1D*)gDirectory->Get("h112");
	Float_t rightmax=h112->GetMaximum();
	Float_t leftmax=h111->GetMaximum();
	Float_t scale=leftmax/rightmax;
	h111->Draw();
	h112->Scale(scale);
	h112->SetLineColor(2);
	h112->Draw("same");
	//draw an axis on the right side 

	c11->SaveAs("dx01.pdf");

	TCanvas *c13=new TCanvas("c13","c13");
	c13->cd();
	tree->Draw("dy[0]>>h131(100,-5,5)","abs(yy[0][0])<150 && nx==5 && ny==5","");
	tree->Draw("dys[0]>>h132(100,-5,5)","abs(yys[0])<150","");
	TH1D *h131 = (TH1D*)gDirectory->Get("h131");
	TH1D *h132 = (TH1D*)gDirectory->Get("h132");
	Float_t rightmax1=h132->GetMaximum();
	Float_t leftmax1=h131->GetMaximum();
	Float_t scale1=leftmax1/rightmax1;
	h131->Draw();
	h132->Scale(scale1);
	h132->SetLineColor(2);
	h132->Draw("same");	
	c13->SaveAs("dy01.pdf");

	TCanvas *c7=new TCanvas("c7","c7");
	c7->cd();
	gPad->SetLogz();
	tree->Draw("dx[0]:dx[1]","","colz");
	c7->Draw();
	c7->SaveAs("dx0dx1.pdf");

	TCanvas *c12=new TCanvas("c12","c12");
	c12->Divide(2,2);
	c12->cd(1);
	gPad->SetLogz();
	tree->Draw("dx[0]:dx[1]","abs(xx[0][0])<150 && abs(xx[1][0])<150 && nx==2","colz");
	c12->cd(2);
	gPad->SetLogz();
	tree->Draw("dx[0]:dx[1]","abs(xx[0][0])<150 && abs(xx[1][0])<150 && nx==3","colz");
	c12->cd(3);
	gPad->SetLogz();
	tree->Draw("dx[0]:dx[1]","abs(xx[0][0])<150 && abs(xx[1][0])<150 && nx==4","colz");
	c12->cd(4);
	gPad->SetLogz();
	tree->Draw("dx[0]:dx[1]","abs(xx[0][0])<150 && abs(xx[1][0])<150 && nx==5","colz");
	c12->SaveAs("dx0dx11.pdf");
	c12->Draw();

	TCanvas *c8=new TCanvas("c8","c8");
	c8->Divide(2,2);
	c8->cd(1);
	gPad->SetLogz();
	tree->Draw("c2ny:dy[0]>>h81(100,-10,10,100,0,10)","abs(yy[0][0])<150 && ny==2","colz");
	c8->cd(2);
	gPad->SetLogz();
	tree->Draw("c2ny:dy[0]>>h82(100,-10,10,100,0,10)","abs(yy[0][0])<150 && ny==3","colz");
	c8->cd(3);
	gPad->SetLogz();
	tree->Draw("c2ny:dy[0]>>h83(100,-10,10,100,0,10)","abs(yy[0][0])<150 && ny==4","colz");
	c8->cd(4);
	gPad->SetLogz();
	tree->Draw("c2ny:dy[0]>>h84(100,-10,10,100,0,10)","abs(yy[0][0])<150 && ny==5","colz");
	c8->SaveAs("c2nydy.pdf");
	c8->Draw();

	TCanvas *c9=new TCanvas("c9","c9");
	c9->Divide(3,1);
	c9->cd(1);
	gPad->SetLogz();
	tree->Draw("tx:ty>>h91(120,-60,60,120,-60,60)","c2nx<10 && c2ny<10 ","colz");
	c9->cd(2);
	gPad->SetLogz();
	tree->Draw("tx:ty>>h92(120,-60,60,120,-60,60)","c2nx<10 && c2ny<10 && nx==5 && ny==5","colz");
	c9->cd(3);
	gPad->SetLogz();
	tree->Draw("txs:tys>>h93(120,-60,60,120,-60,60)","c2nx<10 && c2ny<10","colz");
	c9->SaveAs("txtyC2nx.pdf");
	c9->Draw();

	TCanvas *c10=new TCanvas("c10","c10",800,500);
	c10->cd();
	gPad->SetLogz();
	tree->Draw("tx:ty>>h10(120,-60,60,120,-60,60)","(c2nx>20 || c2ny>20) && beamTrig","colz");
	TH2D *h10 = (TH2D*)gDirectory->Get("h10");
	h10->Draw("colz");
	c10->Draw();
	c10->SaveAs("txtyC2ny.pdf");
	ipf->Close();
}