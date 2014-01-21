/*********************************************************************************\
 * <message.hpp>
 *
 * © 2012-2013 Cray Inc.  All Rights Reserved.
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
 *********************************************************************************/



#ifndef _GUARD_SCI_MESSAGE_HPP
#define _GUARD_SCI_MESSAGE_HPP

#include <stdio.h>
#include <assert.h>
#include <string.h>

#include <vector>
#include <algorithm>

#include "bitset.h"

extern FILE* sci_filter_log;

using namespace std;

class SingleMsg 
{
private:
	int		     seq;
	int		     type;

	size_t	     bs_str_len;
	char		*bs_str;
	bitset		*bs;

	size_t	     content_len;
	char		*content;

public:
	SingleMsg() {
		seq = -1;
		type = -1;
		bs_str = NULL;
		bs = NULL;
		content_len = 0;
		content = NULL;
	}

	~SingleMsg() {
		if(bs != NULL)
			bitset_free(bs);

		if(content != NULL)
			delete[] content;
	}

	int unpack(char* buffer, size_t size) {
		
		char* tmp = buffer;

		if(size < 4*sizeof(int))
			return -1;

		seq = *((int*)tmp);

		tmp += sizeof(int);
		type = *((int*)tmp);

		tmp += sizeof(int);
		bs_str_len = *((int*)tmp);

		if(size < (4*sizeof(int) + bs_str_len + 1))
			return -1;

		tmp += sizeof(int);

		if(strlen(tmp) != bs_str_len)
			return -1;

		bs = str_to_bitset(tmp, NULL);

		if(bs == NULL)
			return -1;

		tmp += bs_str_len + 1;
		content_len = *((int*)tmp);

		if(size < (4*sizeof(int) + bs_str_len + content_len + 2))
			return -1;

		tmp += sizeof(int);

		if(strlen(tmp) != content_len)
			return -1;

		content = strdup(tmp);

		if(sci_filter_log != NULL)
		{
			fprintf(sci_filter_log, "Single::UnpackMsg this[%p], content[%p]\n", this, content);
			fflush(sci_filter_log);
		}

		return 0;
	}

	int getMsgSize() {

		int size = 2;	//two '\0' at the end of string

		size += sizeof(int) * 4; //seq, type, bs_str_len, content_len
		size += bs_str_len;
		size += content_len;

		if(sci_filter_log != NULL)
		{
			fprintf(sci_filter_log, "SingleMsg::getMsgSize size[%d]\n", size);
			fflush(sci_filter_log);
		}

		return size;
	}

	void prePack() {
		bs_str = bitset_to_str(bs);
		bs_str_len = strlen(bs_str);
	}

	int	 pack(char* buffer, int size) {
		char* dest;
		int pos = 0;

		if(getMsgSize() > size)
			return -1;

		dest = buffer;
		memcpy(dest, &seq, sizeof(int));

		pos += sizeof(int);
		memcpy(dest + pos, &type, sizeof(int));

		pos += sizeof(int);
		memcpy(dest + pos, &bs_str_len, sizeof(int));

		pos += sizeof(int);
		memcpy(dest + pos, bs_str, strlen(bs_str) + 1);
		
		pos += strlen(bs_str) + 1;
		memcpy(dest + pos, &content_len, sizeof(int));
	
		pos += sizeof(int);
		memcpy(dest + pos, content, strlen(content) + 1);


		if(sci_filter_log != NULL)
		{
			fprintf(sci_filter_log, "SingleMsg::packMsg [pos]:[%d], strlen(bs_str):[%lu], strlen(conetnt):[%lu]\n", pos, strlen(bs_str), strlen(content));
			fflush(sci_filter_log);
		}

		return 0;
	}

	void print(FILE* fd) {
		if(content != NULL)
		{
			fprintf(fd, "seq[%d] type[%d] %s\n", seq, type, content);
		}
		else
			fprintf(fd, "<NULL MSG>\n");
	}

	int getSeq() {
		return seq;
	}

	int getType() {
		return type;
	}

	bitset * getBS() {
		return bs;
	}

	bitset * fetchBS() {
		bitset* _bs = bs;
		bs = NULL;

		return _bs;
	}

	void setBS(bitset* bs_) {
		if(bs != NULL)
			bitset_free(bs);

		bs = bs_;
	}

	void aggregateBS(bitset* agg) {
		if(bs != NULL)
			bitset_oreq(bs, agg);
	}

	void setBit(int bit) {
		if(bs != NULL)
			bitset_set(bs, bit);
	}

	char * getContent() {
		return content;
	}

	bool  operator > ( SingleMsg &msg) {
		int res;
		assert((content != NULL) && (msg.content != NULL));

		if(seq != msg.seq)
			return (seq > msg.seq);

		res = strcmp(content, msg.content);

		if(res > 0)
			return true;

		return false;
	}

	bool  operator < (SingleMsg* msg) {
		int res;
		assert((content != NULL) && (msg->content != NULL));

		if(seq != msg->seq)
			return (seq < msg->seq);

		res = strcmp(content, msg->content);

		if(res < 0)
			return true;

		return false;
	}

	bool  operator < (SingleMsg &msg) {
		int res;
		assert((content != NULL) && (msg.content != NULL));

		if(seq != msg.seq)
			return (seq < msg.seq);

		res = strcmp(content, msg.content);

		if(res < 0)
			return true;

		return false;
	}

	bool  operator == (SingleMsg &msg) {
		assert((content != NULL) && (msg.content != NULL));
		
		if (seq != msg.seq)
			return false;

		if (type != msg.type)
			return false;

		if(strlen(content) != strlen(msg.content))
			return false;

		if(strcmp(content, msg.content) != 0)
			return false;

		return true;
	}
};

class ConcatMsg 
{
private:
	vector<SingleMsg*>   messages;

public:
	ConcatMsg() {
		messages.clear();
	}

	int getNumOfMsgs() {
		return messages.size();
	}

	void addSingleMsg(SingleMsg* message) {
		messages.push_back(message);
	}

	SingleMsg * getMsg(int index) {
		int num = messages.size();

		if (num == 0 || index < 0 || index >= num)
			return NULL;

		return messages[index];
	}

	ConcatMsg & unpack(const char *str, int size) {
		SingleMsg* single;
		char *from = (char *) str;
		int num, index, len, res;

		messages.clear();
		num = *((int*)from);
		from += sizeof(int);
		for(index = 0; index < num; ++index)
		{
			len = *((int*)from);
			from += sizeof(int);

			single = new SingleMsg();
			single->unpack(from, len);	
			if(res >= 0)
				messages.push_back(single);

			from += len;
		}

		return *this;
	}

	void freeMemory(bool freeAll) { //ConcatMsg is a container
		int index;
		int num = messages.size();
		SingleMsg*	single;

		if(freeAll)
		{
			for(index = 0; index < num; ++index)
			{
				single = (SingleMsg*)messages[index];
				delete single;
			}
		}
		messages.clear();
	}

	void print(FILE* fd) {
		int num = messages.size();

		for (int i=0; i < num; i++) {
		    messages[i]->print(fd);
		}
	}

	void prePack() {
		int index;
		int num = messages.size();

		for (index = 0; index < num; ++index) {
		    messages[index]->prePack();
		}
	}

	int	getMsgSize() {
		int size, index;
		int num = messages.size();

		size = (num +  1)* sizeof(int);
		for (index = 0; index < num; ++index) {
		    size += messages[index]->getMsgSize();
		}

		if(sci_filter_log != NULL)
		{
			fprintf(sci_filter_log, "ConcatMsg::getMsgSize size[%d]\n", size);
			fflush(sci_filter_log);
		}

		return size;
	}

	int pack(char *buffer, int len) {
		char* tmp = buffer;
		int pos, index;
		int num = messages.size();

		if(len < getMsgSize())
			return -1;

		*(int*)tmp = num;
		tmp += sizeof(int);

		pos = 0;
		for(index = 0; index < num; ++index)
		{
			*(int*)(tmp + pos) = messages[index]->getMsgSize();
			pos += sizeof(int);

			messages[index]->pack(tmp + pos, len - pos);
			pos += messages[index]->getMsgSize();
		}

		if(sci_filter_log != NULL)
		{
			fprintf(sci_filter_log, "ConcatMsg::pack pos[%d]\n", pos);
			fflush(sci_filter_log);
		}

		return 0;
	}
};

class MsgCompare {
public:
	bool operator() (SingleMsg* x, SingleMsg* y) 
	{
		return  (*x) < (*y);
	}
};
#endif /* _GUARD_SCI_MESSAGE_HPP */
