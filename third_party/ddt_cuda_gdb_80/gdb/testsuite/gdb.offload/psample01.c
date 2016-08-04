// ============================================================================
//
// INTEL CONFIDENTIAL
//
// Copyright 2012 Intel Corporation All Rights Reserved.
//
// The source code contained or described herein and all documents related to
// the source code ("Material") are owned by Intel Corporation or its
// suppliers or licensors. Title to the Material remains with Intel
// Corporation or its suppliers and licensors. The Material contains trade
// secrets and proprietary and confidential information of Intel or its
// suppliers and licensors. The Material is protected by worldwide copyright
// and trade secret laws and treaty provisions. No part of the Material may be
// used, copied, reproduced, modified, published, uploaded, posted,
// transmitted, distributed, or disclosed in any way without Intel's prior
// express written permission.
//
// No license under any patent, copyright, trade secret or other intellectual
// property right is granted to or conferred upon you by disclosure or
// delivery of the Materials, either expressly, by implication, inducement,
// estoppel or otherwise. Any license under such intellectual property rights
// must be express and approved by Intel in writing.
//
// ============================================================================


extern void pragma_attach_gdb();

float __attribute__((target(mic))) psample01_f1(float a)
{
    return a + 1.0;             /* breakpoint_psample01_f1 */
}

float __attribute__((target(mic))) psample01_f2(float a, float b)
{
    return a + psample01_f1(b); /* breakpoint_psample01_f2 */
}

double __attribute__((target(mic))) psample01_d1(double x)
{
    return x + 1.0;             /* breakpoint_psample01_d1 */
}

double __attribute__((target(mic))) psample01_d2(double x, double y)
{
    return x + psample01_d1(y); /* breakpoint_psample01_d2 */
}

void psample01()
{
    float host_d;
    double host_w;

    #pragma offload target(mic)
    {
        float a, b, c, d;

        d = 0.0;            /* breakpoint_psample01_init */
        a = 1.0;
        b = 2.0;
        c = -1.0;

        d = c + psample01_f1(b);    // 2.0 = -1.0 + 3.0 /* psample01_f1_call */
        d = d + psample01_f2(b, a); // 6.0 = 2.0 + 4.0  /* psample01_f2_call */

        host_d = d;         /* breakpoint_psample01_assign_to_host */
    }

    #pragma offload target(mic)
    {
        double x, y, z, w;

        w = 0.0;
        x = 1.0;
        y = 2.0;
        z = -1.0;

        w = z + psample01_d1(y);    // 2.0 = -1.0 + 3.0 /* psample_01_d1_call */
        w = w + psample01_d2(y, x); // 6.0 = 2.0 + 4.0  /* psample_01_d2_call */

        host_w = w;
    }
}

int main()
{
    pragma_attach_gdb();
    psample01();
    return 0;
}
