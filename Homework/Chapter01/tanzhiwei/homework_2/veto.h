//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Sun Mar 15 17:50:23 2020 by ROOT version 6.12/06
// from TTree tree/tree
// found on file: treeADC.root
//////////////////////////////////////////////////////////

#ifndef veto_h
#define veto_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class veto {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Double_t        x;
   Double_t        e;
   Double_t        tof;
   Double_t        ctof;
   Int_t           pid;
   Double_t        tu;
   Double_t        td;
   Double_t        qu1;
   Double_t        qd1;
   Double_t        qu;
   Double_t        qd;
   Int_t           itu;
   Int_t           iqu;
   Int_t           iqu1;
   Int_t           iqd1;
   Int_t           itd;
   Int_t           iqd;

   //Energy && TOF in plastic scintillator detector
   Double_t q0_dp,qu_dp,qd_dp;
   Int_t iqu_dp,iqd_dp;
   Double_t tu_dp,td_dp;
   Int_t itu_dp,itd_dp;
   Double_t tof_dp;
   Int_t flag_t,flag_e;

   //Energy && TOF in neutron detector
   Double_t q0_dn;

   // List of branches
   TBranch        *b_x;   //!
   TBranch        *b_e;   //!
   TBranch        *b_tof;   //!
   TBranch        *b_ctof;   //!
   TBranch        *b_pid;   //!
   TBranch        *b_tu;   //!
   TBranch        *b_td;   //!
   TBranch        *b_qu1;   //!
   TBranch        *b_qd1;   //!
   TBranch        *b_qu;   //!
   TBranch        *b_qd;   //!
   TBranch        *b_itu;   //!
   TBranch        *b_iqu;   //!
   TBranch        *b_iqu1;   //!
   TBranch        *b_iqd1;   //!
   TBranch        *b_itd;   //!
   TBranch        *b_iqd;   //!

   veto(TTree *tree=0);
   virtual ~veto();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
   virtual Int_t    label_e(Int_t iqu_dp,Int_t iqd_dp);
   virtual Int_t    label_t(Int_t itu_dp,Int_t itd_dp);
};

#endif

#ifdef veto_cxx
veto::veto(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("treeADC.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("treeADC.root");
      }
      f->GetObject("tree",tree);

   }
   Init(tree);
}

veto::~veto()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t veto::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t veto::LoadTree(Long64_t entry)
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

void veto::Init(TTree *tree)
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

   fChain->SetBranchAddress("x", &x, &b_x);
   fChain->SetBranchAddress("e", &e, &b_e);
   fChain->SetBranchAddress("tof", &tof, &b_tof);
   fChain->SetBranchAddress("ctof", &ctof, &b_ctof);
   fChain->SetBranchAddress("pid", &pid, &b_pid);
   fChain->SetBranchAddress("tu", &tu, &b_tu);
   fChain->SetBranchAddress("td", &td, &b_td);
   fChain->SetBranchAddress("qu1", &qu1, &b_qu1);
   fChain->SetBranchAddress("qd1", &qd1, &b_qd1);
   fChain->SetBranchAddress("qu", &qu, &b_qu);
   fChain->SetBranchAddress("qd", &qd, &b_qd);
   fChain->SetBranchAddress("itu", &itu, &b_itu);
   fChain->SetBranchAddress("iqu", &iqu, &b_iqu);
   fChain->SetBranchAddress("iqu1", &iqu1, &b_iqu1);
   fChain->SetBranchAddress("iqd1", &iqd1, &b_iqd1);
   fChain->SetBranchAddress("itd", &itd, &b_itd);
   fChain->SetBranchAddress("iqd", &iqd, &b_iqd);
   Notify();
}

Bool_t veto::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void veto::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t veto::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef veto_cxx
