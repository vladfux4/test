#include <glog/logging.h>
#include <gflags/gflags.h>

#include <controller.h>

DEFINE_uint64(block_size, 8, "Block size");
DEFINE_uint64(block_count, 10, "Count of block that will be generated");
DEFINE_uint64(generator_threads_count, 2, "Count of threads that will generate blocks");
DEFINE_uint64(compute_threads_count, 4, "Count of threads that will compute CRC32 MPEG2");

bool IsGreaterThanZero(const char* flagname, const uint64_t value) {
  return (value > 0);
}

DEFINE_validator(block_size, &IsGreaterThanZero);
DEFINE_validator(block_count, &IsGreaterThanZero);
DEFINE_validator(generator_threads_count, &IsGreaterThanZero);
DEFINE_validator(compute_threads_count, &IsGreaterThanZero);

void InitLogger(const char* argv0) {
  FLAGS_logtostderr = 1;
  FLAGS_minloglevel = 0;
  FLAGS_colorlogtostderr = 1;
  FLAGS_log_prefix = 1;

  google::InitGoogleLogging(argv0);
}

int main(int argc, char* argv[]) {
  gflags::SetUsageMessage(
      "Test APP \n"
      "-block_count (Count of block that will be generated) - default: 10 \n"
      "-block_size (Block size) - default: 8 \n"
      "-generator_threads_count (Count of threads that will generate blocks) - default: 2 \n"
      "-compute_threads_count (Count of threads that will compute CRC32 MPEG2) - default: 4 \n"
  );

  gflags::SetVersionString("0.0.1");
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  InitLogger(argv[0]);

  LOG(INFO) << "Start";

  Controller ctrl(FLAGS_block_size, FLAGS_block_count,
                  FLAGS_generator_threads_count, FLAGS_compute_threads_count);
  ctrl.Start();
  ctrl.Join();

  LOG(INFO) << "Done";
  gflags::ShutDownCommandLineFlags();

  return 0;
}
