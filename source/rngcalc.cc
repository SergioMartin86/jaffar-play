#include "argparse.hpp"
#include "common.h"
#include "state.h"
#include "utils.h"
#include <unistd.h>

int main(int argc, char *argv[])
{
  // Defining arguments
  argparse::ArgumentParser program("jaffar-rngcalc", JAFFAR_VERSION);

  program.add_argument("initialRNG")
    .help("Specifies the initial RNG to start with.")
    .required();

  program.add_argument("targetRNG")
    .help("Specifies the target RNG to meet.")
    .required();

  program.add_argument("newTargetRNG")
    .help("Specifies the new target RNG to backtrace.")
    .required();

  try
  {
    program.parse_args(argc, argv);
  }
  catch (const std::runtime_error &err)
  {
    fprintf(stderr, "[Jaffar] Error parsing command line arguments: %s\n%s", err.what(), program.help().str().c_str());
    exit(-1);
  }

  // Getting RNG values
  const std::string initialRNGString = program.get<std::string>("initialRNG");
  const std::string targetRNGString = program.get<std::string>("targetRNG");
  const std::string newTargetRNGString = program.get<std::string>("newTargetRNG");
  const dword initialRNG = std::stol(initialRNGString);
  const dword targetRNG = std::stol(targetRNGString);
  const dword newTargetRNG = std::stol(newTargetRNGString);

  // Initializing replay generating SDLPop Instance
  SDLPopInstance RNGPop("libsdlPopLib.so", false);

  dword nextRNG = initialRNG;
  size_t advanceCounter = 0;
  while (nextRNG != targetRNG) { nextRNG = RNGPop.advanceRNGState(nextRNG); advanceCounter++; }

  dword newInitialRNG = newTargetRNG;
  for (size_t i = 0; i < advanceCounter+10; i++) { printf("0x%X%s\n", newInitialRNG, i == advanceCounter ? "*" : ""); newInitialRNG = RNGPop.reverseRNGState(newInitialRNG); }

  printf("0x%X, 0x%X, 0x%X\n", initialRNG, nextRNG, targetRNG);
  printf("Advances: %lu\n", advanceCounter);
  printf("Use new initial: 0x%X\n", newInitialRNG);
}
