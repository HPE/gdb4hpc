/*********************************************************************************\
 * Copyright 2024 Hewlett Packard Enterprise Development LP.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Hewlett Packard Enterprise Development LP.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 *********************************************************************************/

#pragma once

#include <string>

class DwarfInfo
{
private: // members
    void *m_dwfl;

private: // helpers
    std::string getDieName(int tag, int attr);

public: // interface
    DwarfInfo(std::string const& binary);
    DwarfInfo(DwarfInfo&& expiring);
    ~DwarfInfo();

    std::string getMainSubprogram();
};
