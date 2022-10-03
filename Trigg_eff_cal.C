/*
  weight = event_weight; 
  should be in the same ntuple, event by event we are going to read it.
    
  Order lepton according to the matching of leading Ak8 jets. 
  Lepton tagging
  
*/

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
  isTTDilep = false;
  isDIB = false;
  isDY = false;
  isQCD = false;
  isST = false;
  isTTX = false;
  isWJ = false;
  
  //GMA
  //Tout = new TTree("leptop","leptop");
  //Tout->Branch("weight",&weight,"weight/F");
  
  Tnewvar = new TTree("newvars","newvars");
  Tnewvar->Branch("irun", &irun, "irun/I");
  Tnewvar->Branch("ilumi", &ilumi, "ilumi/I");
  Tnewvar->Branch("ievt", &ievt, "ievt/i");

  Tnewvar->Branch("nprimi", &nprimi, "nprimi/i");

  Tnewvar->Branch("tt_decay_mode",&tt_decay_mode,"tt_decay_mode/F"); // get decay product info; only relevent for ttbar channel and can give unusal values for other samples
  
  // Add experimental weight along with their variations for systematics uncertainity
  Tnewvar->Branch("weight_toppt",&weight_toppt,"weight_toppt/F");
  Tnewvar->Branch("weight_pileup",&weight_pileup,"weight_pileup/F");
  Tnewvar->Branch("weight_pileup_up",&weight_pileup_up,"weight_pileup_up/F");
  Tnewvar->Branch("weight_pileup_down",&weight_pileup_down,"weight_pileup_down/F");
  Tnewvar->Branch("weight_prefiring",&weight_prefiring,"weight_prefiring/F");
  Tnewvar->Branch("weight_prefiring_up",&weight_prefiring_up,"weight_prefiring_up/F");
  Tnewvar->Branch("weight_prefiring_down",&weight_prefiring_down,"weight_prefiring_down/F");
  Tnewvar->Branch("weight_btag",&weight_btag,"weight_btag/F");
  Tnewvar->Branch("weight_btag_up",&weight_btag_up,"weight_btag_up/F");
  Tnewvar->Branch("weight_btag_down",&weight_btag_down,"weight_btag_down/F");
  
  Tnewvar->Branch("total_weight_ee",&total_weight_ee,"total_weight_ee/F");
  Tnewvar->Branch("weight_lepeff_ee",&weight_lepeff_ee,"weight_lepeff_ee/F");
  Tnewvar->Branch("weight_lepeff_stat_ee",&weight_lepeff_stat_ee,"weight_lepeff_stat_ee/F");
  Tnewvar->Branch("weight_lepeff_syst_ee",&weight_lepeff_syst_ee,"weight_lepeff_syst_ee/F");

  Tnewvar->Branch("total_weight_emu",&total_weight_emu,"total_weight_emu/F");
  Tnewvar->Branch("weight_lepeff_emu",&weight_lepeff_emu,"weight_lepeff_emu/F");
  Tnewvar->Branch("weight_lepeff_stat_emu",&weight_lepeff_stat_emu,"weight_lepeff_stat_emu/F");
  Tnewvar->Branch("weight_lepeff_syst_emu",&weight_lepeff_syst_emu,"weight_lepeff_syst_emu/F");

  Tnewvar->Branch("total_weight_mumu",&total_weight_mumu,"total_weight_mumu/F");
  Tnewvar->Branch("weight_lepeff_mumu",&weight_lepeff_mumu,"weight_lepeff_mumu/F");
  Tnewvar->Branch("weight_lepeff_stat_mumu",&weight_lepeff_stat_mumu,"weight_lepeff_stat_mumu/F");
  Tnewvar->Branch("weight_lepeff_syst_mumu",&weight_lepeff_syst_mumu,"weight_lepeff_syst_mumu/F");

  // Add theory weight along with their variations for theortical systematics uncertainity
  /*  Tnewvar->Branch("gen_weight",&gen_weight,"gen_weight/F");
      Tnewvar->Branch("LHE_weight",&LHE_weight, "LHE_weight/D");
      Tnewvar->Branch("nLHEScaleWeights",&nLHEScaleWeights, "nLHEScaleWeights/I");
      Tnewvar->Branch("LHEScaleWeights",LHEScaleWeights,"LHEScaleWeights[nLHEScaleWeights]/F");
      Tnewvar->Branch("nLHEPDFWeights",&nLHEPDFWeights, "nLHEPDFWeights/I");
      Tnewvar->Branch("LHEPDFWeights",LHEPDFWeights,"LHEPDFWeights[nLHEPDFWeights]/F");
      Tnewvar->Branch("nLHEAlpsWeights",&nLHEAlpsWeights, "nLHEAlpsWeights/I");
      Tnewvar->Branch("LHEAlpsWeights",LHEAlpsWeights,"LHEAlpsWeights[nLHEAlpsWeights]/F");
      Tnewvar->Branch("nLHEPSWeights",&nLHEPSWeights, "nLHEPSWeights/I");
      Tnewvar->Branch("LHEPSWeights",LHEPSWeights,"LHEPSWeights[nLHEPSWeights]/F");
  */
  // Add all event selection variables
  Tnewvar->Branch("n_electrons",&n_electrons,"n_electrons/F");
  Tnewvar->Branch("n_muons",&n_muons,"n_muons/F");
  Tnewvar->Branch("nvetomuons",&nvetomuons,"nvetomuons/F");
  Tnewvar->Branch("nvetoelectrons",&nvetoelectrons,"nvetoelectrons/F");
  Tnewvar->Branch("nak8jets",&nak8jets,"nak8jets/F");
  Tnewvar->Branch("ak8jet1_pt",&ak8jet1_pt,"ak8jet1_pt/F");
  Tnewvar->Branch("ak8jet2_pt",&ak8jet2_pt,"ak8jet2_pt/F");
  Tnewvar->Branch("ak8jet1_sdmass",&ak8jet1_sdmass,"ak8jet1_sdmass/F");
  Tnewvar->Branch("ak8jet2_sdmass",&ak8jet2_sdmass,"ak8jet2_sdmass/F");
  Tnewvar->Branch("ak8jet1_toptagscore",&ak8jet1_toptagscore,"ak8jet1_toptagscore/F");
  Tnewvar->Branch("ak8jet2_toptagscore",&ak8jet2_toptagscore,"ak8jet2_toptagscore/F");
  Tnewvar->Branch("lep1_pdg",&lep1_pdg,"lep1_pdg/F");
  Tnewvar->Branch("lep2_pdg",&lep2_pdg,"lep2_pdg/F");
  Tnewvar->Branch("isttbar_ee",&isttbar_ee,"isttbar_ee/F");
  Tnewvar->Branch("isttbar_emu",&isttbar_emu,"isttbar_emu/F");
  Tnewvar->Branch("isttbar_mumu",&isttbar_mumu,"isttbar_mumu/F");
  Tnewvar->Branch("deltaRlep1",&deltaRlep1,"deltaRlep1/F");
  Tnewvar->Branch("deltaRlep2",&deltaRlep2,"deltaRlep2/F");
  Tnewvar->Branch("lepmatchwithak8",&lepmatchwithak8,"lepmatchwithak8/F");
  
  Tnewvar->Branch("nbjets",&nbjets,"nbjets/F");
  Tnewvar->Branch("chs_met",&chs_met,"chs_met/F");
  Tnewvar->Branch("chs_met_up",&chs_met_up,"chs_met_up/F");
  Tnewvar->Branch("chs_met_down",&chs_met_down,"chs_met_down/F");

  // Add other variables for including in event selection
  Tnewvar->Branch("deltaphi",&deltaphi,"deltaphi/F");
  Tnewvar->Branch("dirglthrmin",&dirglthrmin,"dirglthrmin/F");
  Tnewvar->Branch("dirgltrthr",&dirgltrthr,"dirgltrthr/F");
  Tnewvar->Branch("EventHT",&EventHT,"EventHT/F");
  Tnewvar->Branch("invmll",&invmll,"invmll/F");
  
  // Add varibles for Data vs MC comparison
  Tnewvar->Branch("ak8jet1_eta",&ak8jet1_eta,"ak8jet1_eta/F");
  Tnewvar->Branch("ak8jet1_mass",&ak8jet1_mass,"ak8jet1_mass/F");
  Tnewvar->Branch("ak8jet1_phi",&ak8jet1_phi,"ak8jet1_phi/F");
  Tnewvar->Branch("ak8jet2_eta",&ak8jet2_eta,"ak8jet2_eta/F");
  Tnewvar->Branch("ak8jet2_phi",&ak8jet2_phi,"ak8jet2_phi/F");
  Tnewvar->Branch("ak8jet2_mass",&ak8jet2_mass,"ak8jet2_mass/F");
  Tnewvar->Branch("nak4jets",&nak4jets,"nak4jets/F");
  Tnewvar->Branch("lep1_pt",&lep1_pt,"lep1_pt/F");
  Tnewvar->Branch("lep1_eta",&lep1_eta,"lep1_eta/F");
  Tnewvar->Branch("lep1_phi",&lep1_phi,"lep1_phi/F");
  Tnewvar->Branch("lep2_pt",&lep2_pt,"lep2_pt/F");
  Tnewvar->Branch("lep2_eta",&lep2_eta,"lep2_eta/F");
  Tnewvar->Branch("lep2_phi",&lep2_phi,"lep2_phi/F");

  // Add variables for division of MC samples
  Tnewvar->Branch("jet_trigger_unprescale",&jet_trigger_unprescale,"jet_trigger_unprescale/F");
  Tnewvar->Branch("jet_trigger_prescale",&jet_trigger_prescale,"jet_trigger_prescale/F");
  Tnewvar->Branch("met_trigger_unprescale",&met_trigger_unprescale,"met_trigger_unprescale/F");
  Tnewvar->Branch("met_trigger_prescale",&met_trigger_prescale,"met_trigger_prescale/F");
  Tnewvar->Branch("ee_trigger",&ee_trigger,"ee_trigger/F");
  Tnewvar->Branch("emu_trigger",&emu_trigger,"emu_trigger/F");
  Tnewvar->Branch("mumu_trigger",&mumu_trigger,"mumu_trigger/F");
  Tnewvar->Branch("trig_matching_pass_ee",&trig_matching_pass_ee,"trig_matching_pass_ee/F");
  Tnewvar->Branch("trig_matching_pass_emu",&trig_matching_pass_emu,"trig_matching_pass_emu/F");
  Tnewvar->Branch("trig_matching_pass_mumu",&trig_matching_pass_mumu,"trig_matching_pass_mumu/F");

  // Prescale factors
  Tnewvar->Branch("psfactor_hlt_AK8PFHT800_TrimMass50",&psfactor_hlt_AK8PFHT800_TrimMass50,"psfactor_hlt_AK8PFHT800_TrimMass50/F");
  Tnewvar->Branch("psfactor_hlt_AK8PFHT900_TrimMass50",&psfactor_hlt_AK8PFHT900_TrimMass50,"psfactor_hlt_AK8PFHT900_TrimMass50/F");
  Tnewvar->Branch("psfactor_hlt_AK8PFJet400_TrimMass30",&psfactor_hlt_AK8PFJet400_TrimMass30,"psfactor_hlt_AK8PFJet400_TrimMass30/F");
  Tnewvar->Branch("psfactor_hlt_AK8PFJet420_TrimMass30",&psfactor_hlt_AK8PFJet420_TrimMass30,"psfactor_hlt_AK8PFJet420_TrimMass30/F");
  Tnewvar->Branch("psfactor_hlt_AK8PFJet550",&psfactor_hlt_AK8PFJet550,"psfactor_hlt_AK8PFJet550/F");
  Tnewvar->Branch("psfactor_hlt_CaloJet500_NoJetID",&psfactor_hlt_CaloJet500_NoJetID,"psfactor_hlt_CaloJet500_NoJetID/F");
  Tnewvar->Branch("psfactor_hlt_PFHT1050",&psfactor_hlt_PFHT1050,"psfactor_hlt_PFHT1050/F");
  Tnewvar->Branch("psfactor_hlt_PFJet500",&psfactor_hlt_PFJet500,"psfactor_hlt_PFJet500/F");
  Tnewvar->Branch("psfactor_hlt_AK8PFJet320",&psfactor_hlt_AK8PFJet320,"psfactor_hlt_AK8PFJet320/F");
  Tnewvar->Branch("psfactor_hlt_AK8PFJet330_PFAK8BTagDeepCSV",&psfactor_hlt_AK8PFJet330_PFAK8BTagDeepCSV,"psfactor_hlt_AK8PFJet330_PFAK8BTagDeepCSV/F");
  Tnewvar->Branch("psfactor_hlt_AK8PFJet360_TrimMass30",&psfactor_hlt_AK8PFJet360_TrimMass30,"psfactor_hlt_AK8PFJet360_TrimMass30/F");
  Tnewvar->Branch("psfactor_hlt_DiPFJetAve400",&psfactor_hlt_DiPFJetAve400,"psfactor_hlt_DiPFJetAve400/F");
  Tnewvar->Branch("psfactor_hlt_PFJet400",&psfactor_hlt_PFJet400,"psfactor_hlt_PFJet400/F");
  Tnewvar->Branch("psfactor_hlt_PFJet320",&psfactor_hlt_PFJet320,"psfactor_hlt_PFJet320/F");
  Tnewvar->Branch("psfactor_hlt_PFJet200",&psfactor_hlt_PFJet200,"psfactor_hlt_PFJet200/F");
  Tnewvar->Branch("psfactor_hlt_CaloMET100_HBHECleaned",&psfactor_hlt_CaloMET100_HBHECleaned,"psfactor_hlt_CaloMET100_HBHECleaned/F");
  Tnewvar->Branch("psfactor_hlt_CaloMET250_HBHECleaned",&psfactor_hlt_CaloMET250_HBHECleaned,"psfactor_hlt_CaloMET250_HBHECleaned/F");
  Tnewvar->Branch("psfactor_hlt_CaloMET90_HBHECleaned",&psfactor_hlt_CaloMET90_HBHECleaned,"psfactor_hlt_CaloMET90_HBHECleaned/F");
  Tnewvar->Branch("psfactor_hlt_CaloMET70_HBHECleaned",&psfactor_hlt_CaloMET70_HBHECleaned,"psfactor_hlt_CaloMET70_HBHECleaned/F");
  Tnewvar->Branch("psfactor_hlt_PFMETTypeOne140_PFMHT140_IDTight",&psfactor_hlt_PFMETTypeOne140_PFMHT140_IDTight,"psfactor_hlt_PFMETTypeOne140_PFMHT140_IDTight/F");
  Tnewvar->Branch("psfactor_hlt_PFMETTypeOne120_PFMHT120_IDTight",&psfactor_hlt_PFMETTypeOne120_PFMHT120_IDTight,"psfactor_hlt_PFMETTypeOne120_PFMHT120_IDTight/F");
  Tnewvar->Branch("psfactor_hlt_CaloMET80_HBHECleaned",&psfactor_hlt_CaloMET80_HBHECleaned,"psfactor_hlt_CaloMET80_HBHECleaned/F");
  
  //Tnewvar->Branch("",&,"/F");
  
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

  calib_deepflav = BTagCalibration("DeepJet", (dir + "DeepJet_106XUL18SF_WPonly_V1p1.csv").Data());
  reader_deepflav_tight = BTagCalibrationReader(BTagEntry::OP_TIGHT, "central", {"up", "down"}); 
  reader_deepflav_tight.load(calib_deepflav, BTagEntry::FLAV_B, "comb");
  reader_deepflav_tight.load(calib_deepflav, BTagEntry::FLAV_C, "comb");
  reader_deepflav_tight.load(calib_deepflav, BTagEntry::FLAV_UDSG, "incl");

  reader_deepflav_med = BTagCalibrationReader(BTagEntry::OP_MEDIUM, "central", {"up", "down"}); 
  reader_deepflav_med.load(calib_deepflav, BTagEntry::FLAV_B, "comb");
  reader_deepflav_med.load(calib_deepflav, BTagEntry::FLAV_C, "comb");
  reader_deepflav_med.load(calib_deepflav, BTagEntry::FLAV_UDSG, "incl");
  
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
  // It can be passed to either Trigg_eff_cal::GetEntry() or TBranch::GetEntry()
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


  /// Set default values to tree variables
  weight_toppt = weight_pileup = weight_pileup_up = weight_pileup_down = weight_prefiring = weight_prefiring_up = weight_prefiring_down = weight_btag = weight_btag_up = weight_btag_down = total_weight_ee = weight_lepeff_ee = weight_lepeff_stat_ee = weight_lepeff_syst_ee = total_weight_mumu = weight_lepeff_mumu = weight_lepeff_stat_mumu = weight_lepeff_syst_mumu = total_weight_emu = weight_lepeff_emu = weight_lepeff_stat_emu = weight_lepeff_syst_emu = gen_weight = 1;
  
  nak8jets = nbjets = chs_met = chs_met_up = chs_met_down = deltaphi = EventHT = nak4jets = n_electrons = n_muons = nvetomuons = nvetoelectrons = nak8jets = ak8jet1_pt = ak8jet2_pt = ak8jet1_sdmass = ak8jet1_toptagscore = ak8jet2_toptagscore = lep1_pdg = lep1_pdg = lep2_pdg = isttbar_ee = isttbar_emu = isttbar_mumu = deltaRlep1 = deltaRlep2 = lepmatchwithak8 = ak8jet1_eta = ak8jet1_mass = ak8jet1_phi = ak8jet2_phi = ak8jet2_mass = lep1_pt = lep1_eta = lep1_phi = lep2_pt = lep2_eta = lep2_phi = jet_trigger_unprescale = jet_trigger_prescale = met_trigger_unprescale = met_trigger_prescale = ee_trigger = emu_trigger = mumu_trigger = trig_matching_pass_ee = trig_matching_pass_emu = trig_matching_pass_mumu = -100;
  dirglthrmin = dirgltrthr = -100.;
  
  int icheck = GetEntry(entry);  // Get entries from input trees
  float weight_ch[nchannel] = {1.0,1.0,1.0};  // Define weights for each channel

  if(isMC){
    for(int ich =0; ich < nchannel; ich++)
      weight_ch[ich] = event_weight;
    gen_weight = event_weight;  
  }else{
    for(int ich =0; ich < nchannel; ich++)
      weight_ch[ich] = 1;
    gen_weight = 1;
  }
  
  TString str;
  bool debug_mode = false;
  str = TString::Format("check START for evt %d ",ievt);         
  if(gProofServ && debug_mode) gProofServ->SendAsynMessage(str);
    
  // Get all gen level info for MC only
  vector<GenParton> genpartons;
  vector<GenParton> LHEtops;
  vector<TopQuark> gentops;
  vector<LHEparticle> lheparticles;
  int cntt = 0; int nel=0; int nmu=0;
  tau_decay_mode = 0;
  weight_toppt = 1;
    
  if( isMC ){
    getPartons(genpartons);
    getLHEParticles(lheparticles);
    // Get GEN-level top quarks
    tt_decay_mode = -1;
    if(isTT || isST || isTTX) {
      getGENTops(gentops,genpartons); // after shower (get top quarks from its daughters) --> will tell details about the signature of ttbar events at GEN level
      getLHETops(LHEtops,genpartons); // before shower (get original top quarks which have decayed) --> will be usedto derive top pt reweighting
      tt_decay_mode = 0;
      for(int ip=0; ip <(int)lheparticles.size();ip++)
	{
	  if(abs(lheparticles[ip].pdgId) == 11)
	    { tt_decay_mode +=10; cntt++; nel++;}
	  else if(abs(lheparticles[ip].pdgId) == 13)
	    { tt_decay_mode +=100; cntt++; nmu++;}
	  else if(abs(lheparticles[ip].pdgId) == 15)
	    { tt_decay_mode +=1000; cntt++;}
	}
      tt_decay_mode += 2 - cntt; // +1 for quark, +10 for e, +100 for muon, +1000 for tau
      
      // top pt reweighting //
      if(LHEtops.size()==2 && isTT){
	weight_toppt = SF_TOP(0.0615,0.0005,TMath::Min(float(500),float(LHEtops[0].pt)),TMath::Min(float(500),float(LHEtops[1].pt)));
	weight_ch[0] *= weight_toppt;
	weight_ch[1] *= weight_toppt;
	weight_ch[2] *= weight_toppt;
      }
    }
  }    

  str = TString::Format("checkpost 1 for evt %d ",ievt);         
  if(gProofServ && debug_mode) gProofServ->SendAsynMessage(str);
  
  // Pile up rewighing  and prefire weight//
  if(isMC && npu_vert_true>=0 && npu_vert_true<100){
    float *puweights = Get_PU_Weights(npu_vert_true);
    weight_pileup = puweights[0];
    weight_pileup_up = puweights[1];
    weight_pileup_down = puweights[2];
    weight_prefiring = prefiringweight;
    weight_prefiring_up = prefiringweightup;
    weight_prefiring_down = prefiringweightdown;
    weight_ch[0] *= weight_pileup;
    weight_ch[0] *= weight_prefiring;
    weight_ch[1] *= weight_pileup;
    weight_ch[1] *= weight_prefiring;
    weight_ch[2] *= weight_pileup;
    weight_ch[2] *= weight_prefiring;
  }
  else if(isMC)
    {
      str = TString::Format("npu_vert_true = %d weight = %f nprimi = %d is out of range for evt %d  ;",npu_vert_true,weight_ch[0],nprimi,ievt);          
      if(gProofServ) gProofServ->SendAsynMessage(str);
      return kFALSE;
    }
  
  str = TString::Format("checkpost 2 for evt %d ",ievt);         
  if(gProofServ && debug_mode) gProofServ->SendAsynMessage(str);  

  vector <Electron> velectrons;
  getelectrons(velectrons,10,absetacut);    // electron_pt_cut & absetacut defined in Proof.h
    
  //Here you get muons with your criteria
  vector <Muon> vmuons;
  getmuons(vmuons,10,absetacut);        

  //Make lepton collection from electrons & muons (using only common variables)
  vector <Lepton> vleptons;
  getLeptons(vleptons,vmuons,velectrons,10);
    
  //Here you get AK4 jets with your criteria                                     
      vector <AK4Jet> Jets;
  getAK4jets(Jets,AK4jet_pt_cut,absetacut,isMC);     
  
  //Here you get AK8 jets with your criteria 
      vector <AK8Jet> LJets;
  getAK8jets(LJets,300,absetacut,isMC);
  
  Lepton lepcand1,lepcand2;
    
  if((int)vleptons.size() > 1)
    {
      lep1_pt = vleptons[0].pt;
      lep2_pt = vleptons[1].pt;
      lep1_pdg = vleptons[0].pdgId;
      lep2_pdg = vleptons[1].pdgId;
      lep1_eta = vleptons[0].eta;
      lep2_eta = vleptons[1].eta;
      lep1_phi = vleptons[0].phi;
      lep2_phi = vleptons[1].phi;
      lepcand1= vleptons[0];
      lepcand2 = vleptons[1]; 
    }
  
  for(int ich =0; ich < nchannel; ich++)
    hist_count[ich]->Fill(0.0,weight_ch[ich]);
  
  for(int ich =0; ich < nchannel; ich++)
    if((int)vleptons.size()>1 && (int)LJets.size()>1) hist_count[ich]->Fill(1.0,weight_ch[ich]);
  
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

  nvetoelectrons=0;
  nvetomuons=0;
  
  for(int ie=0; ie<(int)velectrons.size(); ie++) {
    if(velectrons[ie].mvaWPloose_noIso)
      nvetoelectrons++;
  }
  for(int im=0; im<(int)vmuons.size(); im++) {
    if(vmuons[im].isLoose)
      nvetomuons++;
  }
   
  str = TString::Format("checkpost 3 for evt %d ",ievt);         
  if(gProofServ && debug_mode) gProofServ->SendAsynMessage(str);  

  elleptonsf_weight = elleptonsf_weight_stat = elleptonsf_weight_syst = 1;
  muleptonsf_weight = muleptonsf_weight_stat = muleptonsf_weight_syst = 1;
  
  if(isMC){
    for(int ie=0; ie<(int)velectrons.size(); ie++) {
      float *sfvalues;
      sfvalues = Electron_SF(velectrons[ie].pt,velectrons[ie].eta,"reco");
      elleptonsf_weight *= sfvalues[0];
      elleptonsf_weight_stat *= (sfvalues[0] + sqrt(sfvalues[1]*sfvalues[1] + sfvalues[2]*sfvalues[2]));  // like this for time being
      elleptonsf_weight_syst *= (sfvalues[0] + sqrt(sfvalues[3]*sfvalues[3] + sfvalues[4]*sfvalues[4] + sfvalues[5]*sfvalues[5] + sfvalues[6]*sfvalues[6]));  // like this for time being
    }
    
    for(int im=0; im<(int)vmuons.size(); im++) {
      float *sfvalues;
      sfvalues = Muon_SF(vmuons[im].pt,vmuons[im].eta,"reco");
      muleptonsf_weight *= sfvalues[0];
      muleptonsf_weight_stat *= sfvalues[1];
      muleptonsf_weight_syst *= sfvalues[2];
    }
    //weight *= leptonsf_weight;
  }
  weight_ch[0] *=elleptonsf_weight;
  weight_lepeff_ee = elleptonsf_weight; 
  weight_lepeff_stat_ee = elleptonsf_weight_stat;
  weight_lepeff_syst_ee = elleptonsf_weight_syst;
  
  weight_ch[1] *=elleptonsf_weight*muleptonsf_weight;
  weight_lepeff_emu = elleptonsf_weight*muleptonsf_weight; 
  weight_lepeff_stat_emu = elleptonsf_weight_stat*muleptonsf_weight_stat;
  weight_lepeff_syst_emu = elleptonsf_weight_syst*muleptonsf_weight_syst;

  weight_ch[2] *=muleptonsf_weight;
  weight_lepeff_mumu = muleptonsf_weight; 
  weight_lepeff_stat_mumu = muleptonsf_weight_stat;
  weight_lepeff_syst_mumu = muleptonsf_weight_syst;
  
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
  float p_mc, p_data, p_data_up, p_data_dn;
  p_mc = p_data = p_data_up = p_data_dn = 1;

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
	  p_data_up *= jet.btag_DeepFlav_med_SF_up*eff_m;
	  p_data_dn *= jet.btag_DeepFlav_med_SF_dn*eff_m;	
	}
      else
	{
	  p_mc *= max((double)0.00001,1 - eff_m);
	  p_data *= 1 - jet.btag_DeepFlav_med_SF*eff_m;
	  p_data_up *= 1 - jet.btag_DeepFlav_med_SF_up*eff_m;
	  p_data_dn *= 1 - jet.btag_DeepFlav_med_SF_dn*eff_m;
	}
    }
  }

  weight_btag = p_data/p_mc;
  weight_btag_up = p_data_up/p_mc;
  weight_btag_down = p_data_dn/p_mc;
  // for(int ich =0; ich < nchannel; ich++)
  //weight_ch[ich] *= btag_SF;
  
  if(isnan(weight_ch[0]) || isnan(weight_ch[1]) || isnan(weight_ch[2])){
    str = TString::Format("check for evt %d weight %f btagsf %f leptonsf %f",ievt,weight[0],btag_SF,elleptonsf_weight);          
    if(gProofServ) gProofServ->SendAsynMessage(str);
  }

  if(isTT && (weight[0] < -2000 || weight[1] < -2000 || weight[2] < -2000))
    return kFALSE;
  std::vector<std::pair<int,TLorentzVector> > TrigRefObj;

  str = TString::Format("checkpost 4 for evt %d ",ievt);         
  if(gProofServ && debug_mode) gProofServ->SendAsynMessage(str);  
  
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
		  hists1,hist2d_prptangle1,weight_ch[0]
		  );
    
    Match_trigger(vsinglelep_trig,vdoublelep_trig[1],		
		  TrigRefObj,
		  vleptons[0],vleptons[1],Jets,
		  trig_threshold_pass[1],
		  trig_matching_pass[1],
		  hists2,hist2d_prptangle2,weight_ch[1]
		  );
    
    Match_trigger(vsinglelep_trig,vdoublelep_trig[2],		
		  TrigRefObj,
		  vleptons[0],vleptons[1],Jets,
		  trig_threshold_pass[2],
		  trig_matching_pass[2],
		  hists3,hist2d_prptangle3,weight_ch[2]
		  );	   

  }
  str = TString::Format("checkpost 5 for evt %d ",ievt);         
  if(gProofServ && debug_mode) gProofServ->SendAsynMessage(str);  

  bool itrig_ortho_trig = false;
 
  itrig_ortho_trig = hlt_AK8PFHT800_TrimMass50 || hlt_AK8PFHT900_TrimMass50 || hlt_AK8PFJet420_TrimMass30 || hlt_AK8PFJet550 || hlt_CaloJet500_NoJetID || hlt_PFHT1050 || hlt_PFJet500 || hlt_AK8PFJet500;

  trig_met = float(itrig_ortho_trig);
  trig_ee = float(hlt_DoubleEle25);
  trig_emu = float(hlt_Mu27Ele37 || hlt_Mu37Ele27);
  trig_mumu = float(hlt_Mu37TkMu27);
  
  bool object_pass[nchannel] = {false};
  invmll = -100;
  if((int)vleptons.size() >1)
    invmll = (vleptons[0].p4 + vleptons[1].p4).M();


  float ee_chnl_toptaggercut1 = 0.75; float ee_chnl_toptaggercut2 = 0.88;
  float emu_chnl_eltoptaggercut1 = 0.8; float emu_chnl_mutoptaggercut2 = 0.5;
  float mumu_chnl_toptaggercut1 = 0.33; float mumu_chnl_toptaggercut2 = 0.9;

  
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
	  emuchannel_tagcutcondition = (LJets[0].re_tvsb > emu_chnl_mutoptaggercut2 && LJets[1].re_tvsb > emu_chnl_eltoptaggercut1);
	  emulepcand1 = vleptons[1];
	  emulepcand2 = vleptons[0]; 
	}
      else if(abs(vleptons[0].pdgId) == 11)
	{
	  emuchannel_tagcutcondition = (LJets[0].re_tvsb > emu_chnl_eltoptaggercut1 && LJets[1].re_tvsb > emu_chnl_mutoptaggercut2);
	  emulepcand1 = vleptons[0];
	  emulepcand2 = vleptons[1]; 
	}
    }

  deltaphi = -1.0;
  if((int)LJets.size()>1)  deltaphi = fabs(PhiInRange(LJets[0].phi - LJets[1].phi));

  lepmatchwithak8 = 0;

  if((int)vleptons.size()>1 && vleptons[0].pdgId == lepcand1.pdgId && vleptons[0].indexemu == lepcand1.indexemu && vleptons[1].pdgId == lepcand2.pdgId && vleptons[1].indexemu == lepcand2.indexemu)
    lepmatchwithak8 = 1;
  else if( (int)vleptons.size()>1 && vleptons[0].pdgId == lepcand2.pdgId && vleptons[0].indexemu == lepcand2.indexemu && vleptons[1].pdgId == lepcand1.pdgId && vleptons[1].indexemu == lepcand1.indexemu)
    lepmatchwithak8 = 1;
 
  // str = TString::Format("check for evt %d      %d %d  %d %d      %f %f  %f %f      matched = %d",ievt,vleptons[0].pdgId,vleptons[1].pdgId,lepcand1.pdgId,lepcand2.pdgId,vleptons[0].pt,vleptons[1].pt,lepcand1.pt,lepcand2.pt,lepmatchwithak8);          
  //if(gProofServ) gProofServ->SendAsynMessage(str);
  
  isttbar_ee = (isTTDilep && tt_decay_mode == 20) || ( !isTTDilep && !isTT ) || ( !isTTDilep && isTT && tt_decay_mode != 20);
  isttbar_emu = (isTTDilep && tt_decay_mode == 110) || ( !isTTDilep && !isTT ) || ( !isTTDilep && isTT && tt_decay_mode != 110);
  isttbar_mumu = (isTTDilep && tt_decay_mode == 200) || ( !isTTDilep && !isTT ) || ( !isTTDilep && isTT && tt_decay_mode != 200);

    str = TString::Format("checkpost 6 for evt %d ",ievt);         
  if(gProofServ && debug_mode) gProofServ->SendAsynMessage(str);  

  /*  object_pass[0] =    (((int)vleptons.size() >1 && vleptons[1].pt > 10 && vleptons[0].pt > 10 && abs(vleptons[0].pdgId) == 11 && abs(vleptons[1].pdgId) == 11 && vleptons[0].pdgId*vleptons[1].pdgId <0))  && (int)LJets.size()>1 && min(LJets[0].re_tvsb,LJets[1].re_tvsb) > ee_chnl_toptaggercut1 && max(LJets[0].re_tvsb,LJets[1].re_tvsb) > ee_chnl_toptaggercut2 && delta2R(LJets[0].p4,vleptons[0].p4)<0.7 && delta2R(LJets[1].p4,vleptons[1].p4)<0.7 && lepmatchwithak8 == 1 && itrig_ortho_trig && (tt_decay_mode == 20 || !isTT);

  object_pass[1] =    ( ((int)vleptons.size() >1 && vleptons[0].pt > 10  && vleptons[0].pt > 10 && abs(vleptons[0].pdgId) == 13 && abs(vleptons[1].pdgId) == 11 && vleptons[0].pdgId*vleptons[1].pdgId <0)
			|| ((int)vleptons.size() >1 && vleptons[0].pt > 10  && vleptons[0].pt > 10 && abs(vleptons[1].pdgId) == 13 && abs(vleptons[0].pdgId) == 11 && vleptons[0].pdgId*vleptons[1].pdgId <0)) && (int)LJets.size()>1 && emuchannel_tagcutcondition  && delta2R(LJets[0].p4,vleptons[0].p4)<0.7 && delta2R(LJets[1].p4,vleptons[1].p4)<0.7 && lepmatchwithak8 == 1 && itrig_ortho_trig && (tt_decay_mode == 110 || !isTT);
			//             || ((int)vmuons.size() >0 && vmuons[0].pt > 53)*/
  /*             || ((int)velectrons.size() > 0 && velectrons[0].pt > 53 && (int)Jets.size() > 0 && Jets[0].pt > 180)*/

  //object_pass[2] =    (((int)vleptons.size() >1 && vleptons[1].pt > 10 && vleptons[0].pt > 10 && abs(vleptons[0].pdgId) == 13 && abs(vleptons[1].pdgId) == 13 && vleptons[0].pdgId*vleptons[1].pdgId <0)) && (int)LJets.size()>1 && min(LJets[0].re_tvsb,LJets[1].re_tvsb) > mumu_chnl_toptaggercut1 && max(LJets[0].re_tvsb,LJets[1].re_tvsb) > mumu_chnl_toptaggercut2 && delta2R(LJets[0].p4,vleptons[0].p4)<0.7 && delta2R(LJets[1].p4,vleptons[1].p4)<0.7  && abs(lep1pdg) == 13 && abs(lep2pdg) == 13 && lepmatchwithak8 == 1 && itrig_ortho_trig && (tt_decay_mode == 200 || !isTT);
  
  jet_trigger_unprescale = float(hlt_AK8PFHT800_TrimMass50 || hlt_AK8PFHT900_TrimMass50 || hlt_AK8PFJet400_TrimMass30 || hlt_AK8PFJet420_TrimMass30 || hlt_AK8PFJet550 || hlt_CaloJet500_NoJetID || hlt_PFHT1050 || hlt_PFJet500 || hlt_AK8PFJet500);
  
  jet_trigger_prescale = float(hlt_AK8PFJet320 ||  hlt_AK8PFJet330_PFAK8BTagDeepCSV || hlt_AK8PFJet360_TrimMass30 || hlt_DiPFJetAve400 || hlt_PFJet400 || hlt_PFJet320 || hlt_PFJet200);

  met_trigger_unprescale = float(hlt_PFMET250 || hlt_PFMET300 || hlt_PFMET200 || hlt_PFMET200_TypeOne);

  met_trigger_prescale = float(hlt_CaloMET100_HBHECleaned || hlt_CaloMET250_HBHECleaned || hlt_CaloMET90_HBHECleaned || hlt_CaloMET70_HBHECleaned || hlt_PFMETTypeOne140_PFMHT140_IDTight || hlt_PFMETTypeOne120_PFMHT120_IDTight || hlt_CaloMET80_HBHECleaned);

  ee_trigger = float(hlt_DoubleEle25);
  emu_trigger = float(hlt_Mu37Ele27||hlt_Mu27Ele37);
  mumu_trigger = float(hlt_Mu37TkMu27);
  trig_matching_pass_ee = float(trig_matching_pass[0]);
  trig_matching_pass_emu = float(trig_matching_pass[1]);
  trig_matching_pass_mumu = float(trig_matching_pass[2]);
  
  if((int)vleptons.size()>1 && (isnan(vleptons[0].pt) || isnan(vleptons[0].eta) || isnan(vleptons[0].phi) || isnan(vleptons[1].pt) || isnan(vleptons[1].eta) || isnan(vleptons[1].phi) || isnan(Jets.size()))){
    str = TString::Format("check for evt %d",ievt);          
    if(gProofServ) gProofServ->SendAsynMessage(str);
  }

  str = TString::Format("checkpost 7 for evt %d ",ievt);         
  if(gProofServ && debug_mode) gProofServ->SendAsynMessage(str);  

  
  if((int)vleptons.size() >1 && vleptons[1].pt > 10 && vleptons[0].pt > 10 && (int)LJets.size()>1  && delta2R(LJets[1].p4,vleptons[1].p4)<0.8 && delta2R(LJets[0].p4,vleptons[0].p4)<0.8 && lep1_pdg*lep2_pdg < 0 && (jet_trigger_unprescale ==1 ||  jet_trigger_prescale ==1 ||  met_trigger_unprescale ==1 ||  met_trigger_prescale ==1))
  {
    n_electrons = (int)velectrons.size();
    n_muons = (int)vmuons.size();
    nak8jets = (int)LJets.size();
    ak8jet1_pt = LJets[0].pt;
    ak8jet1_sdmass = LJets[0].sdmass;
    ak8jet1_eta = LJets[0].eta;
    ak8jet1_phi = LJets[0].phi;
    ak8jet1_mass = LJets[0].mass;
    ak8jet2_pt = LJets[1].pt;
    ak8jet2_eta = LJets[1].eta;
    ak8jet2_phi = LJets[1].phi;
    ak8jet2_mass = LJets[1].mass;
    ak8jet2_sdmass = LJets[1].sdmass;
    ak8jet1_toptagscore = LJets[0].re_tvsb;
    ak8jet2_toptagscore = LJets[1].re_tvsb;
    nak4jets = (int)Jets.size();
    chs_met_up = miset_UnclusEup;
    chs_met_down = miset_UnclusEdn;

    deltaRlep1 = delta2R(LJets[0].p4,vleptons[0].p4);
    deltaRlep2 = delta2R(LJets[1].p4,vleptons[1].p4);
  
    total_weight_ee = weight_ch[0];
    total_weight_emu = weight_ch[1];
    total_weight_mumu = weight_ch[2];

    dirgltrthr = 0;
    dirglthrmin = 0;
    
    if ((int)Jets.size()>=1) {
      std::vector<TLorentzVector> allsjets_4v;
      double Pt_sum(0.);
      for(int ijet=0; ijet< (int)Jets.size(); ijet++){
	TLorentzVector sjv = Jets[ijet].p4;
	allsjets_4v.push_back(sjv);
	Pt_sum += Jets[ijet].pt;
      }
      
      for(int ilep=0; ilep<(int)velectrons.size(); ilep++){
	TLorentzVector sjv = velectrons[ilep].p4;
	allsjets_4v.push_back(sjv);
	Pt_sum += velectrons[ilep].p4.Pt();
      }
      
      for(int ilep=0; ilep<(int)vmuons.size(); ilep++){
	TLorentzVector sjv = vmuons[ilep].p4;
	allsjets_4v.push_back(sjv);
	Pt_sum += vmuons[ilep].p4.Pt();
      }
      std::vector<double> ThrustAxis;
      std::vector<double> Thrust;
      
      for(unsigned int j =0;j<4;j++){                                                            
	Thrust.push_back(0.01);                                                                    
      }
      Thrust_calculate(allsjets_4v,Thrust);
      dirgltrthr = Thrust[3];
      
      double alpha=atan2(Thrust[1],Thrust[0]);
      for( int i=0; i<(int)allsjets_4v.size(); i++){
	dirglthrmin += fabs(-sin(alpha)*allsjets_4v[i].Px()+cos(alpha)*allsjets_4v[i].Py());
      }
      dirglthrmin = dirglthrmin/max(1.0, Pt_sum);
    }

    EventHT = -1;	
    
    if ((int)Jets.size()>=1) {
      std::vector<TLorentzVector> allsjets_4v;
      double Pt_sum(0.);
      for(int ijet=0; ijet< (int)Jets.size(); ijet++){
	TLorentzVector sjv = Jets[ijet].p4;
	allsjets_4v.push_back(sjv);
	Pt_sum += Jets[ijet].pt;
      }
      
      for(int ilep=0; ilep<(int)velectrons.size(); ilep++){
	TLorentzVector sjv = velectrons[ilep].p4;
	allsjets_4v.push_back(sjv);
	Pt_sum += velectrons[ilep].p4.Pt();
      }
      
      for(int ilep=0; ilep<(int)vmuons.size(); ilep++){
	TLorentzVector sjv = vmuons[ilep].p4;
	allsjets_4v.push_back(sjv);
	Pt_sum += vmuons[ilep].p4.Pt();
      }
      
      EventHT = Pt_sum;	
    }
  
    Tnewvar->Fill();
  }
  str = TString::Format("check END for evt %d ",ievt);         
  if(gProofServ && debug_mode) gProofServ->SendAsynMessage(str);  
     
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
