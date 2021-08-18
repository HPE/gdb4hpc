/* cti_launch
 *
 * wrapper around CTI which launches the program passed via arguments and then prints
 * out an identifying string for gdb4hpc to use to attach to the launched job.
 * 
 * usage is just like the job launcher on the system, e.g. for srun:
 * $ ./cti_launch -n3 hostname
 * 
 * cti_launch loops indefinitely after launching the job so that the job doesn't
 * get cancelled. it is expected that this program will ultimately be stopped
 * by a SIGINT or similar.
*/

#include <common_tools_fe.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    // launch app
    auto appId = cti_launchApp((const char * const *)&argv[1],-1,-1,nullptr,nullptr,nullptr);
    if (appId == 0) {
        fprintf(stderr, "Error: cti_launchApp failed!\n");
        fprintf(stderr, "CTI error: %s\n", cti_error_str());
    }

    auto wlm = cti_current_wlm();

    switch (wlm) {
        case CTI_WLM_SLURM:
        {
            cti_slurm_ops_t * slurm_ops = nullptr;
            cti_wlm_type_t ret = cti_open_ops((void **)&slurm_ops);
            assert(ret == wlm);
            assert(slurm_ops != nullptr);
            /*
             * getSrunInfo - Obtain information about the srun process
             */
            cti_srunProc_t *mysruninfo = slurm_ops->getSrunInfo(appId);
            if (mysruninfo == nullptr) {
                fprintf(stderr, "Error: getSrunInfo failed!\n");
                fprintf(stderr, "CTI error: %s\n", cti_error_str());
            }
            assert(mysruninfo != nullptr);
            fprintf(stderr, "%lu.%lu\n", (long unsigned int)mysruninfo->jobid, (long unsigned int)mysruninfo->stepid);
            free(mysruninfo);
            mysruninfo = nullptr;
        }
            break;

        case CTI_WLM_ALPS:
        {
            cti_alps_ops_t * alps_ops = nullptr;
            cti_wlm_type_t ret = cti_open_ops((void **)&alps_ops);
            assert(ret == wlm);
            assert(alps_ops != nullptr);
            /*
             * getAprunInfo - Obtain information about the aprun process
             */
            cti_aprunProc_t *myapruninfo = alps_ops->getAprunInfo(appId);
            if (myapruninfo == nullptr) {
                fprintf(stderr, "Error: getAprunInfo failed!\n");
                fprintf(stderr, "CTI error: %s\n", cti_error_str());
            }
            assert(myapruninfo != nullptr);
            fprintf(stderr, "%llu\n", (long long unsigned int)myapruninfo->apid);
            free(myapruninfo);
            myapruninfo = nullptr;
            /*
             * getAlpsOverlapOrdinal - Get overlap ordinal for MAMU
             */
            int res = alps_ops->getAlpsOverlapOrdinal(appId);
            if (res < 0) {
                fprintf(stderr, "Error: getAlpsOverlapOrdinal failed!\n");
                fprintf(stderr, "CTI error: %s\n", cti_error_str());
            }
            assert(res >= 0);
            printf("alps overlap ordinal of application: %d\n", res);
        }
            break;

        case CTI_WLM_SSH:
            fprintf(stderr, "%lu\n", appId);
            break;

        case CTI_WLM_PALS:
            fprintf(stderr, "%lu\n", appId);
            break;

        default:
            // do nothing
            fprintf(stderr, "Unsupported wlm!\n");
            assert(0);
            break;
    }

    // wait for signal
    while (1) {
        sleep(1);
    }

    return 0;
}