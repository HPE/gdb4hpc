/*********************************************************************************\
 * dbgsrv_util.c
 *
 * © 2011-2019 Cray Inc.  All Rights Reserved.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Cray Inc.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 *********************************************************************************/



#ifdef HAVE_CONFIG_H
#include	<config.h>
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdarg.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "dbg.h"
#include "useful.h"
#include "cmdtoken.h"
#include "expr.h"
#include "dbgsrv_util.h"
#include "blockmap_util.h"
#include "dbgsrv_p2p.h"
#include "dbgsrv_libmi2aif.h"
#include "dbgsrv_defs.h"

extern int			sock_fd;
extern int			my_rank;
extern appInfo_t	other_app; 

char *				PerformSlicingErr = NULL;

static void	SetPerformSlicingErrorMsg(const char *, ...);

// this function parse the big dbg expression into gdb_var name, reduction function_id and a list of args for such function
void
ParseDBGExpr(const char *expr, int *func_id, char *sliceinfo, char *var, char *args)
{
	char *tok;
	char buf[BUFSIZ];
	char sub_expr[BUFSIZ]="";
	char tmp[100]="";

	strcpy(buf, expr);

	// look for ``
	if (NULL != strstr(buf, "`"))
	{
		char sub_expr2[BUFSIZ]="";

		tok = strtok(buf, "`");
		strcpy(sub_expr, tok);
		tok = strtok(NULL, "`");
		strcpy(sub_expr2, tok);

		if (NULL != strstr(sub_expr2, "slice"))
		{
			char tmp2[BUFSIZ]="";

			sub_expr2[strlen(sub_expr2)-1] = '\0';
			tok = strtok(sub_expr2, "(");
			tok = strtok(NULL, "(");
			strcpy(tmp2, tok);

			tok = strtok(tmp2, ")");
			strcpy(sliceinfo, tok);
			tok = strtok(NULL, ")");
			strcpy(var, tok);
		}
		else	strcpy(var, sub_expr2);

		// now identify the function id
		if (NULL != strstr(sub_expr, "hash"))
		{
			*func_id = EXPR_HASH_BOB;	// by default
			tok = strtok(sub_expr, "hash");
			strcpy(tmp, tok);
			tok = strtok(tmp, "(");
			tok[strlen(tok)-1] = '\0';
			strcpy(args, tok);
		}
		else if (NULL != strstr(sub_expr, "sum"))
			*func_id = EXPR_SUM;
		else if (NULL != strstr(sub_expr, "average"))
			*func_id = EXPR_AVERAGE;
		else if (NULL != strstr(sub_expr, "product"))
			*func_id = EXPR_PRODUCT;
		else if (NULL != strstr(sub_expr, "min"))
			*func_id = EXPR_MIN;
		else if (NULL != strstr(sub_expr, "max"))
			*func_id = EXPR_MAX;
		else if (NULL != strstr(sub_expr, "stdev"))
			*func_id = EXPR_STDEV;
		else if (NULL != strstr(sub_expr, "countelem"))
		{
			*func_id = EXPR_COUNTELEM;
			tok = strtok(sub_expr, "countelem");
			strcpy(tmp, tok);
			tok = strtok(tmp, "(");
			tok[strlen(tok)-1] = '\0';
			strcat(args, tok);
		}
		else if (NULL != strstr(sub_expr, "ptop"))
		{
			*func_id = EXPR_P2P;
			tok = strtok(sub_expr, "ptop");
			strcpy(tmp, tok);
			tok = strtok(tmp, "(");
			tok[strlen(tok)-1] = '\0';
			strcat(args, tok);
		}
	}
	else if (NULL != strstr(buf, "slice"))
	{
		buf[strlen(buf)-1] = '\0';
		tok = strtok(buf, "(");
		tok = strtok(NULL, "(");

		strcpy(tmp, tok);
		tok = strtok(tmp, ")");
		strcpy(sliceinfo, tok);
		tok = strtok(NULL, ")");
		strcpy(var, tok);
	}

	return;
}

void
ParseHashArgs(char *args, int *hash_func_id, int *lower, int *upper, char *hash_info)
{
	char *tok;
	char buf[BUFSIZ]="";

	tok = strtok(args, "#");
	strcpy(hash_info, tok);
	tok = strtok(NULL, "#");
	if (tok==NULL) {
		strcpy(buf, hash_info);
		strcpy(hash_info, "");
	}
	else
		strcpy(buf, tok);

	// now parse the hash args
	tok = strtok(buf, ",");
	*hash_func_id = atoi(tok);
	tok = strtok(NULL, ",");
	*lower = atoi(tok);
	tok = strtok(NULL, ",");
	*upper = atoi(tok);

	return;
}

void
ParseCountelemArgs(char *args, double *lower, double *upper)
{
	char *tok;

	// now parse the countelem args
	tok = strtok(args, ",");
	*lower = atof(tok);
	tok = strtok(NULL, ",");
	*upper = atof(tok);

	return;
}

void
ParseHistogramArgs(char *args, int *numbins, double *start, double *end)
{
	char *tok;

	// now parse the histogram args
	tok = strtok(args, ",");
	*numbins = atoi(tok);
	tok = strtok(NULL, ",");
	*start = atof(tok);
	tok = strtok(NULL, ",");
	*end = atof(tok);

	return;
}

void
ParseBlockMapArgs(char *args, blockmap_info *bm)
{
	int i;
	char *tok;

	tok = strtok(args, ",");
	strcpy(bm->isym, tok);
	tok = strtok(NULL, ",");
	bm->num_procs = atoi(tok);
	tok = strtok(NULL, ",");
	bm->rank = atoi(tok);

	for (i=0; i<bm->rank; i++)
	{
		tok = strtok(NULL, ",");
		bm->m_distribute[i] = atoi(tok);
	}

	for (i=0; i<bm->rank; i++)
	{
		tok = strtok(NULL, ",");
		bm->m_blockcyclic_val[i] = atoi(tok);
	}

	for (i=0; i<bm->rank; i++)
	{
		tok = strtok(NULL, ",");
		bm->m_data[i] = atoi(tok);
	}

	for (i=0; i<bm->rank; i++)
	{
		tok = strtok(NULL, ",");
		bm->m_procs[i] = atoi(tok);
	}

	for (i=0; i<bm->rank; i++)
	{
		tok = strtok(NULL, ",");
		bm->m_dim_order[i] = atoi(tok);
	}

	return;
}


// blockmap_string = $i_sym,num_procs,rank,<distribute>,<blockcyclic_val>,<data>,<procs_grid>,<dim_order>
// P2P args = <owner>-<sender>-<blockmap_string1>-<blockmap_string2>
// example: a-a-$b,256,3,152,165,165,0,0,0,64,64,64,1,64,4,2,3,1-$a,128,3,152,165,165,0,0,0,64,64,64,1,64,4,2,3,1
void
ParseP2PArgs(char *args, char *my_isym, int *is_sender, blockmap_info *my_bm, blockmap_info *o_bm)
{
	char *tok;
	char buf1[BUFSIZ]="";
	char buf2[BUFSIZ]="";
	char buf3[BUFSIZ]="";
	char sender[BUFSIZ]="";

	// get owner proc_set symbol
	tok = strtok(args, "-");
	strcpy(my_isym, tok);
	tok = strtok(NULL, "-");

	// get sender symbol
	strcpy(sender, tok);
	// check if you are the sender
	if (strcmp(my_isym, sender)==0)
		*is_sender = 1;

	tok = strtok(NULL, "-");
	if (NULL != strstr(tok, my_isym))
		strcpy(buf1, tok);
	else
		strcpy(buf2, tok);

	tok = strtok(NULL, "-");
	if (NULL != strstr(tok, my_isym))
		strcpy(buf1, tok);
	else
		strcpy(buf2, tok);

	tok = strtok(NULL, "-");
	strcpy(buf3, tok);

	// get blockmap info for my_bm
	ParseBlockMapArgs(buf1, my_bm);
	// get blockmap info for my_bm
	ParseBlockMapArgs(buf2, o_bm);

	tok = strtok(buf3, ",");
	my_bm->a_lower = atof(tok);
	tok = strtok(NULL, ",");
	my_bm->a_upper = atof(tok);

	return;
}

/* this function takes in data size for a process, block size, the index, the rank
 * and return sclice info for that process
 */
int
_get_slice_info(int *data_size, int *block_size, char *sliceinfo, int index, int rank, int isFortran)
{
	int i;
	char tmpstr[BUFSIZ]="";
	int *start = NULL;
	int *end = NULL;

	start = (int*)Alloc(sizeof(int)*rank);
	end = (int*)Alloc(sizeof(int)*rank);

	for (i=rank-1; i>=0; i--) {
		if (index==0) {
			start[i] = 0;
			end[i] = block_size[i]-1;
		}
		else {
			start[i] = block_size[i]*index;
			index = start[i]/data_size[i];
			start[i] = start[i]%data_size[i];
			end[i] = start[i]+block_size[i]-1;
		}
	}

	// store the slicing info
	strcpy(sliceinfo, "");
	for (i=0; i<rank; i++)
	{
		if (i<rank-1)
			sprintf(tmpstr, "%d,%d,", start[i]+isFortran, end[i]+isFortran);
		else
			sprintf(tmpstr, "%d,%d", start[i]+isFortran, end[i]+isFortran);
		strcat(sliceinfo, tmpstr);
	}

	Free(start);
	Free(end);

	return 0;
}

int _get_slice_destination(int rank, int *global_start_loc, int *data_size, int *proc_grid, int *order_map)
{
	int des = 0;
	int i, carry=1;

	for (i=0; i<rank; i++) {
		int tmp = global_start_loc[order_map[i]]/data_size[order_map[i]];
		if (i>0 && tmp>0) {
			des += tmp * carry;
			carry = 1;
		}
		else
			des += tmp;
		carry *= proc_grid[order_map[i]];
	}

	return des;
}

// Minh
// andrewg fixed implicit calls to Free on 8/10/10
int
_get_overlap_info(int *final_size , blockmap_info bm1, blockmap_info bm2)
{
	int rank;
	int *block_size;
	int *block_size1;
	int *block_size2;
	int i;
	blockmap_info *bm;

	rank =  bm1.rank;

	block_size1 = (int*)Alloc(sizeof(int)*rank);
	block_size2 = (int*)Alloc(sizeof(int)*rank);

	for(i=0; i<2; i++) {
		bm = i==0? &bm1 : &bm2;
		block_size = i==0? block_size1 : block_size2;
		// call a function from blockmap.c
        if (_get_hash_block_size(rank, bm->m_data, bm->m_distribute, bm->m_blockcyclic_val, bm->m_procs, block_size)!=1) {
			if(final_size)          Free(final_size);
			return -1;
		}
	}

	for(i=0; i<rank; i++)
		final_size[i] = gcd(block_size1[i], block_size2[i]);

	Free(block_size1);
	Free(block_size2);

	return 0;
}

int _get_data_index(int rank, int *data, int *index)
{
	int i, j;
	int tmp, result=0;

	for (i=0; i<rank; i++)
	{
		tmp = 1;
		if (i==rank-1)
			result += index[i];
		else
		{
			for (j=rank-1; j>i; j--)
				tmp *= data[j];
			tmp *= index[i];
			result += tmp;
		}
	}
	return result;
}

// Minh
// now we map the index in hash_bound to data_bound_max
uint32_t
_perform_hash(char *datastr, char *fds, int *hash_bound, int *data_bound, int *index, int num_elem_to_hash, int rank, int hashfunc)
{
	int *data_indices = (int *)Alloc(num_elem_to_hash * sizeof(int));
	int *local_index = (int *)Alloc(rank * sizeof(int));
	int i, j, tmp, start_point;
	char *tok, elem[BUFSIZ]="";
	char *cp_datastr = strdup(datastr);
	uint32_t result=0;

	// work out the first start point for this index first
	start_point = _get_data_index(rank, data_bound, index);
	
	tmp = 0;
    for (i=0; i<num_elem_to_hash; i++)
    {
		// translate i into local_index
		int tmp2 = i;
		for (j=rank-1; j>=0; j--) {
			local_index[j] = tmp2%hash_bound[j];
			tmp2 = tmp2/hash_bound[j];
		}
		data_indices[tmp] = _get_data_index(rank, data_bound, local_index);
		// correct data_indices using the index
		data_indices[tmp] += start_point;
		tmp++;
    }

	// reset i, j;
	i = 0;
	j = 0;
	// check the type, if type is Fortran COMPLEX, count "," twice
	if ( strstr(fds, "COMPLEX")!=NULL )
	{
		tok = strtok(cp_datastr, ")");
		TrimStrWithTarget(tok, '(');
	}
	else
		tok = strtok(cp_datastr, ",");
	while(tok != NULL)
	{
		strcpy(elem, tok);
		if (strlen(elem)>0)
		{
			if(i == data_indices[j])
			{
				if(hashfunc==EXPR_HASH_BOB)
					result += BJHash(TrimStr(elem), strlen(TrimStr(elem)), DJBHash(fds, strlen(fds), j) );

				j++;
				if(j>=num_elem_to_hash)
					break;
			}
			i++;
		}
		// check the type, if type is Fortran COMPLEX, count "," twice
		if ( strstr(fds, "COMPLEX")!=NULL )
		{
			tok = strtok(NULL, ")");
			TrimStrWithTarget(tok, ',');
			TrimStrWithTarget(tok, '(');
		}
		else
			tok = strtok(NULL,",");
	}

	if (data_indices)	Free(data_indices);
	if (local_index)	Free(local_index);
	if (cp_datastr)		Free(cp_datastr);

	return result;
}

AIF *
PerformHashMI(char *datastr, char *type, arrayDimension_t * shape,
				char *hashinfo, int hashfunc, int precision_lower, int precision_upper)
{
	AIF		*result = NULL;
	int		*hash_bound = NULL;
	int		*data_bound_max = NULL;
	int		*result_min = NULL;
	int		*result_max = NULL;
	int		*tmparray = NULL;
	int		rank, i, j;
	int		num_elem, num_elem_to_hash, num_hash;
	int		tmpvals[10];
	char    *btype = NULL;
	char    *tok = NULL;
	uint32_t *hash_vals = NULL;
	int **local_start_locs = NULL;  // startpoints for local data
	int isFortran=0;

	// check for fortran type
	if (type[strlen(type) - 1]==ARRAY_FTN_TYPE_END)
		isFortran = 1;
	// now parse shape to get btype and data_bound_max
	// shape look something like this: int [2][3]
	tok = strtok(type, " ");
	btype = strdup(tok);


	if (strlen(hashinfo)==0)        // no hashinfo, just hash the whole block
	{
		if(hashfunc==EXPR_HASH_BOB)
			result = HashToAIF(BJHash(datastr, strlen(datastr), DJBHash(btype, strlen(btype), 0) ));
		return result;
	}

	rank = shape->dim;
	
	hash_bound = (int *)Alloc(rank * sizeof(int));
	data_bound_max = (int *)Alloc(rank * sizeof(int));
	tmparray = (int *)Alloc(rank * sizeof(int));

	// value of data_bound_max
	j = rank-1;
	for (i=0; i<rank; i++) {
		data_bound_max[i] = shape->dimdef[j];
		j--;
	}

	tok = strtok(hashinfo, ",");
	i = 0;
	num_elem_to_hash = 1;
	while(tok != NULL)
	{
		tmpvals[i++] = atoi(tok);
		num_elem_to_hash *= atoi(tok);
		tok = strtok(NULL,",");
	}

	// now fix hash_bound according to rank
	for (j=rank-1; j>=0; j--) {
		hash_bound[j] = tmpvals[--i];
		if (hash_bound[j] > data_bound_max[j]) {
			DVPRINT("*** PerformHashMI: User defined data structure is bigger than actual data structure\n");
			return (AIF *)NULL;
		}
	}

	if (isFortran)
	{
		// revert the order of hash_bound and data_bound
		j=rank-1;
		for (i=0; i<rank; i++)
		{
			tmparray[i] = hash_bound[j];
			j--;
		}
		for (i=0; i<rank; i++)
			hash_bound[i] = tmparray[i];
		j=rank-1;
		for (i=0; i<rank; i++)
		{
			tmparray[i] = data_bound_max[j];
			j--;
		}
		for (i=0; i<rank; i++)
			data_bound_max[i] = tmparray[i];
	}

	num_elem = 1;
	for(i=0; i<rank; i++)
		num_elem *= data_bound_max[i];

	num_hash = num_elem / num_elem_to_hash;
	hash_vals = (uint32_t *)Alloc(num_hash * sizeof(uint32_t));

	result_min = (int *)Alloc(rank * sizeof(int));
	result_max = (int *)Alloc(rank * sizeof(int));

	for(i=0; i<rank; i++)
	{
		result_min[i] = 0;
		result_max[i] = (data_bound_max[i] / hash_bound[i]) - 1;
	}


	local_start_locs = (int **)Alloc(num_hash * sizeof(int*));
	for (i=0; i<num_hash; i++) {
		local_start_locs[i] = (int *)Alloc(rank * sizeof(int));
	}

	for(i=0; i<num_hash; i++)
	{
		// find local_start_points
		if (i==0) {
			for (j=0; j<rank; j++)
				local_start_locs[i][j] = 0;
		}
		else {
			for(j=0; j<rank; j++)
				local_start_locs[i][j] = local_start_locs[i-1][j];
			for (j=rank-1; j>=0; j--) {
				if ( (local_start_locs[i][j]+hash_bound[j]) < (data_bound_max[j]) ) {
					local_start_locs[i][j] += hash_bound[j];
					break;
				}
				else
					local_start_locs[i][j] = local_start_locs[0][j];
			}
		}

		hash_vals[i] = _perform_hash(datastr, btype, hash_bound, data_bound_max,
				 local_start_locs[i], num_elem_to_hash, rank, hashfunc);
	}

	// now AIF the hash array
	if (isFortran)
		result = FtnArrayToAIF(rank, result_min, result_max, (char*)hash_vals,
					sizeof(uint32_t)*num_hash, AIF_HASHED_TYPE(sizeof(uint32_t)));
	else
		result = ArrayToAIF(rank, result_min, result_max, (char*)hash_vals,
					sizeof(uint32_t)*num_hash, AIF_HASHED_TYPE(sizeof(uint32_t)));

	if (data_bound_max)     Free(data_bound_max);
	if (hash_bound)         Free(hash_bound);
	if (hash_vals)          Free(hash_vals);
	if (result_min)         Free(result_min);
	if (result_max)         Free(result_max);
	for (i=0; i<num_hash; i++) {
		Free(local_start_locs[i]);
	}
	Free(local_start_locs);

	return result;
}

/* Minh add this function to perform hashing on AIF data result
 * 19/06/2009
 */
AIF *
PerformHash(AIF *data, char *hashinfo, int hashfunc, int precision_lower, int precision_upper)
{
	AIF	*result = NULL;
	char	*datastr = NULL;
	int	*hash_bound = NULL;
	int	*data_bound_min = NULL;
	int	*data_bound_max = NULL;
	int	*result_min = NULL;
	int	*result_max = NULL;
	char	*btype = NULL;
	char	*tok = NULL;
	uint32_t *hash_vals = NULL;
	int rank, i, j;
	int num_elem, num_elem_to_hash, num_hash;
	int tmpvals[10];
	int **local_start_locs = NULL;  // startpoints for local data

	if ( precision_lower<0 )	// no precision specified, justgo normal values
		AIFToStr(&datastr, 0, data);
	else
		AIFToStrPrecision(&datastr, 0, data, precision_lower);

	if ( FDSType(AIF_FORMAT(data))!=AIF_ARRAY )
		btype = AIF_FORMAT(data);
	else {
		btype = FDSBaseType(AIF_FORMAT(data));
		// remov the square brackets
		datastr++;
		datastr[strlen(datastr)-1]='\0';
	}

	if (strlen(hashinfo)==0 || FDSType(AIF_FORMAT(data))!=AIF_ARRAY)
	{
		if(hashfunc==EXPR_HASH_BOB)
			result = HashToAIF(BJHash(datastr, strlen(datastr), DJBHash(btype, strlen(btype), 0) ));
		return result;
	}

	rank = FDSArrayRank(AIF_FORMAT(data));
	hash_bound = (int *)Alloc(rank * sizeof(int));
	// get the data boundaries
	FDSArrayBounds(AIF_FORMAT(data), rank, &data_bound_min, &data_bound_max, NULL);
	
	// fix value of data_bound
	for (i=0; i<rank; i++)
		data_bound_max[i] = data_bound_max[i]+1;
		
	tok = strtok(hashinfo, ",");
	i = 0;
	num_elem_to_hash = 1;
	while(tok != NULL)
	{	
		tmpvals[i++] = atoi(tok);
		num_elem_to_hash *= atoi(tok);
		tok = strtok(NULL,",");
	}

	// now fix hash_bound according to rank
	for (j=rank-1; j>=0; j--) {
		hash_bound[j] = tmpvals[--i];
		if (hash_bound[j] > data_bound_max[j]) {
			DVPRINT("*** PerformHash: User defined data structure is bigger than actual data structure\n");
			return (AIF *)NULL;
		}
	}

	num_elem = 1;
	for(i=0; i<rank; i++)
		num_elem *= data_bound_max[i];

	num_hash = num_elem / num_elem_to_hash;
	hash_vals = (uint32_t *)Alloc(num_hash * sizeof(uint32_t));

	result_min = (int *)Alloc(rank * sizeof(int));
	result_max = (int *)Alloc(rank * sizeof(int));

	for(i=0; i<rank; i++)
	{	result_min[i] = 0;	
		result_max[i] = (data_bound_max[i] / hash_bound[i]) - 1;
	}

	local_start_locs = (int **)Alloc(num_hash * sizeof(int*));
	for (i=0; i<num_hash; i++) {
		local_start_locs[i] = (int *)Alloc(rank * sizeof(int));
	}

	for(i=0; i<num_hash; i++)
	{
		// find local_start_points
		if (i==0) {
			for (j=0; j<rank; j++)
				local_start_locs[i][j] = 0;
		}
		else {
			for(j=0; j<rank; j++)
				local_start_locs[i][j] = local_start_locs[i-1][j];
			for (j=rank-1; j>=0; j--) {
				if ( (local_start_locs[i][j]+hash_bound[j]) < (data_bound_max[j]) ) {
					local_start_locs[i][j] += hash_bound[j];
					break;
				}
				else
					local_start_locs[i][j] = local_start_locs[0][j];
			}
		}

		hash_vals[i] = _perform_hash(datastr, btype, hash_bound, data_bound_max,
				 local_start_locs[i], num_elem_to_hash, rank, hashfunc);
	}

	// now AIF the hash array
	result = ArrayToAIF(rank, result_min, result_max, (char*)hash_vals,
			sizeof(uint32_t)*num_hash, AIF_HASHED_TYPE(sizeof(uint32_t)));

	if (data_bound_min)	Free(data_bound_min);
	if (data_bound_max)	Free(data_bound_max);
	if (hash_bound)		Free(hash_bound);
	if (hash_vals)		Free(hash_vals);
	if (result_min)		Free(result_min);
	if (result_max)		Free(result_max);
	//if (datastr)		Free(datastr);
	for (i=0; i<num_hash; i++) {
		Free(local_start_locs[i]);
	}
	Free(local_start_locs);

	return result;
}

void
SetPerformSlicingErrorMsg(const char *fmt, ...)
{
	va_list	ap;

	if (PerformSlicingErr != NULL)
		Free(PerformSlicingErr);
		
	va_start(ap, fmt);
	vasprintf(&PerformSlicingErr, fmt, ap);
	va_end(ap);
}

/* Minh
*/
AIF *
PerformSlicing(AIF *a, char *sliceinfo, int is_revert)
{
	int	i=0,j=0;
	int	nd, nds;
	int *	min;
	int *	max;
	char *	tok;
	AIF *	res;
	
	DVPRINT("PerformSlicing: aif format: %s aif len: %d\n", AIF_FORMAT(a), AIF_LEN(a));
	DVPRINT("PerformSlicing: sliceinfo is %s\n", sliceinfo);

	if ( AIFType(a) != AIF_ARRAY && AIFType(a) != AIF_FTN_ARRAY )
	{
		DVPRINT("*** Expected array on input 0, got \"%s\"\n", AIF_FORMAT(a));
		SetPerformSlicingErrorMsg("Attempted to slice something other than an array");
		return (AIF *)NULL;
	}

	nd = AIFArrayRank(a);
	min = (int *)Alloc(nd * sizeof(int));
	max = (int *)Alloc(nd * sizeof(int));
	
	DVPRINT("PerformSlicing: rank is %d\n", nd);

	// now parse the sliceinfo to get the slicing dim info
	tok = strtok(sliceinfo, ",");
	
	// Fortran arrays are reversed for the time being.
	if ( is_revert )
		i = nd - 1;

	nds = 0;
	while(tok != NULL)
	{
		// if nds is equal to nd, then there are too many dimensions in the slicing information
		// we need to do this check here to avoid a heap corruption.
		if (nds++ == nd)
		{
			Free(min);
			Free(max);
			DVPRINT("*** Invalid slice: %d, Out of bounds", i);
			SetPerformSlicingErrorMsg("Invalid slice: Too many subscripts");
			return (AIF *)NULL;
		}
		min[i] = atoi(TrimStr(tok));
		tok = strtok(NULL, ",");
		max[i] = atoi(TrimStr(tok));
		tok = strtok(NULL, ",");
		if ( min[i] > max[i] )
		{
			Free(min);
			Free(max);
			DVPRINT("*** Invalid slice: %d, min > max (%d > %d)", i, min[i], max[i]);
			SetPerformSlicingErrorMsg("Invalid slice: array dimension %d, min > max (%d > %d)", i, min[i], max[i]);
			return (AIF *)NULL;
		}
		if ( is_revert )
			--i;
		else
			++i;
	}
	/*
	if ( 2*nd + 1 != n->n_ninp )
	{
		DFError(n, "Tried to slice array dim %d with %d inputs", nd, n->n_ninp);
		return (AIF *)NULL;
	}
	*/
	
	if (debug_flag)
	{
		for (j=0; j<nd; ++j)
		{
			fprintf(stderr, "PerformSlicing: For rank %d slice_lo=%d slice_hi=%d\n", j, min[j], max[j]);
		}
	}
	
	if ( (res = AIFArraySlice(a, nd, min, max)) == NULL )
	{
		Free(min);
		Free(max);
		DVPRINT("*** AIFArraySlice: %s", AIFErrorStr());
		SetPerformSlicingErrorMsg("%s", AIFErrorStr());
		return (AIF *)NULL;
	}
	Free(min);
	Free(max);
	
	DVPRINT("PerformSlicing: returned aif format: %s aif len: %d\n", AIF_FORMAT(res), AIF_LEN(res));

	return res;
}

void
SliceSocketSend(int opset_rank, AIF *tosend)
{
	int connected = -1;
	char opset_name[MAXHOSTNAMELEN];
	struct hostent *host;
	struct sockaddr_in server_addr;
	char *str_data;
	int data_len=0, bytes_sent;

	// convert AIF to Socproto
	str_data = AIFToSocProto(tosend);
	
	strcpy(opset_name, other_app.rank[opset_rank]->nid);

	DVPRINT("%d - %s\n%s\n", opset_rank, other_app.rank[opset_rank]->nid, opset_name);

	host = gethostbyname(opset_name);

	if (host == (struct hostent *)NULL)
	{
		DVPRINT("Could not find name of %s\n", opset_name);
		return;
	}

	server_addr.sin_family = AF_INET;     
	server_addr.sin_port = htons(other_app.rank[opset_rank]->port);
	server_addr.sin_addr = *((struct in_addr *)host->h_addr);
	bzero(&(server_addr.sin_zero),8); 

	DVPRINT("Trying to connect %s:%d\n", other_app.rank[opset_rank]->nid, other_app.rank[opset_rank]->port);
	while (connected < 0 )
	{
		connected = connect(sock_fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr));
		/*
		if (connected < 0)
		{
			switch (errno) {
				case EADDRNOTAVAIL:
					DVPRINT("The specified address is not available from the local machine.\n");
					break;
				case EAFNOSUPPORT:
					DVPRINT("The specified address is not a valid address for the address family of the specified socket.\n");
					break;
				case EALREADY:
					DVPRINT("A connection request is already in progress for the specified socket.\n");
					break;
				case EBADF:
					DVPRINT("The socket argument is not a valid file descriptor.\n");
					break;
				case ECONNREFUSED:
					DVPRINT("The target address was not listening for connections or refused the connection request.\n");
					break;
				case EINPROGRESS:
					DVPRINT("O_NONBLOCK is set for the file descriptor for the socket and the connection cannot be immediately established; the connection shall be established asynchronously.\n");
					break;
				case EINTR:
				DVPRINT("The attempt to establish a connection was interrupted by delivery of a signal that was caught; the connection shall be established asynchronously.\n");
					break;
				case EISCONN:
					DVPRINT("The specified socket is connection-mode and is already connected.\n");
					break;
				case ENETUNREACH:
					DVPRINT("No route to the network is present.\n");
					break;
				case ENOTSOCK:
					DVPRINT("The socket argument does not refer to a socket.\n");
					break;
				case EPROTOTYPE:
					DVPRINT("The specified address has a different type than the socket bound to the specified peer address\n");
					break;
				case ETIMEDOUT:
					DVPRINT("The attempt to connect timed out before a connection was made.\n");
					break;
			}
		}
		*/
	}
	DVPRINT("Successfully connect to %s:%d\n", other_app.rank[opset_rank]->nid, other_app.rank[opset_rank]->port);

	// now send how much data will be sent
	data_len = strlen(str_data);
	bytes_sent = send(sock_fd, (void *)&data_len, sizeof(int), 0);
	if (bytes_sent < int(sizeof(int))) {
		DVPRINT("bad send: %d\n", (int)sizeof(int));
		goto returnblock;
	}
	// now send the data
	bytes_sent = send(sock_fd, str_data, strlen(str_data), 0);
	if (bytes_sent<int(strlen(str_data))) {
		DVPRINT("bad send: %d\n", (int)strlen(str_data));
		goto returnblock;
	}

returnblock:
	// now close socket and reopen it for another connection
	close (sock_fd);
	if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("Socket");
    }

	Free(str_data);

	return;
}

int
ReceiverSetSocketPort(void)
{
	int true_bool = 1;  
	struct sockaddr_in server_addr;
	int port = my_rank+10000;

	if (setsockopt(sock_fd,SOL_SOCKET,SO_REUSEADDR,&true_bool,sizeof(int)) == -1)
	{
		DVPRINT("in SliceSocketReceive: Setsockopt\n");
		return -1;
	}
 
	server_addr.sin_family = AF_INET;         
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = INADDR_ANY; 
	bzero(&(server_addr.sin_zero),8); 

	if (bind(sock_fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1)
	{
		DVPRINT("in SliceSocketReceive: Unable to bind\n");
		return -1;
	}

	return 0;
}

AIF *
SliceSocketReceive(void)
{
	int connected, bytes_received;
	struct sockaddr_in client_addr;
	char *str_data=NULL;
	AIF * torecv;
	socklen_t sin_size;
	int data_len=0;
	// used only for debug
	int port = my_rank+10000;

	if (listen(sock_fd, 5) == -1)
	{
		DVPRINT("SliceSocketReceive: Listen\n");
		return (AIF *)NULL;
	}
	
	DVPRINT("SliceSocketReceive: TCPServer Waiting for client on port %d\n", port);

	sin_size = sizeof(struct sockaddr_in);
	connected= -1;
	while( connected < 0 )
	{
		connected = accept(sock_fd, (struct sockaddr *)&client_addr,&sin_size);
		/*
		if (connected < 0)
		{
			switch (errno) {
			case EAGAIN:
				DVPRINT("O_NONBLOCK is set for the socket file descriptor and no connections are present to be accepted.\n");
				break;
			case EBADF:
				DVPRINT("Argument is not a valid file descriptor\n");
				break;
			case ECONNABORTED:
				DVPRINT("A connection has been aborted.\n");
				break;
			case EINTR:
				DVPRINT("The accept() function was interrupted by a signal that was caught before a valid connection arrived.\n");
				break;
			case EINVAL:
				DVPRINT("The socket is not accepting connections\n");
				break;
			case EMFILE:
				DVPRINT("{OPEN_MAX} file descriptors are currently open in the calling process.\n");
				break;
			case ENFILE:
				DVPRINT("The maximum number of file descriptors in the system are already open.\n");
				break;
			case ENOTSOCK:
				DVPRINT("The socket argument does not refer to a socket.\n");
				break;
			case EOPNOTSUPP:
				DVPRINT("The socket type of the specified socket does not support accepting connections.\n");
				break;
			}
		}
		*/
	}
	DVPRINT("Got a connection from (%s , %d)\n",
                   inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));

	// need to receive how much data will be received
	bytes_received = recv(connected, (void *)&data_len, sizeof(int), 0);
	if (bytes_received <= 0)
	{
		DVPRINT("in SliceSocketReceive: bad receive %d\n", bytes_received);
		close(connected);
		return (AIF *)NULL;
	}

	str_data = (char *)Alloc(data_len);
	bytes_received = recv(connected, str_data, data_len, 0);

	if (bytes_received <= 0)
	{
		DVPRINT("in SliceSocketReceive: bad receive %d\n", bytes_received);
		close(connected);
		Free(str_data);
		return (AIF *)NULL;
	}

	//convert socProto to AIF
	torecv = SocProtoToAIF(str_data);

	close(connected);
	Free(str_data);

	return torecv;
}
void
_fix_fortran_blockmap_info(blockmap_info *my_bm)
{
	int i, j;
	int tmpbuf[10];

	// fix value of other blockmap attributes
	j=0;
	for (i=my_bm->rank-1; 0<=i; --i)
	{
		tmpbuf[i] = my_bm->m_data[j];
		j++;
	}
	for(i=0; i<my_bm->rank; i++)
		my_bm->m_data[i] = tmpbuf[i];

	j=0;
	for (i=my_bm->rank-1; 0<=i; --i)
	{
		tmpbuf[i] = my_bm->m_distribute[j];
		j++;
	}
	for(i=0; i<my_bm->rank; i++)
		my_bm->m_distribute[i] = tmpbuf[i];

	j=0;
	for (i=my_bm->rank-1; 0<=i; --i)
	{
		tmpbuf[i] = my_bm->m_procs[j];
		j++;
	}
	for(i=0; i<my_bm->rank; i++)
		my_bm->m_procs[i] = tmpbuf[i];

	j=0;
	for (i=my_bm->rank-1; 0<=i; --i)
	{
		tmpbuf[i] = my_bm->m_blockcyclic_val[j];
		j++;
	}
	for(i=0; i<my_bm->rank; i++)
		my_bm->m_blockcyclic_val[i] = tmpbuf[i];

	j=0;
	for (i=my_bm->rank-1; 0<=i; --i)
	{
		tmpbuf[i] = my_bm->m_dim_order[j];
		j++;
	}
	for(i=0; i<my_bm->rank; i++)
		my_bm->m_dim_order[i] = tmpbuf[i];

	_get_order_map(my_bm->rank, my_bm->m_dim_order, my_bm->order_map);

	return;
}


/*
** Called to perform a comparison and display the result when the
** dataflow interpreter is executing.
**
** Returns 1 if interpreter is to continue, 0 to stop the interpreter.
**
** g - the graph that is executing
**
** id - the assertion id. Can be used to associate assertion specific
**      data with the comparison. For example this might be used to
**      associate upper and lower bounds, display type and an output file
**      with the assertion.
**
** name - the name of the variable being compared.
**
** data - the data being compared.
**
** compare operator 
**
** FIXME: This is broken as of the AIFCompare change.
*/
int
P2PCompareData(AIF *data1, AIF *data2, double a_lower, double a_upper, int cmpop)
{
	int		res;
	int		type;
	AIF *	ar;
	AIF *	abs_ar;
	AIF *	tmp_ar;

	AIF *aiflower = DoubleToAIF(a_lower);
	AIF *aifupper = DoubleToAIF(a_upper);

	/*if ( (ar = AIFDiff(0, data1, data2)) == (AIF *)NULL )
	{
			DVPRINT("copare problem 0\n");
		DVPRINT("AIFDiff Error: %s\n", AIFErrorStr());
		AIFFree(aiflower);
		AIFFree(aifupper);
		return 0;
	}*/

	// Now check if the operator is not just equal sign
	if (cmpop != EXPR_OPEQUAL)
	{
		// backup the differences
		tmp_ar = CopyAIF(ar);
		AIFFree(ar);
		
		//ar = AIFBool(tmp_ar, cmpop, &res);
		
		// want 1 to exit, 0 to continue, due to logic in AIFEPS
		// AIFBool returns 1 to continue, 0 to exit.
		AIFFree(tmp_ar);
		AIFFree(ar);
		AIFFree(aiflower);
		AIFFree(aifupper);

		return res;
	}

	type = AIFType(ar);
		
	switch (type)
	{
	case AIF_ARRAY:
	case AIF_FTN_ARRAY:
		type = AIFBaseType(ar);
		break;
	        
	default:
		break;
	}

	switch (type)
	{
		case AIF_FLOATING:
		// FIXME: This *really* needs to be fixed. I cannot figure out
		// why AIFEPS is not working for the fortran types, especially
		// arrays.
		case AIF_FTN_REAL:
		case AIF_FTN_COMPLEX:
			DVPRINT("receiver compare real/complex or float\n");
			// calculate the absolute value of ar
			if ((abs_ar = AIFAbs(ar)) == (AIF *)NULL)
			{
				DVPRINT("copare problem 1\n");
				PrintOutError("AIFAbs Error: %s\n", AIFErrorStr());
				return 1;
			}
 
			/*if ( AIFEPS(aiflower, aifupper, abs_ar, &res) < 0 )
			{
				DVPRINT("copare problem 2\n");
				PrintOutError("AIFEPS Error: %s\n", AIFErrorStr());
				AIFFree(ar);
				AIFFree(abs_ar);
				AIFFree(aiflower);
				AIFFree(aifupper);
				return 1;
			}*/		        
			AIFFree(abs_ar);
			break;
		default:
			/*if ( AIFCompare(0, data1, data2, &res) < 0 )
			{
				DVPRINT("copare problem 3\n");
				PrintOutError("AIFCompare Error: %s\n", AIFErrorStr());
				AIFFree(ar);
				return 0;
			}*/
			if (res == 0)	res = -1;
			break;
	}
	
	AIFFree(aiflower);
	AIFFree(aifupper);
	AIFFree(ar);
 
	// need to return 1 to continue, 0 to exit. Its opposite of what we actually have
	// due to AIFEPS logic.
	return res;
}

AIF *
PerformP2P(AIF *data, char *args, int my_rank, int *is_sender)
{
	char my_isym[BUFSIZ]="";
	blockmap_info my_bm, o_bm;
	int *overlap_info=NULL;
	int *my_data_size=NULL;
	int *o_data_size=NULL;
	int *my_pid=NULL;

	int i;
	int des = -1;
	int num_blocks;
	int isFortran = 0;

	AIF * res;

	ParseP2PArgs(args, my_isym, is_sender, &my_bm, &o_bm);

	overlap_info = (int*)Alloc(sizeof(int)*my_bm.rank);
	my_data_size = (int*)Alloc(sizeof(int)*my_bm.rank);
	o_data_size = (int*)Alloc(sizeof(int)*my_bm.rank);
	my_pid = (int*)Alloc(sizeof(int)*my_bm.rank);

	if (AIFType(data) == AIF_FTN_ARRAY) {
		_fix_fortran_blockmap_info(&my_bm);
		_fix_fortran_blockmap_info(&o_bm);
		isFortran = 1;
	}

	_get_overlap_info(overlap_info, my_bm, o_bm);
	_get_pid_grid(my_bm.rank, my_rank, my_bm.m_procs, my_bm.order_map, my_pid);

	for (i=0; i<my_bm.rank; i++) {
		my_data_size[i] = my_bm.m_data[i] / my_bm.m_procs[i];
		o_data_size[i] = o_bm.m_data[i] / o_bm.m_procs[i];
	}

	// call a function from blockmap.c
	num_blocks = _get_num_blocks(my_bm.rank, my_data_size, overlap_info);

	// work out info for sender
	if (*is_sender)
	{
		char sliceinfo[BUFSIZ]="";
		int **global_start_locs = NULL;
		AIF *tosend;

		DVPRINT("%s[%d] is a sender\n", my_isym, my_rank);

		global_start_locs = (int**)Alloc(sizeof(int*)*num_blocks);
		for (i=0; i<num_blocks; i++) {
			global_start_locs[i] = (int*)Alloc(sizeof(int)*my_bm.rank);
		}

		// find all global_start_locs
		_get_global_start_locs(my_bm.rank, num_blocks, overlap_info, my_pid,  my_bm.order_map, my_bm.m_procs, my_bm.m_data, my_data_size, global_start_locs);

		for(i=0;i<num_blocks; i++)
		{
			// find slicing info and destination for the slice
			_get_slice_info(my_data_size, overlap_info, sliceinfo, i, my_bm.rank, isFortran);

			des = _get_slice_destination(my_bm.rank, global_start_locs[i], o_data_size, o_bm.m_procs, o_bm.order_map);
			DVPRINT("sending to %d\n", des);

			// perform slicing
			tosend = PerformSlicing(data, sliceinfo, 0);

			// perform peer communication
			SliceSocketSend(des, tosend);

			AIFFree(tosend);
		}

		// just return an empty AIF
		res = NewAIF(0,0);
		DVPRINT("sender finishes\n");
	}
	else
	{
		int *diffs, min, max;
		char sliceinfo[BUFSIZ]="";
		char data_str[BUFSIZ];
		char *dest = data_str;

		min = 0;
		max = num_blocks - 1;

		DVPRINT("%s[%d] is a receiver\n", my_isym, my_rank);

		diffs = (int *)Alloc(sizeof(int)*num_blocks);

		ReceiverSetSocketPort();

		for(i=0;i<num_blocks; i++)
		{
			AIF * torecv;
			AIF * tocompare;
			char *datastr;

			// perform peer communication
			torecv = SliceSocketReceive();
			AIFToStr(&datastr, 0, torecv);

			// get the right slice for comparing
			_get_slice_info(my_data_size, overlap_info, sliceinfo, i, my_bm.rank, isFortran);
			tocompare = PerformSlicing(data, sliceinfo, 0);

			AIFToStr(&datastr, 0, tocompare);

			// perform comparison
			diffs[i] = P2PCompareData(tocompare, torecv, my_bm.a_lower, my_bm.a_upper, EXPR_OPEQUAL);

			AIFFree(torecv);
			AIFFree(tocompare);
			Free(datastr);
		}

		// convert the result array into AIF
		for (i=0; i<num_blocks; i++)
		{
			AIFNormalise(dest, sizeof(int), (char *) (diffs+i), sizeof(int));
			dest += sizeof(int);
		}
		res = ArrayToAIF(1, &min, &max, data_str, num_blocks*sizeof(int), AIF_FORMAT(IntToAIF(0)));

		Free(diffs);

	}

	Free(overlap_info);
	Free(my_data_size);
	Free(o_data_size);

	DVPRINT("finish PerformP2P\n");

	return res;
}
