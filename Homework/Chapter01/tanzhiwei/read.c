void read(){
		TFile *ipf=new TFile("tof.root");
		TTree *tree=(TTree*)ipf->Get("tree");

		Double_t x;
		Double_t e;
		Double_t tof,ctof;
		Double_t tu,td;
		Double_t qu,qd;
		int pid;

		tree->SetBranchAddress("ctof",&ctof);
		tree->SetBranchAddress("pid",&pid);
		tree->SetBranchAddress("tu",&tu);
		tree->SetBranchAddress("td",&td);
		tree->SetBranchAddress("qu",&qu);
		tree->SetBranchAddress("qd",&qd);
		tree->SetBranchAddress("x",&x);

		TH1D *hTOF=new TH1D("hTOF","time of filght",1000,0,100);
		TH1D *tdiff=new TH1D("tdiff","td-tu",140,-20,50);
		TH1D *dtd=new TH1D("dtd","dt/dx",141,-20.25,50.25);
		TH1D *htx=new TH1D("htx","htx",500,-120,120);
		TH2D *hdx=new TH2D("hdx","htx-hx:hx",100,-20,20,500,-120,120);
		TH1D *hgctof=new TH1D("hgctof","hgctof",100,39,48);
		TH2D *hgtofx=new TH2D("hgtofx","hgtofx",100,-120,120,100,39,48);
		TH2D *hgtofx_correct=new TH2D("hgtof_correct","corrected TOF",100,-120,120,100,15,19);
		TH1D *htof_correct=new TH1D("htof_correct","htof_correct",200,0,100);
		TH1D *hqx=new TH1D("hqx","hqx",500,-120,120);
		TH2D *hqx_dif=new TH2D("hqx_dif","hqx-hx:hx",100,-20,20,500,-120,120);
		
		TH1D *hntof=new TH1D("hntof","hntof",160,0,16);
		TH1D *hce=new TH1D("hce","hce",200,0,200);
		Long64_t nentries=tree->GetEntries();

		Double_t tx,qx,ntof,ce;

		TFile *opf2=new TFile("result.root","recreate");
		TTree *opt=new TTree("tree","tree");
		opt->Branch("x",&x,"x/D");
		opt->Branch("ctof",&ctof,"ctof/D");
		opt->Branch("tu",&tu,"tu/D");
		opt->Branch("td",&td,"td/D");
		opt->Branch("qu",&qu,"qu/D");
		opt->Branch("qd",&qd,"qd/D");
		opt->Branch("pid",&pid,"x/I");
		opt->Branch("tx",&tx,"tx/D");
		opt->Branch("qx",&qx,"qx/D");
		opt->Branch("ntof",&ntof,"ntof/D");
		opt->Branch("ce",&ce,"ce/D");

		for(Long64_t jentry=0;jentry<nentries;jentry++){
				tree->GetEntry(jentry);
				hTOF->Fill(ctof);
				tdiff->Fill(td-tu);
				tx=3.75*(td-tu-14.86);//
				htx->Fill(tx);
				hdx->Fill(tx-x,x);
				qx=190*TMath::Log(qu/qd);//
				hqx->Fill(qx);
				hqx_dif->Fill(qx-x,x);
				if(ctof>42&&ctof<44){
						hgtofx->Fill(tx,ctof);
						if(abs(tx<5))
								hgctof->Fill(ctof);
								}
				Double_t d=TMath::Sqrt(502.5*502.5+tx*tx);
				Double_t tof_correct=(ctof-26.305)/d*500;
				hgtofx_correct->Fill(tx,tof_correct);
				htof_correct->Fill(tof_correct);

				ntof=(ctof-26.305)/d*100; //normalized to 100cm
				hntof->Fill(ntof);
				if(ntof>5&&ntof<11){
						ce=(72*1/ntof)*(72*1/ntof);
						hce->Fill(ce);
						}
				opt->Fill();
				if(jentry%10000==0)
						cout<<"process "<<jentry<<" of "<<nentries<<endl;
		}

		tdiff->SetTitle("tdiff:td-tu");
		auto c1=new TCanvas("c1","c1");
		tdiff->Draw();
//		hTOF->Draw();
		
		for(int i=1;i<tdiff->GetNbinsX();i++){
				Double_t df=tdiff->GetBinContent(i+1)-tdiff->GetBinContent(i);
				dtd->Fill(tdiff->GetBinLowEdge(i+1),df);

		}

		dtd->Sumw2(0);
		auto c2=new TCanvas("c2","c2");
		dtd->Draw();
		dtd->Fit("gaus","","",-14,-10);
		TF1 *f1=new TF1("f1","[0]*TMath::Exp(-0.5*((x-[1])/[2])^2)",39.5,43);
		f1->SetParameter(0,-350);
		f1->SetParameter(1,41.5);
		f1->SetParameter(2,0.5);
		dtd->Fit("f1","R+");

        //评估计算结果
		auto c3=new TCanvas("c3","c3");
		hdx->Draw("colz");
/*
		auto c4=new TCanvas("c4","c4");
		TH1D *hdx1=hdx->ProjectionX("projx of hdx");
		hdx1->SetTitle("hdx1");
		hdx1->Draw();
		hdx1->Fit("gaus","","",-5,5);
*/
		///TOF绝对刻度,利用光子飞行时间进行飞行距离修正
		auto c5=new TCanvas("c5","c5");
//		hgtofx->Draw("colz");
		hgctof->Draw();
		hgctof->Fit("gaus","","",42,44);
		
		auto c6=new TCanvas("c6","c6");
//		hgtofx_correct->Draw("colz");
		c6->SetLogy();
		htof_correct->Draw();

		auto c7=new TCanvas("c7","c7");
//		hqx->Draw();
		hqx_dif->Draw("colz");

		auto c8=new TCanvas("c8","c8");
//		hntof->Draw();	
		hce->Draw();
		hce->Fit("gaus","","",40,180);

		opt->Write();
		opf2->Close();

}
