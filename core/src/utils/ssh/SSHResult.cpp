#include "SSHResult.hpp"

SSHResult::SSHResult() {
}

SSHResult::~SSHResult() {
}

int
SSHResult::getStatus() {
  return mstatus;
}

void
SSHResult::setStatus(int st) {
  mstatus = st;
}

string
SSHResult::getOutput() {
  return mout;
}

void
SSHResult::setOutput(string out) {
  mout = out;
}

string
SSHResult::getError() {
  return merr;
}

void
SSHResult::setError(string err) {
  merr = err;
}
