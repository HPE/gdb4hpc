
#include "Compare.h"
#include "IStream.h"
#include "OStream.h"
#include "Error.h"
#include <iostream>

Compare::Compare(std::shared_ptr<IStream> a, std::shared_ptr<IStream> b, std::shared_ptr<OStream> diffs)
    : BinaryOp(a, b, diffs)
{
}

void Compare::apply() 
{
    std::any v1, v2;
    if (!getIn1()->read(v1) || !getIn2()->read(v2)) {
        throw Error("unexpected EOF during read");
    }

    if (v1.type() == typeid(double) || v2.type() == typeid(double)) {
        double d1 = std::any_cast<double>(v1);
        double d2 = std::any_cast<double>(v2);
        std::cout << "Compare is outputting " << d1-d2 << " from " << d1 << "," << d2 << "\n";
        //getOut()->putDouble(d1 - d2);
    } else if (v1.type() == typeid(int) || v2.type() == typeid(int)) {
        int i1 = std::any_cast<int>(v1);
        int i2 = std::any_cast<int>(v2);
        std::cout << "Compare is outputting " << i1-i2 << " from " << i1 << "," << i2 << "\n";
        //getOut()->putDouble(i1 - i2);
    } else {
        throw Error("unhandled types in compare");
    }
}
