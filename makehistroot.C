#include "plotscripts.h"
#include "filesystem.h"

void makehistroot()
{

  string ee_chnl_toptaggercut1 = "0.75"; string ee_chnl_toptaggercut2 = "0.88";
  string emu_chnl_eltoptaggercut1 = "0.8"; string emu_chnl_mutoptaggercut2 = "0.5";
  string mumu_chnl_toptaggercut1 = "0.33"; string mumu_chnl_toptaggercut2 = "0.9";

  string ee_chnl_toptaggercut1_sys = "0.7"; string ee_chnl_toptaggercut2_sys = "0.7";
  string emu_chnl_eltoptaggercut1_sys = "0.7"; string emu_chnl_mutoptaggercut2_sys = "0.4";
  string mumu_chnl_toptaggercut1_sys = "0.4"; string mumu_chnl_toptaggercut2_sys = "0.4";
  
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////////// For ee trigger hisotgram ///////////////////////////////////////////////////////////////////////////

  //string el_combine_filename = "combine_eltop.root";
  
  const int nvar_ee = 4;
  
  string weight_ee = "total_weight_ee";
  string event_selection_cuts_ee = "n_electrons>1 && nak8jets>1 && ak8jet1_pt > 300 && ak8jet2_pt > 300 && lep1_pdg*lep2_pdg == -121 && deltaRlep1 < 0.7 && deltaRlep2 < 0.7 && lepmatchwithak8 == 1 && isttbar_ee ==1 && jet_trigger_unprescale == 1 && min(ak8jet1_toptagscore,ak8jet2_toptagscore) > " + ee_chnl_toptaggercut1 + " && max(ak8jet1_toptagscore,ak8jet2_toptagscore) > " + ee_chnl_toptaggercut2;

  string trigcut_ee[nvar_ee] = {""," && ee_trigger == 1 && trig_matching_pass_ee == 1",""," && ee_trigger == 1 && trig_matching_pass_ee == 1"};
    
  TH2D *hist_basic_ee[nvar_ee];
  string histname_ee[nvar_ee] = {"ee_hist_orthotrig_lepptvsleppt","ee_hist_trig_eff_lepptvsleppt","ee_hist_orthotrig_metvsinvmass","ee_hist_trig_eff_metvsinvmass"};
  const int nbins_ee = 5;
  float binsx_ee[nbins_ee+1] = {10,30,70,120,300,500};
  float binsy_ee[nbins_ee+1] = {10,30,70,120,300,500};
  //int nbins_eltop[nvar_eltop] = {25,25,25,25,25,25};
  //  float binlow_eltop[nvar_eltop] = {0,0,0,0,0,0};
  //float binup_eltop[nvar_eltop] = {250,250,250,250,250,250};

  string var_ee[nvar_ee] = {"min(lep2_pt,499.9):min(lep1_pt,499.9)","min(lep2_pt,499.9):min(lep1_pt,499.9)","min(invmll,499.9):min(chs_met,499.9)","min(invmll,499.9):min(chs_met,499.9)"};
  string vartitlex_ee[nvar_ee] = {"Leading Electron p_{T} (in GeV)","Leading Electron p_{T} (in GeV)","MET","MET"};
  string vartitley_ee[nvar_ee] = {"Subleading Electron p_{T} (in GeV)","Subleading Electron p_{T} (in GeV)","Inv. mass of dilep","Inv. mass of dilep"};
  
  for(int iv=0; iv<nvar_ee; iv++)
    {
      string evt_sel = event_selection_cuts_ee + trigcut_ee[iv];
      hist_basic_ee[iv] = new TH2D(histname_ee[iv].c_str(),histname_ee[iv].c_str(),nbins_ee,binsx_ee,nbins_ee,binsy_ee);
      makerootfile(var_ee[iv],weight_ee,evt_sel,hist_basic_ee[iv],histname_ee[iv],vartitlex_ee[iv],vartitley_ee[iv],jettriggfiles);
      makerootfile(var_ee[iv],weight_ee,evt_sel,hist_basic_ee[iv],histname_ee[iv] + "_lumiweighted0",vartitlex_ee[iv],vartitley_ee[iv],{jethtdataA,allsample});
      makerootfile(var_ee[iv],weight_ee,evt_sel,hist_basic_ee[iv],histname_ee[iv] + "_lumiweighted1",vartitlex_ee[iv],vartitley_ee[iv],{jethtdataB,allsample});
      makerootfile(var_ee[iv],weight_ee,evt_sel,hist_basic_ee[iv],histname_ee[iv] + "_lumiweighted2",vartitlex_ee[iv],vartitley_ee[iv],{jethtdataC,allsample});
      makerootfile(var_ee[iv],weight_ee,evt_sel,hist_basic_ee[iv],histname_ee[iv] + "_lumiweighted3",vartitlex_ee[iv],vartitley_ee[iv],{jethtdataD,allsample});

    }
  
  for(int ifile=0;ifile<jettriggfiles.size();ifile++)
    {
      for(int iv=0; iv<nvar_ee; iv++)
	{
	  savehist2d(jettriggfiles[ifile].out_name,histname_ee[iv], jettriggfiles[ifile].out_name.substr(30,9) + "_" + histname_ee[iv]);
	}
    }
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////////// For ee trigger sysmatics hisotgram ///////////////////////////////////////////////////////////////////////////

  const int nweightsys_ee = 3;

  string sys_selection_ee[nweightsys_ee] = {"n_electrons>1 && nak8jets>1 && ak8jet1_pt > 300 && ak8jet2_pt > 300 && lep1_pdg*lep2_pdg == -121 && deltaRlep1 < 0.7 && deltaRlep2 < 0.7 && lepmatchwithak8 == 1 && isttbar_ee ==1 && jet_trigger_unprescale == 1 && min(ak8jet1_toptagscore,ak8jet2_toptagscore) > " + ee_chnl_toptaggercut1_sys + " && max(ak8jet1_toptagscore,ak8jet2_toptagscore) > " + ee_chnl_toptaggercut2_sys, " && nprimi > 30"," && nprimi < 30"};
      
  string histname_sys_ee[nweightsys_ee] = {"toptaggersys","primivertexUp","primivertexDown"};

  for(int iv=0; iv<2; iv++)
    {
      for(int isys = 0; isys < nweightsys_ee; isys++)
	{
	  string evt_sel;
	  if(isys == 0)
	    evt_sel = sys_selection_ee[isys] + trigcut_ee[iv];
	  else
	    evt_sel = event_selection_cuts_ee + trigcut_ee[iv] + sys_selection_ee[isys];
	  string hname = histname_ee[iv] + "_" + histname_sys_ee[isys];
	  cout<<evt_sel<<" with hist name = "<<hname<<endl<<endl<<endl;
	  hist_basic_ee[iv] = new TH2D(hname.c_str(),hname.c_str(),nbins_ee,binsx_ee,nbins_ee,binsy_ee);
	  makerootfile(var_ee[iv],weight_ee,evt_sel,hist_basic_ee[iv],hname,vartitlex_ee[iv],vartitley_ee[iv],jettriggfiles);
	}
    }
  makelumiweightedsf(lumiweightedjethtdata,"combined_Trig_Eff_Summer20UL18_AllSample_output.root",histname_ee[0],histname_ee[1]);
  calculatetriggsf("combined_Trig_Eff_Summer20UL18_JetHT_2018_output.root","combined_Trig_Eff_Summer20UL18_AllSample_output.root",histname_ee[0],histname_ee[1],histname_sys_ee,nweightsys_ee);
 

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////////// For emu hisotgram ///////////////////////////////////////////////////////////////////////////

  const int nvar_emu = 4;
  
  string weight_emu = "total_weight_emu";
  string event_selection_cuts_emu = "n_electrons>0 && n_muons > 0 && nak8jets>1 && ak8jet1_pt > 300 && ak8jet2_pt > 300 && lep1_pdg*lep2_pdg == -143 && deltaRlep1 < 0.7 && deltaRlep2 < 0.7 && lepmatchwithak8 == 1 && isttbar_emu ==1 && jet_trigger_unprescale == 1 && ( (abs(lep1_pdg) == 11 && abs(lep2_pdg) == 13 && ak8jet1_toptagscore > " + emu_chnl_eltoptaggercut1 + " && ak8jet2_toptagscore > " + emu_chnl_mutoptaggercut2 +" ) || (abs(lep2_pdg) == 11 && abs(lep1_pdg) == 13 && ak8jet2_toptagscore > " + emu_chnl_eltoptaggercut1 + " && ak8jet1_toptagscore > " + emu_chnl_mutoptaggercut2 + ") )";

  string trigcut_emu[nvar_emu] = {""," && emu_trigger == 1 && trig_matching_pass_emu == 1",""," && emu_trigger == 1 && trig_matching_pass_emu == 1"};
    
  TH2D *hist_basic_emu[nvar_emu];
  string histname_emu[nvar_emu] = {"emu_hist_orthotrig_lepptvsleppt","emu_hist_trig_eff_lepptvsleppt","emu_hist_orthotrig_metvsinvmass","emu_hist_trig_eff_metvsinvmass"};
  const int nbins_emu = 5;
  float binsx_emu[nbins_emu+1] = {10,30,70,120,300,500};
  float binsy_emu[nbins_emu+1] = {10,30,70,120,300,500};
  //int nbins_eltop[nvar_eltop] = {25,25,25,25,25,25};
  //  float binlow_eltop[nvar_eltop] = {0,0,0,0,0,0};
  //float binup_eltop[nvar_eltop] = {250,250,250,250,250,250};

  string var_emu[nvar_emu] = {"min( abs(lep2_pdg) == 13 ? lep2_pt : lep1_pt,499.9):min( abs(lep2_pdg) == 11 ? lep2_pt : lep1_pt ,499.9)","min( abs(lep2_pdg) == 13 ? lep2_pt : lep1_pt,499.9):min( abs(lep2_pdg) == 11 ? lep2_pt : lep1_pt ,499.9)","min(invmll,499.9):min(chs_met,499.9)","min(invmll,499.9):min(chs_met,499.9)"};
  string vartitlex_emu[nvar_emu] = {"Leading Electron p_{T} (in GeV)","Leading Electron p_{T} (in GeV)","MET","MET"};
  string vartitley_emu[nvar_emu] = {"Leading Muon p_{T} (in GeV)","Leading Muon p_{T} (in GeV)","Inv. mass of dilep","Inv. mass of dilep"};
  
  for(int iv=0; iv<nvar_emu; iv++)
    {
      string evt_sel = event_selection_cuts_emu + trigcut_emu[iv];
      hist_basic_emu[iv] = new TH2D(histname_emu[iv].c_str(),histname_emu[iv].c_str(),nbins_emu,binsx_emu,nbins_emu,binsy_emu);
      makerootfile(var_emu[iv],weight_emu,evt_sel,hist_basic_emu[iv],histname_emu[iv],vartitlex_emu[iv],vartitley_emu[iv],jettriggfiles);
      makerootfile(var_emu[iv],weight_emu,evt_sel,hist_basic_emu[iv],histname_emu[iv]+ "_lumiweighted0",vartitlex_emu[iv],vartitley_emu[iv],{jethtdataA,allsample});
      makerootfile(var_emu[iv],weight_emu,evt_sel,hist_basic_emu[iv],histname_emu[iv]+ "_lumiweighted1",vartitlex_emu[iv],vartitley_emu[iv],{jethtdataB,allsample});
      makerootfile(var_emu[iv],weight_emu,evt_sel,hist_basic_emu[iv],histname_emu[iv]+ "_lumiweighted2",vartitlex_emu[iv],vartitley_emu[iv],{jethtdataC,allsample});
      makerootfile(var_emu[iv],weight_emu,evt_sel,hist_basic_emu[iv],histname_emu[iv]+ "_lumiweighted3",vartitlex_emu[iv],vartitley_emu[iv],{jethtdataD,allsample});
    }
  
  for(int ifile=0;ifile<jettriggfiles.size();ifile++)
    {
      for(int iv=0; iv<nvar_emu; iv++)
	{
	  savehist2d(jettriggfiles[ifile].out_name,histname_emu[iv], jettriggfiles[ifile].out_name.substr(30,9) + "_" + histname_emu[iv]);
	}
    }
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////////// For emu trigger sysmatics hisotgram ///////////////////////////////////////////////////////////////////////////

  const int nweightsys_emu = 3;

  string sys_selection_emu[nweightsys_emu] = {"n_electrons>0 && n_muons > 0 && nak8jets>1 && ak8jet1_pt > 300 && ak8jet2_pt > 300 && lep1_pdg*lep2_pdg == -143 && deltaRlep1 < 0.7 && deltaRlep2 < 0.7 && lepmatchwithak8 == 1 && isttbar_emu ==1 && jet_trigger_unprescale == 1 && ( (abs(lep1_pdg) == 11 && abs(lep2_pdg) == 13 && ak8jet1_toptagscore > " + emu_chnl_eltoptaggercut1_sys + " && ak8jet2_toptagscore > " + emu_chnl_mutoptaggercut2_sys +" ) || (abs(lep2_pdg) == 11 && abs(lep1_pdg) == 13 && ak8jet2_toptagscore > " + emu_chnl_eltoptaggercut1_sys + " && ak8jet1_toptagscore > " + emu_chnl_mutoptaggercut2_sys + ") )", " && nprimi > 30"," && nprimi < 30"};
      
  string histname_sys_emu[nweightsys_emu] = {"toptaggersys","primivertexUp","primivertexDown"};

  for(int iv=0; iv<2; iv++)
    {
      for(int isys = 0; isys < nweightsys_emu; isys++)
	{
	  string evt_sel;
	  if(isys == 0)
	    evt_sel = sys_selection_emu[isys] + trigcut_emu[iv];
	  else
	    evt_sel = event_selection_cuts_emu + trigcut_emu[iv] + sys_selection_emu[isys];
	  string hname = histname_emu[iv] + "_" + histname_sys_emu[isys];
	  cout<<evt_sel<<" with hist name = "<<hname<<endl<<endl<<endl;
	  hist_basic_emu[iv] = new TH2D(hname.c_str(),hname.c_str(),nbins_emu,binsx_emu,nbins_emu,binsy_emu);
	  makerootfile(var_emu[iv],weight_emu,evt_sel,hist_basic_emu[iv],hname,vartitlex_emu[iv],vartitley_emu[iv],jettriggfiles);
	}
    }
  makelumiweightedsf(lumiweightedjethtdata,"combined_Trig_Eff_Summer20UL18_AllSample_output.root",histname_emu[0],histname_emu[1]);
  calculatetriggsf("combined_Trig_Eff_Summer20UL18_JetHT_2018_output.root","combined_Trig_Eff_Summer20UL18_AllSample_output.root",histname_emu[0],histname_emu[1],histname_sys_emu,nweightsys_emu);

  
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////////// For mumu trigger hisotgram ///////////////////////////////////////////////////////////////////////////

  //string el_combine_filename = "combine_eltop.root";
  
  const int nvar_mumu = 4;
  
  string weight_mumu = "total_weight_mumu";
  string event_selection_cuts_mumu = "n_muons>1 && nak8jets>1 && ak8jet1_pt > 300 && ak8jet2_pt > 300 && lep1_pdg*lep2_pdg == -169 && deltaRlep1 < 0.7 && deltaRlep2 < 0.7 && lepmatchwithak8 == 1 && isttbar_mumu ==1 && jet_trigger_unprescale == 1 && min(ak8jet1_toptagscore,ak8jet2_toptagscore) > " + mumu_chnl_toptaggercut1 + " && max(ak8jet1_toptagscore,ak8jet2_toptagscore) > " + mumu_chnl_toptaggercut2;

  string trigcut_mumu[nvar_mumu] = {""," && mumu_trigger == 1 && trig_matching_pass_mumu == 1",""," && mumu_trigger == 1 && trig_matching_pass_mumu == 1"};
    
  TH2D *hist_basic_mumu[nvar_mumu];
  string histname_mumu[nvar_mumu] = {"mumu_hist_orthotrig_lepptvsleppt","mumu_hist_trig_eff_lepptvsleppt","mumu_hist_orthotrig_metvsinvmass","mumu_hist_trig_eff_metvsinvmass"};
  const int nbins_mumu = 5;
  float binsx_mumu[nbins_mumu+1] = {10,30,70,120,300,500};
  float binsy_mumu[nbins_mumu+1] = {10,30,70,120,300,500};
  //int nbins_eltop[nvar_eltop] = {25,25,25,25,25,25};
  //  float binlow_eltop[nvar_eltop] = {0,0,0,0,0,0};
  //float binup_eltop[nvar_eltop] = {250,250,250,250,250,250};

  string var_mumu[nvar_mumu] = {"min(lep2_pt,499.9):min(lep1_pt,499.9)","min(lep2_pt,499.9):min(lep1_pt,499.9)","min(invmll,499.9):min(chs_met,499.9)","min(invmll,499.9):min(chs_met,499.9)"};
  string vartitlex_mumu[nvar_mumu] = {"Leading Electron p_{T} (in GeV)","Leading Electron p_{T} (in GeV)","MET","MET"};
  string vartitley_mumu[nvar_mumu] = {"Subleading Electron p_{T} (in GeV)","Subleading Electron p_{T} (in GeV)","Inv. mass of dilep","Inv. mass of dilep"};
  
  for(int iv=0; iv<nvar_mumu; iv++)
    {
      string evt_sel = event_selection_cuts_mumu + trigcut_mumu[iv];
      hist_basic_mumu[iv] = new TH2D(histname_mumu[iv].c_str(),histname_mumu[iv].c_str(),nbins_mumu,binsx_mumu,nbins_mumu,binsy_mumu);
      makerootfile(var_mumu[iv],weight_mumu,evt_sel,hist_basic_mumu[iv],histname_mumu[iv],vartitlex_mumu[iv],vartitley_mumu[iv],jettriggfiles);
      makerootfile(var_mumu[iv],weight_mumu,evt_sel,hist_basic_mumu[iv],histname_mumu[iv] + "_lumiweighted0",vartitlex_mumu[iv],vartitley_mumu[iv],{jethtdataA,allsample});
      makerootfile(var_mumu[iv],weight_mumu,evt_sel,hist_basic_mumu[iv],histname_mumu[iv] + "_lumiweighted1",vartitlex_mumu[iv],vartitley_mumu[iv],{jethtdataB,allsample});
      makerootfile(var_mumu[iv],weight_mumu,evt_sel,hist_basic_mumu[iv],histname_mumu[iv] + "_lumiweighted2",vartitlex_mumu[iv],vartitley_mumu[iv],{jethtdataC,allsample});
      makerootfile(var_mumu[iv],weight_mumu,evt_sel,hist_basic_mumu[iv],histname_mumu[iv] + "_lumiweighted3",vartitlex_mumu[iv],vartitley_mumu[iv],{jethtdataD,allsample});

    }
  
  for(int ifile=0;ifile<jettriggfiles.size();ifile++)
    {
      for(int iv=0; iv<nvar_mumu; iv++)
	{
	  savehist2d(jettriggfiles[ifile].out_name,histname_mumu[iv], jettriggfiles[ifile].out_name.substr(30,9) + "_" + histname_mumu[iv]);
	}
    }
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////////// For mumu trigger sysmatics hisotgram ///////////////////////////////////////////////////////////////////////////

  const int nweightsys_mumu = 3;

  string sys_selection_mumu[nweightsys_mumu] = {"n_muons>1 && nak8jets>1 && ak8jet1_pt > 300 && ak8jet2_pt > 300 && lep1_pdg*lep2_pdg == -169 && deltaRlep1 < 0.7 && deltaRlep2 < 0.7 && lepmatchwithak8 == 1 && isttbar_mumu ==1 && jet_trigger_unprescale == 1 && min(ak8jet1_toptagscore,ak8jet2_toptagscore) > " + mumu_chnl_toptaggercut1_sys + " && max(ak8jet1_toptagscore,ak8jet2_toptagscore) > " + mumu_chnl_toptaggercut2_sys, " && nprimi > 30"," && nprimi < 30"};
      
  string histname_sys_mumu[nweightsys_mumu] = {"toptaggersys","primivertexUp","primivertexDown"};

  for(int iv=0; iv<2; iv++)
    {
      for(int isys = 0; isys < nweightsys_mumu; isys++)
	{
	  string evt_sel;
	  if(isys == 0)
	    evt_sel = sys_selection_mumu[isys] + trigcut_mumu[iv];
	  else
	    evt_sel = event_selection_cuts_mumu + trigcut_mumu[iv] + sys_selection_mumu[isys];
	  string hname = histname_mumu[iv] + "_" + histname_sys_mumu[isys];
	  cout<<evt_sel<<" with hist name = "<<hname<<endl<<endl<<endl;
	  hist_basic_mumu[iv] = new TH2D(hname.c_str(),hname.c_str(),nbins_mumu,binsx_mumu,nbins_mumu,binsy_mumu);
	  makerootfile(var_mumu[iv],weight_mumu,evt_sel,hist_basic_mumu[iv],hname,vartitlex_mumu[iv],vartitley_mumu[iv],jettriggfiles);
	}
    }
  makelumiweightedsf(lumiweightedjethtdata,"combined_Trig_Eff_Summer20UL18_AllSample_output.root",histname_mumu[0],histname_mumu[1]);

  calculatetriggsf("combined_Trig_Eff_Summer20UL18_JetHT_2018_output.root","combined_Trig_Eff_Summer20UL18_AllSample_output.root",histname_mumu[0],histname_mumu[1],histname_sys_mumu,nweightsys_mumu);
  

  /*  string event_selection_cuts_ee = "n_electrons>1 && nak8jets>1 && ak8jet1_pt > 300 && ak8jet2_pt > 300 && lep1_pdg*lep2_pdg == -121 && deltaRlep1 < 0.7 && deltaRlep2 < 0.7 && lepmatchwithak8 == 1 && isttbar_ee ==1 && jet_trigger_unprescale == 1 && min(ak8jet1_toptagscore,ak8jet2_toptagscore) > " + ee_chnl_toptaggercut1 + " && max(ak8jet1_toptagscore,ak8jet2_toptagscore) > " + ee_chnl_toptaggercut2;
  string event_selection_cuts_emu = "n_electrons>0 && n_muons > 0 && nak8jets>1 && ak8jet1_pt > 300 && ak8jet2_pt > 300 && lep1_pdg*lep2_pdg == -143 && deltaRlep1 < 0.7 && deltaRlep2 < 0.7 && lepmatchwithak8 == 1 && isttbar_emu ==1 && jet_trigger_unprescale == 1 && ( (abs(lep1_pdg) == 11 && abs(lep2_pdg) == 13 && ak8jet1_toptagscore > " + emu_chnl_eltoptaggercut1 + " && ak8jet2_toptagscore > " + emu_chnl_mutoptaggercut2 +" ) || (abs(lep2_pdg) == 11 && abs(lep1_pdg) == 13 && ak8jet2_toptagscore > " + emu_chnl_eltoptaggercut1 + " && ak8jet1_toptagscore > " + emu_chnl_mutoptaggercut2 + ") )";
  string event_selection_cuts_mumu = "n_muons>1 && nak8jets>1 && ak8jet1_pt > 300 && ak8jet2_pt > 300 && lep1_pdg*lep2_pdg == -169 && deltaRlep1 < 0.7 && deltaRlep2 < 0.7 && lepmatchwithak8 == 1 && isttbar_mumu ==1 && jet_trigger_unprescale == 1 && min(ak8jet1_toptagscore,ak8jet2_toptagscore) > " + mumu_chnl_toptaggercut1 + " && max(ak8jet1_toptagscore,ak8jet2_toptagscore) > " + mumu_chnl_toptaggercut2;*/

  const int nvar = 6;
  const int nvarplot = 6;

  string weight[3] ={" total_weight_ee","total_weight_emu","total_weight_mumu"};
  string event_selection_cuts[3] = {event_selection_cuts_ee,event_selection_cuts_emu,event_selection_cuts_mumu};

  TH1D *hist_basic[nvar][3];
  string histname[nvar] = {"lep1_pt","lep2_pt","invmll","chs_met","ak8jet1_pt","ak8jet2_pt"};
  int nbins[nvar] = {30,30,30,30,30,30};
  float binlow[nvar] = {0,0,0,0,200,200};
  float binup[nvar] = {750,750,750,750,1700,1700};

  string var[nvar] = {"abs(lep1_pdg) == 11 ? lep1_pt : lep2_pt","abs(lep1_pdg) == 13 ? lep1_pt : lep2_pt","invmll","chs_met","ak8jet1_pt","ak8jet2_pt"};
  string vartitle[nvar] = {"Leading electron p_{T} (in GeV)","Leading muon p_{T} (in GeV)","inv mass of dilep (in GeV)","chs_met (in GeV)","Leading AK8 jet p_{T} (in GeV)","subleading AK8 p_{T} (in GeV)"};
  
  for(int iv=0; iv<nvarplot; iv++)
    {
      string hname[3];
      hname[0] = "hist_ee_" + histname[iv];
      hname[1] = "hist_emu_" + histname[iv];
      hname[2] = "hist_mumu_" + histname[iv];
      hist_basic[iv][0] = new TH1D(hname[0].c_str(),hname[0].c_str(),nbins[iv],binlow[iv],binup[iv]);
      hist_basic[iv][1] = new TH1D(hname[1].c_str(),hname[1].c_str(),nbins[iv],binlow[iv],binup[iv]);
      hist_basic[iv][2] = new TH1D(hname[2].c_str(),hname[2].c_str(),nbins[iv],binlow[iv],binup[iv]);
      if(iv==1)makerootfile(var[iv],weight[0],event_selection_cuts[0],hist_basic[iv][0],hname[0],"Subleading electron p_{T} (in GeV)",jettriggfiles);
      else makerootfile(var[iv],weight[0],event_selection_cuts[0],hist_basic[iv][0],hname[0],vartitle[iv],jettriggfiles);
      makerootfile(var[iv],weight[1],event_selection_cuts[1],hist_basic[iv][1],hname[1],vartitle[iv],jettriggfiles);
      if(iv==0)makerootfile(var[iv],weight[2],event_selection_cuts[2],hist_basic[iv][2],hname[2],"Subleading muon p_{T} (in GeV)",jettriggfiles);
      else makerootfile(var[iv],weight[2],event_selection_cuts[2],hist_basic[iv][2],hname[2],vartitle[iv],jettriggfiles);
    }

}
  
