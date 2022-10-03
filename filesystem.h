#ifndef FILESYSTEM_H
#define FILESYSTEM_H

struct fileinfo {
  string out_name;
  vector<string> in_names;
  vector<float> weights;
  string treename;
};

fileinfo ttbar_signal = {
"combined_Trig_Eff_Summer20UL18_TTBar_Signal_output.root",
{"Trig_Eff_Summer20UL18_TTBar_DiLeptonic_signal_output.root" },
{0.000008392346225},
"newvars"};

fileinfo ttbar = {
"combined_Trig_Eff_Summer20UL18_TTBar_nonSignal_output.root",
{"Trig_Eff_Summer20UL18_TTBar_DiLeptonic_non-signal_output.root","Trig_Eff_Summer20UL18_TTBar_Hadronic_non-signal_output.root","Trig_Eff_Summer20UL18_TTBar_SemiLeptonic_non-signal_output.root"},
{0.000008392346225,0.00000350989933,0.000002535797824},
"newvars"};

fileinfo qcd = {
"combined_Trig_Eff_Summer20UL18_QCD_output.root",
{"Trig_Eff_Summer20UL18_QCD_MG_100to200_output.root","Trig_Eff_Summer20UL18_QCD_MG_200to300_output.root","Trig_Eff_Summer20UL18_QCD_MG_300to500_output.root","Trig_Eff_Summer20UL18_QCD_MG_500to700_output.root","Trig_Eff_Summer20UL18_QCD_MG_700to1000_output.root","Trig_Eff_Summer20UL18_QCD_MG_1000to1500_output.root","Trig_Eff_Summer20UL18_QCD_MG_1500to2000_output.root","Trig_Eff_Summer20UL18_QCD_MG_2000toInf_output.root"},
{279.569398710166,27.0047762342897,5.25723336464978,0.617883220356092,0.132911785406591,0.0772537011545917,0.00993655692414248,0.00409219364282649},
"newvars"};

fileinfo dyjets = {
"combined_Trig_Eff_Summer20UL18_DYJets_output.root",
{"Trig_Eff_Summer20UL18_DYJetsToLL_M-10to50_output.root","Trig_Eff_Summer20UL18_DYJetsToLL_M-50_HT-70to100_output.root","Trig_Eff_Summer20UL18_DYJetsToLL_M-50_HT-100To200_output.root","Trig_Eff_Summer20UL18_DYJetsToLL_M-50_HT-200To400_output.root","Trig_Eff_Summer20UL18_DYJetsToLL_M-50_HT-400To600_output.root","Trig_Eff_Summer20UL18_DYJetsToLL_M-50_HT-600To800_output.root","Trig_Eff_Summer20UL18_DYJetsToLL_M-50_HT-800To1200_output.root","Trig_Eff_Summer20UL18_DYJetsToLL_M-50_HT-1200To2500_output.root","Trig_Eff_Summer20UL18_DYJetsToLL_M-50_HT-2500toInf_output.root"},
{0.160045074494486,0.00823520256157152,0.00535634323707072,0.00208387002202267,0.000585764320786333,0.000180040709678952,0.000085206186385,0.000021641791044776,0.00000151046020238245},
"newvars"};

fileinfo wjets = {
"combined_Trig_Eff_Summer20UL18_WJets_output.root",
{"Trig_Eff_Summer20UL18_WJetsToLNu_HT-70to100_output.root","Trig_Eff_Summer20UL18_WJetsToLNu_HT-100To200_output.root","Trig_Eff_Summer20UL18_WJetsToLNu_HT-200To400_output.root","Trig_Eff_Summer20UL18_WJetsToLNu_HT-400To600_output.root","Trig_Eff_Summer20UL18_WJetsToLNu_HT-600To800_output.root","Trig_Eff_Summer20UL18_WJetsToLNu_HT-800To1200_output.root","Trig_Eff_Summer20UL18_WJetsToLNu_HT-1200To2500_output.root","Trig_Eff_Summer20UL18_WJetsToLNu_HT-2500toInf_output.root"},
{0.0191326739569619,0.024355693418915,0.000006191206795345,0.00608026596190775,0.00142323531459009,0.000674262942555235,0.000178573192475046,0.00000125076056542},
"newvars"};

fileinfo stop = {
"combined_Trig_Eff_Summer20UL18_ST_output.root",
{"Trig_Eff_Summer20UL18_ST_t4f_antitop_output.root","Trig_Eff_Summer20UL18_ST_t4f_top_output.root","Trig_Eff_Summer20UL18_ST_s_lep_output.root","Trig_Eff_Summer20UL18_ST_tW_antitop_output.root","Trig_Eff_Summer20UL18_ST_tW_top_output.root"},
{0.000011088168322084,0.000005969929245283,0.00005161536156931,0.00009081462196423,0.000088769860375541},
"newvars"};

fileinfo ttx = {
"combined_Trig_Eff_Summer20UL18_TTX_output.root",
{"Trig_Eff_Summer20UL18_TTGJets_output.root","Trig_Eff_Summer20UL18_TTZToQQ_output.root","Trig_Eff_Summer20UL18_TTZToLLNuNu_output.root","Trig_Eff_Summer20UL18_TTWJetsToQQ_output.root","Trig_Eff_Summer20UL18_TTWJetsToLNu_output.root"},
{0.000101151663305643,0.0000050487795244490,0.000051017846833504,0.00066612998149046,0.00006127986648425},
"newvars"};

fileinfo dib = {
"combined_Trig_Eff_Summer20UL18_DiBoson_output.root",
{"Trig_Eff_Summer20UL18_WW_output.root","Trig_Eff_Summer20UL18_WZ_output.root","Trig_Eff_Summer20UL18_ZZ_output.root"},
{0.00484370842023713,0.00347468025193574,0.00344134660204614},
"newvars"};

fileinfo jethtdata = {
"combined_Trig_Eff_Summer20UL18_JetHT_2018_output.root",
{"Trig_Eff_Summer20UL18_JetHT_2018A_output.root","Trig_Eff_Summer20UL18_JetHT_2018B_output.root","Trig_Eff_Summer20UL18_JetHT_2018C_output.root","Trig_Eff_Summer20UL18_JetHT_2018D_output.root"},
{1,1,1,1},
"newvars"};

fileinfo lumiweightedjethtdata = {
"combined_Trig_Eff_Summer20UL18_lumiweightedJetHT_2018_output.root",
{"combined_Trig_Eff_Summer20UL18_JetHT_2018A_output.root","combined_Trig_Eff_Summer20UL18_JetHT_2018B_output.root","combined_Trig_Eff_Summer20UL18_JetHT_2018C_output.root","combined_Trig_Eff_Summer20UL18_JetHT_2018D_output.root"},
{13.52954,6.78216,6.60858,32.03516},
"newvars"};

fileinfo jethtdataA = {
"combined_Trig_Eff_Summer20UL18_JetHT_2018A_output.root",
{"Trig_Eff_Summer20UL18_JetHT_2018A_output.root"},
{1},
"newvars"};

fileinfo jethtdataB = {
"combined_Trig_Eff_Summer20UL18_JetHT_2018B_output.root",
{"Trig_Eff_Summer20UL18_JetHT_2018B_output.root"},
{1},
"newvars"};

fileinfo jethtdataC = {
"combined_Trig_Eff_Summer20UL18_JetHT_2018C_output.root",
{"Trig_Eff_Summer20UL18_JetHT_2018C_output.root"},
{1},
"newvars"};

fileinfo jethtdataD = {
"combined_Trig_Eff_Summer20UL18_JetHT_2018D_output.root",
{"Trig_Eff_Summer20UL18_JetHT_2018D_output.root"},
{1},
"newvars"};

fileinfo metdata = {
"combined_Trig_Eff_Summer20UL18_MET_2018_output.root",
{"Trig_Eff_Summer20UL18_MET_2018A_output.root","Trig_Eff_Summer20UL18_MET_2018B_output.root","Trig_Eff_Summer20UL18_MET_2018C_output.root","Trig_Eff_Summer20UL18_MET_2018D_output.root"},
{1,1,1,1},
"newvars"};

fileinfo allsample = {
"combined_Trig_Eff_Summer20UL18_AllSample_output.root",
{"Trig_Eff_Summer20UL18_TTBar_DiLeptonic_signal_output.root","Trig_Eff_Summer20UL18_TTBar_DiLeptonic_non-signal_output.root","Trig_Eff_Summer20UL18_TTBar_Hadronic_non-signal_output.root","Trig_Eff_Summer20UL18_TTBar_SemiLeptonic_non-signal_output.root","Trig_Eff_Summer20UL18_QCD_MG_100to200_output.root","Trig_Eff_Summer20UL18_QCD_MG_200to300_output.root","Trig_Eff_Summer20UL18_QCD_MG_300to500_output.root","Trig_Eff_Summer20UL18_QCD_MG_500to700_output.root","Trig_Eff_Summer20UL18_QCD_MG_700to1000_output.root","Trig_Eff_Summer20UL18_QCD_MG_1000to1500_output.root","Trig_Eff_Summer20UL18_QCD_MG_1500to2000_output.root","Trig_Eff_Summer20UL18_QCD_MG_2000toInf_output.root","Trig_Eff_Summer20UL18_DYJetsToLL_M-10to50_output.root","Trig_Eff_Summer20UL18_DYJetsToLL_M-50_HT-70to100_output.root","Trig_Eff_Summer20UL18_DYJetsToLL_M-50_HT-100To200_output.root","Trig_Eff_Summer20UL18_DYJetsToLL_M-50_HT-200To400_output.root","Trig_Eff_Summer20UL18_DYJetsToLL_M-50_HT-400To600_output.root","Trig_Eff_Summer20UL18_DYJetsToLL_M-50_HT-600To800_output.root","Trig_Eff_Summer20UL18_DYJetsToLL_M-50_HT-800To1200_output.root","Trig_Eff_Summer20UL18_DYJetsToLL_M-50_HT-1200To2500_output.root","Trig_Eff_Summer20UL18_DYJetsToLL_M-50_HT-2500toInf_output.root","Trig_Eff_Summer20UL18_WJetsToLNu_HT-70to100_output.root","Trig_Eff_Summer20UL18_WJetsToLNu_HT-100To200_output.root","Trig_Eff_Summer20UL18_WJetsToLNu_HT-200To400_output.root","Trig_Eff_Summer20UL18_WJetsToLNu_HT-400To600_output.root","Trig_Eff_Summer20UL18_WJetsToLNu_HT-600To800_output.root","Trig_Eff_Summer20UL18_WJetsToLNu_HT-800To1200_output.root","Trig_Eff_Summer20UL18_WJetsToLNu_HT-1200To2500_output.root","Trig_Eff_Summer20UL18_WJetsToLNu_HT-2500toInf_output.root","Trig_Eff_Summer20UL18_ST_t4f_antitop_output.root","Trig_Eff_Summer20UL18_ST_t4f_top_output.root","Trig_Eff_Summer20UL18_ST_s_lep_output.root","Trig_Eff_Summer20UL18_ST_tW_antitop_output.root","Trig_Eff_Summer20UL18_ST_tW_top_output.root","Trig_Eff_Summer20UL18_TTGJets_output.root","Trig_Eff_Summer20UL18_TTZToQQ_output.root","Trig_Eff_Summer20UL18_TTZToLLNuNu_output.root","Trig_Eff_Summer20UL18_TTWJetsToQQ_output.root","Trig_Eff_Summer20UL18_TTWJetsToLNu_output.root","Trig_Eff_Summer20UL18_WW_output.root","Trig_Eff_Summer20UL18_WZ_output.root","Trig_Eff_Summer20UL18_ZZ_output.root"},
{0.000008392346225,0.000008392346225,0.00000350989933,0.000002535797824,279.569398710166,27.0047762342897,5.25723336464978,0.617883220356092,0.132911785406591,0.0772537011545917,0.00993655692414248,0.00409219364282649,0.160045074494486,0.00823520256157152,0.00535634323707072,0.00208387002202267,0.000585764320786333,0.000180040709678952,0.000085206186385,0.000021641791044776,0.00000151046020238245,0.0191326739569619,0.024355693418915,0.000006191206795345,0.00608026596190775,0.00142323531459009,0.000674262942555235,0.000178573192475046,0.00000125076056542,0.000011088168322084,0.000005969929245283,0.00005161536156931,0.00009081462196423,0.000088769860375541,0.000101151663305643,0.0000050487795244490,0.000051017846833504,0.00066612998149046,0.00006127986648425,0.00484370842023713,0.00347468025193574,0.00344134660204614},
"newvars"};

vector<fileinfo> jettriggfiles{jethtdata,ttbar_signal,ttbar,qcd,stop,dyjets,wjets,dib,ttx,allsample};
vector<fileinfo> lumiweighteddatajettriggfiles{lumiweightedjethtdata,ttbar_signal,ttbar,qcd,stop,dyjets,wjets,dib,ttx,allsample};
vector<fileinfo> mettriggfiles{metdata,ttbar_signal,ttbar,qcd,stop,dyjets,wjets,dib,ttx,allsample};

/*fileinfo ;
.out_name="";
.in_names.assign({""});
.weights.assign({});*/


#endif
