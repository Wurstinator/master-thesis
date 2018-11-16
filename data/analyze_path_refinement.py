
from analyze_common import *

general_analysis_v1(name='Path Refinement', input_prefix='path_refinement/', output_prefix='path_refinement/')

# Plot detnbaut special ap1
detnbaut_ap1_safra = filelines_to_json('raw/path_refinement/detnbaut_special_ap1.json')
title = 'Path Refinement state reduction on a DPA with |Σ|=2 that was created by nbautils from an NBA, using the Safra relation.'
plot_dataset_to_pdf(detnbaut_ap1_safra, 'analysis/path_refinement/detnbaut_safra_ap1.pdf', title)

# Plot detnbaut special ap2
detnbaut_ap2_safra = filelines_to_json('raw/path_refinement/detnbaut_special_ap1.json')
title = 'Path Refinement state reduction on a DPA with |Σ|=4 that was created by nbautils from an NBA, using the Safra relation.'
plot_dataset_to_pdf(detnbaut_ap2_safra, 'analysis/path_refinement/detnbaut_safra_ap2.pdf', title)
