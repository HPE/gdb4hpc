//  Copyright 2023 Hewlett Packard Enterprise Development LP.

#include "config.h"

#include "client.h"
#include "gram.h"
#include "interpreter_console.h"
#include "bttree.h"

#define PY_SSIZE_T_CLEAN
#include <Python.h>

namespace {
// commands to handle python as a cli mode
class PythonMode : public CommandNode {
public:
    PythonMode();

    static void enter(ConsoleInterpreter& console);
private:
    static int executeLine(rec_set_t *rec_set, int argc, const char* const argv[]);
    static int exit(ConsoleInterpreter& console);

    static std::shared_ptr<CommandNode> make_command_tree();
};

// liaison class to connect the python objects to the gdb4hpc functionality
class PyGdb {
public:
    PyGdb();
    ~PyGdb();

    void executeLine(rec_set_t* rec_set, const std::string& command);
    std::unique_ptr<Value> eval(const std::string& expr);
private:
    rec_set_t* m_rec_set = nullptr;
};

// Defines the C++ backing function for the gdb4hpc python object
class PyImpl {
public:
    static PyObject* eval(PyObject* self, PyObject* args);
    static void setGdb(PyGdb* gdb) { m_gdb = gdb; }
private:
    static PyGdb* m_gdb;
};


// class for translating a program value into a python object
class PyObjectMaker : public ValueVisitor {
public:
    ~PyObjectMaker() override;

    void beginReply(const std::string& label) override;
    using ValueVisitor::operator();
    void operator()(long long val) override;
    void operator()(long val) override;
    void operator()(double val) override;
    void operator()(long double val) override;
    void operator()(const std::string& val) override;
    void operator()(const ais::Error& val) override;
    void nest(const ais::Type* t, int refIdx=-1) override;
    void unnest() override;

    PyObject* getPyObject();
private:
    void error(const std::string& msg);
    void finishReply();
    PyObject* makePyObject();
    
    PyObject* m_pyObject = nullptr;
    std::string m_key;
    std::vector<PyObject*> m_pyObjs;
    std::unique_ptr<PyObjectMaker> m_child;
};

}

static int cmd_python_cmd(ConsoleInterpreter& console, int argc, const char* const []) {
    if (argc < 2) {
        PythonMode::enter(console);
        return Command::Success;
    } else {
        PrintOutError("inline python mode is not set up.\n");
        return Command::Failure;
    }
}

static int set_python(rec_set_t* rec_set, int argc, const char* const argv[]) {
    if (argc < 2) {
        PrintOutError("Expected: set python [auto|on|off]\n");
        return Command::Failure;
    }

    using enum PyMode;
    auto setting = Auto;
    auto arg = std::string_view{argv[1]};
    if (arg == "auto") setting = Auto;
    else if (arg == "on") setting = On;
    else if (arg == "off") setting = Off;
    else {
        PrintOutError("Expected: set python [auto|on|off]\n");
        return Command::Failure;
    }

    Client.python = setting;

    auto had_error = false;
    if (rec_set) {
        auto rep_set = DbgPyBESetPython(rec_set, setting);
        had_error = ProcessErrorInRepSet(rep_set);

        for (const auto& rep : rep_set) {
            auto* di = rep->rep_di.lock();
            if (!di) continue;
            switch (setting) {
                case On: di->setPython(*rep->rep_group); break;
                case Off: di->unsetPython(*rep->rep_group); break;
                case Auto: break; // we never made a network request in the first place
            }
        }
    }

    return had_error ? Command::Failure : Command::Success;
}

static int show_python(rec_set_t* rec_set) {
    using enum PyMode;
    const auto ptype_to_str = [](PyMode ptype) {
        switch (ptype) {
            case Auto: return "auto";
            case On: return "on";
            case Off: return "off";
        }
        // unreachable
        return "";
    };

    PrintOutMsg("Python mode for future launches is %s.\n", ptype_to_str(Client.python));

    PrintOutMsg("For current procsets:\n");
    auto had_error = false;
    auto rep_set = DbgPyBEGetPython(rec_set);
    had_error = ProcessErrorInRepSet(rep_set);
    for (const auto* rep : rep_set) {
        const auto value = std::string{rep->rep_event->ev_string};
        const auto message = std::string{"Python mode is "} + value + ".";
        PrintMsgWithSetInfo(rep->rep_di.c_str(), rep->rep_group.get(), "%s\n", message.c_str());
        // XXX: encoding the return value as an untyped string is unfortunate...
        if (value == "on") {
            rep->rep_di.lock()->setPython(*rep->rep_group);
        } else {
            rep->rep_di.lock()->unsetPython(*rep->rep_group);
        }
    }

    return had_error ? Command::Failure : Command::Success;
}

static int pybe_inject(rec_set_t* rec_set) {
    auto rep_set = DbgPyBEInject(rec_set);
    auto had_error = static_cast<bool>(ProcessErrorInRepSet(rep_set));
    return had_error ? Command::Failure : Command::Success;
}

static int pybe_get_GIL(rec_set_t* rec_set) {
    auto rep_set = DbgPyBEGetGIL(rec_set);
    auto had_error = static_cast<bool>(ProcessErrorInRepSet(rep_set));
    return had_error ? Command::Failure : Command::Success;
}

PythonMode::PythonMode() : CommandNode(proc_optional, executeLine) {}

std::shared_ptr<CommandNode> PythonMode::make_command_tree() {
    auto root = std::make_shared<PythonMode>();

    root->addSubCmd({
        .name = "exit", .ptype = proc_optional, .fn = exit,
        .help = dummy_help, .desc = "Exit python mode"});

    return root;
}

// Start python mode
void PythonMode::enter(ConsoleInterpreter& console) {
    PrintOutMsg("Entering python interpreter, use exit to leave.\n");
    console.push_commands(PythonMode::make_command_tree(), "python> ");
}

// Leave shell mode
int PythonMode::exit(ConsoleInterpreter& console) {
    PrintOutMsg("\nExiting python interpreter.\n");
    console.pop_commands();
    return 1;
}

int PythonMode::executeLine(rec_set_t *rec_set, int argc, const char* const argv[]) {
    static std::unique_ptr<PyGdb> py_interpreter;
    if (!py_interpreter)
        py_interpreter.reset(new PyGdb());

    auto command = ArgsToStringUnquoted(argv, argc);
    py_interpreter->executeLine(rec_set, command);
    return 1;
}

void ConsoleCommandsFactory::insert_python_commands(CommandNode& root) {
    root.addSubCmd({
        .name = "python", .ptype = proc_optional,
        .fn = cmd_python_cmd,
        .help = cmd_python_help, .desc = "Enter python interpreter (experimental)."});

    root.addSubCmd({
        .name = "show python", .ptype = proc_optional,
        .fn = show_python,
        .help = cmd_show_python_help, .desc = "Show python mode."});

    root.addSubCmd({
        .name = "set python", .ptype = proc_optional,
        .fn = set_python,
        // intentionally reusing show help text
        .help = cmd_show_python_help, .desc = "Enable or disable python mode."});

    // pybe inject and pybe gil are manual buttons for functionality that is handled automatically by the backend.

    root.addSubCmd({
        .name = "maint pybe inject", .ptype = proc_required,
        .fn = pybe_inject,
        .help = nullptr, .desc = "Inject the python backend into the user's app."});

    root.addSubCmd({
        .name = "maint pybe gil", .ptype = proc_required,
        .fn = pybe_get_GIL,
        .help = nullptr, .desc = "Obtain the cpython GIL and halt the user's app."});

}

//===========================================================================
// Connection between the python interface and the gdb4hpc engine
//===========================================================================


PyGdb* PyImpl::m_gdb = nullptr;

PyObject* PyImpl::eval(PyObject* /*self*/, PyObject* args) {
    if (!PyTuple_Check(args)) {
        PrintOutMsg("Python arguments is invalid");
        Py_RETURN_NONE;
    }

    if (PyTuple_Size(args) != 1) {
        PrintOutMsg("Program expression must be a string\n");
        Py_RETURN_NONE;
    }

    auto arg = PyTuple_GetItem(args, 0);
    if (!PyUnicode_Check(arg)) {
        PrintOutMsg("Program expression must be a string\n");
        Py_RETURN_NONE;
    }

    const char* argStr = PyUnicode_AsUTF8(arg); // owned by python - don't delete

    try {
        auto val = m_gdb->eval(argStr);
        auto maker = PyObjectMaker{};
        val->apply(nullptr, maker);

        return maker.getPyObject();

    } catch (EvalErrorAlreadyReported& err) {
        Py_RETURN_NONE; // this was not reported as the command failing
    } catch (EvalError& err) {
        PrintOutError("%s\n", err.what());
        Py_RETURN_NONE;
    } catch (std::exception& err) {
        PrintOutError("%s\n", err.what());
        Py_RETURN_NONE;
    }
    
    return PyLong_FromLong(2);
}



//===========================================================================
// Python class definition
//===========================================================================
static PyMethodDef gdb4hpcMethods[] = {
                                   { "e", PyImpl::eval, METH_VARARGS,
                                     "Evaluate a program expression."},
                                   { nullptr, nullptr, 0, nullptr } };

static struct PyModuleDef gdb4hpcModule = {
                                       PyModuleDef_HEAD_INIT,
                                       "gdb4hpc",
                                       nullptr,
                                       -1,
                                       gdb4hpcMethods,
                                       nullptr,  // m_slots
                                       nullptr, // m_travers
                                       nullptr, // m_clear
                                       nullptr  // m_free
};

PyMODINIT_FUNC PyInit_gdb4hpc() {
    return PyModule_Create(&gdb4hpcModule);
}


//===========================================================================
// Python mode setup manager
//===========================================================================
PyGdb::PyGdb() {
    if (PyImport_AppendInittab("gdb4hpc", PyInit_gdb4hpc) == -1) {
        PrintOutMsg("Could not add gdb4hpc module for python mode\n");
        return;
    }

    Py_Initialize();

    // make this configurable in the future
    PyRun_SimpleString("import gdb4hpc as gdb");
    PyImpl::setGdb(this);
}

PyGdb::~PyGdb() {
    Py_FinalizeEx();
}

void PyGdb::executeLine(rec_set_t* rec_set, const std::string& command) {
    m_rec_set = rec_set;
    PyRun_SimpleString(command.c_str());
}

std::unique_ptr<Value> PyGdb::eval(const std::string& exprStr) {
    auto expr = ParseString(exprStr);
    return expr->evalTop(m_rec_set);
}

//===========================================================================
// Translate data into a py object
//===========================================================================

void PyObjectMaker::beginReply(const std::string& label) {
    if (label.empty()) return;
    finishReply();
    m_key = label;
}

void PyObjectMaker::finishReply() {
    if (m_key.empty()) return;
    if (!m_pyObject) {
        m_pyObject = PyDict_New();
    }

    auto obj = makePyObject();
    PyDict_SetItemString(m_pyObject, m_key.c_str(), obj);
    Py_DECREF(obj);
    m_key.clear();
}

void PyObjectMaker::operator()(long long val) {
    if (m_child) {
        (*m_child)(val);
    } else if (auto obj = PyLong_FromLongLong(val)) {
        m_pyObjs.push_back(obj);
    }
        
}
void PyObjectMaker::operator()(long val) {
    if (m_child) {
        (*m_child)(val);
    } else if (auto obj = PyLong_FromLong(val)) {
        m_pyObjs.push_back(obj);
    }
}
void PyObjectMaker::operator()(double val) {
    if (m_child) {
        (*m_child)(val);
    } else if (auto obj = PyFloat_FromDouble(val)) {
        m_pyObjs.push_back(obj);
    }
}
void PyObjectMaker::operator()(long double val) {
    if (m_child) {
        (*m_child)(val);
    } else if (auto obj = PyFloat_FromDouble(val)) {
        m_pyObjs.push_back(obj);
    }
}
void PyObjectMaker::operator()(const std::string& val) {
    if (m_child) {
        (*m_child)(val);
    } else if (auto obj = PyUnicode_FromString(val.c_str())) {
        m_pyObjs.push_back(obj);
    }
}
void PyObjectMaker::operator()(const ais::Error& val) {
    error(val.what());
}

void PyObjectMaker::nest(const ais::Type* t, int refIdx) {
    if (m_child) m_child->nest(t,refIdx);
    else m_child.reset(new PyObjectMaker);
}

void PyObjectMaker::unnest() {
    if (!m_child) {
        return; // mismatched nest/unnest
    } else if (m_child->m_child) {
        m_child->unnest();
    } else {
        m_pyObjs.push_back(m_child->getPyObject());
        m_child.reset();
    }
}

void PyObjectMaker::error(const std::string& err) {
    PrintOutMsg("error: %s\n", err.c_str());
}

PyObjectMaker::~PyObjectMaker() {
    if (m_pyObject) {
        Py_DECREF(m_pyObject);
    }
}

PyObject* PyObjectMaker::makePyObject() {
    if (m_pyObjs.empty()) {
        Py_RETURN_NONE;
    }
    auto n = m_pyObjs.size();
    PyObject* obj = nullptr;
    if (n == 1) {
        obj = m_pyObjs[0];
    } else {
        obj = PyTuple_New(n);
        for (size_t i=0; i<n; ++i) PyTuple_SetItem(obj, i, m_pyObjs[i]);
    }
    m_pyObjs.clear();
    return obj;
}
    
PyObject* PyObjectMaker::getPyObject() {
    finishReply();
    if (!m_pyObject) {
        m_pyObject = makePyObject();
    }
            
    Py_INCREF(m_pyObject);
    return m_pyObject;
}
