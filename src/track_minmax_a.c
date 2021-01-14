/**
 * @file    track_minmax_a.c
 * @brief   Track the variations of the semi-major axis and log results.
 * @author  Antoine Petit <apetit@astro.lu.se
 * 
 * @section     LICENSE
 * Copyright (c) 2015 Dan Tamayo, Hanno Rein, Antoine Petit
 *
 * This file is part of reboundx.
 *
 * reboundx is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * reboundx is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with rebound.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 *
 * $Miscellaneous Utilities$     // Effect category (must be the first non-blank line after dollar signs and between dollar signs to be detected by script). 
 * 
 * ======================= ===============================================
 * Authors                 Antoine Petit
 * Implementation Paper    `Tamayo, Rein, Shi and Hernandez, 2019 <https://ui.adsabs.harvard.edu/abs/2020MNRAS.491.2885T/abstract>`_.
 * Based on                None
 * C Example               To do :ref:`c_example_track_min_distance`
 * Python Example          To do `TrackMinDistance.ipynb <https://github.com/dtamayo/reboundx/blob/master/ipython_examples/TrackMinDistance.ipynb>`_.
 * ======================= ===============================================
 * 
 * For a given particle, this keeps track of that particle's minimum and maximum semi-major axis.  User
 * should add parameters to the particular particle whose distance should be tracked.
 *
 * **Effect Parameters**
 * 
 * *None*
 * 
 * **Particle Parameters**
 * 
 * Only particles with their ``min_a`` and ``max_a`` parameter set initially will track their semi-major axis. The effect will
 * update this parameter when the particle semi-major axis get out of the bounds, so the user has to set it
 * initially.  
 * 
 * ================================ =========== =======================================================
 * Name (C type)                    Required    Description
 * ================================ =========== =======================================================
 * min_a (double)                   Yes         Particle's minimum semi-major axis.
 * max_a (double)                   Yes         Particle's maximum semi-major axis
 * ================================ =========== =======================================================
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "rebound.h"
#include "reboundx.h"

void rebx_track_minmax_a(struct reb_simulation* const sim, struct rebx_operator* const operator, const double dt){
    const struct rebx_extras* const rebx = sim->extras;
    const int N = sim->N - sim->N_var;
    for(int i=1; i<N; i++){
        struct reb_particle* const p = &sim->particles[i];

        double* min_a = rebx_get_param(rebx, p->ap, "min_a");
        double* max_a = rebx_get_param(rebx, p->ap, "max_a");

        if (min_a != NULL && max_a != NULL){
            struct reb_particle* source;
            source = &sim->particles[0];
            int err=0;
            struct reb_orbit o = reb_tools_particle_to_orbit_err(sim->G, *p, *source, &err);

            if (!err){
                if ( o.a < *min_a){
                    *min_a = o.a;
                }
                if ( o.a > *max_a){
                    *max_a = o.a;
                }
            }
        }
    }
}

