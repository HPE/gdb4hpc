#include "command_invoke.h"
#include "unittest_macros.h"

#include "interpreter_console.h"
#include "interpreter_mi.h"

#include <iostream>

namespace {

int cmd() {
    return Command::Success;
}
int cmd_r(rec_set_t*) {
    return Command::Success;
}
int cmd_i_cc(int, const char* const[]) {
    return Command::Success;
}
int cmd_r_i_cc(rec_set_t*, int, const char* const[]) {
    return Command::Success;
}

int cmd_I(Interpreter&) {
    return Command::Success;
}
int cmd_I_r(Interpreter&, rec_set_t*) {
    return Command::Success;
}
int cmd_I_i_cc(Interpreter&, int, const char* const[]) {
    return Command::Success;
}
int cmd_I_r_i_cc(Interpreter&, rec_set_t*, int, const char* const[]) {
    return Command::Success;
}

int cmd_C(ConsoleInterpreter&) {
    return Command::Success;
}
int cmd_C_r(ConsoleInterpreter&, rec_set_t*) {
    return Command::Success;
}
int cmd_C_i_cc(ConsoleInterpreter&, int, const char* const[]) {
    return Command::Success;
}
int cmd_C_r_i_cc(ConsoleInterpreter&, rec_set_t*, int, const char* const[]) {
    return Command::Success;
}

int cmd_M(MiInterpreter&) {
    return Command::Success;
}
int cmd_M_r(MiInterpreter&, rec_set_t*) {
    return Command::Success;
}
int cmd_M_i_cc(MiInterpreter&, int, const char* const[]) {
    return Command::Success;
}
int cmd_M_r_i_cc(MiInterpreter&, rec_set_t*, int, const char* const[]) {
    return Command::Success;
}

TEST_CASE("construct Commands") {
    // Command's templated constructor lets you construct Commands out of
    // anything that matches a supported filter implementation.
    {
        auto CMD_cmd = Command(cmd);
        auto CMD_cmd_r = Command(cmd_r);
        auto CMD_cmd_i_cc = Command(cmd_i_cc);
        auto CMD_cmd_r_i_cc = Command(cmd_r_i_cc);

        auto CMD_cmd_I = Command(cmd_I);
        auto CMD_cmd_I_r = Command(cmd_I_r);
        auto CMD_cmd_I_i_cc = Command(cmd_I_i_cc);
        auto CMD_cmd_I_r_i_cc = Command(cmd_I_r_i_cc);

        auto CMD_cmd_C = Command(cmd_C);
        auto CMD_cmd_C_r = Command(cmd_C_r);
        auto CMD_cmd_C_i_cc = Command(cmd_C_i_cc);
        auto CMD_cmd_C_r_i_cc = Command(cmd_C_r_i_cc);

        auto CMD_cmd_M = Command(cmd_M);
        auto CMD_cmd_M_r = Command(cmd_M_r);
        auto CMD_cmd_M_i_cc = Command(cmd_M_i_cc);
        auto CMD_cmd_M_r_i_cc = Command(cmd_M_r_i_cc);
    }
    {
        Command CMD_cmd = cmd;
        Command CMD_cmd_r = cmd_r;
        Command CMD_cmd_i_cc = cmd_i_cc;
        Command CMD_cmd_r_i_cc = cmd_r_i_cc;

        Command CMD_cmd_I = cmd_I;
        Command CMD_cmd_I_r = cmd_I_r;
        Command CMD_cmd_I_i_cc = cmd_I_i_cc;
        Command CMD_cmd_I_r_i_cc = cmd_I_r_i_cc;

        Command CMD_cmd_C = cmd_C;
        Command CMD_cmd_C_r = cmd_C_r;
        Command CMD_cmd_C_i_cc = cmd_C_i_cc;
        Command CMD_cmd_C_r_i_cc = cmd_C_r_i_cc;

        Command CMD_cmd_M = cmd_M;
        Command CMD_cmd_M_r = cmd_M_r;
        Command CMD_cmd_M_i_cc = cmd_M_i_cc;
        Command CMD_cmd_M_r_i_cc = cmd_M_r_i_cc;
    }
    {
        auto cmd_generic_lambda = [](Interpreter&, int, const char* const[]) {
            return Command::Success;
        };
        auto cmd_console_lambda = [](ConsoleInterpreter&, int, const char* const[]) {
            return Command::Success;
        };
        auto cmd_mi_lambda = [](MiInterpreter&, int, const char* const[]) {
            return Command::Success;
        };
        auto cmd_anon_lambda = [](int, const char* const[]) {
            return Command::Success;
        };
        auto generic = Command(cmd_generic_lambda);
        auto console = Command(cmd_console_lambda);
        Command mi = cmd_mi_lambda;
        Command anonymous = cmd_anon_lambda;
    }

    auto as_arg = [](Command fn){ return fn; };

    {
        auto generic = as_arg(cmd_I_r_i_cc);
        auto console = as_arg(cmd_C);
        auto mi = as_arg(cmd_M_i_cc);
        auto anonymous = as_arg(cmd_i_cc);
    }

    struct WeirdArgs {
        Command cmd;
        int widget;
    };
    auto as_weird_args = [](WeirdArgs&& args){ return args; };

    {
        auto generic = as_weird_args({ .cmd = cmd_I_r_i_cc, .widget = 0 });
        auto console = as_weird_args({ .cmd = cmd_C, .widget = 1 });
        auto mi = as_weird_args({ .cmd = cmd_M_i_cc, .widget = 2 });
        auto anonymous = as_weird_args({ .cmd = cmd_i_cc, .widget = 3 });
    }
    {
        // nullptr is also supported, but is not intended to be called.
        auto nullfn1 = Command(nullptr);
        Command nullfn2 = nullptr;
        auto nullfn3 = as_arg(nullptr);
        auto nullfn4 = as_weird_args({ .cmd = nullptr, .widget = -1 });
    }
}

class TestInterpreter : public Interpreter {
public:
    std::string_view name() const override { return "test"; }
    std::string complete_text(std::string_view text) const override { return std::string{text}; }
    bool can_interpret(std::string_view) const override { return true; }
    bool interpret(std::string_view) override { return true; }
    ~TestInterpreter() override = default;

    class Printer : public Interpreter::Printer {
    public:
        void print_message_v(const char*, va_list) override {};
        std::ostream& get_message_stream() override {
            // (not actually intended to be meaninfully used, just satisfying the api);
            return std::cerr;
        };
        void print_error_v(const char*, va_list) override {};
        void print_inferior_output(const DbgInvoke&, std::string_view, FILE*) override {};

        ~Printer() override = default;
    };
private:
    Printer& get_printer() override {
        static Printer the_printer;
        return the_printer;
    };
};

TEST_CASE("matching/mismatching calls") {
    // note that all the test functions return Success, so any Failure comes
    // from the Command filtering logic.
    auto intr_impl = TestInterpreter();
    Interpreter& intr = intr_impl;
    auto rec_set = (rec_set_t*){nullptr};
    auto argv = ArgvStream();
    auto argc = static_cast<int>(argv.argc());

    SUBCASE("a null Command returns Failure") {
        auto null_cmd = Command{nullptr};
        CHECK(null_cmd(intr, rec_set, argc, argv.c_strs().data()) == Command::Failure);
    }

    SUBCASE("a mismatched interpreter returns Failure") {
        auto console_cmd = Command(cmd_C);
        CHECK(console_cmd(intr, rec_set, argc, argv.c_strs().data()) == Command::Failure);
    }

    {
        auto test_string = std::string{};

        SUBCASE("calls are matched to apis 1") {
            const auto impl = [&]() -> int {
                test_string = "int()";
                return Command::Success;
            };
            auto cmd = Command(impl);
            REQUIRE(test_string == "");
            CHECK(cmd(intr, rec_set, argc, argv.c_strs().data()) == Command::Success);
            CHECK(test_string == "int()");
        }
        SUBCASE("calls are matched to apis 2") {
            const auto impl_r_i_cc = [&](rec_set_t*, int, const char* const[]) -> int {
                test_string = "int(int, rec_set_t*, const char* const[])";
                return Command::Success;
            };
            auto cmd = Command(impl_r_i_cc);
            REQUIRE(test_string == "");
            CHECK(cmd(intr, rec_set, argc, argv.c_strs().data()) == Command::Success);
            CHECK(test_string == "int(int, rec_set_t*, const char* const[])");
        }
        SUBCASE("calls are matched to apis 3") {
            const auto impl_I = [&](Interpreter&) -> int {
                test_string = "int(Interpreter&)";
                return Command::Success;
            };
            auto cmd = Command(impl_I);
            REQUIRE(test_string == "");
            CHECK(cmd(intr, rec_set, argc, argv.c_strs().data()) == Command::Success);
            CHECK(test_string == "int(Interpreter&)");
        }
    }
}

} // anonymous namespace
