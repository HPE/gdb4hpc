/*********************************************************************************\
 * Guard debugger
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * © 2010-2013 Cray Inc.  All Rights Reserved.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Cray Inc.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 * $HeadURL$
 * $Date$
 * $Rev$
 * $Author$
 *
 **********************************************************************************/

#ifdef HAVE_CONFIG_H
#include	<config.h>
#endif /* HAVE_CONFIG_H */

#include	<string.h>
#include	<unistd.h>

#include	"client.h"
#include	"useful.h"
#include	"df.h"
#include	"rd.h"
#include	"ainfo.h"

#ifdef HDF
extern void	HDFDump(char *, char *, AIF *);
#endif /* HDF */
#ifdef HDF5	
extern void	HDF5Dump(char *, char *, AIF *); 
#endif /* HDF5 */

enum class ComparisonResult { PASS, FAIL, WARN, INDETERMINATE, UNKNOWN };

static ComparisonResult OpResult(int aifResult, int cmpop)
{
	if (aifResult == 3)
		return ComparisonResult::INDETERMINATE;
	else if (aifResult == -2 || aifResult == 2)
		return ComparisonResult::WARN;

	// the aifResult is reversed from the expected:
	// -1: A > B
	//  0: A == B
	//  1: A < B
	bool pass = true;
	switch (cmpop) {
	case EXPR_OPNOTEQUAL:
		pass = (aifResult != 0);
		break;
	case EXPR_OPLEQ:
		pass = (aifResult != -1);
		break;
	case EXPR_OPGEQ:
		pass = (aifResult != 1);
		break;
	case EXPR_OPLT:
		pass = (aifResult == 1);
		break;
	case EXPR_OPGT:
		pass = (aifResult == -1);
		break;
	case EXPR_OPEQUAL:
		pass = (aifResult == 0);
		break;
	default:
		return ComparisonResult::UNKNOWN;
	}
	return (pass ? ComparisonResult::PASS : ComparisonResult::FAIL);
}

/*
** Called to perform a comParison and display the result when the
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
*/
int
CompareData(graph_t *g, int id, char *name1, AIF *data1, char *name2, AIF *data2, int cmpop)
{
	int			rtn = 1;
	int			aifResult;
	AIF *		ar = NULL;
	ainfo_t *	ai;

	if ( (ai = FindAssertInfo(g->g_assert_info_tab, id)) == (ainfo_t *)NULL )
	{
		PrintOutError("No assertion information found!\n");
		return 0;
	}
	
#ifdef DEBUG
/*	fprintf(Client.cli_out_stream, "data1 is:\n");
	AIFPrint(Client.cli_out_stream, 0, data1);
	fprintf(Client.cli_out_stream, "\ndata2 is:\n");
	AIFPrint(Client.cli_out_stream, 0, data2);
	fprintf(Client.cli_out_stream, "\n");
	fflush(Client.cli_out_stream);*/
#endif

	// Ensure the error ranges get created
	if (ai->a_aifupper == NULL || ai->a_aiflower == NULL)
	{
		ai->a_aiflower = DoubleToAIF(ai->a_lower);
		ai->a_aifupper = DoubleToAIF(ai->a_upper);
	}
	
	// Call the compare function. This handles EPS/diff calculation for us.
	if (AIFCompare(data1, data2, &aifResult, &ar, ai->a_aiflower, ai->a_aifupper))
	{
		PrintOutError("AIFCompare Error: %s\n", AIFErrorStr());
		if (ai->a_stop)
			return 0;
		else
			return 1;
	}
	
	/* 
	** save result information that is independent from display type.
	*/ 
	ComparisonResult res = OpResult(aifResult, cmpop);
	switch (res)
	{
		case ComparisonResult::PASS:
			// no differences
			g->g_pass++;
			g->g_assert_pass[id]++;
			// continue on pass
			rtn = 1;
			break;
		case ComparisonResult::WARN:
			// between lo and hi
			g->g_warn++;
			g->g_assert_warn[id]++;
			NewGraphDiff(g, id, CopyAIF(ar), "Warning");
			// continue on warning
			rtn = 1;
			break;
		case ComparisonResult::FAIL:
			// fatal difference
			g->g_fail++;
			g->g_assert_fail[id]++;
			NewGraphDiff(g, id, CopyAIF(ar), "Failure");
			// if stop is true, we want to stop
			if (ai->a_stop)
				rtn = 0;
			else
				rtn = 1;
			break;
		case ComparisonResult::INDETERMINATE:
			PrintOutError("Warning: A function type, pointer, or some other type that has an \n");
			PrintOutError("         indeterminate comparison result was encountered.\n");
			PrintOutError("No comparison took place. Continuing graph execution.\n");
			break;
		case ComparisonResult::UNKNOWN:
			PrintOutError("Unknown difference result.\n");
			AIFFree(ar);
			if (ai->a_stop)
				return 0;
			else
				return 1;
	}
	
	// increment the iteration count
	// we only increment the count when a full assertion has completed its evauluation.
	g->g_iterations++;

	// Always display XML results if XML mode is on
	if (Client.XML_mode)
	{
		PrintOutLine(XML_RESULT, g->g_valname, id+1, g->g_assert_pass[id], g->g_assert_warn[id], g->g_assert_fail[id]);
	}

	if ( strcmp(ai->a_display, "none") == 0 )
	{
		AIFFree(ar);
		return rtn;
	}
	
	if ( strcmp(ai->a_display, "text") == 0 )
	{
		switch (res)
		{
			case ComparisonResult::WARN:
				// display a warning
				PrintOutLine("*** Tolerable difference found in AssertID %d ***\n", id+1);
				break;
			case ComparisonResult::FAIL:
				// display an error
				PrintOutLine("*** Difference found in AssertID %d ***\n", id+1);
/*
#ifdef DEBUG
	fprintf(Client.cli_out_stream, "data1 is:\n");
	AIFPrint(Client.cli_out_stream, 0, data1);
	fprintf(Client.cli_out_stream, "\ndata2 is:\n");
	AIFPrint(Client.cli_out_stream, 0, data2);
	fprintf(Client.cli_out_stream, "\n");
	fprintf(Client.cli_out_stream, "\ndiff is:\n");
	AIFPrint(Client.cli_out_stream, 0, ar);
	fprintf(Client.cli_out_stream, "\n");
	fflush(Client.cli_out_stream);
#endif
*/
				break;
			default:
				// do nothing
				break;
		}

		AIFFree(ar);
		return rtn;
	}

#ifdef OPENDX
	if ( strcmp(ai->a_display, "vis_opendx") == 0 )
	{
		GuardVisualization(ar, ai->a_display);
		AIFFree(ar);
		return rtn;
	}
#endif /* OPENDX */

	if ( ai->a_file == NULL )
	{
		PrintOutError("Output file has not been set.\n");
		AIFFree(ar);
		return rtn;
	}

#ifdef HDF
	if ( strcmp(ai->a_display, "hdf") == 0 )
	{
		HDFDump(ai->a_file, "", ar);
		AIFFree(ar);
		return rtn;
	}
#endif /* HDF */

#ifdef HDF5
	if ( strcmp(ai->a_display, "hdf5") == 0 )
	{
		HDF5Dump(ai->a_file, "", ar);
		AIFFree(ar);
		return rtn;
	}
#endif /* HDF5 */

	if ( strcmp(ai->a_display, "aif") == 0 )
	{
		AIFFILE *	af;

		if ( access(ai->a_file, F_OK) == 0 )
			af = AIFOpenSet(ai->a_file, AIFMODE_APPEND);
		else
			af = AIFOpenSet(ai->a_file, AIFMODE_CREATE);

		if ( af == (AIFFILE *)NULL )
		{
			PrintOutError("AIF Error: %s: %s\n", ai->a_file, AIFErrorStr());
			AIFFree(ar);
			return rtn;
		}

		AIFWriteSet(af, ar, "");
		AIFCloseSet(af);

		AIFFree(ar);
		return rtn;
	}

	PrintOutError("Invalid display type: \"%s\"\n", ai->a_display);

	AIFFree(ar);
	return rtn;
}

/*
** Called to display the result of the p2p operation when the
** dataflow interpreter is executing.
**
** Returns 1 if interpreter is to continue, 0 to stop the interpreter.
**
** g - the graph that is executing
**
** name - the name of the variable being compared.
**
** data - the data being compared.
**
** FIXME: This is not supported. It needs to be done properly... Stop this two
**        function non-sense. Integrate this with CompareData.
** FIXME: This doesn't support the AIF changes...
*/
int
DisplayP2P(graph_t *g, int id, char *name, AIF *data)
{
	int		res=0;
	AIF *	zero_aif = IntToAIF(0);
	ainfo_t *	ai;

	if ( (ai = FindAssertInfo(g->g_assert_info_tab, id)) == (ainfo_t *)NULL )
	{
		PrintOutError("No assertion information found!\n");
		return 0;
	}
	/*
	fprintf(Client.cli_out_stream, "%s: ", name);
	AIFPrint(Client.cli_out_stream, 0, data);
	fprintf(Client.cli_out_stream, "\n");
	*/
	//tmp = AIFCompare(0, data, zero_aif, &res);
	AIFFree(zero_aif);

	/* 
	** save result information that is independent from display type.
	** Note that res has three states. 
	** -1: No differences found
	** 0: Inside of the lo and hi precision limits.
	** 1: Outside hi precision limit, causes fatal difference.
	*/ 
	switch (res)
	{
	        case -1:
	                // no differences
	                g->g_pass++;
	                g->g_assert_pass[id]++;
	                break;
	        case 0:
	                // between lo and hi
	                g->g_warn++;
	                g->g_assert_warn[id]++;
	                NewGraphDiff(g, id, CopyAIF(data), "Warning");
	                break;
	        case 1:
	                // fatal difference
	                g->g_fail++;
	                g->g_assert_fail[id]++;
	                NewGraphDiff(g, id, CopyAIF(data), "Failure");
	                break;
	        default:
	                PrintOutError("Unknown difference result.\n");
	                return 0;
	}

	// increment the iteration count
	// we only increment the count when a full assertion has completed its evauluation.
	g->g_iterations++;

	if ( strcmp(ai->a_display, "none") == 0 )
	{
		return res;
	}
 
	if ( strcmp(ai->a_display, "text") == 0 )
	{
		switch (res)
		{
			case -1:
				// do nothing
				break;
			case 0:
				// display a warning
				PrintOutLine("*** Tolerable difference found ***\n"); // between %s and %s ***\n", name2, name1);
				break;
			case 1:
				PrintOutLine("*** Difference found ***\n"); //between %s and %s! ***\n", name2, name1);
				PrintOutLine("In AssertID:%d\n", id+1);
				break;
			default:
				// shouldn't get here.
				break;
		}

		return (res == 1) ? 0 : 1;	// correct one
	}

#ifdef OPENDX
	if ( strcmp(ai->a_display, "vis_opendx") == 0 )
	{
		GuardVisualization(ar, ai->a_display);
		return res;
	}
#endif /* OPENDX */

	if ( ai->a_file == NULL )
	{
		PrintOutError("Output file has not been set.\n");
		return 0;
	}

#ifdef HDF
	if ( strcmp(ai->a_display, "hdf") == 0 )
	{
		HDFDump(ai->a_file, "", ar);
		return res;
	}
#endif /* HDF */

#ifdef HDF5
	if ( strcmp(ai->a_display, "hdf5") == 0 )
	{
		HDF5Dump(ai->a_file, "", ar);
		return res;
	}
#endif /* HDF5 */

	if ( strcmp(ai->a_display, "aif") == 0 )
	{
		AIFFILE *	af;

		if ( access(ai->a_file, F_OK) == 0 )
			af = AIFOpenSet(ai->a_file, AIFMODE_APPEND);
		else
			af = AIFOpenSet(ai->a_file, AIFMODE_CREATE);

		if ( af == (AIFFILE *)NULL )
		{
			PrintOutError("%s: %s\n", ai->a_file, AIFErrorStr());
			return 0;
		}

		AIFWriteSet(af, data, "");
		AIFCloseSet(af);

		return res;
	}

	PrintOutError("Invalid display type: \"%s\"\n", ai->a_display);

	return 0;
}
/*
** Called to display the result of a comparison when the
** dataflow interpreter is executing.
*/
int
DisplayData(char *type, char *file, char *name, AIF *data)
{
	if ( strcmp(type, "none") == 0 )
		return 1;

	if ( strcmp(type, "text") == 0 )
	{
		PrintOutLine("The difference: ");
		AIFPrint(Client.cli_out_stream, 0, data);
		PrintOutLine("\n");
		return 1;
	}

	if ( file == NULL )
	{
		PrintOutError("Output file has not been set.\n");
		return 0;
	}

#ifdef HDF
	if ( strcmp(type, "hdf") == 0 )
	{
		HDFDump(file, name, data);
		return 1;
	}
#endif /* HDF */

#ifdef HDF5
	if ( strcmp(type, "hdf5") == 0 )
	{
		HDF5Dump(file, name, data);
		return 1;
	}
#endif /* HDF5 */

	if ( strcmp(type, "aif") == 0 )
	{
		AIFFILE *	af;

		PrintOutLine("Generating data for %s\n", name);

		if ( access(file, F_OK) == 0 )
			af = AIFOpenSet(file, AIFMODE_APPEND);
		else
			af = AIFOpenSet(file, AIFMODE_CREATE);

		if ( af == (AIFFILE *)NULL )
		{
			PrintOutError("AIF Error: %s: %s\n", file, AIFErrorStr());
			return 0;
		}

		AIFWriteSet(af, data, name);
		AIFCloseSet(af);

		return 1;
	}

	PrintOutError("Invalid display type: \"%s\"\n", type);

	return 0;
}
