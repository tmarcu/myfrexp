/*
 *  myfrexp.c
 *
 *  Copyright (C) 2013  Tudor Marcu. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or modify
 *  it under the terms and conditions of the GNU Lesser General Public
 *  License version 2.1 as published by the Free Software Foundation.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef union {
	double d;
	struct {
		unsigned long long int mantisa : 52;
		unsigned long long int exponent : 11;
		unsigned long long int sign : 1;
	} comp;
} double_bits;

double my_frexp(double x, int *exp)
{
	char sign;
	double mantissa;
	unsigned long long int *p = (unsigned long long int *) &x;

	sign = *p >> 63;
	printf("Sign: %d\n", sign);

	if (!sign)
		*exp = (*p >> 52) - 1022;
	else
		*exp = (*p >> 52) - 3070;
	printf("Exponent: %d\n", *exp);

	/* Extract only the mantissa 2^52 range */
	mantissa = *p & 0xFFFFFFFFFFFFFL;
	mantissa = (((mantissa/pow(2,52)) + 1)/2);

	printf("Mantissa: %f\n\n", pow(-1, sign) * mantissa);

	return pow(-1, sign) * mantissa;
}

double union_frexp(double x, int *exp)
{
	double mantissa;
	double_bits d;

	d.d = x;
	*exp = d.comp.exponent - 1022;
	mantissa = (((double) d.comp.mantisa/pow(2,52)) + 1)/2;
	printf("Sign = %d\n",d.comp.sign);
	printf("Exponent = %d\n", *exp);
	printf("Mantissa = %f\n\n", pow(-1, d.comp.sign) * mantissa);

	return pow(-1, d.comp.sign) * mantissa;
}

int main(int argc, char *argv[])
{
	unsigned int e;
	double input;
	double q;

	if (argc < 2)
		exit(EXIT_FAILURE);

	input = atof(argv[1]);

	q = my_frexp(input, &e);

	printf("\tmyfrexp: %f * 2^%d\n\n", q, e);

	q = union_frexp(input, &e);

	printf("\tunion_frexp: %f * 2^%d\n\n", q, e);

	q = frexp(input, &e);

	printf("\tREAL frexp: %f * 2^%d\n\n", q, e);

	return 0;
}
