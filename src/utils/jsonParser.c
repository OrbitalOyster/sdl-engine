#include "utils/jsonParser.h"

#include "utils/JSON/token.h"
#include "utils/JSON/tokenMap.h"
#include "utils/JSON/stringify.h"
#include "utils/JSON/parser.h"

#include <stdio.h>

#include "utils/debug.h"

void tokenTest() {

  readFile("s.json");

  //char *json = "{ hello: \"world\" }";
  //struct Token *JSON = stringToToken(json);

  /*
  struct TokenMap *map = createTokenMap();
  struct Token *intToken = createNumberToken(14);
  struct Token *strToken = createStringToken("Hello");

  struct Token *arrToken = createArrayToken();
  struct Token *int1 = createNumberToken(10);
  struct Token *int2 = createNumberToken(20);
  expandArrayToken(arrToken, int1);
  expandArrayToken(arrToken, int2);

  struct Token *objToken = createObjectToken();
  struct Token *boolToken = createBooleanToken(1);
  expandObjectToken(objToken, "bool", boolToken);

  expandTokenMap(map, "num", intToken);
  expandTokenMap(map, "str", strToken);
  expandTokenMap(map, "arr", arrToken);
  expandTokenMap(map, "obj", objToken);

  char *s = tokenToString(intToken);
  printf("Token to string: %s\n", s);
  free(s);
  s = tokenToString(strToken);
  printf("Token to string: %s\n", s);
  free(s);

  printf("Array token size: %u\n", getArrayTokenSize(arrToken));
  struct Token *arrElem = getArrayTokenElement(arrToken, 1);
  s = tokenToString(arrElem);
  printf("Array token to string: %s\n", s); free(s);

  s = tokenMapToString(map, 0);
  printf("Token map to string: %s\n", s);
  free(s);

  s = tokenToString(arrToken);
  printf("Array token to string: %s\n", s);
  free(s);
  s = objectTokenToString(map);
  printf("Object token to string: %s\n", s);
  free(s);

  destroyTokenMap(map);
  */
}
