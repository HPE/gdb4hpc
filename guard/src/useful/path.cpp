/*********************************************************************************\
 * path.c - Functions relating to searching and setting path variables.
 *
 * © 2011-2014 Cray Inc.  All Rights Reserved.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Cray Inc.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 *********************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <strings.h>

#include <sys/types.h>
#include <sys/stat.h>

#include "path.h"

// external error printing function
extern void	PrintOutError(char *, ...);

/*
 * Try to locate 'file' using PATH.
 *
 * Return pointer to path/file that can be opened, or NULL on failure.
 *
 * It is the responsiblity of the caller to free the returned buffer when done.
 */
char *
pathFind(const char *file, const char *envPath) 
{
	struct stat stat_buf;
	char	buf[PATH_MAX];
	char	*path;
	char	*tmp;
	char	*p_entry;
	char	*savePtr = NULL;
	char	*retval;

	if (file == (char *)NULL) 
	{
		return (char *)NULL;
	}

	/* Check for possible relative or absolute path */
	if (file[0] == '.' || file[0] == '/') 
	{
		if (stat(file, &stat_buf) == 0) 
		{
			// stat resolves symbolic links
			if (!S_ISREG(stat_buf.st_mode))
			{
				/* can't execute a directory, or special files */
				return (char *)NULL;
			} else
			{
				return strdup(file);
			}
		} else 
		{
			/* can't access file */
			return (char *)NULL;
		}
	}

	if (envPath == NULL) 
	{
		// default to using PATH
		envPath = "PATH";
	}

	if ((tmp = getenv(envPath)) == (char *)NULL)
	{
		/* nothing in path to search */
		PrintOutError("Could not getenv %s.\n", envPath);
		return (char *)NULL;
	}
	path = strdup(tmp);

	/*
	* Start searching the colon-delimited PATH, prepending each
	* directory and checking to see if stat succeeds
	*/
	// grab the first p_entry in the path
	p_entry = strtok_r(path, ":", &savePtr);
	while (p_entry != NULL) 
	{
		// create the full path string
		snprintf(buf, PATH_MAX+1, "%s/%s", p_entry, file);
		// check to see if we can stat it
		if (stat(buf, &stat_buf) == 0) 
		{
			// we can stat it so make sure its a regular file.
			if ((stat_buf.st_mode & S_IFMT) == S_IFREG)
			{
				retval = strdup(buf);
				free(path);
				return retval;
			}
		}
		// grab the next p_entry in the path
		p_entry = strtok_r(NULL, ":", &savePtr);
	}

	free(path);
	
	// not found
	return (char *)NULL;
}

/*
 * Try to locate 'library' using LD_LIBRARY_PATH.
 *
 * Return pointer to path/library that can be opened, or NULL on failure.
 *
 * It is the responsiblity of the caller to free the returned buffer when done.
 */
char *
libFind(const char *file, const char *envPath)
{
	struct stat stat_buf;
	char	buf[PATH_MAX];
	char	*path;
	char	*extraPath = NULL;
	char	*tmp;
	char	*p_entry;
	char	*savePtr = NULL;
	char	*retval;
	
	/* Check for possible relative or absolute path */
	if (file[0] == '.' || file[0] == '/') 
	{
		if (stat(file, &stat_buf) == 0) 
		{
			if (!S_ISREG(stat_buf.st_mode))
			{
				/* can't execute a directory, or special files */
				return (char *)NULL;
			} else
			{
				return strdup(file);
			}
		} else 
		{
			/* can't access file */
			return (char *)NULL;
		}
	}
	
	if (envPath == NULL) 
	{
		// default to using LD_LIBRARY_PATH
		envPath = "LD_LIBRARY_PATH";
		extraPath = strdup(EXTRA_LIBRARY_PATH);
	}
	
	if ((tmp = getenv(envPath)) == (char *)NULL)
	{
		/* nothing in path to search */
		PrintOutError("Could not getenv %s.\n", envPath);
		return (char *)NULL;
	}
	path = strdup(tmp);
	
	/*
	* Start searching the colon-delimited PATH, prepending each
	* directory and checking to see if stat succeeds
	*/
	// grab the first p_entry in the path
	p_entry = strtok_r(path, ":", &savePtr);
	while (p_entry != NULL) 
	{
		// create the full path string
		snprintf(buf, PATH_MAX+1, "%s/%s", p_entry, file);
		// check to see if we can stat it
		if (stat(buf, &stat_buf) == 0) 
		{
			// we can stat it so make sure its a regular file.
			if ((stat_buf.st_mode & S_IFMT) == S_IFREG)
			{
				retval = strdup(buf);
				free(path);
				return retval;
			}
		}
		// grab the next p_entry in the path
		p_entry = strtok_r(NULL, ":", &savePtr);
	}
	
	free(path);
	
	// see if there is an extraPath defined, if not the file was not found in PATH
	if (extraPath == NULL) 
	{
		return (char *)NULL;
	}
	
	/*
	* Search the additional path for the file
	*/
	p_entry = strtok_r(extraPath, ":", &savePtr);
	while (p_entry != NULL) 
	{
		sprintf(buf, "%s/%s", p_entry, file);
		if (stat(buf, &stat_buf) == 0) 
		{
			// we can stat it so make sure its a regular file or sym link.
			if (S_ISREG(stat_buf.st_mode)) 
			{
				retval = strdup(buf);
				free(extraPath);
				return retval;
			}
		} 
		p_entry = strtok_r(NULL, ":", &savePtr);
	}
	
	free(extraPath);
	
	// not found
	return (char *)NULL;
}

/*
 * Add the path to LD_LIBRARY_PATH.
 */
int
adjustLibPath(const char *path)
{
	char *oldpath, *newpath;
	int len;
	
	// sanity check
	if (path == NULL)
		return 1;
	
	// add path to the LD_LIBRARY_PATH variable
	if ((oldpath = getenv("LD_LIBRARY_PATH")) == NULL)
	{
		len = strlen(path) + 1;
		if ((newpath = (char*)malloc(len*sizeof(char))) == NULL)
		{
			return 1;
		}
		snprintf(newpath, len, "%s", path);
	} else
	{
		len = strlen(oldpath) + strlen(":") + strlen(path) + 1;
		if ((newpath = (char*)malloc(len*sizeof(char))) == NULL)
		{
			return 1;
		}
		snprintf(newpath, len, "%s:%s", path, oldpath);
	}
	
	if (setenv("LD_LIBRARY_PATH", newpath, 1) != 0)
	{
		free(newpath);
		return 1;
	}
	
	free(newpath);
	
	return 0;
}

/*
 * Add the path to LD_LIBRARY_PATH.
 */
int
adjustExecPath(const char *path)
{
	char *oldpath, *newpath;
	int len;
	
	// sanity check
	if (path == NULL)
		return 1;
	
	// add path to the PATH variable
	if ((oldpath = getenv("PATH")) == NULL)
	{
		len = strlen(path) + 1;
		if ((newpath = (char*)malloc(len*sizeof(char))) == NULL)
		{
			return 1;
		}
		snprintf(newpath, len, "%s", path);
	} else
	{
		len = strlen(oldpath) + strlen(":") + strlen(path) + 1;
		if ((newpath = (char*)malloc(len*sizeof(char))) == NULL)
		{
			return 1;
		}
		snprintf(newpath, len, "%s:%s", path, oldpath);
	}
	
	if (setenv("PATH", newpath, 1) != 0)
	{
		free(newpath);
		return 1;
	}
	
	free(newpath);
	
	return 0;
}

char *
getinstallbase(void)
{
	pid_t			pid;
	char			linkname[64]; // /proc/<pid>/exe
	char *			exe_path = NULL;
	char *			tmp;
	char *			exe_name;
	char *			needle = NULL;
	char *			ptr;
	int				ret;
	
	// get pid
	pid = getpid();
	
	snprintf(linkname, sizeof(linkname), "/proc/%i/exe", pid);
	
	if ((exe_path = (char*)malloc(PATH_MAX + 1)) == NULL)
	{
		return NULL;
	}
	memset(exe_path, '\0', PATH_MAX + 1);
	
	ret = readlink(linkname, exe_path, PATH_MAX + 1);
	
	if (ret == -1)
	{
		free(exe_path);
		return NULL;
	}
	
	if (ret > PATH_MAX)
	{
		free(exe_path);
		return NULL;
	}
	
	exe_path[PATH_MAX] = '\0';
	
	// get the basename of the executable and remove it from the path
	tmp = strdup(exe_path);
	exe_name = basename(tmp);
	if (asprintf(&needle, "/bin/%s", exe_name) == -1)
	{
		free(tmp);
		free(exe_path);
		return NULL;
	}
	if ((ptr = strstr(exe_path, needle)) == NULL)
	{
		// Try checking in /libexec too since this could be a daemon calling the
		// function
		free(needle);
		needle = NULL;
		if (asprintf(&needle, "/libexec/%s", exe_name) == -1)
		{
			free(tmp);
			free(exe_path);
			return NULL;
		}
		if ((ptr = strstr(exe_path, needle)) == NULL)
		{
			free(needle);
			free(tmp);
			free(exe_path);
			return NULL;
		}
	}
	free(tmp);
	free(needle);
	// set the start of the substring to null
	*ptr = '\0';
	
	return exe_path;
}

char *
getexepath(void)
{
	pid_t	pid;
	char	linkname[64]; // /proc/<pid>/exe
	char *	exe_path = NULL;
	int		ret;
	
	// get pid
	pid = getpid();
	
	snprintf(linkname, sizeof(linkname), "/proc/%i/exe", pid);
	
	if ((exe_path = (char*)malloc(PATH_MAX + 1)) == NULL)
	{
		return NULL;
	}
	memset(exe_path, '\0', PATH_MAX + 1);
	
	ret = readlink(linkname, exe_path, PATH_MAX + 1);
	
	if (ret == -1)
	{
		free(exe_path);
		return NULL;
	}
	
	if (ret > PATH_MAX)
	{
		free(exe_path);
		return NULL;
	}
	
	exe_path[PATH_MAX] = '\0';
	
	return exe_path;
}

