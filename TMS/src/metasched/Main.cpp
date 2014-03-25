#include "OneHostPool.hpp"


int main()
{
  OneHostPool hostPool("http://localhost:2633/RPC2", "oneadmin:oneadmin");
  return 0;
}
