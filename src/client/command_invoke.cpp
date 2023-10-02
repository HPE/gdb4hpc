#include "command_invoke.h"

#include "eval.h"
#include "interpreter.h"
#include "interpreter_console.h"
#include "interpreter_mi.h"

#include "cli_funcs.h"

// the template specializations in this file are what define how the Command
// class filters/casts the arguments to those which the command implementation
// wants. only functions with these signatures are supported. trying to
// construct a Command with a non-supported function signature will result in an
// undefined implementation linker error.
//
// c++ template syntax does not do us any favors here for readability. the main
// thing that differs between specializations is the signature in the Impl<>
// template arguments. the first argument is the function's return type, and the
// rest are the arguments.

// special implementation for Commands constructed with a nullptr
template <>
int Command::Impl<void>::call(Interpreter&, rec_set_t*, int, const char* const[]) const {
    // hopefully unreachable. users of the Command should check before
    // calling with if (my_command) { my_command(...); } to avoid this.
    //
    // FIXME: change this to an exception throw. We probably shouldn't be
    // printing things this deep down in the command implementation. for
    // example, when the unit tests for null commands are run, it prints errors,
    // but the test is testing the error case... which means the test is
    // succeessful! it probably shouldn't be printing anything.
    PrintOutError("Interpreter error: tried to invoke an empty function\n");
    return Failure;
}

// specializations for commands that don't accept an interpreter

template <>
int Command::Impl<int>::
    call(Interpreter&, rec_set_t*, int, const char* const[]) const
{
    return fn();
}

template <>
int Command::Impl<int, rec_set_t*>::
    call(Interpreter&, rec_set_t* rec_set, int, const char* const[]) const
{
    return fn(rec_set);
}

template <>
int Command::Impl<int, int, const char* const*>::
    call(Interpreter&, rec_set_t*, int argc, const char* const argv[]) const
{
    return fn(argc, argv);
}

template <>
int Command::Impl<int, rec_set_t*, int, const char* const*>::
    call(Interpreter&, rec_set_t* rec_set, int argc, const char* const argv[]) const
{
    return fn(rec_set, argc, argv);
}

// invoke_with_downcast is a helper function for all the command functions that
// accept an interpreter. it attempts to cast the Interpreter& down to the given
// INTERPRETER& and calls the command function if it succeeds. we also use it
// for the abstract Interpreter& case for implementation convenience, where the
// cast is a noop.
template <typename INTERPRETER, typename FN, typename... Args>
static int invoke_with_downcast(const FN fn, Interpreter& intr, Args&&... args) {
    if (auto* lowered = dynamic_cast<INTERPRETER*>(&intr)) {
        return fn(*lowered, std::forward<Args>(args)...);
    }
    // hopefully unreachable. the only way for the dynamic cast to fail is for a function
    // that accepts e.g. MiInterpreter& to be attached to a ConsoleInterpreter command tree.
    PrintOutError("Interpreter error: unsupported command on %.*s interpreter.\n",
                  (int)intr.name().size(), intr.name().data());
    return Command::Failure;
}

// these are partial specializations of Command::Impl<>, later used to instantiate
// commands with apis that accept an interpreter. they live here instead of in
// the header file to keep the header file small and easy(ish) to read.

template <typename INTERPRETER>
struct Command::Impl<int, INTERPRETER&> : Command::Interface {
    Impl(std::function<int(INTERPRETER&)>&& fn) : fn{std::move(fn)} {}
    virtual int call(Interpreter& intr, rec_set_t*, int, const char* const[]) const override {
        return invoke_with_downcast<INTERPRETER>(fn, intr);
    };
    virtual bool to_bool() const override {
        return static_cast<bool>(fn);
    }
    std::function<int(INTERPRETER&)> fn;
};

template <typename INTERPRETER>
struct Command::Impl<int, INTERPRETER&, rec_set_t*> : Command::Interface {
    Impl(std::function<int(INTERPRETER&, rec_set_t*)>&& fn) : fn{std::move(fn)} {}
    virtual int call(Interpreter& intr, rec_set_t* rec_set, int, const char* const[]) const override {
        return invoke_with_downcast<INTERPRETER>(fn, intr, rec_set);
    };
    virtual bool to_bool() const override {
        return static_cast<bool>(fn);
    }
    std::function<int(INTERPRETER&, rec_set_t*)> fn;
};

template <typename INTERPRETER>
struct Command::Impl<int, INTERPRETER&, int, const char* const*> : Command::Interface {
    Impl(std::function<int(INTERPRETER&, int, const char* const[])>&& fn) : fn{std::move(fn)} {}
    virtual int call(Interpreter& intr, rec_set_t*, int argc, const char* const argv[]) const override {
        return invoke_with_downcast<INTERPRETER>(fn, intr, argc, argv);
    };
    virtual bool to_bool() const override {
        return static_cast<bool>(fn);
    }
    std::function<int(INTERPRETER&, int, const char* const[])> fn;
};

template <typename INTERPRETER>
struct Command::Impl<int, INTERPRETER&, rec_set_t*, int, const char* const*> : Command::Interface {
    Impl(std::function<int(INTERPRETER&, rec_set_t*, int, const char* const[])>&& fn) : fn{std::move(fn)} {}
    virtual int call(Interpreter& intr, rec_set_t* rec_set, int argc, const char* const argv[]) const override {
        return invoke_with_downcast<INTERPRETER>(fn, intr, rec_set, argc, argv);
    };
    virtual bool to_bool() const override {
        return static_cast<bool>(fn);
    }
    std::function<int(INTERPRETER&, rec_set_t*, int, const char* const[])> fn;
};

// instantiations of the above specializations for each supported interpreter.
// instantiating these here instead of the header file lets the header file stay
// generic. i.e. it keeps the header file free from needing to include e.g.
// interpreter_console.h.
//
// XXX: maybe a future c++ template wizard more powerful than I can figure out
// how to get these down to one line per interpreter type... (without macros!)

template struct Command::Impl<int, Interpreter&>;
template struct Command::Impl<int, Interpreter&, rec_set_t*>;
template struct Command::Impl<int, Interpreter&, int, const char* const*>;
template struct Command::Impl<int, Interpreter&, rec_set_t*, int, const char* const*>;
template struct Command::Impl<int, ConsoleInterpreter&>;
template struct Command::Impl<int, ConsoleInterpreter&, rec_set_t*>;
template struct Command::Impl<int, ConsoleInterpreter&, int, const char* const*>;
template struct Command::Impl<int, ConsoleInterpreter&, rec_set_t*, int, const char* const*>;
template struct Command::Impl<int, MiInterpreter&>;
template struct Command::Impl<int, MiInterpreter&, rec_set_t*>;
template struct Command::Impl<int, MiInterpreter&, int, const char* const*>;
template struct Command::Impl<int, MiInterpreter&, rec_set_t*, int, const char* const*>;
