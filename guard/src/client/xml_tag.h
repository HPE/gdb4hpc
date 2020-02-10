/******************************************************************************\
 * xml_tag.h - XML tag definitions.
 *
 * © 2013,2020 Cray Inc.
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
 ******************************************************************************/

#ifndef _XML_TAG_H
#define _XML_TAG_H

#define XML_MAX_SEQ 256

#define XML_REQUEST_START "<request seq=\"%d\" %n"
#define XML_REQUEST_END " />\n"

#define XML_SYNC "<sync seq=\"%d\" />\n"

#define XML_RESPONSE_START "<response seq=\"%d\">\n"
#define XML_RESPONSE_END "</response>\n"
#define XML_RESPONSE_ERROR "<error>\n"
#define XML_RESPONSE_ERROR_END "\n</error>\n"

#define XML_PROG_OUTPUT_START "<output app=\"$%s\" len=\"%u\">\n"
#define XML_PROG_OUTPUT_END "</output>\n"

#define XML_RESULT \
    "<result script=\"%s\" id=\"%d\" Pass: %d Warn: %d Fail: %d />\n"

#define XML_STATUS_START "<status set=\"%s\">\n"
#define XML_STATUS_INIT "%s Initializing\n"
#define XML_STATUS_WAITING "%s Waiting %s %s:%d\n"
#define XML_STATUS_RUNNING "%s Running\n"
#define XML_STATUS_STOPPED "%s Stopped %s %s:%d\n"
#define XML_STATUS_EXIT "%s Exit %s %s:%d\n"
#define XML_STATUS_ERROR "%s Error\n"
#define XML_STATUS_GRAPH "%s Graph\n"
#define XML_STATUS_END "</status>\n"

#define XML_REMOVED "<removed set=\"%s\" />\n"

#endif /* _XML_TAG_H */
