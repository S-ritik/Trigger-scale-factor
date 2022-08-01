#include "Trigg_eff_cal_getobjects.h"
  
#include <TH2.h>
#include <TStyle.h>
#include <TVector3.h>
#include <TH1F.h>
#include <TH2F.h>
#include <fstream>
#include <TProofOutputFile.h>
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <TProofServ.h>

void Trigg_eff_cal::Begin(TTree * /*tree*/)
{
  // The Begin() function is called at the start of the query.
  // When running with PROOF Begin() is only called on the client.
  // The tree argument is deprecated (on PROOF 0 is passed).
  
  TString option = GetOption();

}
 
void Trigg_eff_cal::SlaveBegin(TTree * /*tree*/)
{
  //The SlaveBegin() function is called after the Begin() function.
  //When running with PROOF SlaveBegin() is called on each slave server.
  //The tree argument is deprecated (on PROOF 0 is passed).
  
  TString option = GetOption();

  OutFile = new TProofOutputFile("test_output.root");
  
  fileOut = OutFile->OpenFile("RECREATE");
  if ( !(fileOut = OutFile->OpenFile("RECREATE")) )
    {
      Warning("SlaveBegin", "problems opening file: %s/%s",OutFile->GetDir(), OutFile->GetFileName());
    }

  isMC = true;
  isTT = true;
  isST = false;
  isDIB = false;
  isWJ = false;
  isDY = false;
  isQCD = false;
  isTTX = false;

  Tnewvar = new TTree("newvars","newvars");
  Tnewvar->Branch("ievt", &ievt, "ievt/i");
  Tnewvar->Branch("weight",&weight,"weight/F");
  Tnewvar->Branch("lep1pt",&lep1pt,"lep1pt/F");
  Tnewvar->Branch("lep2pt",&lep2pt,"lep2pt/F");
  Tnewvar->Branch("lep1pdg",&lep1pdg,"lep1pdg/F");
  Tnewvar->Branch("lep2pdg",&lep2pdg,"lep2pdg/F");
  Tnewvar->Branch("ntriggerobjs",&ntriggerobjs,"ntriggerobjs/F");
  Tnewvar->Branch("delpt_el",delpt_el,"delpt_el[ntrigobjmx]/F");
  Tnewvar->Branch("delpt_mu",delpt_mu,"delpt_mu[ntrigobjmx]/F");
  Tnewvar->Branch("delN_el",delN_el,"delN_el[ntrigobjmx]/F");
  Tnewvar->Branch("delN_mu",delN_mu,"delN_mu[ntrigobjmx]/F");
  Tnewvar->Branch("nbjets",&nbjets,"nbjets/F");
  Tnewvar->Branch("invmll",&invmll,"invmll/F");
  Tnewvar->Branch("trig_ee",&trig_ee,"trig_ee/F");
  Tnewvar->Branch("trig_emu",&trig_emu,"trig_emu/F");
  Tnewvar->Branch("trig_mumu",&trig_mumu,"trig_mumu/F");
  Tnewvar->Branch("trig_met",&trig_met,"trig_met/F");
  Tnewvar->Branch("response_ak81",&response_ak81,"response_ak81/F");
  Tnewvar->Branch("response_ak82",&response_ak82,"response_ak82/F");
    //  Tnewvar->Branch("",&,"/F");
  
  //Tnewvar->Branch("",&,"/F");
  /* for(int init=0; init<nhist_in; init++){
    char namein[1000]; //nameinup[1000], nameindn[1000];
    char titlein[1000];
    
    sprintf(namein,"hist_%s",initnames[init]);
    sprintf(titlein,"%s",titlenames[init]);
    hist_init[init] = new TH1D(namein,titlein,ini_nbins[init],ini_low[init],ini_up[init]);
    hist_init[init]->Sumw2();
  }

  for (int ij=0; ij<npr_angle; ij++) 
    hist_prptangle[ij] = new TH2D(pr_angle[ij], pr_angle[ij], 6, 10.0, 510.0, 7, 0.0, 2.1);
*/
    for(int init=0; init<nhist_in_N; init++){
    char namein[1000]; //nameinup[1000], nameindn[1000];
    char titlein[1000];
    
    sprintf(namein,"hist_%s",initnames_N[init]);
    sprintf(titlein,"%s",titlenames_N[init]);
    hist_init_N[init] = new TH1D(namein,titlein,ini_nbins_N[init],ini_low_N[init],ini_up_N[init]);
    hist_init_N[init]->Sumw2();
  }

  for (int ij=0; ij<npr_angle_N; ij++) 
    hist_prptangle_N[ij] = new TH2D(pr_angle_N[ij], pr_angle_N[ij], 6, 10.0, 510.0, 7, 0.0, 2.1);

  for(int ich=0; ich< nchannel; ich++){
    hist_count[ich] = new TH1D((channel[ich]+"Counter").c_str(),(channel[ich]+"Counter").c_str(),10,-0.5,9.5);
    hist_count[ich]->Sumw2();  
  }
  
  for(int ich=0; ich< nchannel; ich++){   /// nchannel declared in .h file

    hist_ortho_trig_2d[0][ich] = new TH2D((channel[ich]+"_hist_orthotrig_lepptvsleppt").c_str(),(channel[ich]+"_hist_orthotrig_lepptvsleppt").c_str(),nbins_ptlep,ptbins_lep,nbins_ptlep,ptbins_lep);
    hist_ortho_trig_2d[1][ich] = new TH2D((channel[ich]+"_hist_orthotrig_lepetavslepeta").c_str(),(channel[ich]+"_hist_orthotrig_lepetavslepeta").c_str(),5,-2.5,2.5,5,-2.5,2.5);
    hist_ortho_trig_2d[2][ich] = new TH2D((channel[ich]+"_hist_orthotrig_lepphivslepphi").c_str(),(channel[ich]+"_hist_orthotrig_lepphivslepphi").c_str(),5,-1*TMath::Pi(),TMath::Pi(),5,-1*TMath::Pi(),TMath::Pi());
    hist_ortho_trig_2d[3][ich] = new TH2D((channel[ich]+"_hist_orthotrig_lepptvsleppt_withoutweight").c_str(),(channel[ich]+"_hist_orthotrig_lepptvslepptt_withoutweight").c_str(),nbins_ptlep,ptbins_lep,nbins_ptlep,ptbins_lep);
    hist_ortho_trig[0][ich] = new TH1D((channel[ich]+"_hist_orthotrig_leadleppt").c_str(),(channel[ich]+"_hist_orthotrig_leadleppt").c_str(),5,0,1000);
    hist_ortho_trig[1][ich] = new TH1D((channel[ich]+"_hist_orthotrig_dilepmass").c_str(),(channel[ich]+"_hist_orthotrig_dilepmass").c_str(),8,50,130);
    hist_ortho_trig[2][ich] = new TH1D((channel[ich]+"_hist_orthotrig_leadlepphi").c_str(),(channel[ich]+"_hist_orthotrig_leadlepphi").c_str(),5,-1*TMath::Pi(),TMath::Pi());
    hist_ortho_trig[3][ich] = new TH1D((channel[ich]+"_hist_orthotrig_leadlepeta").c_str(),(channel[ich]+"_hist_orthotrig_leadlepeta").c_str(),5,-2.5,2.5);
    hist_ortho_trig[4][ich] = new TH1D((channel[ich]+"_hist_orthotrig_njets").c_str(),(channel[ich]+"_hist_orthotrig_njets").c_str(),11,-0.5,10.5);

    hist_trig_eff_2d[0][ich] = new TH2D((channel[ich]+"_hist_trig_eff_lepptvsleppt").c_str(),(channel[ich]+"_hist_trig_eff_lepptvsleppt").c_str(),nbins_ptlep,ptbins_lep,nbins_ptlep,ptbins_lep);
    hist_trig_eff_2d[1][ich] = new TH2D((channel[ich]+"_hist_trig_eff_lepetavslepeta").c_str(),(channel[ich]+"_hist_trig_eff_lepetavslepeta").c_str(),5,-2.5,2.5,5,-2.5,2.5);
    hist_trig_eff_2d[2][ich] = new TH2D((channel[ich]+"_hist_trig_eff_lepphivslepphi").c_str(),(channel[ich]+"_hist_trig_eff_lepphivslepphi").c_str(),5,-1*TMath::Pi(),TMath::Pi(),5,-1*TMath::Pi(),TMath::Pi());
    hist_trig_eff_2d[3][ich] = new TH2D((channel[ich]+"_hist_trig_eff_lepptvsleppt_withoutweight").c_str(),(channel[ich]+"_hist_trig_eff_lepptvsleppt_withoutweight").c_str(),nbins_ptlep,ptbins_lep,nbins_ptlep,ptbins_lep);
    hist_trig_eff[0][ich] = new TH1D((channel[ich]+"_hist_trig_eff_leadleppt").c_str(),(channel[ich]+"_hist_trig_eff_leadleppt").c_str(),5,0,1000);
    hist_trig_eff[1][ich] = new TH1D((channel[ich]+"_hist_trig_eff_dilepmass").c_str(),(channel[ich]+"_hist_trig_eff_dilepmass").c_str(),8,50,130);
    hist_trig_eff[2][ich] = new TH1D((channel[ich]+"_hist_trig_eff_leadlepphi").c_str(),(channel[ich]+"_hist_trig_eff_leadlepphi").c_str(),5,-1*TMath::Pi(),TMath::Pi());
    hist_trig_eff[3][ich] = new TH1D((channel[ich]+"_hist_trig_eff_leadlepeta").c_str(),(channel[ich]+"_hist_trig_eff_leadlepeta").c_str(),5,-2.5,2.5);
    hist_trig_eff[4][ich] = new TH1D((channel[ich]+"_hist_trig_eff_njets").c_str(),(channel[ich]+"_hist_trig_eff_njets").c_str(),11,-0.5,10.5);

    hist_trig_eff_2d_unmatched[0][ich] = new TH2D((channel[ich]+"_hist_trig_eff_lepptvsleppt_unmatched").c_str(),(channel[ich]+"_hist_trig_eff_lepptvsleppt_unmatched").c_str(),nbins_ptlep,ptbins_lep,nbins_ptlep,ptbins_lep);
    hist_trig_eff_2d_unmatched[1][ich] = new TH2D((channel[ich]+"_hist_trig_eff_lepetavslepeta_unmatched").c_str(),(channel[ich]+"_hist_trig_eff_lepetavslepeta_unmatched").c_str(),5,-2.5,2.5,5,-2.5,2.5);
    hist_trig_eff_2d_unmatched[2][ich] = new TH2D((channel[ich]+"_hist_trig_eff_lepphivslepphi_unmatched").c_str(),(channel[ich]+"_hist_trig_eff_lepphivslepphi_unmatched").c_str(),5,-1*TMath::Pi(),TMath::Pi(),5,-1*TMath::Pi(),TMath::Pi());
    hist_trig_eff_2d_unmatched[3][ich] = new TH2D((channel[ich]+"_hist_trig_eff_lepptvsleppt_withoutweight_unmatched").c_str(),(channel[ich]+"_hist_trig_eff_lepptvsleppt_withoutweight_unmatched").c_str(),nbins_ptlep,ptbins_lep,nbins_ptlep,ptbins_lep);
    hist_trig_eff_unmatched[0][ich] = new TH1D((channel[ich]+"_hist_trig_eff_leadleppt_unmatched").c_str(),(channel[ich]+"_hist_trig_eff_leadleppt_unmatched").c_str(),5,0,1000);
    hist_trig_eff_unmatched[1][ich] = new TH1D((channel[ich]+"_hist_trig_eff_dilepmass_unmatched").c_str(),(channel[ich]+"_hist_trig_eff_dilepmass_unmatched").c_str(),8,50,130);
    hist_trig_eff_unmatched[2][ich] = new TH1D((channel[ich]+"_hist_trig_eff_leadlepphi_unmatched").c_str(),(channel[ich]+"_hist_trig_eff_leadlepphi_unmatched").c_str(),5,-1*TMath::Pi(),TMath::Pi());
    hist_trig_eff_unmatched[3][ich] = new TH1D((channel[ich]+"_hist_trig_eff_leadlepeta_unmatched").c_str(),(channel[ich]+"_hist_trig_eff_leadlepeta_unmatched").c_str(),5,-2.5,2.5);
    hist_trig_eff_unmatched[4][ich] = new TH1D((channel[ich]+"_hist_trig_eff_njets_unmatched").c_str(),(channel[ich]+"_hist_trig_eff_njets_unmatched").c_str(),11,-0.5,10.5);
      
  }

  calib_deepflav = BTagCalibration("DeepJet", (dir + "DeepJet_106XUL18SF_WPonly_V1p1.csv").Data());
  reader_deepflav_med = BTagCalibrationReader(BTagEntry::OP_MEDIUM, "central", {"up", "down"}); 
  reader_deepflav_med.load(calib_deepflav, BTagEntry::FLAV_B, "comb");
  reader_deepflav_med.load(calib_deepflav, BTagEntry::FLAV_C, "comb");
  reader_deepflav_med.load(calib_deepflav, BTagEntry::FLAV_UDSG, "incl");

  reader_deepflav_loose = BTagCalibrationReader(BTagEntry::OP_LOOSE, "central", {"up", "down"}); 
  reader_deepflav_loose.load(calib_deepflav, BTagEntry::FLAV_B, "comb");
  reader_deepflav_loose.load(calib_deepflav, BTagEntry::FLAV_C, "comb");
  reader_deepflav_loose.load(calib_deepflav, BTagEntry::FLAV_UDSG, "incl");

  reader1 = new TMVA::Reader( "BDTG_Re" );
  reader1->AddVariable("selpfjetAK8NHadF", &in_pfjetAK8NHadF);
  reader1->AddVariable("selpfjetAK8neunhadfrac", &in_pfjetAK8neunhadfrac);
  reader1->AddVariable("selpfjetAK8subhaddiff", &in_pfjetAK8subhaddiff);
  reader1->AddVariable("selpfjetAK8tau21", &in_pfjetAK8tau21);
  reader1->AddVariable("selpfjetAK8chrad", &in_pfjetAK8chrad);
  reader1->AddVariable("selpfjetAK8sdmass", &in_pfjetAK8sdmass);
  reader1->AddVariable("selpfjetAK8matchedeldxy_sv", &in_pfjetAK8matchedeldxy_sv);
  reader1->AddVariable("selpfjetAK8matchedelcleta", &in_pfjetAK8matchedelcleta);
  reader1->AddVariable("selpfjetAK8matchedelpt", &in_pfjetAK8matchedelpt);
  reader1->AddVariable("selpfjetAK8matchedelsigmaieta", &in_pfjetAK8matchedelsigmaieta);
  reader1->AddVariable("selpfjetAK8matchedelsigmaiphi", &in_pfjetAK8matchedelsigmaiphi);
  reader1->AddVariable("selpfjetAK8matchedelr9full", &in_pfjetAK8matchedelr9full);
  reader1->AddVariable("selpfjetAK8matchedelsupcl_etaw", &in_pfjetAK8matchedelsupcl_etaw);
  reader1->AddVariable("selpfjetAK8matchedelsupcl_phiw", &in_pfjetAK8matchedelsupcl_phiw);
  reader1->AddVariable("selpfjetAK8matchedelhcaloverecal", &in_pfjetAK8matchedelhcaloverecal);
  reader1->AddVariable("selpfjetAK8matchedelcloctftrkn", &in_pfjetAK8matchedelcloctftrkn);
  reader1->AddVariable("selpfjetAK8matchedelcloctftrkchi2", &in_pfjetAK8matchedelcloctftrkchi2);
  reader1->AddVariable("selpfjetAK8matchedele1x5bye5x5", &in_pfjetAK8matchedele1x5bye5x5);
  reader1->AddVariable("selpfjetAK8matchedelnormchi2", &in_pfjetAK8matchedelnormchi2);
  reader1->AddVariable("selpfjetAK8matchedelhitsmiss", &in_pfjetAK8matchedelhitsmiss);
  reader1->AddVariable("selpfjetAK8matchedeltrkmeasure", &in_pfjetAK8matchedeltrkmeasure);
  reader1->AddVariable("selpfjetAK8matchedelecloverpout", &in_pfjetAK8matchedelecloverpout);
  reader1->AddVariable("selpfjetAK8matchedelecaletrkmomentum", &in_pfjetAK8matchedelecaletrkmomentum);
  reader1->AddVariable("selpfjetAK8matchedeldeltaetacltrkcalo", &in_pfjetAK8matchedeldeltaetacltrkcalo);
  reader1->AddVariable("selpfjetAK8matchedelsupcl_preshvsrawe", &in_pfjetAK8matchedelsupcl_preshvsrawe);
  reader1->AddVariable("selpfjetAK8matchedelpfisolsumphet", &in_pfjetAK8matchedelpfisolsumphet);
  reader1->AddVariable("selpfjetAK8matchedelpfisolsumchhadpt", &in_pfjetAK8matchedelpfisolsumchhadpt);
  reader1->AddVariable("selpfjetAK8matchedelpfisolsumneuhadet", &in_pfjetAK8matchedelpfisolsumneuhadet);
  reader1->AddVariable("selpfjetAK8matchedeletain", &in_pfjetAK8matchedeletain);
  reader1->AddVariable("selpfjetAK8matchedelphiin", &in_pfjetAK8matchedelphiin);
  reader1->AddVariable("selpfjetAK8matchedelfbrem", &in_pfjetAK8matchedelfbrem);
  reader1->AddVariable("selpfjetAK8matchedeleoverp", &in_pfjetAK8matchedeleoverp);
  reader1->AddVariable("selpfjetAK8matchedelhovere", &in_pfjetAK8matchedelhovere);
  reader1->AddVariable("selpfjetAK8matchedelRho", &in_pfjetAK8matchedelRho);
  reader1->BookMVA("BDTG method", weightfile1);
    
  reader4 = new TMVA::Reader( "BDTG_Rmu" );
  reader4->AddVariable( "selpfjetAK8NHadF", &in_mupfjetAK8NHadF);
  reader4->AddVariable( "selpfjetAK8neunhadfrac", &in_mupfjetAK8neunhadfrac);
  reader4->AddVariable( "selpfjetAK8subhaddiff", &in_mupfjetAK8subhaddiff);
  reader4->AddVariable( "selpfjetAK8tau21", &in_mupfjetAK8tau21);
  reader4->AddVariable( "selpfjetAK8chrad", &in_mupfjetAK8chrad);
  reader4->AddVariable( "selpfjetAK8sdmass", &in_mupfjetAK8sdmass);

  
  reader4->AddVariable("selpfjetAK8matchedmuonchi", &in_pfjetAK8matchedmuonchi);
  reader4->AddVariable("selpfjetAK8matchedmuonposmatch", &in_pfjetAK8matchedmuonposmatch);
  reader4->AddVariable("selpfjetAK8matchedmuontrkink", &in_pfjetAK8matchedmuontrkink);
  reader4->AddVariable("selpfjetAK8matchedmuonsegcom", &in_pfjetAK8matchedmuonsegcom);
  reader4->AddVariable("selpfjetAK8matchedmuonhit", &in_pfjetAK8matchedmuonhit);
  reader4->AddVariable("selpfjetAK8matchedmuonmst", &in_pfjetAK8matchedmuonmst);
  reader4->AddVariable("selpfjetAK8matchedmuontrkvtx", &in_pfjetAK8matchedmuontrkvtx);
  reader4->AddVariable("selpfjetAK8matchedmuondz", &in_pfjetAK8matchedmuondz);
  reader4->AddVariable("selpfjetAK8matchedmuonpixhit", &in_pfjetAK8matchedmuonpixhit);
  reader4->AddVariable("selpfjetAK8matchedmuontrklay", &in_pfjetAK8matchedmuontrklay);
  reader4->AddVariable("selpfjetAK8matchedmuonvalfrac", &in_pfjetAK8matchedmuonvalfrac);
  reader4->AddVariable("selpfjetAK8muinsubptrat", &in_pfjetAK8muinsubptrat);
  reader4->AddVariable("selpfjetAK8muinsubmassrat", &in_pfjetAK8muinsubmassrat);
  reader4->AddVariable("selpfjetAK8muinsubinvmass", &in_pfjetAK8muinsubinvmass);
  reader4->AddVariable("selpfjetAK8muinsubIfarbyI0", &in_pfjetAK8muinsubIfarbyI0);
  reader4->AddVariable("selpfjetAK8muinsubInearbyI0", &in_pfjetAK8muinsubInearbyI0);
  reader4->BookMVA("BDTG method", weightfile4);
  
}

Bool_t Trigg_eff_cal::Process(Long64_t entry)
{
  // The Process() function is called for each entry in the tree (or possibly
  // keyed object in the case of PROOF) to be processed. The entry argument
  // specifies which entry in the currently loaded tree is to be processed.
  // It can be passed to either Anal_Leptop_PROOF::GetEntry() or TBranch::GetEntry()
  // to read either all or the required parts of the data. When processing
  // keyed objects with PROOF, the object is already loaded and is available
  // via the fObject pointer.
  //
  // This function should contain the "body" of the analysis. It can contain
  // simple or elaborate selection criteria, run algorithms on the data
  // of the event and typically fill histograms.
  //
  // The processing can be stopped by calling Abort().
  //
  // Use fStatus to set the return value of TTree::Process().
  //
  // The return value is currently not used.

  int icheck = GetEntry(entry);
  if(isMC){
    for(int ich =0; ich < nchannel; ich++)
      weight[ich] = event_weight;
  }else{
        for(int ich =0; ich < nchannel; ich++)
	  weight[ich] = 1;
  }
  TString str;
  // str = TString::Format("check for evt %d ",ievt);          
  //if(gProofServ) gProofServ->SendAsynMessage(str);
  
  vector<GenParton> genpartons;
  vector<GenParton> LHEtops;
  vector<TopQuark> gentops;
  vector<LHEparticle> lheparticles;
  int cntt = 0; int nel=0; int nmu=0;
  tau_decay_mode = 0;
  toppt_wt = 1;
  
 if(isMC && npu_vert_true>=0 && npu_vert_true<100){
    float *puweights = Get_PU_Weights(npu_vert_true);
    puWeight = puweights[0];
    puWeightup = puweights[1];
    puWeightdown = puweights[2];
    for(int ich =0; ich < nchannel; ich++)
      {
	weight[ich] *= puWeight;
	weight[ich] *= prefiringweight;
      }
 }
 else if(isMC)
   {
     weight[0] = 0;
     str = TString::Format("npu_vert_true = %d weight = %f nprimi = %d is out of range for evt %d  ; ttree = %d",npu_vert_true,weight[0],nprimi,ievt,icheck);          
     if(gProofServ) gProofServ->SendAsynMessage(str);
     return kFALSE;
   }
 
  if( isMC ){
    getPartons(genpartons);
    getLHEParticles(lheparticles);
    if(isTT) {
      getGENTops(gentops,genpartons); // after shower (get top quarks from its daughters) --> will tell details about the signature of ttbar events at GEN level
      getLHETops(LHEtops,genpartons); // before shower (get original top quarks which have decayed) --> will be usedto derive top pt reweighting

      // top pt reweighting //
      if(LHEtops.size()==2){
	toppt_wt = SF_TOP(0.0615,0.0005,TMath::Min(float(500),float(LHEtops[0].pt)),TMath::Min(float(500),float(LHEtops[1].pt)));
	for(int ich =0; ich < nchannel; ich++)
	  weight[ich] *= toppt_wt;
      }
    }
  }    
    
  bool itrig_ortho_trig = false;
 
  itrig_ortho_trig = hlt_PFMET250 || hlt_PFMET300 || hlt_PFMET200 || hlt_PFMET200_TypeOne;

  vector <Electron> velectrons;
  getelectrons(velectrons, 10 ,absetacut);   
  
  vector <Muon> vmuons;
  getmuons(vmuons,10 ,absetacut);

  vector <Lepton> vleptons;
  getLeptons(vleptons,vmuons,velectrons,10);

  Lepton lepcand1,lepcand2;
    
  if((int)vleptons.size() > 1)
    {
      lep1pt = vleptons[0].pt;
      lep2pt = vleptons[1].pt;
      lep1pdg = vleptons[0].pdgId;
      lep2pdg = vleptons[1].pdgId;
      lepcand1= vleptons[0];
      lepcand2 = vleptons[1]; 
    }
  
  for(int ich =0; ich < nchannel; ich++)
    hist_count[ich]->Fill(0.0,weight[ich]);

  if(!itrig_ortho_trig || (int)vleptons.size()<2) return kFALSE;             

  for(int ich =0; ich < nchannel; ich++)
    hist_count[ich]->Fill(1.0,weight[ich]);
  
 vector <AK8Jet> LJets;
  getAK8jets(LJets,300,absetacut,isMC);

  if ((int)LJets.size()>0 && (int)vleptons.size() > 1) {
    for(int ijet=0;ijet<min(2,int(LJets.size()));ijet++){
      //Match lepton with AK8 jets
      LJets[ijet].match_lepton_index = get_nearest_lepton(vleptons,LJets[ijet].p4);
      if (LJets[ijet].match_lepton_index >=0 && ijet!=LJets[ijet].match_lepton_index && ijet<int(vleptons.size())) {
	Lepton tmplep = vleptons[LJets[ijet].match_lepton_index];
	vleptons.erase(vleptons.begin() + LJets[ijet].match_lepton_index);
	vleptons.insert(vleptons.begin()+ijet, tmplep);
      }
    }
  }
    
      /*    // Matched highest b tag AK4 jets  with AK8 jets //                                         
      LJets[ijet].match_AK4_index = get_nearest_AK4(Jets,LJets[ijet].p4);
      if(LJets[ijet].match_AK4_index>=0 && LJets[ijet].match_AK4_index<int(Jets.size())){
        LJets[ijet].matchAK4deepb = Jets[LJets[ijet].match_AK4_index].btag_DeepFlav;	
      }
      if (LJets[ijet].match_AK4_index >=0 && ijet!=LJets[ijet].match_AK4_index && ijet<int(Jets.size())) {
	AK4Jet tmpjet = Jets[LJets[ijet].match_AK4_index];
	Jets.erase(Jets.begin() + LJets[ijet].match_AK4_index);
	Jets.insert(Jets.begin() + ijet, tmpjet);
      }
    }
  }*/
  
  elleptonsf_weight = elleptonsf_weight_stat = elleptonsf_weight = 1;
  muleptonsf_weight = muleptonsf_weight_stat = muleptonsf_weight = 1;
  
  if(isMC){
    for(int ie=0; ie<(int)velectrons.size(); ie++) {
      float *sfvalues;
      //      if( LJets.size() > 0 && (delta2R(velectrons[ie].p4,LJets[min(0,int(LJets.size()-1))].p4)<0.7 || delta2R(velectrons[ie].p4,LJets[min(1,int(LJets.size()-1))].p4)<0.7))
	sfvalues = Electron_SF(velectrons[ie].pt,velectrons[ie].eta,"reco");
	//else if(!velectrons[ie].vetoid_tight)
	//	sfvalues = Electron_SF(velectrons[ie].pt,velectrons[ie].eta,"reco");
	//else
	//	sfvalues = Electron_SF(velectrons[ie].pt,velectrons[ie].eta,"Tight");
	elleptonsf_weight *= sfvalues[0];
      elleptonsf_weight_stat *= (sfvalues[0] + sqrt(sfvalues[1]*sfvalues[1] + sfvalues[2]*sfvalues[2]));  // like this for time being
      elleptonsf_weight_syst *= (sfvalues[0] + sqrt(sfvalues[3]*sfvalues[3] + sfvalues[4]*sfvalues[4] + sfvalues[5]*sfvalues[5] + sfvalues[6]*sfvalues[6]));  // like this for time being
      }
    
    for(int im=0; im<(int)vmuons.size(); im++) {
      float *sfvalues;
      //      if( LJets.size() > 0 && (delta2R(vmuons[im].p4,LJets[min(0,int(LJets.size()-1))].p4)<0.7 || delta2R(vmuons[im].p4,LJets[min(1,int(LJets.size()-1))].p4)<0.7))
	sfvalues = Muon_SF(vmuons[im].pt,vmuons[im].eta,"reco");
	//      else if(!vmuons[im].vetoid_tight)
	//	sfvalues = Muon_SF(vmuons[im].pt,vmuons[im].eta,"reco");
	//else
	//sfvalues = Muon_SF(vmuons[im].pt,vmuons[im].eta,"Tight");
      muleptonsf_weight *= sfvalues[0];
      muleptonsf_weight_stat *= sfvalues[1];
      muleptonsf_weight_syst *= sfvalues[2];
    }
    //weight *= leptonsf_weight;
  }
  weight[0] *=elleptonsf_weight;
  weight[1] *=elleptonsf_weight*muleptonsf_weight;
  weight[2] *=muleptonsf_weight;
  
  vector <AK4Jet> Jets;
  getAK4jets(Jets, 30 ,absetacut,isMC);

  for(auto & jet: Jets){
    
    BTagEntry::JetFlavor btv_flav;
    if(abs(jet.hadronFlavour)==5){ btv_flav = BTagEntry::FLAV_B; }
    else if (abs(jet.hadronFlavour)==4){ btv_flav = BTagEntry::FLAV_C; }
    else { btv_flav = BTagEntry::FLAV_UDSG; }
    
    jet.btag_DeepFlav_med_SF = reader_deepflav_med.eval_auto_bounds("central",btv_flav,fabs(jet.eta),jet.pt); 
    jet.btag_DeepFlav_med_SF_up = reader_deepflav_med.eval_auto_bounds("up",btv_flav,fabs(jet.eta),jet.pt);
    jet.btag_DeepFlav_med_SF_dn = reader_deepflav_med.eval_auto_bounds("down",btv_flav,fabs(jet.eta),jet.pt);
    
    //jet.btag_DeepFlav_loose_SF = reader_deepflav_loose.eval_auto_bounds("central",btv_flav,fabs(jet.eta),jet.pt); 
    //jet.btag_DeepFlav_loose_SF_up = reader_deepflav_loose.eval_auto_bounds("up",btv_flav,fabs(jet.eta),jet.pt);
    //jet.btag_DeepFlav_loose_SF_dn = reader_deepflav_loose.eval_auto_bounds("down",btv_flav,fabs(jet.eta),jet.pt);
  }

  btag_SF = btag_SF_up = btag_SF_dn = 1;
  float p_mc, p_data;
  p_mc = p_data = 1;

  float btag_loosecut,btag_medcut,btag_tightcut;
  btag_loosecut = 0.0490;   btag_medcut = 0.2783;   btag_tightcut = 0.7100; //for UL18

  vector <AK4Jet> BJets;
  for(auto & jet: Jets){
    if(isBJet(jet,btag_medcut)){
      BJets.push_back(jet);
    }
  }
  nbjets = (int)BJets.size();
  if(isMC){
    for(auto & jet: Jets){
    
      BTagEntry::JetFlavor btv_flav;
      if(abs(jet.hadronFlavour)==5){ btv_flav = BTagEntry::FLAV_B; }
      else if (abs(jet.hadronFlavour)==4){ btv_flav = BTagEntry::FLAV_C; }
      else { btv_flav = BTagEntry::FLAV_UDSG; }
      
      jet.btag_DeepFlav_med_SF = reader_deepflav_med.eval_auto_bounds("central",btv_flav,fabs(jet.eta),jet.pt); 
      jet.btag_DeepFlav_med_SF_up = reader_deepflav_med.eval_auto_bounds("up",btv_flav,fabs(jet.eta),jet.pt);
      jet.btag_DeepFlav_med_SF_dn = reader_deepflav_med.eval_auto_bounds("down",btv_flav,fabs(jet.eta),jet.pt);
      
      //jet.btag_DeepFlav_loose_SF = reader_deepflav_loose.eval_auto_bounds("central",btv_flav,fabs(jet.eta),jet.pt); 
      //jet.btag_DeepFlav_loose_SF_up = reader_deepflav_loose.eval_auto_bounds("up",btv_flav,fabs(jet.eta),jet.pt);
      //jet.btag_DeepFlav_loose_SF_dn = reader_deepflav_loose.eval_auto_bounds("down",btv_flav,fabs(jet.eta),jet.pt);

      double eff_l, eff_m;
      string sample;
      if(isTT) sample = "tt";
      else if(isQCD) sample = "qcd";
      else if(isWJ) sample = "wj";
      else if(isST) sample = "st";
      else if(isTTX) sample = "ttx";
      else if(isDY) sample = "dy";
      else if(isDIB) sample = "dib";
      else sample = "tt";
	
      eff_m = BTag_MCEfficiency_M(sample,abs(jet.hadronFlavour),jet.pt,fabs(jet.eta));
      if(jet.btag_DeepFlav > btag_medcut)
	{
	  p_mc *= max((double)0.00001,eff_m);
	  p_data *= jet.btag_DeepFlav_med_SF*eff_m;
	}
      else
	{
	  p_mc *= max((double)0.00001,1 - eff_m);
	  p_data *= 1 - jet.btag_DeepFlav_med_SF*eff_m;
	}
    }
    }

  btag_SF = p_data/p_mc;
  // for(int ich =0; ich < nchannel; ich++)
      //weight[ich] *= btag_SF;
  
  if(isnan(weight[0]) || isnan(weight[1]) || isnan(weight[2])){
    str = TString::Format("check for evt %d weight %f btagsf %f leptonsf %f",ievt,weight[0],btag_SF,elleptonsf_weight);          
    if(gProofServ) gProofServ->SendAsynMessage(str);
  }

  if(isTT && (weight[0] < -2000 || weight[1] < -2000 || weight[2] < -2000))
    return kFALSE;
  std::vector<std::pair<int,TLorentzVector> > TrigRefObj;
  
  for (int tr=0; tr<ntrigobjs; tr++) {
    TLorentzVector trigobj;
    trigobj.SetPtEtaPhiM(trigobjpt[tr],trigobjeta[tr],trigobjphi[tr],trigobjmass[tr]);
    if(abs(trigobjpdgId[tr]) == 13 || (trigobjpdgId[tr] == 0 && trigobjmass[tr] < 1.e-3) )
      TrigRefObj.push_back(std::make_pair(trigobjpdgId[tr],trigobj));
  }

  ntriggerobjs = (int)TrigRefObj.size();
  
  bool trig_threshold_pass[3] = {false,false,false};
  bool trig_matching_pass[3] = {false,false,false};

  /*int nelmatch = 0, nmumatch = 0;
  int nobjs=0;
  for (int trv=0; trv<(int)TrigRefObj.size(); trv++) {
    float mindelpt_el = 100, mindelpt_mu = 100;
    float mindelN_el = 100, mindelN_mu = 100;
    for(int il=0; il<min(2,(int)vleptons.size()); il++) {
      float dr = delta2R(vleptons[il].p4,TrigRefObj[trv].second);
      if(dr > 0.2) continue;
      float deltaN = fabs((vleptons[il].p4 - TrigRefObj[trv].second).P()/max(float(0.00001),(float)vleptons[il].p4.P()));
      float deltapt = fabs((vleptons[il].pt - TrigRefObj[trv].second.Pt())/max(float(0.00001),vleptons[il].pt));
      if(abs(vleptons[il].pdgId) == abs(TrigRefObj[trv].first))  //for muon
	{
	  if(hlt_Mu37TkMu27){
	    hist_init[3]->Fill(deltapt,weight);
	    hist_prptangle[3]->Fill(vleptons[il].p4.Pt(),deltaN,weight);
	  }
	  if(hlt_Mu27Ele37 || hlt_Mu37Ele27){
	    hist_init[2]->Fill(deltapt,weight);
	    hist_prptangle[2]->Fill(vleptons[il].p4.Pt(),deltaN,weight);
	  }
	  if(deltapt<mindelpt_mu)    mindelpt_mu = deltapt;
	  if(deltaN<mindelN_mu)    mindelN_mu = deltaN;
	}
      else if(abs(vleptons[il].pdgId) == 11 && TrigRefObj[trv].first == 0) // for electron
	{
	  if(hlt_DoubleEle25){
	    hist_init[0]->Fill(deltapt,weight);
	    hist_prptangle[0]->Fill(vleptons[il].p4.Pt(),deltaN,weight);
	  }
	  if(hlt_Mu27Ele37 || hlt_Mu37Ele27){
	    hist_init[1]->Fill(deltapt,weight);
	    hist_prptangle[1]->Fill(vleptons[il].p4.Pt(),deltaN,weight);
	  }
	  if(deltapt<mindelpt_el)   mindelpt_el = deltapt;
	  if(deltaN<mindelN_el)   mindelN_el = deltaN;
	}
    }
    if(mindelpt_el < 0.1) nelmatch++;
    else if(mindelpt_mu < 0.05) nmumatch++;
    if(nobjs<25)
      {
	delpt_el[nobjs] = mindelpt_el;
	delpt_mu[nobjs] = mindelpt_mu;
	delN_el[nobjs] = mindelN_el;
	delN_mu[nobjs] = mindelN_mu;
      }
  }

  if(nelmatch > 1) trig_matching_pass[0] = true;
  if(nelmatch > 0 && nmumatch >0) trig_matching_pass[1] = true;
  if(nmumatch >1) trig_matching_pass[2] = true;
  */

  vector<TH1D*> hists1, hists2, hists3;
  hists1.push_back(hist_init_N[0]);
  hists1.push_back(hist_init_N[1]);

  hists2.push_back(hist_init_N[2]);
  hists2.push_back(hist_init_N[3]);

  hists3.push_back(hist_init_N[4]);
  hists3.push_back(hist_init_N[5]);
  
  vector<TH2D*> hist2d_prptangle1, hist2d_prptangle2, hist2d_prptangle3;
  hist2d_prptangle1.push_back(hist_prptangle_N[0]);
  hist2d_prptangle1.push_back(hist_prptangle_N[1]);
  
  hist2d_prptangle2.push_back(hist_prptangle_N[2]);
  hist2d_prptangle2.push_back(hist_prptangle_N[3]);

  hist2d_prptangle3.push_back(hist_prptangle_N[4]);
  hist2d_prptangle3.push_back(hist_prptangle_N[5]);

  vector<Single_Trigger> vsinglelep_trig;
  vector<Double_Trigger> vdoublelep_trig[3];
  
  Double_Trigger dtrig;
  
  dtrig.double_hlts = hlt_DoubleEle25;
  dtrig.double_pt_cuts = {10,10};
  dtrig.double_pids = {11,11};
  vdoublelep_trig[0].push_back(dtrig);

  dtrig.double_hlts = hlt_Mu37Ele27;
  dtrig.double_pt_cuts = {10,10};
  dtrig.double_pids = {13,11};
  vdoublelep_trig[1].push_back(dtrig);
  
  dtrig.double_hlts = hlt_Mu27Ele37;
  dtrig.double_pt_cuts  = {10,10};
  dtrig.double_pids = {11,13};
  vdoublelep_trig[1].push_back(dtrig);

  dtrig.double_hlts = hlt_Mu37TkMu27;
  dtrig.double_pt_cuts = {10,10};
  dtrig.double_pids = {13,13};
  vdoublelep_trig[2].push_back(dtrig);

  if((int)vleptons.size() > 1){
    Match_trigger(vsinglelep_trig,vdoublelep_trig[0],		
		  TrigRefObj,
		  vleptons[0],vleptons[1],Jets,
		  trig_threshold_pass[0],
		  trig_matching_pass[0],
		  hists1,hist2d_prptangle1,weight[0]
		  );
    
    Match_trigger(vsinglelep_trig,vdoublelep_trig[1],		
		  TrigRefObj,
		  vleptons[0],vleptons[1],Jets,
		  trig_threshold_pass[1],
		  trig_matching_pass[1],
		  hists2,hist2d_prptangle2,weight[1]
		  );
    
    Match_trigger(vsinglelep_trig,vdoublelep_trig[2],		
		  TrigRefObj,
		  vleptons[0],vleptons[1],Jets,
		  trig_threshold_pass[2],
		  trig_matching_pass[2],
		  hists3,hist2d_prptangle3,weight[2]
		  );	   
  }
  trig_met = float(itrig_ortho_trig);
  trig_ee = float(hlt_DoubleEle25);
  trig_emu = float(hlt_Mu27Ele37 || hlt_Mu37Ele27);
  trig_mumu = float(hlt_Mu37TkMu27);
  
  bool object_pass[nchannel] = {false};
  invmll = -100;
  if((int)vleptons.size() >1)
    invmll = (vleptons[0].p4 + vleptons[1].p4).M();


  float ee_chnl_toptaggercut1 = 0.3; float ee_chnl_toptaggercut2 = 0.3;
  float emu_chnl_eltoptaggercut1 = 0.3; float emu_chnl_mutoptaggercut2 = 0.0;
  float mumu_chnl_toptaggercut1 = 0.0; float mumu_chnl_toptaggercut2 = 0.0;

  
  bool emuchannel_tagcutcondition = false;
  Lepton emulepcand1,emulepcand2;
  if((int)vleptons.size()>1)
    {
      emulepcand1 = vleptons[0];
      emulepcand2 = vleptons[1];
    }
  if ((int)LJets.size()>1 && (int)vleptons.size()>1)
    {
      ReadTagger(LJets,vleptons,vmuons,velectrons,reader1,reader4);
      /*if(abs(vleptons[0].pdgId) == 13 && abs(vleptons[1].pdgId) == 11)
	{
	  emuchannel_tagcutcondition = (LJets[0].re_tvsb > emu_chnl_mutoptaggercut2) && (LJets[1].re_tvsb > emu_chnl_eltoptaggercut1);
	  emulepcand1= vleptons[1];
	  emulepcand2 = vleptons[0]; 
	}
      else if(abs(vleptons[0].pdgId) == 11 && abs(vleptons[1].pdgId) == 13)
	{
	  emuchannel_tagcutcondition = (LJets[0].re_tvsb > emu_chnl_eltoptaggercut1) && (LJets[1].re_tvsb > emu_chnl_mutoptaggercut2);
	  emulepcand1= vleptons[0];
	  emulepcand2 = vleptons[1]; 
        }*/
      response_ak81 = LJets[0].re_tvsb;
      response_ak82 = LJets[1].re_tvsb;

      if(abs(vleptons[0].pdgId) == 13)
	{
	  emuchannel_tagcutcondition = (LJets[0].re_tvsb > emu_chnl_mutoptaggercut2);
	  emulepcand1 = vleptons[1];
	  emulepcand2 = vleptons[0]; 
	}
      else if(abs(vleptons[0].pdgId) == 11)
	{
	  emuchannel_tagcutcondition = (LJets[0].re_tvsb > emu_chnl_eltoptaggercut1);
	  emulepcand1 = vleptons[0];
	  emulepcand2 = vleptons[1]; 
	}
    }

  bool lepmatchwithak8 = false;

  if((int)vleptons.size()>1 && vleptons[0].pdgId == lepcand1.pdgId && vleptons[0].indexemu == lepcand1.indexemu && ( (abs(lepcand2.pdgId) == 11 && velectrons[lepcand2.indexemu].mvaWPloose_noIso ) || (abs(lepcand2.pdgId) == 13 ) ))
    lepmatchwithak8 = true;
  else if( (int)vleptons.size()>1 && vleptons[0].pdgId == lepcand2.pdgId && vleptons[0].indexemu == lepcand2.indexemu && ( (abs(lepcand1.pdgId) == 11 && velectrons[lepcand1.indexemu].mvaWPloose_noIso ) || (abs(lepcand1.pdgId) == 13 ) ))
    lepmatchwithak8 = true;
 
  // str = TString::Format("check for evt %d      %d %d  %d %d      %f %f  %f %f      matched = %d",ievt,vleptons[0].pdgId,vleptons[1].pdgId,lepcand1.pdgId,lepcand2.pdgId,vleptons[0].pt,vleptons[1].pt,lepcand1.pt,lepcand2.pt,lepmatchwithak8);          
  //if(gProofServ) gProofServ->SendAsynMessage(str);
 
     object_pass[0] =    (((int)vleptons.size() >1 && vleptons[1].pt > 10 && vleptons[0].pt > 10 && abs(vleptons[0].pdgId) == 11 && abs(vleptons[1].pdgId) == 11 && vleptons[0].pdgId*vleptons[1].pdgId <0)) && chs_met >200 && (int)LJets.size()>0 && LJets[0].re_tvsb > ee_chnl_toptaggercut1  && delta2R(LJets[0].p4,vleptons[0].p4)<0.7 && lepmatchwithak8 && itrig_ortho_trig;

  object_pass[1] =    ( ((int)vleptons.size() >1 && vleptons[0].pt > 10  && vleptons[0].pt > 10 && abs(vleptons[0].pdgId) == 13 && abs(vleptons[1].pdgId) == 11 && vleptons[0].pdgId*vleptons[1].pdgId <0)
			|| ((int)vleptons.size() >1 && vleptons[0].pt > 10  && vleptons[0].pt > 10 && abs(vleptons[1].pdgId) == 13 && abs(vleptons[0].pdgId) == 11 && vleptons[0].pdgId*vleptons[1].pdgId <0)) && chs_met > 200 && (int)LJets.size()>0 && emuchannel_tagcutcondition  && delta2R(LJets[0].p4,vleptons[0].p4)<0.7 && lepmatchwithak8 && itrig_ortho_trig;
    //             || ((int)vmuons.size() >0 && vmuons[0].pt > 53)
    /*             || ((int)velectrons.size() > 0 && velectrons[0].pt > 53 && (int)Jets.size() > 0 && Jets[0].pt > 180)*/

  object_pass[2] =    (((int)vleptons.size() >1 && vleptons[1].pt > 10 && vleptons[0].pt > 10 && abs(vleptons[0].pdgId) == 13 && abs(vleptons[1].pdgId) == 13 && vleptons[0].pdgId*vleptons[1].pdgId <0)) && chs_met > 200 && (int)LJets.size()>0 && LJets[0].re_tvsb > mumu_chnl_toptaggercut1 && delta2R(LJets[0].p4,vleptons[0].p4)<0.7 && abs(lep1pdg) == 13 && abs(lep2pdg) == 13 && lepmatchwithak8 && itrig_ortho_trig;
  
  for(int ich =0; ich < nchannel; ich++)   
    {
      if(object_pass[ich] && ich != 1){
	hist_count[ich]->Fill(2.0,weight[ich]);
	hist_ortho_trig_2d[0][ich]->Fill(min(float(499.0),(float)lepcand1.pt),min(float(499.0),(float)lepcand2.pt), weight[ich]);
	hist_ortho_trig_2d[1][ich]->Fill(lepcand1.eta,lepcand2.eta, weight[ich]);
	hist_ortho_trig_2d[2][ich]->Fill(lepcand1.phi,lepcand2.phi, weight[ich]);
	hist_ortho_trig_2d[3][ich]->Fill(min(float(499.0),(float)lepcand1.pt),min(float(499.0),(float)lepcand2.pt));
	hist_ortho_trig[0][ich]->Fill(lepcand1.pt + lepcand2.pt, weight[ich]);
	hist_ortho_trig[1][ich]->Fill((lepcand1.p4 + lepcand2.p4).M(), weight[ich]);
	hist_ortho_trig[2][ich]->Fill((lepcand1.p4 + lepcand2.p4).Phi(), weight[ich]);
	hist_ortho_trig[3][ich]->Fill((lepcand1.p4 + lepcand2.p4).Eta(), weight[ich]);
	hist_ortho_trig[4][ich]->Fill(Jets.size(), weight[ich]);
      }
      else if(object_pass[ich] && ich == 1)
	{
	  hist_count[ich]->Fill(2.0,weight[ich]);
	  hist_ortho_trig_2d[0][ich]->Fill(min(float(499.0),(float)emulepcand1.pt),min(float(499.0),(float)emulepcand2.pt), weight[ich]);
	  hist_ortho_trig_2d[1][ich]->Fill(emulepcand1.eta,emulepcand2.eta, weight[ich]);
	  hist_ortho_trig_2d[2][ich]->Fill(emulepcand1.phi,emulepcand2.phi, weight[ich]);
	  hist_ortho_trig_2d[3][ich]->Fill(min(float(499.0),(float)emulepcand1.pt),min(float(499.0),(float)emulepcand2.pt));
	  hist_ortho_trig[0][ich]->Fill(emulepcand1.pt + emulepcand2.pt, weight[ich]);
	  hist_ortho_trig[1][ich]->Fill((emulepcand1.p4 + emulepcand2.p4).M(), weight[ich]);
	  hist_ortho_trig[2][ich]->Fill((emulepcand1.p4 + emulepcand2.p4).Phi(), weight[ich]);
	  hist_ortho_trig[3][ich]->Fill((emulepcand1.p4 + emulepcand2.p4).Eta(), weight[ich]);
	  hist_ortho_trig[4][ich]->Fill(Jets.size(), weight[ich]);
	}
    }

  bool trig_check[nchannel] = {false};
  
  trig_check[0] =    hlt_DoubleEle25;

  trig_check[1] =    ( hlt_Mu37Ele27   || hlt_Mu27Ele37 )
    //            || (hlt_Mu50 && (int)vmuons.size() >0 && vmuons[].pt > 53)
    /*            || (hlt_Ele50_PFJet165 && (int)velectrons.size() > 0 && velectrons[0].pt > 53 && (int)Jets.size() > 0 && Jets[0].pt > 180) )*/ ;

  trig_check[2] =    hlt_Mu37TkMu27;

  if(isnan(vleptons[0].pt) || isnan(vleptons[0].eta) || isnan(vleptons[0].phi) || isnan(vleptons[1].pt) || isnan(vleptons[1].eta) || isnan(vleptons[1].phi) || isnan(Jets.size())){
    str = TString::Format("check for evt %d",ievt);          
    if(gProofServ) gProofServ->SendAsynMessage(str);
  }
  
  for(int ich =0; ich < nchannel; ich++)   
    {
      if(object_pass[ich] && trig_check[ich] && ich!=1){
	hist_count[ich]->Fill(3,weight[ich]);
	hist_trig_eff_2d_unmatched[0][ich]->Fill(min(float(499.0),(float)lepcand1.pt),min(float(499.0),(float)lepcand2.pt), weight[ich]);
	hist_trig_eff_2d_unmatched[1][ich]->Fill(lepcand1.eta,lepcand2.eta, weight[ich]);
	hist_trig_eff_2d_unmatched[2][ich]->Fill(lepcand1.phi,lepcand2.phi, weight[ich]);
	hist_trig_eff_2d_unmatched[3][ich]->Fill(min(float(499.0),(float)lepcand1.pt),min(float(499.0),(float)lepcand2.pt));
	hist_trig_eff_unmatched[0][ich]->Fill(lepcand1.pt, weight[ich]);
	hist_trig_eff_unmatched[1][ich]->Fill((lepcand1.p4 + lepcand2.p4).M(), weight[ich]);
	hist_trig_eff_unmatched[2][ich]->Fill(lepcand1.p4.Phi(), weight[ich]);
	hist_trig_eff_unmatched[3][ich]->Fill(lepcand1.p4.Eta(), weight[ich]);
	hist_trig_eff_unmatched[4][ich]->Fill(Jets.size(), weight[ich]);
      }
      else if(object_pass[ich] && trig_check[ich] && ich==1){
	hist_count[ich]->Fill(3,weight[ich]);
	hist_trig_eff_2d_unmatched[0][ich]->Fill(min(float(499.0),(float)emulepcand1.pt),min(float(499.0),(float)emulepcand2.pt), weight[ich]);
	hist_trig_eff_2d_unmatched[1][ich]->Fill(emulepcand1.eta,emulepcand2.eta, weight[ich]);
	hist_trig_eff_2d_unmatched[2][ich]->Fill(emulepcand1.phi,emulepcand2.phi, weight[ich]);
	hist_trig_eff_2d_unmatched[3][ich]->Fill(min(float(499.0),(float)emulepcand1.pt),min(float(499.0),(float)emulepcand2.pt));
	hist_trig_eff_unmatched[0][ich]->Fill(emulepcand1.pt, weight[ich]);
	hist_trig_eff_unmatched[1][ich]->Fill((emulepcand1.p4 + emulepcand2.p4).M(), weight[ich]);
	hist_trig_eff_unmatched[2][ich]->Fill(emulepcand1.p4.Phi(), weight[ich]);
	hist_trig_eff_unmatched[3][ich]->Fill(emulepcand1.p4.Eta(), weight[ich]);
	hist_trig_eff_unmatched[4][ich]->Fill(Jets.size(), weight[ich]);
      }
    }

  for(int ich =0; ich < nchannel; ich++)   
    {
      if(object_pass[ich] && trig_check[ich] && trig_matching_pass[ich] && ich !=1){
	hist_count[ich]->Fill(4,weight[ich]);
	hist_trig_eff_2d[0][ich]->Fill(min(float(499.0),(float)lepcand1.pt),min(float(499.0),(float)lepcand2.pt), weight[ich]);
	hist_trig_eff_2d[1][ich]->Fill(lepcand1.eta,lepcand2.eta, weight[ich]);
	hist_trig_eff_2d[2][ich]->Fill(lepcand1.phi,lepcand2.phi, weight[ich]);
	hist_trig_eff_2d[3][ich]->Fill(min(float(499.0),(float)lepcand1.pt),min(float(499.0),(float)lepcand2.pt));
	hist_trig_eff[0][ich]->Fill(lepcand1.pt, weight[ich]);
	hist_trig_eff[1][ich]->Fill((lepcand1.p4 + lepcand2.p4).M(), weight[ich]);
	hist_trig_eff[2][ich]->Fill(lepcand1.p4.Phi(), weight[ich]);
	hist_trig_eff[3][ich]->Fill(lepcand1.p4.Eta(), weight[ich]);
	hist_trig_eff[4][ich]->Fill(Jets.size(), weight[ich]);	
      }
      if(object_pass[ich] && trig_check[ich] && trig_matching_pass[ich] && ich==1){
	hist_count[ich]->Fill(4,weight[ich]);
	hist_trig_eff_2d[0][ich]->Fill(min(float(499.0),(float)emulepcand1.pt),min(float(499.0),(float)emulepcand2.pt), weight[ich]);
	hist_trig_eff_2d[1][ich]->Fill(emulepcand1.eta,emulepcand2.eta, weight[ich]);
	hist_trig_eff_2d[2][ich]->Fill(emulepcand1.phi,emulepcand2.phi, weight[ich]);
	hist_trig_eff_2d[3][ich]->Fill(min(float(499.0),(float)emulepcand1.pt),min(float(499.0),(float)emulepcand2.pt));
	hist_trig_eff[0][ich]->Fill(emulepcand1.pt, weight[ich]);
	hist_trig_eff[1][ich]->Fill((emulepcand1.p4 + emulepcand2.p4).M(), weight[ich]);
	hist_trig_eff[2][ich]->Fill(emulepcand1.p4.Phi(), weight[ich]);
	hist_trig_eff[3][ich]->Fill(emulepcand1.p4.Eta(), weight[ich]);
	hist_trig_eff[4][ich]->Fill(Jets.size(), weight[ich]);
      }
    }

  if(nbjets>0){
    if(object_pass[0]) hist_count[0]->Fill(5.0,weight[0]);
    if(object_pass[1]) hist_count[1]->Fill(5.0,weight[1]);
    if(object_pass[2]) hist_count[2]->Fill(5.0,weight[2]);
  }

  if((int)vleptons.size() > 1 && (int)LJets.size()>1  && delta2R(LJets[1].p4,vleptons[1].p4)<0.7 && delta2R(LJets[0].p4,vleptons[0].p4)<0.7)
    {
      Tnewvar->Fill();
    }
  
  return kTRUE;     
}

void Trigg_eff_cal::SlaveTerminate()
{
  // The SlaveTerminate() function is called after all entries or objects
  // have been processed. When running with PROOF SlaveTerminate() is called
  // on each slave server.
  fileOut->cd();
  fileOut->Write();
  fOutput->Add(OutFile);
  fileOut->Close();
}

void Trigg_eff_cal::Terminate()
{
  // The Terminate() function is the last function to be called during
  // a query. It always runs on the client, it can be used to present
  // the results graphically or save the results to file.
  
}


       /*  int extra_el=0;
  int extra_mu=0;
  for(int ie=0; ie<(int)velectrons.size(); ie++) {
    if(velectrons[ie].looseid)
      extra_el++;
  }
  for(int im=0; im<(int)vmuons.size(); im++) {
    if(vmuons[im].looseid)
      extra_mu++;
      }

  double Pt_sum(0.);
  if (Jets.size()>=1) {
    for(unsigned ijet=0; ijet< Jets.size(); ijet++)
      Pt_sum += Jets[ijet].pt;
    
    for(unsigned ilep=0; ilep<velectrons.size(); ilep++)
      Pt_sum += velectrons[ilep].p4.Pt();
    
    for(unsigned ilep=0; ilep<vmuons.size(); ilep++)
      Pt_sum += vmuons[ilep].p4.Pt();
  }
  */
