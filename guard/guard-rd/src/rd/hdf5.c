/*
** Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
** See COPYRIGHT in top-level directory.
**
** HDF5 handling routines
**
*/

#ifdef HAVE_CONFIG_H
#include	<config.h>
#endif /* HAVE_CONFIG_H */

#ifdef HDF5

#include	<sys/param.h>

#include        <stdint.h>
#include	<stdio.h>
#include	<string.h>
#include	<hdf5.h>

#include	"defs.h"
#include	"useful.h"
#include	"aif.h"
#include        "client.h"

void
HDF5Dump(char *file, char *var, AIF *a)
{
	int			i;
	int			off;
	int *			id;
	int32_t			rank;
	int32_t *		sizes;
	int32_t *		min;
	int32_t *		max;
	char *			s;
	char *			btype;
	char *			ftype;
	char *			fname;
	char			buf[MAXPATHLEN];
	hid_t			fid;
	hid_t			spid;
	hid_t			sid;
	hid_t			tid;
	hid_t			ftid;
	herr_t			status;
	hentry_t *		h;
	static htable_t *	ntab;

	if ( AIFType(a) != AIF_ARRAY )
	{
		PrintOutError("HDF5Dump: Data type must be an array\n");
		return;
	}

	if ( ntab == (htable_t *)NULL )
		ntab = HashCreate(7);

	FDSArrayInfo(AIF_FORMAT(a), &rank, &btype, NULL);

	switch ( FDSType(btype) )
	{
	case AIF_INTEGER:
		if ( (FDSTypeSize(btype) > 4) )
			tid = H5Tcopy(H5T_NATIVE_INT64);
		else
			tid = H5Tcopy(H5T_NATIVE_INT32);

		status = H5Tset_order(tid, H5T_ORDER_BE);
		break;

	case AIF_FLOATING:
		if ( (FDSTypeSize(btype) > 4) )
			tid = H5Tcopy(H5T_NATIVE_DOUBLE);
		else
			tid = H5Tcopy(H5T_NATIVE_FLOAT);

		status = H5Tset_order(tid, H5T_ORDER_BE);
		break;

	case AIF_STRUCT:
		tid = H5Tcreate (H5T_COMPOUND, FDSTypeSize(btype));

		off = 0;

		for ( i = 0 ; i < FDSNumFields(btype) ; i++ )
		{
			FDSStructFieldByNumber(btype, i, &fname, &ftype);

			switch ( FDSType(ftype) )
			{
			case AIF_INTEGER:
				if ( FDSTypeSize(ftype) > 4 )
					ftid = H5Tcopy(H5T_NATIVE_INT64);
				else
					ftid = H5Tcopy(H5T_NATIVE_INT32);

				status = H5Tset_order(tid, H5T_ORDER_BE);
				break;

			case AIF_FLOATING:
				if ( FDSTypeSize(ftype) > 4 )
					ftid = H5Tcopy(H5T_NATIVE_DOUBLE);
				else
					ftid = H5Tcopy(H5T_NATIVE_FLOAT);

				status = H5Tset_order(tid, H5T_ORDER_BE);
				break;
			
			default:
				PrintOutError("HDF5Dump: complex structures not supported\n");
				H5Tclose(tid);
				return;

			}

			status = H5Tinsert(tid, fname, off, ftid);

			off += FDSTypeSize(ftype) * 8;

		}
		break;

	default:
		PrintOutError("HDF5Dump: data type not supported\n");
		H5Tclose(tid);
		return;
	}

	if ( (h = HashSearch(ntab, file)) == (hentry_t *)NULL )
	{
		id = (int *)Alloc(sizeof(int));
		*id = 0;
		HashInsert(ntab, file, (void *)id);
	}
	else
		id = (int *)h->h_data;

	if ( (s = strrchr(file, '.')) != NULL )
	{
		*s++ = '\0';
		sprintf(buf, "%s-%d.%s", file, *id, s);
	}
	else
		sprintf(buf, "%s-%d", file, *id);

	(*id)++;

	fid = H5Fcreate(buf, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

	FDSArrayBounds(AIF_FORMAT(a), rank, &min, &max, &sizes);

	spid = H5Pcreate_simple(rank, sizes, NULL);

	sid = H5Dcreate(fid, var, tid, spid, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

	status = H5Dwrite(sid, tid, H5S_ALL, H5S_ALL, H5P_DEFAULT, (void *)AIF_DATA(a));

	H5Tclose(tid);
	H5Pclose(spid);
	H5Dclose(sid);
	H5Fclose(fid);
			
	Free(min);
	Free(max);
	Free(sizes);
}

#endif /* HDF5 */
