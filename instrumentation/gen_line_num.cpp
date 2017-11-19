// Generates line number of all the if statements in the input file
#include "rose.h"

using namespace std;

int main( int argc, char * argv[] )
{
    ROSE_INITIALIZE;
    SgProject* project = frontend(argc,argv);
    ROSE_ASSERT(project != NULL);
    Rose_STL_Container<SgNode*> ifStmtList = NodeQuery::querySubTree (project,V_SgIfStmt);

    for (Rose_STL_Container<SgNode*>::iterator i = ifStmtList.begin(); i != ifStmtList.end(); i++) {
        SgIfStmt *ifStmt = isSgIfStmt(*i);
        ROSE_ASSERT(ifStmt != NULL);

        if ( (*i)->get_file_info()->isCompilerGenerated() == false)
            printf ("%d\n", ifStmt->get_file_info()->get_line());
    }
    return 0;
}
