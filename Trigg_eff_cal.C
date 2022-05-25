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
  
  for(int init=0; init<nhist_in; init++){
    char namein[1000]; //nameinup[1000], nameindn[1000];
    char titlein[1000];
    
    sprintf(namein,"hist_%s",initnames[init]);
    sprintf(titlein,"%s",titlenames[init]);
    hist_init[init] = new TH1D(namein,titlein,ini_nbins[init],ini_low[init],ini_up[init]);
    hist_init[init]->Sumw2();
  }

  for (int ij=0; ij<npr_angle; ij++) 
    hist_prptangle[ij] = new TH2D(pr_angle[ij], pr_angle[ij], 60, 30.0, 510.0, 120, 0.0, 2.0);

  for(int ich=0; ich< nchannel; ich++){
    hist_count[ich] = new TH1D((channel[ich]+"Counter").c_str(),(channel[ich]+"Counter").c_str(),10,-0.5,9.5);
    hist_count[ich]->Sumw2();  
  }
  
  for(int ich=0; ich< nchannel; ich++){   /// nchannel declared in .h file

    hist_ortho_trig_2d[0][ich] = new TH2D((channel[ich]+"_hist_orthotrig_lepptvsleppt").c_str(),(channel[ich]+"_hist_orthotrig_lepptvsleppt").c_str(),nbins_ptlep,ptbins_lep,nbins_ptlep,ptbins_lep);
    hist_ortho_trig_2d[1][ich] = new TH2D((channel[ich]+"_hist_orthotrig_lepetavslepeta").c_str(),(channel[ich]+"_hist_orthotrig_lepetavslepeta").c_str(),5,-2.5,2.5,5,-2.5,2.5);
    hist_ortho_trig_2d[2][ich] = new TH2D((channel[ich]+"_hist_orthotrig_lepphivslepphi").c_str(),(channel[ich]+"_hist_orthotrig_lepphivslepphi").c_str(),5,-1*TMath::Pi(),TMath::Pi(),5,-1*TMath::Pi(),TMath::Pi());
    hist_ortho_trig[0][ich] = new TH1D((channel[ich]+"_hist_orthotrig_dileppt").c_str(),(channel[ich]+"_hist_orthotrig_dileppt").c_str(),5,0,1000);
    hist_ortho_trig[1][ich] = new TH1D((channel[ich]+"_hist_orthotrig_dilepmass").c_str(),(channel[ich]+"_hist_orthotrig_dilepmass").c_str(),8,50,130);
    hist_ortho_trig[2][ich] = new TH1D((channel[ich]+"_hist_orthotrig_dilepphi").c_str(),(channel[ich]+"_hist_orthotrig_dilepphi").c_str(),5,-1*TMath::Pi(),TMath::Pi());
    hist_ortho_trig[3][ich] = new TH1D((channel[ich]+"_hist_orthotrig_dilepeta").c_str(),(channel[ich]+"_hist_orthotrig_dilepeta").c_str(),5,-2.5,2.5);
    hist_ortho_trig[4][ich] = new TH1D((channel[ich]+"_hist_orthotrig_njets").c_str(),(channel[ich]+"_hist_orthotrig_njets").c_str(),11,-0.5,10.5);

    hist_trig_eff_2d[0][ich] = new TH2D((channel[ich]+"_hist_trig_eff_lepptvsleppt").c_str(),(channel[ich]+"_hist_trig_eff_lepptvsleppt").c_str(),nbins_ptlep,ptbins_lep,nbins_ptlep,ptbins_lep);
    hist_trig_eff_2d[1][ich] = new TH2D((channel[ich]+"_hist_trig_eff_lepetavslepeta").c_str(),(channel[ich]+"_hist_trig_eff_lepetavslepeta").c_str(),5,-2.5,2.5,5,-2.5,2.5);
    hist_trig_eff_2d[2][ich] = new TH2D((channel[ich]+"_hist_trig_eff_lepphivslepphi").c_str(),(channel[ich]+"_hist_trig_eff_lepphivslepphi").c_str(),5,-1*TMath::Pi(),TMath::Pi(),5,-1*TMath::Pi(),TMath::Pi());
    hist_trig_eff[0][ich] = new TH1D((channel[ich]+"_hist_trig_eff_dileppt").c_str(),(channel[ich]+"_hist_trig_eff_dileppt").c_str(),5,0,1000);
    hist_trig_eff[1][ich] = new TH1D((channel[ich]+"_hist_trig_eff_dilepmass").c_str(),(channel[ich]+"_hist_trig_eff_dilepmass").c_str(),8,50,130);
    hist_trig_eff[2][ich] = new TH1D((channel[ich]+"_hist_trig_eff_dilepphi").c_str(),(channel[ich]+"_hist_trig_eff_dilepphi").c_str(),5,-1*TMath::Pi(),TMath::Pi());
    hist_trig_eff[3][ich] = new TH1D((channel[ich]+"_hist_trig_eff_dilepeta").c_str(),(channel[ich]+"_hist_trig_eff_dilepeta").c_str(),5,-2.5,2.5);
    hist_trig_eff[4][ich] = new TH1D((channel[ich]+"_hist_trig_eff_njets").c_str(),(channel[ich]+"_hist_trig_eff_njets").c_str(),11,-0.5,10.5);
      
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
    weight = event_weight;
  }else{
    weight = 1;
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
    weight *= puWeight;
    weight *= prefiringweight;
  }
  else if(isMC)
    {
      weight = 0;
      str = TString::Format("npu_vert_true = %d weight = %f nprimi = %d is out of range for evt %d  ; ttree = %d",npu_vert_true,weight,nprimi,ievt,icheck);          
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
	weight *= toppt_wt;
      }
    }
  }    
    
  bool itrig_ortho_trig = false;
 
  itrig_ortho_trig = hlt_PFMET250 || hlt_PFMET300 || hlt_PFMET200 || hlt_PFMET200_TypeOne;

  hist_count[0]->Fill(0.0,weight);
  hist_count[1]->Fill(0.0,weight);
  hist_count[2]->Fill(0.0,weight);

  if(!itrig_ortho_trig) return kFALSE;             

  hist_count[0]->Fill(1.0,weight);
  hist_count[1]->Fill(1.0,weight);
  hist_count[2]->Fill(1.0,weight);

  vector <Electron> velectrons;
  getelectrons(velectrons, 10 ,absetacut);   
  
  vector <Muon> vmuons;
  getmuons(vmuons,10 ,absetacut);

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
	  p_mc *= eff_m;
	  p_data *= jet.btag_DeepFlav_med_SF*eff_m;
	}
      else
	{
	  p_mc *= 1 - eff_m;
	  p_data *= 1 - jet.btag_DeepFlav_med_SF*eff_m;
	}
    }
  }

  btag_SF = p_data/p_mc;
  weight *= btag_SF;

  vector <Lepton> vleptons;
  getLeptons(vleptons,vmuons,velectrons,10);
  
 vector <AK8Jet> LJets;
  getAK8jets(LJets,300,absetacut,isMC);

  /*  if ((int)LJets.size()>0 && (int)vleptons.size() > 1) {
    for(int ijet=0;ijet<min(2,int(LJets.size()));ijet++){
      //Match lepton with AK8 jets
      LJets[ijet].match_lepton_index = get_nearest_lepton(vleptons,LJets[ijet].p4);
      if (LJets[ijet].match_lepton_index >=0 && ijet!=LJets[ijet].match_lepton_index && ijet<int(vleptons.size())) {
	Lepton tmplep = vleptons[LJets[ijet].match_lepton_index];
	vleptons.erase(vleptons.begin() + LJets[ijet].match_lepton_index);
	vleptons.insert(vleptons.begin()+ijet, tmplep);
      }
      
      // Matched highest b tag AK4 jets  with AK8 jets //                                         
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
  
  leptonsf_weight = leptonsf_weight_stat = leptonsf_weight = 1;
  if(isMC){
    for(int ie=0; ie<(int)velectrons.size(); ie++) {
      float *sfvalues;
      //      if( LJets.size() > 0 && (delta2R(velectrons[ie].p4,LJets[min(0,int(LJets.size()-1))].p4)<0.7 || delta2R(velectrons[ie].p4,LJets[min(1,int(LJets.size()-1))].p4)<0.7))
	sfvalues = Electron_SF(velectrons[ie].pt,velectrons[ie].eta,"reco");
	//else if(!velectrons[ie].vetoid_tight)
	//	sfvalues = Electron_SF(velectrons[ie].pt,velectrons[ie].eta,"reco");
	//else
	//	sfvalues = Electron_SF(velectrons[ie].pt,velectrons[ie].eta,"Tight");
      leptonsf_weight *= sfvalues[0];
      leptonsf_weight_stat *= (sfvalues[0] + sqrt(sfvalues[1]*sfvalues[1] + sfvalues[2]*sfvalues[2]));  // like this for time being
      leptonsf_weight_syst *= (sfvalues[0] + sqrt(sfvalues[3]*sfvalues[3] + sfvalues[4]*sfvalues[4] + sfvalues[5]*sfvalues[5] + sfvalues[6]*sfvalues[6]));  // like this for time being
    }
    
    for(int im=0; im<(int)vmuons.size(); im++) {
      float *sfvalues;
      //      if( LJets.size() > 0 && (delta2R(vmuons[im].p4,LJets[min(0,int(LJets.size()-1))].p4)<0.7 || delta2R(vmuons[im].p4,LJets[min(1,int(LJets.size()-1))].p4)<0.7))
	sfvalues = Muon_SF(vmuons[im].pt,vmuons[im].eta,"reco");
	//      else if(!vmuons[im].vetoid_tight)
	//	sfvalues = Muon_SF(vmuons[im].pt,vmuons[im].eta,"reco");
	//else
	//sfvalues = Muon_SF(vmuons[im].pt,vmuons[im].eta,"Tight");
      leptonsf_weight *= sfvalues[0];
      leptonsf_weight_stat *= sfvalues[1];
      leptonsf_weight_syst *= sfvalues[2];
    }
    weight *= leptonsf_weight;
  }

  if(isnan(weight)){
    str = TString::Format("check for evt %d weight %f btagsf %f leptonsf %f",ievt,weight,btag_SF,leptonsf_weight);          
    if(gProofServ) gProofServ->SendAsynMessage(str);
  }
  
  std::vector<std::pair<int,TLorentzVector> > TrigRefObj;
  
  for (int tr=0; tr<ntrigobjs; tr++) {
    TLorentzVector trigobj;
    trigobj.SetPtEtaPhiM(trigobjpt[tr],trigobjeta[tr],trigobjphi[tr],trigobjmass[tr]);
    TrigRefObj.push_back(std::make_pair(trigobjpdgId[tr],trigobj));
  }
  
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


  bool trig_threshold_pass[3] = {false,false,false};
  bool trig_matching_pass[3] = {false,false,false};
  
  vector<TH1D*> hists1, hists2, hists3;
  hists1.push_back(hist_init[0]);
  hists1.push_back(hist_init[1]);

  hists2.push_back(hist_init[2]);
  hists2.push_back(hist_init[3]);

  hists3.push_back(hist_init[4]);
  hists3.push_back(hist_init[5]);
  
  vector<TH2D*> hist2d_prptangle1, hist2d_prptangle2, hist2d_prptangle3;
  hist2d_prptangle1.push_back(hist_prptangle[0]);
  hist2d_prptangle1.push_back(hist_prptangle[1]);
  
  hist2d_prptangle2.push_back(hist_prptangle[2]);
  hist2d_prptangle2.push_back(hist_prptangle[3]);

  hist2d_prptangle3.push_back(hist_prptangle[4]);
  hist2d_prptangle3.push_back(hist_prptangle[5]);
	 
  if((int)vleptons.size() > 1){
    Match_trigger(vsinglelep_trig,vdoublelep_trig[0],		
		  TrigRefObj,
		  vleptons[0],vleptons[1],Jets,
		  trig_threshold_pass[0],
		  trig_matching_pass[0],
		  hists1,hist2d_prptangle1
		  );
    
    Match_trigger(vsinglelep_trig,vdoublelep_trig[1],		
		  TrigRefObj,
		  vleptons[0],vleptons[1],Jets,
		  trig_threshold_pass[1],
		  trig_matching_pass[1],
		  hists2,hist2d_prptangle2
		  );
    
    Match_trigger(vsinglelep_trig,vdoublelep_trig[2],		
		  TrigRefObj,
		  vleptons[0],vleptons[1],Jets,
		  trig_threshold_pass[2],
		  trig_matching_pass[2],
		  hists3,hist2d_prptangle3
		  );	   
  }
  
  bool object_pass[nchannel] = {false};

  float inv_mass = -100;
  if((int)vleptons.size() >1)
    inv_mass = (vleptons[0].p4 + vleptons[1].p4).M();
  
  object_pass[0] =    (((int)vleptons.size() >1 && vleptons[1].pt > 10 && vleptons[0].pt > 10 && abs(vleptons[0].pdgId) == 11 && abs(vleptons[1].pdgId) == 11 && vleptons[0].pdgId*vleptons[1].pdgId <0)) && inv_mass > 20 && (inv_mass < 76 || inv_mass > 106) && chs_met >200 && (int)LJets.size()>-1 && (int)BJets.size()>0;

  object_pass[1] =    ( ((int)vleptons.size() >1 && vleptons[0].pt > 10  && vleptons[0].pt > 10 && abs(vleptons[0].pdgId) == 13 && abs(vleptons[1].pdgId) == 11 && vleptons[0].pdgId*vleptons[1].pdgId <0)
		     || ((int)vleptons.size() >1 && vleptons[0].pt > 10  && vleptons[0].pt > 10 && abs(vleptons[1].pdgId) == 13 && abs(vleptons[0].pdgId) == 11 && vleptons[0].pdgId*vleptons[1].pdgId <0)) && inv_mass > 20 && chs_met > 200 && (int)LJets.size()>-1 && (int)BJets.size()>0;
    //             || ((int)vmuons.size() >0 && vmuons[0].pt > 53)
    /*             || ((int)velectrons.size() > 0 && velectrons[0].pt > 53 && (int)Jets.size() > 0 && Jets[0].pt > 180)*/

  object_pass[2] =    (((int)vleptons.size() >1 && vleptons[1].pt > 10 && vleptons[0].pt > 10 && abs(vleptons[0].pdgId) == 13 && abs(vleptons[1].pdgId) == 13 && vleptons[0].pdgId*vleptons[1].pdgId <0)) && inv_mass > 20 && (inv_mass < 76 || inv_mass > 106) && chs_met > 200 && (int)LJets.size()>-1 && (int)BJets.size()>0; 
  

  
  for(int ich =0; ich < nchannel; ich++)   
    {
      if(object_pass[ich]){
	hist_count[ich]->Fill(2.0,weight);
	hist_ortho_trig_2d[0][ich]->Fill(vleptons[0].pt,vleptons[1].pt, weight);
	hist_ortho_trig_2d[1][ich]->Fill(vleptons[0].eta,vleptons[1].eta, weight);
	hist_ortho_trig_2d[2][ich]->Fill(vleptons[0].phi,vleptons[1].phi, weight);
	hist_ortho_trig[0][ich]->Fill((vleptons[0].p4 + vleptons[1].p4).Pt(), weight);
	hist_ortho_trig[1][ich]->Fill((vleptons[0].p4 + vleptons[1].p4).M(), weight);
	hist_ortho_trig[2][ich]->Fill((vleptons[0].p4 + vleptons[1].p4).Phi(), weight);
	hist_ortho_trig[3][ich]->Fill((vleptons[0].p4 + vleptons[1].p4).Eta(), weight);
	hist_ortho_trig[4][ich]->Fill(Jets.size(), weight);
      }
    }

  bool trig_check[nchannel] = {false};
  
  trig_check[0] =    hlt_DoubleEle25 && trig_matching_pass[0];

  trig_check[1] =    ( hlt_Mu37Ele27   || hlt_Mu27Ele37 ) && trig_matching_pass[1]
    //            || (hlt_Mu50 && (int)vmuons.size() >0 && vmuons[].pt > 53)
    /*            || (hlt_Ele50_PFJet165 && (int)velectrons.size() > 0 && velectrons[0].pt > 53 && (int)Jets.size() > 0 && Jets[0].pt > 180) )*/ ;

  trig_check[2] =    hlt_Mu37TkMu27 && trig_matching_pass[2];

  if(isnan(vleptons[0].pt) || isnan(vleptons[0].eta) || isnan(vleptons[0].phi) || isnan(vleptons[1].pt) || isnan(vleptons[1].eta) || isnan(vleptons[1].phi)){
    str = TString::Format("check for evt",ievt);          
    if(gProofServ) gProofServ->SendAsynMessage(str);
  }
  
  for(int ich =0; ich < nchannel; ich++)   
    {
      if(object_pass[ich] && trig_check[ich]){
	hist_count[ich]->Fill(3,weight);
	hist_trig_eff_2d[0][ich]->Fill(vleptons[0].pt,vleptons[1].pt, weight);
	hist_trig_eff_2d[1][ich]->Fill(vleptons[0].eta,vleptons[1].eta, weight);
	hist_trig_eff_2d[2][ich]->Fill(vleptons[0].phi,vleptons[1].phi, weight);
	hist_trig_eff[0][ich]->Fill((vleptons[0].p4 + vleptons[1].p4).Pt(), weight);
	hist_trig_eff[1][ich]->Fill((vleptons[0].p4 + vleptons[1].p4).M(), weight);
	hist_trig_eff[2][ich]->Fill((vleptons[0].p4 + vleptons[1].p4).Phi(), weight);
	hist_trig_eff[3][ich]->Fill((vleptons[0].p4 + vleptons[1].p4).Eta(), weight);
	hist_trig_eff[4][ich]->Fill(Jets.size(), weight);
      }
    }

  if((int)BJets.size()==2){
    if(object_pass[0]) hist_count[0]->Fill(4.0,weight);
    if(object_pass[1]) hist_count[1]->Fill(4.0,weight);
    if(object_pass[2]) hist_count[2]->Fill(4.0,weight);
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
