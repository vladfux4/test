#include <glog/logging.h>
#include <controller.h>

void InitLogger(const char* argv0) {
  FLAGS_logtostderr = 1;
  FLAGS_minloglevel = 0;
  FLAGS_colorlogtostderr = 1;
  FLAGS_log_prefix = 1;
  FLAGS_v = 1;

  google::InitGoogleLogging(argv0);
}


int main(int argc, char* argv[]) {
  InitLogger(argv[0]);

  LOG(INFO) << "Start";

  Controller ctrl;
  ctrl.Start();
  ctrl.Join();

  LOG(INFO) << "Done";

  return 0;
}
