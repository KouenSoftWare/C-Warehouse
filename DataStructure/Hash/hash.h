#ifndef JK_HASH_H  
#define JK_HASH_H

#include <string>
#include <memory>

namespace jk{
namespace ds{

const int cInitHashSize = 1007;
const double cHashChangeRate = 0.5;
enum HashMethod{Insert, Remove};

class HashError{};
class HashRehashError:HashError{};


} // ds namespace end
} // jk namespace end

#endif
