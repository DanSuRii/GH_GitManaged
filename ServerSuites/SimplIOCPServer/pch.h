// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

#ifndef PCH_H
#define PCH_H

// TODO: add headers that you want to pre-compile here
#include <WinSock2.h>

#include <cctype>

#include <memory>

#include <atomic>
#include <thread>
#include <mutex>
#include <shared_mutex>

#include <string>

#include <list>
#include <queue>
#include <stack>

#include "types.h"
#include "constants.h"
#include "utilities.h"


#include <sstream>

#include "logger.h"

#include "AgentDestructor.h"

#include "ICompletionKey.h"
#include "IOMsgDispatcher.h"

#include "msgs.h"
#include "msgtypes.h"

#endif //PCH_H
