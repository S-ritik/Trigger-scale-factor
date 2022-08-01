#include "TROOT.h"
#include "TChain.h"
#include "TProof.h"
#include "TProofServ.h"
#include "TSystem.h"
//#include "boost/config.hpp"
//#include "boost/lexical_cast.hpp"
//using namespace boost;

void runAll(){

 TProof *plite = TProof::Open("");

 char datafile[100];
// TChain *fChain = new TChain("ttDM__noSyst");
 TChain *fChain = new TChain("T1");

 char rootfiles[100];
 /*
 //sprintf(rootfiles, "TTbar_Hadronic_Jan2021_19UL18.log");
 //sprintf(rootfiles, "QCD_HT2000toInfJan2021_19UL18.log");
 //sprintf(rootfiles, "QCD_BEnriched_HT2000toInfJan2021.log");
 //sprintf(rootfiles, "TTbar_Dileptonic_Jan2021.log");
 //sprintf(rootfiles, "QCD_BEnriched_HT2000toInfJan2021.log");
 //sprintf(rootfiles, "MuEG2018C_Feb2021.log");
 sprintf(rootfiles,"test.log");
 //sprintf(rootfiles,"MuEG2018D_Feb2021.log");
 //sprintf(rootfiles,"WJetsToLNu_2500toInfHTFeb2021.log");
 //sprintf(rootfiles,"DYJetsToLL_M-50_2500toInfHTFeb2012.log");

 ifstream file_db;
 file_db.open(rootfiles); 

 while(!(file_db.eof())){
   file_db >> datafile;
   cout <<"datafile name is "<<datafile<<endl;
   if (strstr(datafile,"#")) continue;
    
   if(file_db.eof()) break;
 */
   fChain->Add(/*datafile*/"rootuple_jerc_l5_dilep_june_2022.root");
   /* cout<<"Added "<<datafile<<endl;
   }
   */
  std::cout<<"Entries:" << fChain->GetEntries()<<std::endl;
  
  fChain->SetProof();
  fChain->Process("Trigg_eff_cal.C+");
  fChain->SetProof(0);
  //  gSystem->Exit(0);
}

//void main()
//{
  //runAll();
  //}
