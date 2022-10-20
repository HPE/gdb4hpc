/*******************************************************************************
 * Compare Utilities
 * Copyright 2022 Hewlett Packard Enterprise Development LP.
 ******************************************************************************/
#include "compare_util.h"

#include "cli_funcs.h"
#include "client.h"
#include "dense_bitset.h"

void printCompareResult(val_t* result, val_t* difference, rec_set_t& recSet) {
    // Process the result val based on its rval type
    auto& os = Client.cli_out->ostream();
    switch (RVAL_TYPE(result)) {
        case rval_aif: {
            // point at the aif result
            auto a = RVAL_AIF(result);
            int res;
            // Set res to the comparison result
            a->toInt(res);
            switch (res) {
                case 0:
                    if (Client.MI_mode){
                        PrintOutMIResult(MIResult::DONE,"compare={result=\"True. \"}\n");
                    }else{
                        PrintOutMsg("Comparison is true.\n");
                    }
                    break;
                case -1:
                case 1:
                    if (Client.MI_mode){
                        std::ostringstream buf;
                        if (PrintVal(buf, difference, &recSet, "") < 0) PrintError();
                        std::string buf_str = buf.str();
                        buf_str.pop_back();
                        PrintOutMIResult(MIResult::DONE,"compare={result=\"False. The difference delta is: %s\"}\n", buf_str.c_str());
                    }else{
                        PrintOutMsg(
                            "Comparison is false. The difference delta is:\n");
                        if (PrintVal(os, difference, &recSet, "") < 0) PrintError();
                    }
                    break;
                case -2:
                case 2:
                    if (Client.MI_mode){
                        std::ostringstream buf;
                        if (PrintVal(buf, difference, &recSet, "") < 0) PrintError();
                        std::string buf_str = buf.str();
                        buf_str.pop_back();
                        PrintOutMIResult(MIResult::DONE,"compare={result=\"Warn. The difference delta is: %s\"}\n", buf_str.c_str());
                    }else{
                        PrintOutMsg(
                            "Comparison is warn. The difference delta is:\n");
                        if (PrintVal(os, difference, &recSet, "") < 0) PrintError();
                    }
                    break;
                case 3:
                    PrintOutMsg(
                        "Comparison cannot be performed.\n");
                    break;
            }
            break;
        }

        case rval_reply: {
            // Point at the reply set
            auto rep_set = RVAL_REPLY(result);
            std::stringstream mi_result;

            std::unique_ptr<DenseBitset> true_bitset = nullptr;
            std::unique_ptr<DenseBitset> false_bitset = nullptr;
            std::unique_ptr<DenseBitset> warn_bitset = nullptr;
            std::unique_ptr<DenseBitset> indet_bitset = nullptr;
            int print_diff = 0;

            // Fix for BUG 817334 - This used to print the results of the comp
            // operation directly, but now it will try to group like messages
            // together and only print a summary over all the ranks.

            // Need to loop over the reply
            for (auto&& reply : rep_set) {
                // point at the aif result
                auto a = EVENT_DATA(reply->rep_event);
                // Set res to the comparison result
                int res;
                a->toInt(res);
                switch (res) {
                    case 0:
                        if (true_bitset == nullptr) {
                            // create a new true bitset based off the reply
                            // group. Note that every reply will have the same
                            // set size.
                            true_bitset =
                                std::make_unique<DenseBitset>(reply->rep_group->sizeInBits());
                        }
                        // Add this group to the true_bitset
                        *true_bitset |= *reply->rep_group;
                        break;
                    case -1:
                    case 1:
                        if (false_bitset == nullptr) {
                            // create a new false bitset based off the reply
                            // group. Note that every reply will have the same
                            // set size.
                            false_bitset =
                                std::make_unique<DenseBitset>(reply->rep_group->sizeInBits());
                        }
                        // Add this group to the false_bitset
                        *false_bitset |= *reply->rep_group;
                        print_diff++;
                        break;
                    case -2:
                    case 2:
                        if (warn_bitset == nullptr) {
                            // create a new warn bitset based off the reply
                            // group. Note that every reply will have the same
                            // set size.
                            warn_bitset =
                                std::make_unique<DenseBitset>(reply->rep_group->sizeInBits());
                        }
                        // Add this group to the warn_bitset
                        *warn_bitset |= *reply->rep_group;
                        print_diff++;
                        break;
                    case 3:
                        if (indet_bitset == nullptr) {
                            // create a new indeterminate bitset based off the
                            // reply group. Note that every reply will have the
                            // same set size.
                            indet_bitset =
                                std::make_unique<DenseBitset>(reply->rep_group->sizeInBits());
                        }
                        // Add this group to the warn_bitset
                        *indet_bitset |= *reply->rep_group;
                        break;
                }
            }

            // XXX: Note that for the PrintMsgWithSetInfo command we don't want
            // to use the handle in the set,
            //      since that is not really a valid representation of what we
            //      are looking at. What we have is a delta between the two
            //      sets, and since a di was needed to be set in the reply, it
            //      just arbitrarily chose the first one. This is a flaw and
            //      someday should be fixed. For now just use the string ""

            // print the result summary
            if (true_bitset != nullptr) {
                if (Client.MI_mode){
                    mi_result<<"True. ";
                }else{
                    PrintMsgWithSetInfo("", true_bitset.get(), "Comparison is true.\n");
                }
            }
            if (false_bitset != nullptr) {
                if (Client.MI_mode){
                    mi_result<<"False. ";
                }else{
                    PrintMsgWithSetInfo("", false_bitset.get(), "Comparison is false.\n");
                }
            }
            if (warn_bitset != nullptr) {
                if (Client.MI_mode){
                    mi_result<<"Warn. ";
                }else{
                    PrintMsgWithSetInfo("", warn_bitset.get(), "Comparison is warn.\n");
                }
            }
            if (indet_bitset != nullptr) {
                if (Client.MI_mode){
                    mi_result<<"Indeterminate. ";
                }else{
                    PrintMsgWithSetInfo(
                        "", indet_bitset.get(),
                        "Comparison is indeterminate due to pointers.\n");
                }
            }
            // print out the difference deltas once
            if (print_diff) {
                if (Client.MI_mode){
                    std::ostringstream buf;
                    if (PrintVal(buf, difference, &recSet, "") < 0) PrintError();
                    std::string buf_str = buf.str();
                    buf_str.pop_back();
                    mi_result <<"The difference delta is: "<<buf_str;
                    PrintOutMIResult(MIResult::DONE,"compare={result=\"%s\"}\n", mi_result.str().c_str());
                }else{
                    PrintOutMsg("The difference delta is:\n");
                    if (PrintVal(os, difference, &recSet, "") < 0) PrintError();
                }
            }else{
                PrintOutMIResult(MIResult::DONE,"compare={result=\"%s\"}\n", mi_result.str().c_str());
            }
            break;
        }

        default:
            // shouldn't get here
            break;
    }
}

CompareResult simpleCompareResult(const val_t& result) {
    const auto int2Res = [](int res){
        switch (res) {
            case 0:
                return CompareResult::Pass;
            case -1:
            case 1:
                return CompareResult::Fail;
            case -2:
            case 2:
                return CompareResult::Warn;
            default:
                return CompareResult::Unknown;
        }
        return CompareResult::Unknown; // should never get here
    };
    
    switch (RVAL_TYPE(&result)) {
        case rval_aif: {
            // point at the aif result
            auto a = RVAL_AIF(&result);
            auto res = int{3}; // default to unknown. if toInt fails, value won't be changed
            // Set res to the comparison &
            a->toInt(res);
            return int2Res(res);
        }

        case rval_reply: {
            auto indetFlag = false;
            auto warnFlag = false;
            auto failFlag = false;

            // Set flags
            for (auto&& reply : RVAL_REPLY(&result)) {
                auto a = EVENT_DATA(reply->rep_event);
                auto res = int{3}; // default to unknown. if toInt fails, value won't be changed
                a->toInt(res);
                switch (int2Res(res)) {
                    case CompareResult::Pass: {
                        // ignore
                        break;
                    }
                    case CompareResult::Warn: {
                        warnFlag = true;
                        break;
                    }
                    case CompareResult::Fail: {
                        failFlag = true;
                        break;
                    }
                    default: {
                        indetFlag = true;
                    }
                }
            }

            // fail has the most precedence; if *anything* fails, we consider
            // the assertion to fail entirely. etc. for warn/indet
            if (failFlag) return CompareResult::Fail;
            if (warnFlag) return CompareResult::Warn;
            if (!indetFlag) return CompareResult::Pass;
            return CompareResult::Unknown;
        }

        default: {
            return CompareResult::Unknown; 
        }
    }
}
