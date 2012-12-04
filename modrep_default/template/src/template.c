/* 
 * Copyright (c) 2012, Ismael Gomez-Miguelez <ismael.gomez@tsc.upc.edu>.
 * This file is part of ALOE++ (http://flexnets.upc.edu/)
 * 
 * ALOE++ is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * ALOE++ is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with ALOE++.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <aloe/swapi_static.h>
#include <aloe/params.h>
#include <aloe/skeleton.h>

#include "template.h"

pmid_t gain_id;
int block_length;

/*
 * Function documentation
 *
 * @returns 0 on success, -1 on error
 */
int initialize() {
	int size;

	gain_id = param_id("gain");

	if (!param_get_int_name("block_length", &block_length)) {
		block_length=0;
	}

	modinfo_msg("Parameter block_length is %d\n",block_length);

	/* Verify control parameters */
	if (block_length > input_max_samples || block_length < 0) {
		moderror_msg("Invalid block length %d\n", block_length);
		return -1;
	}


	/* do some other initialization stuff */

	return 0;
}



/**
 * @brief Function documentation
 *
 * @param inp Input interface buffers. Data from other interfaces is stacked in the buffer.
 * Use in(ptr,idx) to access the address. To obtain the number of received samples use the function
 * int get_input_samples(int idx) where idx is the interface index.
 *
 * @param out Input interface buffers. Data to other interfaces must be stacked in the buffer.
 * Use out(ptr,idx) to access the address.
 *
 * @return On success, returns a non-negative number indicating the output
 * samples that should be transmitted through all output interface. To specify a different length
 * for certain interface, use the function set_output_samples(int idx, int len)
 * On error returns -1.
 *
 *
 */
int work(void **inp, void **out) {
	int rcv_samples = get_input_samples(0); /** number of samples at itf 0 buffer */
	int i;
	input_t *input = inp[0];
	output_t *output = out[0];
	float gain;

	if (param_get_float(gain_id,&gain)) {
		gain = 2.0;
	} else {
		modinfo_msg("gain is %.2f\n",gain);
	}

	/* do DSP stuff here */
	for (i=0;i<rcv_samples;i++) {
		output[i]=input[i]*gain;
	}
	return rcv_samples;
}

/** @brief Deallocates resources created during initialize().
 * @return 0 on success -1 on error
 */
int stop() {
	return 0;
}

