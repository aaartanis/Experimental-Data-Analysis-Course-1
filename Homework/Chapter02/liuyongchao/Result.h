//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Mar 11 20:48:48 2020 by ROOT version 6.18/04
// from TTree tree/ppac
// found on file: ppac.root
//////////////////////////////////////////////////////////

#ifndef Result_h
#define Result_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class Result {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Double_t        ppac_x;
   Double_t        ppac_e;
   Double_t        ppac_tof;
   Int_t           ppac_pid;
   Double_t        ppac_tu;
   Double_t        ppac_td;
   Double_t        ppac_qu;
   Double_t        ppac_qd;
   Int_t           ppac_itu;
   Int_t           ppac_itd;
   Int_t           ppac_iqu;
   Int_t           ppac_iqd;
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
   TBranch        *b_ppac_x;   //!
   TBranch        *b_ppac_e;   //!
   TBranch        *b_ppac_tof;   //!
   TBranch        *b_ppac_pid;   //!
   TBranch        *b_ppac_tu;   //!
   TBranch        *b_ppac_td;   //!
   TBranch        *b_ppac_qu;   //!
   TBranch        *b_ppac_qd;   //!
   TBranch        *b_ppac_itu;   //!
   TBranch        *b_ppac_itd;   //!
   TBranch        *b_ppac_iqu;   //!
   TBranch        *b_ppac_iqd;   //!
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

   Result(TTree *tree=0);
   virtual ~Result();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef Result_cxx
Result::Result(TTree *tree) : fChain(0) 
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

Result::~Result()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t Result::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t Result::LoadTree(Long64_t entry)
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

void Result::Init(TTree *tree)
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

   fChain->SetBranchAddress("ppac_x", &ppac_x, &b_ppac_x);
   fChain->SetBranchAddress("ppac_e", &ppac_e, &b_ppac_e);
   fChain->SetBranchAddress("ppac_tof", &ppac_tof, &b_ppac_tof);
   fChain->SetBranchAddress("ppac_pid", &ppac_pid, &b_ppac_pid);
   fChain->SetBranchAddress("ppac_tu", &ppac_tu, &b_ppac_tu);
   fChain->SetBranchAddress("ppac_td", &ppac_td, &b_ppac_td);
   fChain->SetBranchAddress("ppac_qu", &ppac_qu, &b_ppac_qu);
   fChain->SetBranchAddress("ppac_qd", &ppac_qd, &b_ppac_qd);
   fChain->SetBranchAddress("ppac_itu", &ppac_itu, &b_ppac_itu);
   fChain->SetBranchAddress("ppac_itd", &ppac_itd, &b_ppac_itd);
   fChain->SetBranchAddress("ppac_iqu", &ppac_iqu, &b_ppac_iqu);
   fChain->SetBranchAddress("ppac_iqd", &ppac_iqd, &b_ppac_iqd);
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
   fChain->SetBranchAddress("tof_iqd", &tof_iqd, &b_ppac_iqd);
   Notify();
}

Bool_t Result::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void Result::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t Result::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef Result_cxx
