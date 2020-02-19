#pragma once

#include <string>
#include <vector>

class IStream; class OStream; class Op; class UnaryOp; class FrontEnd;

//! Action class to build an operation tree from a string
/*! This class is the main point of interaction when building up a 
  operator tree.   

  Lookat: this class is currently both building the operation tree and 
  evaluating it.   The evaluation should be separated out, which would
  probably mean pulling out the command-stream and result-stream operations
  to something that could be applied at evaluation time.

  The parsing functionality here should be treated as strictly temporary.
  In practice, we'd tie into the yacc based approach already set up in the 
  real front end.  Our current interest is more in tying into an abstract
  factory to make adding operators easy.
 */
class OpBuilder
{
public:
    // Constructor.   This object exists only during a single debugger command.
    OpBuilder(const FrontEnd& frontEnd);

    // Recursively translate an expression string into an operator tree.   
    /* \a head and \a tail are output that contain the first and last operators
       in the chain - they can be the same obviously.   
       Lookat: the head/tail approach may very well need to be extended to 
       handle more complicated expressions */
    bool parse(const std::string& expr, std::shared_ptr<UnaryOp>& head, 
               std::shared_ptr<Op>& tail);

    // Helper functions to connecting VariableEval operations to the back-end.   
    // Consider moving these to an OpEvaluator class or similar.
    std::shared_ptr<OStream> getCommandStreamForProcSet(const std::string& procSet) const;
    std::shared_ptr<IStream> getResultStreamForProcSet(const std::string& procSet) const;

    // For function formed operators, this will contain the string for the expression 
    // inside the parenthesis.  The parsing will typically evaluate it as a literal or 
    // create an input operator(s) by parsing it resursively.
    const std::string& getOpArgs() { return m_opArgs; }

    // Add a printout operator if the expression doesn't otherwise have a terminator expression
    void addDefaultTerminator();

    // Evaluate the expression.
    void evaluate();

private:
    void addOp(std::shared_ptr<Op> op);

    const FrontEnd& m_frontEnd;
    std::string m_expression;
    std::string m_opArgs;
    std::vector<std::shared_ptr<Op>> m_ops;
};

