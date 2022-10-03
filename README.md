# Trigger-scale-factor

BTagCalibrationStandalone.h
Functions.h
Objects.h
Trigg_eff_cal_getobjects.h
Trigg_eff_cal.h
BTagCalibrationStandalone.cpp
Trigg_eff_cal.C

These files are used to run on crab output files and make trees with selected events only. In the selected events, only useful variables for 
analysis are stored.

After getting output from offline code. 
do "root -l -b -q makehistroot.C" to get the plots. This will use following files for mentioned reasons.

My_Style.C : Contains plotting style for CMS

filesystem.h : Contains info regarding offline output root files. The info for each MC/Data Sample is stored in structure named "fileinfo".
It contains variable "in_names" for each MC samples for the process and their corresponding cross section weights are stored in vector weights.
The variables treename will have name of ttree to extract all variables and make plots.
All the plots will be in saved in root file whose name is stored in out_name.

plotscripts.h : It contians all function for plotting the variables and calculation of SF. Presently, it is completed but many new function will be added
for different uses.

makehistroot.C : This is main code which is run by root -l -b -q makehistroot.C to get all the plots in ./plots folder. This files will contain 
analysis specific information like which variables to plot and what event selection are to be applied.
