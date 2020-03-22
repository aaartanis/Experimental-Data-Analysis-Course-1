//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Mar 18 21:25:09 2020 by ROOT version 6.12/06
// from TTree tree/ppac
// found on file: ppac.root
//////////////////////////////////////////////////////////

#ifndef AnalyzeData_h
#define AnalyzeData_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class AnalyzeData {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Double_t        veto_x;
   Double_t        veto_e;
   Double_t        veto_tof;
   Int_t           veto_pid;
   Double_t        veto_tu;
   Double_t        veto_td;
   Double_t        veto_qu;
   Double_t        veto_qd;
   Int_t           veto_itu;
   Int_t           veto_itd;
   Int_t           veto_iqu;
   Int_t           veto_iqd;
   Double_t        tof_x;
   Double_t        tof_e;
   Double_t        tof_tof;
   Int_t           tof_pid;
   Double_t        tof_tu;
   Double_t        tof_td;
   Double_t        tof_qu;
   Double_t        tof_qd;
   Int_t           tof_itu;
   Int_t           tof_itd;
   Int_t           tof_iqu;
   Int_t           tof_iqd;

   // List of branches
   TBranch        *b_veto_x;   //!
   TBranch        *b_veto_e;   //!
   TBranch        *b_veto_tof;   //!
   TBranch        *b_veto_pid;   //!
   TBranch        *b_veto_tu;   //!
   TBranch        *b_veto_td;   //!
   TBranch        *b_veto_qu;   //!
   TBranch        *b_veto_qd;   //!
   TBranch        *b_veto_itu;   //!
   TBranch        *b_veto_itd;   //!
   TBranch        *b_veto_iqu;   //!
   TBranch        *b_veto_iqd;   //!
   TBranch        *b_tof_x;   //!
   TBranch        *b_tof_e;   //!
   TBranch        *b_tof_tof;   //!
   TBranch        *b_tof_pid;   //!
   TBranch        *b_tof_tu;   //!
   TBranch        *b_tof_td;   //!
   TBranch        *b_tof_qu;   //!
   TBranch        *b_tof_qd;   //!
   TBranch        *b_tof_itu;   //!
   TBranch        *b_tof_itd;   //!
   TBranch        *b_tof_iqu;   //!
   TBranch        *b_tof_iqd;   //!

   AnalyzeData(TTree *tree=0);
   virtual ~AnalyzeData();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef AnalyzeData_cxx
AnalyzeData::AnalyzeData(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("ppac.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("ppac.root");
      }
      f->GetObject("tree",tree);

   }
   Init(tree);
}

AnalyzeData::~AnalyzeData()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t AnalyzeData::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t AnalyzeData::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void AnalyzeData::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("veto_x", &veto_x, &b_veto_x);
   fChain->SetBranchAddress("veto_e", &veto_e, &b_veto_e);
   fChain->SetBranchAddress("veto_tof", &veto_tof, &b_veto_tof);
   fChain->SetBranchAddress("veto_pid", &veto_pid, &b_veto_pid);
   fChain->SetBranchAddress("veto_tu", &veto_tu, &b_veto_tu);
   fChain->SetBranchAddress("veto_td", &veto_td, &b_veto_td);
   fChain->SetBranchAddress("veto_qu", &veto_qu, &b_veto_qu);
   fChain->SetBranchAddress("veto_qd", &veto_qd, &b_veto_qd);
   fChain->SetBranchAddress("veto_itu", &veto_itu, &b_veto_itu);
   fChain->SetBranchAddress("veto_itd", &veto_itd, &b_veto_itd);
   fChain->SetBranchAddress("veto_iqu", &veto_iqu, &b_veto_iqu);
   fChain->SetBranchAddress("veto_iqd", &veto_iqd, &b_veto_iqd);
   fChain->SetBranchAddress("tof_x", &tof_x, &b_tof_x);
   fChain->SetBranchAddress("tof_e", &tof_e, &b_tof_e);
   fChain->SetBranchAddress("tof_tof", &tof_tof, &b_tof_tof);
   fChain->SetBranchAddress("tof_pid", &tof_pid, &b_tof_pid);
   fChain->SetBranchAddress("tof_tu", &tof_tu, &b_tof_tu);
   fChain->SetBranchAddress("tof_td", &tof_td, &b_tof_td);
   fChain->SetBranchAddress("tof_qu", &tof_qu, &b_tof_qu);
   fChain->SetBranchAddress("tof_qd", &tof_qd, &b_tof_qd);
   fChain->SetBranchAddress("tof_itu", &tof_itu, &b_tof_itu);
   fChain->SetBranchAddress("tof_itd", &tof_itd, &b_tof_itd);
   fChain->SetBranchAddress("tof_iqu", &tof_iqu, &b_tof_iqu);
   fChain->SetBranchAddress("tof_iqd", &tof_iqd, &b_tof_iqd);
   Notify();
}

Bool_t AnalyzeData::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void AnalyzeData::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t AnalyzeData::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef AnalyzeData_cxx
