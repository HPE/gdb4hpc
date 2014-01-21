/*********************************************************************************\
 * <dmr.cpp>
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


#include <stdlib.h>
#include <string.h>
#include <vector>

#include "dmr.h"
#include "smart_bitset.h"

using namespace std;

namespace DMR /* DebuggingMessageReduction */
{

Message::Message(char* mes, char* bt) : m_release(true), m_content(mes), m_btStr(bt), m_sb(NULL) 
{ }

Message::~Message() 
{
	if(m_content != NULL && m_release)
	{
		free(m_content);
	}
	m_content = NULL;

	if(m_btStr != NULL && m_release)
	{
		free(m_btStr);
	}
	m_btStr = NULL;

	if(m_sb != NULL)
	{
		smart_bitset_free(m_sb);
		m_sb = NULL;
	}
}

void 
Message::SetReleaseMemory(bool release)
{
	m_release = release;
}

char* 
Message::GetContent()
{
	return m_content;
}

char* 
Message::GetBitsetStr()
{
	if(m_sb != NULL)
		m_btStr = smart_bitset_serialize(m_sb);

	return m_btStr;
}

/*
less (<0); equal (0); more (>0)
*/
int
Message::Compare(Message* pMessage)
{
	if(pMessage == NULL && m_content== NULL)
		return 0;
	else if(m_content == NULL)
		return -1;
	else if(pMessage == NULL)
		return 1;

	char* leftM = m_content;
	char* rightM = pMessage->GetContent();

	return strcmp(leftM, rightM);
}

bool
Message::Less(Message* pMessage)
{
	int	comp = Compare(pMessage);
	return comp < 0;
}

bool
Message::Equal(Message* pMessage)
{
	int	comp = Compare(pMessage);
	return comp == 0;
}

bool
Message::LessEqual(Message* pMessage)
{
	int	comp = Compare(pMessage);
	return comp <= 0;
}

void 
Message::MergeBitset(char* btStr)
{
	smart_bitset* sb = NULL;

	if(btStr == NULL)
		return;

	sb = smart_bitset_deserialize(btStr);

	if(m_sb == NULL)
		m_sb = smart_bitset_deserialize(m_btStr);

	smart_bitset_oreq(m_sb, sb);

	smart_bitset_free(sb);
}

MessageVector::MessageVector(bool releaseMemory) 
{
	m_release = releaseMemory;
}

MessageVector::~MessageVector()
{
	if(!m_release)
		return;

	Message* pMessage;
	MessageVector::iterator it;
	for(it = vector<Message*>::begin(); it < vector<Message*>::end(); ++it)
	{
		pMessage = *it;
		delete pMessage;
	}
}

void 
MessageVector::Initialization(char* messages[], int number)
{
	int index;

	if(messages == NULL || number <= 0 || number % 2 != 0)
		return;

	for(index = 0; index < number; index += 2)
	{
		Message* pMessage = new Message(messages[index], messages[index+1]);
		vector<Message*>::push_back(pMessage);
	}
	
	m_iterator = vector<Message*>::begin();
}

void
MessageVector::Append(Message* pMessage)
{
	Message* lastMessage;
	if(vector<Message*>::size() == 0)
		vector<Message*>::push_back(pMessage);
	else
	{
		lastMessage = vector<Message*>::back();
		if(lastMessage->Equal(pMessage))
		{
			lastMessage->MergeBitset(pMessage->GetBitsetStr());
		}
		else
		{
			vector<Message*>::push_back(pMessage);
		}
	}
}

vector<Message*>::iterator 
MessageVector::Current()
{
	return m_iterator;
}

bool 
MessageVector::MoveNext()
{
	m_iterator++;
	return (m_iterator < vector<Message*>::end());
}

} //namespace DMR
