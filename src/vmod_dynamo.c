#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>

#include "vcl.h"
#include "vrt.h"
#include "cache/cache.h"

#include "vcc_if.h"

static size_t
find_query_value(char *qs, const char* name, char* buffer)
{
	if (qs == NULL || *qs == '\0')
		return (0);

	if (name == NULL || *name == '\0')
		return (0);

	int nl, vl;
	nl = strlen(name);

	qs = strstr(qs, name);
	
	while (qs != NULL && qs[nl] != '=' && qs[nl] != '\0') {
		qs = qs + nl;
		qs = strstr(qs, name);
	}

	/* Check for a missing param, or an empty value */
	 if (qs == NULL 
	 	|| qs[nl] != '=' 
	 	|| qs[nl + 1] == '\0' 
	 	|| qs[nl + 1] == '&') {
	 	return (0);
	 }

	/* Update the search pointer to point at just the value */
	 qs = qs + nl + 1;

	 vl = 0;
	 while (qs[0] != '\0' && qs[0] != '&') {
	 	buffer[vl] = qs[0];
	 	++vl;
	 	++qs;
	 }

	 buffer[vl] = '\0';

	 return (vl);
}

VCL_STRING
vmod_query_get(VRT_CTX, VCL_STRING url, VCL_STRING name)
{
	char *cq;
	int s;
	char* b;

	CHECK_OBJ_NOTNULL(ctx, VRT_CTX_MAGIC);

	if (url == NULL || name == NULL)
		return (NULL);

	/* Make sure we have a valid query string */
	cq = strchr(url, '?');
	if (cq == NULL)
		return (NULL);

	WS_Reserve(ctx->ws, 0);

	b = ctx->ws->f;

	s = find_query_value(++cq, name, ctx->ws->f);

	if (s <= 0) {
		WS_Release(ctx->ws, 0);
		return (NULL);
	}
	WS_Release(ctx->ws, s);
	return (b);
}
