// Generates a CSV file containing information about how many times a specific
// branch was taken during runtime corresponding to the given input C file.

#include "rose.h"
#include <stdlib.h>

using namespace SageBuilder;
using namespace SageInterface;

class SimpleInstrumentation : public SgSimpleProcessing
{
    public:
        void visit(SgNode* astNode);
        void insertCalls(SgIfStmt *astNode, int arr_index, SgBasicBlock *func_body1, SgBasicBlock *func_body2);
        int counter;
};

void SimpleInstrumentation::insertCalls(SgIfStmt *astNode, int arr_index, SgBasicBlock *func_body1, SgBasicBlock *func_body2)
{
    SgName name1 = "increment_if";
    SgName name2 = "increment_else";
    SgName param_name = "index";
    SgExprStatement *callStmt_1 = buildFunctionCallStmt(name1, buildVoidType(), buildExprListExp(buildIntVal(arr_index)), astNode);
    SgExprStatement *callStmt_2 = buildFunctionCallStmt(name2, buildVoidType(), buildExprListExp(buildIntVal(arr_index)), astNode);
    insertStatement(astNode->get_true_body(), callStmt_1);

    if (astNode->get_false_body()!=NULL)
        insertStatement(astNode->get_false_body(), callStmt_2);
    else
        astNode->set_false_body(callStmt_2);
}

void SimpleInstrumentation::visit(SgNode *astNode)
{
    SgGlobal *globalScope = isSgGlobal(astNode);
    Rose_STL_Container<SgNode*> ifStmtList;

    if (globalScope != NULL) {
        // Count number of if nodes and create an array of pointers to them
        ifStmtList = NodeQuery::querySubTree (globalScope,V_SgIfStmt);
        counter = 0;
        for (Rose_STL_Container<SgNode*>::iterator i = ifStmtList.begin(); i != ifStmtList.end(); i++) {
            SgIfStmt* ifStmt = isSgIfStmt(*i);
            ROSE_ASSERT(ifStmt != NULL);

            if ((*i)->get_file_info()->isCompilerGenerated() == false)
                counter++;
        }

        // Create a parameter list with a parameter
        SgTypeInt *ref_type = buildIntType();
        SgInitializedName *var1_init_name = buildInitializedName("index", ref_type);
        SgFunctionParameterList *parameterList = buildFunctionParameterList();
        appendArg(parameterList, var1_init_name);

        // Create a defining functionDeclaration (with a function body)
        SgName func_name1 = "increment_if";
        SgFunctionDeclaration * func1 = buildDefiningFunctionDeclaration (func_name1, buildVoidType(), parameterList, globalScope);
        SgBasicBlock *func_body1 = func1->get_definition()->get_body();
        SgName lhs_expr_name1 = "CountIf";
        SgName rhs_expr_name1 = "index";
        SgVarRefExp *lhs_var_ref1 = buildVarRefExp(lhs_expr_name1, func_body1);
        SgVarRefExp *rhs_var_ref1 = buildVarRefExp(rhs_expr_name1, func_body1);
        SgExprStatement* new_stmt1 = buildExprStatement(buildPlusPlusOp(buildPntrArrRefExp(lhs_var_ref1, rhs_var_ref1)));

        // Create a defining functionDeclaration (with a function body)
        SgName func_name2 = "increment_else";
        SgFunctionDeclaration *func2 = buildDefiningFunctionDeclaration(func_name2, buildVoidType(), parameterList, globalScope);
        SgBasicBlock *func_body2 = func2->get_definition()->get_body();
        SgName lhs_expr_name2 = "CountElse";
        SgName rhs_expr_name2 = "index";
        SgVarRefExp *lhs_var_ref2 = buildVarRefExp(lhs_expr_name2, func_body2);
        SgVarRefExp *rhs_var_ref2 = buildVarRefExp(rhs_expr_name2, func_body2);
        SgExprStatement* new_stmt2 = buildExprStatement(buildPlusPlusOp(buildPntrArrRefExp(lhs_var_ref2, rhs_var_ref2)));

        // Insert a statement into the function body
        prependStatement(new_stmt1, func_body1);
        prependStatement(new_stmt2, func_body2);
        prependStatement(func1, globalScope);
        prependStatement(func2, globalScope);

        // Create a defining functionDeclaration (with a function body)
        SgName evaluate_func = "evaluate";
        SgFunctionDeclaration *func = buildDefiningFunctionDeclaration(evaluate_func, buildVoidType(), buildFunctionParameterList(), globalScope);
        SgBasicBlock *func_body = func->get_definition()->get_body();

        // Construct a while loop
        SgName i_name = "i";
        SgVarRefExp *i_ref = buildVarRefExp(i_name, func_body);
        SgPlusPlusOp *pp_expression = buildPlusPlusOp(i_ref);
        SgExprStatement* while_body = buildExprStatement(pp_expression);
        SgVarRefExp *while_lhs_ref = buildVarRefExp(i_name,func_body);
        SgExprStatement *while_condition = buildExprStatement(buildLessThanOp(while_lhs_ref, buildIntVal(counter)));
        SgWhileStmt *while_stmt = buildWhileStmt(while_condition, while_body, NULL);
        SgVariableDeclaration *i_var = buildVariableDeclaration(i_name, buildIntType());

        prependStatement(while_stmt, func_body);
        SgExprStatement *print_func = buildFunctionCallStmt("printf", buildIntType(), buildExprListExp(buildStringVal("%d %d\\n"),
                                                            buildPntrArrRefExp(lhs_var_ref1, while_lhs_ref),
                                                            buildPntrArrRefExp(lhs_var_ref2, while_lhs_ref)), func_body);
        insertStatementBefore(while_body, print_func, true);
        prependStatement(i_var, func_body);
        SgExprStatement *assign_i = buildAssignStatement(buildVarRefExp(i_name, func_body), buildIntVal(0));
        insertStatementBefore(while_stmt, assign_i);
        prependStatement(func, globalScope);

        SgVariableDeclaration *variableDeclaration1 = buildVariableDeclaration("CountIf", buildArrayType(buildIntType(), buildIntVal(counter)));
        prependStatement(variableDeclaration1, globalScope);
        SgVariableDeclaration *variableDeclaration2 = buildVariableDeclaration("CountElse", buildArrayType(buildIntType(), buildIntVal(counter)));
        prependStatement(variableDeclaration2, globalScope);

        int lineNumber[counter];

        for (Rose_STL_Container<SgNode*>::iterator i = ifStmtList.begin(); i != ifStmtList.end(); i++) {
            SgIfStmt* ifStmt = isSgIfStmt(*i);
            ROSE_ASSERT(ifStmt != NULL);

            if ((*i)->get_file_info()->isCompilerGenerated() == false) {
                int arr_index = i - ifStmtList.begin();
                SgIfStmt *ifBlock = isSgIfStmt(astNode);
                ifBlock = ifStmt;
                if (ifBlock != NULL)
                    lineNumber[arr_index] = ifBlock->get_file_info()->get_line();
                insertCalls(ifStmt, (int)arr_index, func_body1, func_body2);
            }
        }

        std::vector<SgNode*> funcDefList = NodeQuery::querySubTree(globalScope, V_SgFunctionDefinition);
        std::vector<SgNode*>::iterator iter;

        for (iter = funcDefList.begin(); iter != funcDefList.end(); iter++) {
            SgFunctionDefinition *cur_def = isSgFunctionDefinition(*iter);
            if (cur_def->get_mangled_name().getString().substr(0, 4).compare("main"))
                continue;

            ROSE_ASSERT(cur_def);
            SgBasicBlock *body = cur_def->get_body();
            // Build the call statement for each place
            SgExprStatement *callStmt1 = buildFunctionCallStmt("evaluate", buildIntType(), buildExprListExp(), body);

            // Instrument the function
            SgFunctionDeclaration *mainFunc = findMain(globalScope);
            SgBasicBlock *main_body = mainFunc->get_definition()->get_body();
            pushScopeStack(main_body);
            SgStatement *laststmt = getLastStatement(topScopeStack());
            laststmt = getLastStatement(topScopeStack());
            insertStatementBefore(laststmt, callStmt1);
        }
    }
}

int main(int argc, char * argv[])
{
    // Initialize and check compatibility. See rose::initialize
    ROSE_INITIALIZE;
    SgProject *project = frontend(argc, argv);
    ROSE_ASSERT(project != NULL);
    SimpleInstrumentation treeTraversal;
    treeTraversal.traverseInputFiles(project, preorder);
    AstTests::runAllTests(project);
    return backend(project);
}
