//  Copyright 2023 Hewlett Packard Enterprise Development LP.

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include "client.h"
#include "gram.h"

#define PY_SSIZE_T_CLEAN
#include "/usr/include/python3.6m/Python.h"

namespace {
// commands to handle python as a cli mode
class PythonMode : public Command {
public:
    PythonMode();
    void failed() override { cli_dec_prompt(); SetCommands(m_oldCmds); }
    
    static void init();
    static void enter();
private:
    static int executeLine(rec_set_t *rec_set, int argc, char**argv);
    static int exit(rec_set_t* rec_set, int argc, char **argv);

    static std::shared_ptr<Command> m_cmds;
    static Command *m_oldCmds;
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

static int cmd_python_cmd(rec_set_t *rec_set, int argc, char **argv) {
    if (argc < 2) {
        PythonMode::enter();
        return 1;
    } else {
        PrintOutError("inline python mode is not set up.\n");
        return 0;
    }
}

std::shared_ptr<Command> PythonMode::m_cmds = nullptr;
Command* PythonMode::m_oldCmds = nullptr;

PythonMode::PythonMode() : Command(proc_optional, executeLine) {}

void PythonMode::init() {
    m_cmds = std::make_shared<PythonMode>();

    m_cmds->addSubCmd("exit", nullptr, proc_optional, exit,
                      dummy_help, "Exit python mode");
}

// Start python mode
void PythonMode::enter() {
    PrintOutMsg("Entering python interpreter, use exit to leave.\n");
    m_oldCmds = SetCommands(m_cmds.get());
    cli_inc_prompt();
}

// Leave shell mode
int PythonMode::exit(rec_set_t *rec_set, int argc, char**argv) {
    PrintOutMsg("\nExiting python interpreter.\n");
    cli_dec_prompt();
    SetCommands(m_oldCmds);
    return 1;
}

int PythonMode::executeLine(rec_set_t *rec_set, int argc, char**argv) {
    static std::unique_ptr<PyGdb> interpreter;
    if (!interpreter)
        interpreter.reset(new PyGdb());

    auto command = ArgsToStringUnquoted(argv, argc);
    interpreter->executeLine(rec_set, command);
    return 1;
}

void InitCmd_python() {
    AddCommand("python", nullptr, proc_optional,
               cmd_python_cmd, cmd_python_help, "Enter python interpreter (experimental).");

    PythonMode::init();

};

//===========================================================================
// Connection between the python interface and the gdb4hpc engine
//===========================================================================


PyGdb* PyImpl::m_gdb = nullptr;

PyObject* PyImpl::eval(PyObject* self, PyObject* args) {
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
                                       gdb4hpcMethods
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
