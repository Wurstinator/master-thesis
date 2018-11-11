
from analyze_common import *

general_analysis_v1(name='Path Refinement', input_prefix='path_refinement/', output_prefix='path_refinement/')

# Plot detnbaut special ap1
detnbaut_ap1 = filelines_to_json('raw/path_refinement/detnbaut_special_ap1.json')
title = 'Path Refinement state reduction on a DPA with |Σ|=2 that was created by nbautils from an NBA, using the Safra relation.'
plot_statenum_time(detnbaut_ap1, 'analysis/path_refinement/detnbaut_safra_ap1_time.pdf', title)
plot_statenum_statereduction(detnbaut_ap1, 'analysis/path_refinement/detnbaut_safra_ap1_statereduction.pdf', title)
plot_statenum_statereductionrel(detnbaut_ap1, 'analysis/path_refinement/detnbaut_safra_ap1_statereductionrelative.pdf', title)
plot_sccnum_statereduction(detnbaut_ap1, 'analysis/path_refinement/detnbaut_safra_ap1_sccreduction.pdf', title)
plot_histogram_statereduction(detnbaut_ap1, 'analysis/path_refinement/detnbaut_safra_ap1_statereduction_hist.pdf', title)

# Plot detnbaut special ap2
detnbaut_ap2 = filelines_to_json('raw/path_refinement/detnbaut_special_ap2.json')
title = 'Path Refinement state reduction on a DPA with |Σ|=4 that was created by nbautils from an NBA, using the Safra relation.'
plot_statenum_time(detnbaut_ap2, 'analysis/path_refinement/detnbaut_safra_ap2_time.pdf', title)
plot_statenum_statereduction(detnbaut_ap2, 'analysis/path_refinement/detnbaut_safra_ap2_statereduction.pdf', title)
