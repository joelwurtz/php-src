/* divmod.c: bcmath library file. */
/*
    Copyright (C) 1991, 1992, 1993, 1994, 1997 Free Software Foundation, Inc.
    Copyright (C) 2000 Philip A. Nelson

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.  (LICENSE)

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to:

      The Free Software Foundation, Inc.
      59 Temple Place, Suite 330
      Boston, MA 02111-1307 USA.

    You may contact the author by:
       e-mail:  philnelson@acm.org
      us-mail:  Philip A. Nelson
                Computer Science Department, 9062
                Western Washington University
                Bellingham, WA 98226-9062

*************************************************************************/

#include "bcmath.h"
#include <stdbool.h>
#include <stddef.h>


/* Division *and* modulo for numbers.  This computes both NUM1 / NUM2 and
   NUM1 % NUM2  and puts the results in QUOT and REM, except that if QUOT
   is NULL then that store will be omitted.
   false is returned if divisor is 0.
   true otherwise for success.
 */

bool bc_divmod(bc_num num1, bc_num num2, bc_num *quot, bc_num *rem, size_t scale)
{
	bc_num quotient = NULL;
	bc_num temp;
	size_t rscale;

	/* Cannot divide/mod by 0. */
	if (bc_is_zero(num2)) {
		return false;
	}

	/* Calculate final scale. */
	rscale = MAX (num1->n_scale, num2->n_scale + scale);
	bc_init_num(&temp);

	/* Calculate it. */
	bc_divide(num1, num2, &temp, 0);
	if (quot) {
		quotient = bc_copy_num(temp);
	}
	bc_multiply_ex(temp, num2, &temp, rscale);
	bc_sub_ex(num1, temp, rem, rscale);
	bc_free_num (&temp);

	if (quot) {
		bc_free_num (quot);
		*quot = quotient;
	}

	/* The value of rscale changes during processing. Here we use the value of scale. It's not a typo. */
	(*rem)->n_scale = MIN(scale, (*rem)->n_scale);
	if (bc_is_zero(*rem)) {
		(*rem)->n_sign = PLUS;
		(*rem)->n_scale = 0;
	}

	return true;
}


/* Modulo for numbers.  This computes NUM1 % NUM2  and puts the
   result in RESULT.   */

bool bc_modulo(bc_num num1, bc_num num2, bc_num *result, size_t scale)
{
	return bc_divmod(num1, num2, NULL, result, scale);
}
