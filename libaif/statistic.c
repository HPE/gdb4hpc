#ifdef HAVE_CONFIG_H
#include	<config.h>
#endif /* HAVE_CONFIG_H */

#include	<stdio.h>
#include	<stdlib.h>
#include	<memory.h>
#include	<string.h>
#include	<math.h>
#include	"aif.h"
#include	"aiferr.h"

#define AVERAGE_ID		"average"

#define STDEV_ID		"stdev"
#define STDEV_MEAN		"mean"
#define STDEV_SUM2		"sum2"
#define STDEV_SIZE		"size"

#define CANVAS_SIZE 100.0   /* nominal size of user coordinate frame */
#define GAP 20.0

// list of reduce operations can be used on a set of data
enum reduce_ops
{
	sum, product, max, min, average, stdev, countelem
};

// list of reduce operations can be used on a set of data
enum stat_test_ops
{
	exact, least_chi2, kolmogorov
};

/* Minh adds this function to perform
 * various simple reduction operation on all elements in a given AIF array
 * sum, product, max/min, average, standard deviation
 */

AIF *
_aif_reduce(AIF * a, enum reduce_ops op, double *params)
{
	double result = 0.0;
	double *elems;
	double lower_bound = 0.0, upper_bound = 0.0;
	int tmpval;
	int i;
	int type;

	AIFIndex * ix;
	AIF * res = NULL;

	ix = AIFArrayIndexInit(a);

	type = AIFBaseType(a);

	switch (op)
	{
		case product:
			result = 1.0;
			break;
		case countelem:
			lower_bound = params[0];
			upper_bound = params[1];
			break;
		case sum:
		case average:
		case stdev:
		default:
			// do nothing
			break;
	}

	elems = (double *)malloc(sizeof(double) * ix->i_nel);

	for (i=0 ; i<ix->i_nel ; i++)
	{
		switch (type) {
		case AIF_INTEGER:
		case AIF_FTN_INTEGER:
			AIFArrayElementToInt(a, ix, &tmpval);
			elems[i] = tmpval;
			break;
		case AIF_FLOATING:
		case AIF_FTN_REAL:
			AIFArrayElementToDouble(a, ix, elems+i);
			break;
		default:
			DPRINT("*** _aif_reduce: type not found %d\n", type);
			SetAIFError(AIFERR_TYPE, NULL);
			return (AIF *)NULL;
		}
		AIFArrayIndexInc(ix);
	}
	AIFArrayIndexFree(ix);

	for (i=0 ; i<ix->i_nel ; i++)
	{
		switch (op) {
		case sum:
		case average:
		case stdev:
			result += elems[i];
			break;
		case product:
			result *= elems[i];
			break;
		case max:
		case min:
			if (i==0)       result = elems[i];
			else
			{
				if ( (op==max && result<elems[i]) ||
				     (op==min && result>elems[i]) )
				{
					result = elems[i];
				}
			}
			break;
		case countelem:
			if (elems[i]>=lower_bound && elems[i]<=upper_bound)
				result += 1;
			break;
		default:
			SetAIFError(AIFERR_TYPE, NULL);
			if (elems != NULL)
				free(elems);
			return (AIF *)NULL;
		}
	}

	if (op == average)
	{
		AIF * mean_aif;
		AIF * size_aif;

		res = EmptyStructToAIF(AVERAGE_ID); 

		mean_aif = DoubleToAIF((float)result/ix->i_nel);
		size_aif = DoubleToAIF((float)ix->i_nel);

		AIFAddFieldToStruct(res, STDEV_MEAN, mean_aif);
		AIFAddFieldToStruct(res, STDEV_SIZE, size_aif);
		AIFFree(mean_aif);
		AIFFree(size_aif);
	}
	else if (op == stdev)
	{
		double sum2 = 0;
		double mean = (float)result/ix->i_nel;
		AIF * mean_aif;
		AIF * sum2_aif;
		AIF * size_aif;

		// work out sum of squares of differences
		for (i=0; i<ix->i_nel; i++)
			sum2 += (elems[i]-mean)*(elems[i]-mean);

		res = EmptyStructToAIF(STDEV_ID);

		mean_aif = DoubleToAIF(mean);
		sum2_aif = DoubleToAIF(sum2);
		size_aif = DoubleToAIF((float)ix->i_nel);

		AIFAddFieldToStruct(res, STDEV_MEAN, mean_aif);
		AIFAddFieldToStruct(res, STDEV_SUM2, sum2_aif);
		AIFAddFieldToStruct(res, STDEV_SIZE, size_aif);
		AIFFree(mean_aif);
		AIFFree(sum2_aif);
		AIFFree(size_aif);
	}
	else if (type==AIF_INTEGER || type==AIF_FTN_INTEGER || op==countelem)
	{
		tmpval = result;
		res = IntToAIF(tmpval);
	}
	else if (type == AIF_FLOATING || type==AIF_FTN_REAL)
		res = DoubleToAIF(result);

	if (elems != NULL)
		free(elems);
	return res;
}

AIF *
_aif_reduce_array(AIF **a, int size, enum reduce_ops op)
{
	int i;
	int type;
	int tmpval;
	double elem;
	double result = 0.0;

	AIF * res = NULL;

	type = AIFType(a[0]);
	for (i=0; i<size; i++)
	{
		switch (type) {
		case AIF_INTEGER:
		case AIF_FTN_INTEGER:
			AIFToInt(a[i], &tmpval);
			elem = tmpval;
			break;
		case AIF_FLOATING:
		case AIF_FTN_REAL:
			AIFToDouble(a[i], &elem);
			break;
		default:
			SetAIFError(AIFERR_TYPE, NULL);
			return (AIF *)NULL;
		}

		switch (op) {
		case sum:
		case countelem:
			result += elem;
			break;
		case product:
			result *= elem;
			break;
		case max:
		case min:
			if (i==0)
				result = elem;
			else
			{
				if ( (op==max && result<elem) ||
				     (op==min && result>elem) )
				{
					result = elem;
				}
			}
			break;
		default:
			SetAIFError(AIFERR_TYPE, NULL);
			return (AIF *)NULL;
		}	
	}

	if (type == AIF_INTEGER || type == AIF_FTN_INTEGER )
	{
		tmpval = result;
		res = IntToAIF(tmpval);
	}
	else if (type == AIF_FLOATING || type == AIF_FTN_REAL )
		res = DoubleToAIF(result);

	return res;
}

AIF *
AIFSum(AIF * a)
{
	DPRINT("in AIFSum1\n");
	char *type = strdup(AIF_FORMAT(a));

	AIF * res;

	if (FDSType(type) == AIF_ARRAY || FDSType(type) == AIF_FTN_ARRAY )
		res = _aif_reduce(a, sum, NULL);
	else
		res = CopyAIF(a);

	return res;
}

// Assumption: the array contents single number AIF
AIF *
AIFSumArray(AIF **a, int size)
{
	return _aif_reduce_array(a, size, sum);
}

AIF *
AIFProduct(AIF * a)
{
	char *type = strdup(AIF_FORMAT(a));

	AIF * res;

	if (FDSType(type) == AIF_ARRAY || FDSType(type) == AIF_FTN_ARRAY )
		res = _aif_reduce(a, product, NULL);
	else
		res = CopyAIF(a);

	return res;
}

// Assumption: the array contents single number AIF
AIF *
AIFProductArray(AIF **a, int size)
{
	return _aif_reduce_array(a, size, product);
}

AIF *
AIFAverage(AIF * a)
{
	char *type = strdup(AIF_FORMAT(a));

	AIF * res;

	if (FDSType(type) == AIF_ARRAY || FDSType(type) == AIF_FTN_ARRAY )
		res = _aif_reduce(a, average, NULL);
	else
		res = CopyAIF(a);

	return res;
}

// Assumption: the array contents single number AIF
AIF *
AIFAverageArray(AIF **a, int size)
{
	AIF * res;
	AIF * mean_aif;
	AIF * size_aif;

	double sigma;
	double current_mean, current_size;
	double elem_mean, elem_size;

	int i;
	
	// setup the zeroeth element
	mean_aif = AIFGetStruct(a[0], STDEV_MEAN);
	size_aif = AIFGetStruct(a[0], STDEV_SIZE);
	
	AIFToDouble(mean_aif, &current_mean);
	AIFToDouble(size_aif, &current_size);

	for(i=1; i<size; i++)
	{
		mean_aif = AIFGetStruct(a[i], STDEV_MEAN);
		size_aif = AIFGetStruct(a[i], STDEV_SIZE);
		AIFToDouble(mean_aif, &elem_mean);
		AIFToDouble(size_aif, &elem_size);
		
		// work out current mean from sub means
		current_size += elem_size;
		sigma = elem_mean - current_mean;
		current_mean = current_mean + sigma*elem_size/current_size;
	}
	
	res = DoubleToAIF(current_mean);

	return res;
}

AIF *
AIFMax(AIF * a)
{
	char *type = strdup(AIF_FORMAT(a));

	AIF * res;

	if (FDSType(type) == AIF_ARRAY || FDSType(type) == AIF_FTN_ARRAY )
		res = _aif_reduce(a, max, NULL);
	else
		res = a;

	return res;
}

// Assumption: the array contents single number AIF
AIF *
AIFMaxArray(AIF **a, int size)
{
	return _aif_reduce_array(a, size, max);
}

AIF *
AIFMin(AIF * a)
{
	char *type = strdup(AIF_FORMAT(a));

	AIF * res;

	if (FDSType(type) == AIF_ARRAY || FDSType(type) == AIF_FTN_ARRAY )
		res = _aif_reduce(a, min, NULL);
	else
		res = a;

	return res;
}

// Assumption: the array contents single number AIF
AIF *
AIFMinArray(AIF **a, int size)
{
	return _aif_reduce_array(a, size, min);
}

AIF *
AIFStdev(AIF * a)
{
	char *type = strdup(AIF_FORMAT(a));

	AIF *res;

	if (FDSType(type) == AIF_ARRAY || FDSType(type) == AIF_FTN_ARRAY )
		res = _aif_reduce(a, stdev, NULL);
	else
	{
		SetAIFError(AIFERR_TYPE, NULL);
		return (AIF *)NULL;
	}
	return res;
}

AIF *
AIFStdevArray(AIF **a, int size)
{
	AIF * res;
	AIF * mean_aif;
	AIF * sum2_aif;
	AIF * size_aif;

	double stdev, sigma;
	double current_mean, current_sum2, current_size;
	double elem_mean, elem_sum2, elem_size;
	
	int i;
	
	// setup the zeroeth element
	mean_aif = AIFGetStruct(a[0], STDEV_MEAN);
	sum2_aif = AIFGetStruct(a[0], STDEV_SUM2);
	size_aif = AIFGetStruct(a[0], STDEV_SIZE);
	
	AIFToDouble(mean_aif, &current_mean);
	AIFToDouble(sum2_aif, &current_sum2);
	AIFToDouble(size_aif, &current_size);

	for(i=1; i<size; i++)
	{
		mean_aif = AIFGetStruct(a[i], STDEV_MEAN);
		sum2_aif = AIFGetStruct(a[i], STDEV_SUM2);
		size_aif = AIFGetStruct(a[i], STDEV_SIZE);
		
		AIFToDouble(mean_aif, &elem_mean);
		AIFToDouble(sum2_aif, &elem_sum2);
		AIFToDouble(size_aif, &elem_size);
		
		// work out current mean from sub means
		sigma = elem_mean - current_mean;
		current_mean = current_mean + sigma*elem_size/(current_size+elem_size);
		// work out new sum of squares of differences from current mean
		current_sum2 = current_sum2 + elem_sum2 + sigma*sigma*current_size*elem_size/(current_size+elem_size);
		current_size += elem_size;
	}
	
	// now calculate standard deviation
	stdev = sqrt(current_sum2/current_size);

	res = DoubleToAIF(stdev);

	return res;
}

AIF *
AIFCountElem(AIF * a, double lower, double upper)
{
	char *type = strdup(AIF_FORMAT(a));
	double params[2];

	AIF * res;

	params[0] = lower;
	params[1] = upper;

	if (FDSType(type) == AIF_ARRAY || FDSType(type) == AIF_FTN_ARRAY )
		res = _aif_reduce(a, countelem, params);
	else
		res = a;

	return res;
}

// Assumption: the array contents single number AIF
AIF *
AIFCountElemArray(AIF **a, int size)
{
	return _aif_reduce_array(a, size, countelem);
}

