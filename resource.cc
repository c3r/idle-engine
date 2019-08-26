#include "resource.h"

bool RGroupUpdate(RGroup *rgroup, RGroupUpdateEvt *ev) {
  if (rgroup == nullptr || ev == nullptr) {
    return false;
  }

  Resource *res = &(rgroup->map.at(ev->RGroupId));
  if (res == nullptr) {
    return false;
  }

  res->int_val += ev->IntDiff;
  res->str_val = ev->StrVal;

  return true;
}

Resource ResourceCreate(RMeta *meta, RIntVal int_val) {
  Resource resource;
  resource.meta = meta;
  resource.int_val = int_val;
  return resource;
}

RGroup RGroupCreate(RMeta *rmeta) {
  RGroup rg;
  RMap map;
  rg.meta = rmeta;
  rg.map = map;
  return rg;
}
