#ifndef _JSON_H_
#define _JSON_H_

#include <string>
#include <map>
#include <vector>
#include "libjsonspirit/json_spirit.h"

namespace json_spirit {
typedef json_spirit::mValue value;
typedef json_spirit::mArray array;
typedef json_spirit::mObject object;
}

namespace json = json_spirit;

#endif //_JSON_H_
