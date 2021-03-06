#include "argparse.hpp"
#include "common.h"
#include "state.h"
#include "utils.h"
#include <ncurses.h>
#include <unistd.h>

// Function to check for keypress taken from https://github.com/ajpaulson/learning-ncurses/blob/master/kbhit.c
int kbhit()
{
  int ch, r;

  // turn off getch() blocking and echo
  nodelay(stdscr, TRUE);
  noecho();

  // check for input
  ch = getch();
  if (ch == ERR) // no input
    r = FALSE;
  else // input
  {
    r = TRUE;
    ungetch(ch);
  }

  // restore block and echo
  echo();
  nodelay(stdscr, FALSE);

  return (r);
}

int getKeyPress()
{
  while (!kbhit())
  {
    usleep(100000ul);
    refresh();
  }
  return getch();
}

int main(int argc, char *argv[])
{
  // Defining arguments
  argparse::ArgumentParser program("jaffar-play", JAFFAR_VERSION);

  program.add_argument("savFile")
    .help("Specifies the path to the SDLPop savefile (.sav) from which to start.")
    .required();

  program.add_argument("solutionFile")
    .help("path to the Jaffar solution (.sol) file to run.")
    .required();

  program.add_argument("--reproduce")
    .help("Plays the entire sequence without interruptions")
    .default_value(false)
    .implicit_value(true);

  // Parsing command line
  try
  {
    program.parse_args(argc, argv);
  }
  catch (const std::runtime_error &err)
  {
    fprintf(stderr, "[Jaffar] Error parsing command line arguments: %s\n%s", err.what(), program.help().str().c_str());
    exit(-1);
  }

  // Getting reproduce path
  bool isReproduce = program.get<bool>("--reproduce");

  // Getting savefile path
  std::string saveFilePath = program.get<std::string>("savFile");

  // Loading save file contents
  std::string saveString;
  bool status = loadStringFromFile(saveString, saveFilePath.c_str());
  if (status == false) EXIT_WITH_ERROR("[ERROR] Could not load save state from file: %s\n", saveFilePath.c_str());

  // If sequence file defined, load it and play it
  std::string moveSequence;
  std::string solutionFile = program.get<std::string>("solutionFile");
  status = loadStringFromFile(moveSequence, solutionFile.c_str());
  if (status == false) EXIT_WITH_ERROR("[ERROR] Could not find or read from solution file: %s\n%s \n", solutionFile.c_str(), program.help().str().c_str());

  // Initializing ncurses screen
  initscr();
  cbreak();
  noecho();
  nodelay(stdscr, TRUE);
  scrollok(stdscr, TRUE);

  // Getting move list
  const auto moveList = split(moveSequence, ' ');

  // Getting sequence size
  const int sequenceLength = moveList.size()-1;

  // Printing info
  printw("[Jaffar] Playing sequence file: %s\n", solutionFile.c_str());
  printw("[Jaffar] Sequence Size: %d moves.\n", sequenceLength-1);
  printw("[Jaffar] Generating frame sequence...\n");

  refresh();

  // Initializing replay generating SDLPop Instance
  SDLPopInstance genSDLPop("libsdlPopLib.so", false);
  genSDLPop.initialize(false);

  // Storage for sequence frames
  std::vector<std::string> frameSequence;

  // Starting replay creation
  genSDLPop.init_record_replay();
  genSDLPop.start_recording();

  // Initializing generating State Handler
  State genState(&genSDLPop, saveString);

  // Saving initial frame
  frameSequence.push_back(genState.saveState());

  // Iterating move list in the sequence
  for (int i = 0; i < sequenceLength-1; i++)
  {
    genSDLPop.performMove(moveList[i]);
    genSDLPop.advanceFrame();

    // Storing new frame
    frameSequence.push_back(genState.saveState());
  }

//  saveStringToFile(sequenceJs.dump(2).c_str(), "sequence.js");
  printw("[Jaffar] Opening SDLPop window...\n");

  // Initializing showing SDLPop Instance
  SDLPopInstance showSDLPop("libsdlPopLib.so", false);
  showSDLPop.initialize(true);

  // Initializing State Handler
  State showState(&showSDLPop, saveString);

  // Setting window title
  SDL_SetWindowTitle(*showSDLPop.window_, "Jaffar Play");

  // Printing initial frame info
  showSDLPop.draw();

  // Variable for current step in view
  int currentStep = 0;

  // Print command list
  if (isReproduce == false)
  {
   printw("[Jaffar] Available commands:\n");
   printw("[Jaffar]  n: -1 m: +1 | h: -10 | j: +10 | y: -100 | u: +100 \n");
   printw("[Jaffar]  g: set RNG | l: loose tile sound | s: quicksave | r: create replay | q: quit  \n");
   printw("[Jaffar]  1: set lvl1 music | w: set current hp | e: set max hp \n");
  }

  // Flag to display frame information
  bool showFrameInfo = true;

  // Interactive section
  int command;
  do
  {
    // Loading requested step
    showState.loadState(frameSequence[currentStep]);

    // Calculating timing
    size_t curMins = currentStep / 720;
    size_t curSecs = (currentStep % 720) / 12;
    size_t curMilliSecs = floor((double)(currentStep % 12) / 0.012);

    showSDLPop._IGTMins = curMins;
    showSDLPop._IGTSecs = curSecs;
    showSDLPop._IGTMillisecs = curMilliSecs;
    showSDLPop._move = moveList[currentStep];

    size_t sequenceStep = sequenceLength-1;
    size_t maxMins = sequenceStep / 720;
    size_t maxSecs = (sequenceStep % 720) / 12;
    size_t maxMilliSecs = floor((double)(sequenceStep % 12) / 0.012);

    // Draw requested step
    showSDLPop.draw();

    if (showFrameInfo)
    {
      printw("[Jaffar] ----------------------------------------------------------------\n");
      printw("[Jaffar] Current Step #: %d / %d\n", currentStep, sequenceLength-1);
      printw("[Jaffar]  + Current IGT:    %2lu:%02lu.%03lu / %2lu:%02lu.%03lu\n", curMins, curSecs, curMilliSecs, maxMins, maxSecs, maxMilliSecs);
      printw("[Jaffar]  + Move: %s\n", moveList[currentStep].c_str());

      printw("[Jaffar]  + [Kid]   Room: %d, Pos.x: %3d, Pos.y: %3d, Row: %2d, Col: %2d, Fall.y: %d, Frame: %3d, HP: %d/%d, Dir: %d, SeqId: %d\n",
             int(showSDLPop.Kid->room),
             int(showSDLPop.Kid->x),
             int(showSDLPop.Kid->y),
             int(showSDLPop.Kid->curr_row),
             int(showSDLPop.Kid->curr_col),
             int(showSDLPop.Kid->fall_y),
             int(showSDLPop.Kid->frame),
             int(*showSDLPop.hitp_curr),
             int(*showSDLPop.hitp_max),
             int(showSDLPop.Kid->direction),
             showSDLPop.Kid->curr_seq);

      printw("[Jaffar]  + [Guard] Room: %d, Pos.x: %3d, Pos.y: %3d, Row: %2d, Col: %2d, Fall.y: %d, Frame: %3d, HP: %d/%d, Dir: %d, SeqId: %d\n",
             int(showSDLPop.Guard->room),
             int(showSDLPop.Guard->x),
             int(showSDLPop.Guard->y),
             int(showSDLPop.Guard->curr_row),
             int(showSDLPop.Guard->curr_col),
             int(showSDLPop.Guard->fall_y),
             int(showSDLPop.Guard->frame),
             int(*showSDLPop.guardhp_curr),
             int(*showSDLPop.guardhp_max),
             int(showSDLPop.Guard->direction),
             showSDLPop.Guard->curr_seq);

      // Level-Specific Settings
      if (*showSDLPop.current_level == 9) printw("[Jaffar]  + Rightmost Door: %d\n", showSDLPop.level->bg[349]);

      printw("[Jaffar]  + Guard Can See Kid: %d\n", *showSDLPop.can_guard_see_kid);
      printw("[Jaffar]  + Is Guard Notice: %d\n", *showSDLPop.is_guard_notice);
      printw("[Jaffar]  + Guard Refrac: %d\n", *showSDLPop.guard_refrac);
      printw("[Jaffar]  + Guard Notice Timer: %d\n", *showSDLPop.guard_notice_timer);
      printw("[Jaffar]  + Exit Door Open: %s (%d)\n", showSDLPop.isLevelExitDoorOpen() ? "Yes" : "No", *showSDLPop.leveldoor_open);
      printw("[Jaffar]  + Reached Checkpoint: %s (%d)\n", *showSDLPop.checkpoint ? "Yes" : "No", *showSDLPop.checkpoint);
      printw("[Jaffar]  + Feather Fall: %d\n", *showSDLPop.is_feather_fall);
      printw("[Jaffar]  + Need Lvl1 Music: %d\n", *showSDLPop.need_level1_music);
      printw("[Jaffar]  + RNG State: 0x%08X (Last Loose Tile Sound Id: %d)\n", *showSDLPop.random_seed, *showSDLPop.last_loose_sound);
      printw("[Jaffar]  + Demo Index: %d, Time: %d\n", *showSDLPop.demo_index, *showSDLPop.demo_time);
      printw("[Jaffar]  + Exit Room Timer: %d\n", *showSDLPop.exit_room_timer);

      // Mobs are moving objects (only falling tiles)
      printw("[Jaffar]  + Moving Objects:\n");
      for (int i = 0; i < *showSDLPop.mobs_count; ++i)
      {
        const auto &mob = (*showSDLPop.mobs)[i];
        printw("[Jaffar]    + Room: %d, X: %d, Y: %d, Speed: %d, Type: %d, Row: %d\n", mob.room, mob.xh, mob.y, mob.speed, mob.type, mob.row);
      }

      // Trobs are stationary animated objects.
      printw("[Jaffar]  + Active Objects:\n");
      for (int i = 0; i < *showSDLPop.trobs_count; ++i)
      {
        const auto &trob = (*showSDLPop.trobs)[i];
        const auto idx = (trob.room - 1) * 30 + trob.tilepos;
        const auto fgstate = showSDLPop.level->fg[idx];
        const auto bgstate = showSDLPop.level->bg[idx];
        const auto type = showSDLPop.level->fg[idx] & 0x1f;
        printw("[Jaffar]    + Index: %d, FG State: %d, BG State: %d, Type: ", idx, fgstate, bgstate);

        switch (type)
        {
        case tiles_0_empty: printw("Empty"); break;
        case tiles_1_floor: printw("Floor"); break;
        case tiles_3_pillar: printw("Pillar"); break;
        case tiles_5_stuck: printw("Stuck"); break;
        case tiles_6_closer: printw("Closer"); break;
        case tiles_7_doortop_with_floor: printw("Doortop With Floor"); break;
        case tiles_8_bigpillar_bottom: printw("Big Pillar Bottom"); break;
        case tiles_9_bigpillar_top: printw("Big Pillar Top"); break;
        case tiles_10_potion: printw("Potion"); break;
        case tiles_12_doortop: printw("Doortop"); break;
        case tiles_14_debris: printw("Debris"); break;
        case tiles_15_opener: printw("Opener"); break;
        case tiles_17_level_door_right: printw("Level Door Right"); break;
        case tiles_19_torch: printw("Torch"); break;
        case tiles_20_wall: printw("Wall"); break;
        case tiles_21_skeleton: printw("Skeleton"); break;
        case tiles_22_sword: printw("Sword"); break;
        case tiles_23_balcony_left: printw("Balcony Left"); break;
        case tiles_24_balcony_right: printw("Balcony Right"); break;
        case tiles_25_lattice_pillar: printw("Lattice Pillar"); break;
        case tiles_26_lattice_down: printw("Lattice Down"); break;
        case tiles_27_lattice_small: printw("Lattice Small"); break;
        case tiles_28_lattice_left: printw("Lattice Left"); break;
        case tiles_29_lattice_right: printw("Lattice Right"); break;
        case tiles_30_torch_with_debris: printw("Torch With Debris"); break;
        case tiles_13_mirror: printw("Mirror"); break;
        case tiles_4_gate: printw("Gate"); break;
        case tiles_2_spike: printw("Spike"); break;
        case tiles_11_loose: printw("Loose Tile"); break;
        case tiles_16_level_door_left: printw("Level Door Left"); break;
        case tiles_18_chomper: printw("Chomper"); break;
        default:
          EXIT_WITH_ERROR("Unknown trob type: %d\n", int(type));
       }
       printw("\n");
      }

      // Tracking special tiles in level
      printw("[Jaffar]  + Static Tile Information:\n");
      for (int i = 0; i < 720; i++)
      {
        const auto type = showSDLPop.level->fg[i] & 0x1f;
        const auto fgstate = showSDLPop.level->fg[i];
        const auto bgstate = showSDLPop.level->bg[i];

        switch (type)
        {
         case tiles_14_debris:
          printw("[Jaffar]    + Index: %i, FG State: %d, BG State: %d, Type: Debris\n", i, fgstate, bgstate);
          break;
         case tiles_4_gate:
          printw("[Jaffar]    + Index: %i, FG State: %d, BG State: %d, Type: Gate\n", i, fgstate, bgstate);
          break;
         case tiles_16_level_door_left:
          printw("[Jaffar]    + Index: %i, FG State: %d, BG State: %d, Type: Level Door Left\n", i, fgstate, bgstate);
          break;
         default:
         break;
       }
      }
   }

    // Resetting show frame info flag
    showFrameInfo = true;

    // If we're reproducing do not have an interactive interface
    if (isReproduce)
    {
     currentStep++;
     if (currentStep >= sequenceLength) break;
     continue;
    }

    // Get command
    command = getKeyPress();

    // Advance/Rewind commands
    if (command == 'n') currentStep = currentStep - 1;
    if (command == 'm') currentStep = currentStep + 1;
    if (command == 'h') currentStep = currentStep - 10;
    if (command == 'j') currentStep = currentStep + 10;
    if (command == 'y') currentStep = currentStep - 100;
    if (command == 'u') currentStep = currentStep + 100;

    // Correct current step if requested more than possible
    if (currentStep < 0) currentStep = 0;
    if (currentStep >= sequenceLength) currentStep = sequenceLength-1;

    // Replay creation command
    if (command == 'r')
    {
      // Storing replay file
      std::string replayFileName = "jaffar.p1r";
      genSDLPop.save_recorded_replay(replayFileName.c_str());
      printw("[Jaffar] Replay saved in '%s'.\n", replayFileName.c_str());

      // Do no show frame info again after this action
      showFrameInfo = false;
    }

    // Quicksave creation command
    if (command == 's')
    {
      // Storing replay file
      std::string saveFileName = "jaffar.sav";

      // Saving frame info to file
      bool status = saveStringToFile(frameSequence[currentStep], saveFileName.c_str());
      if (status == true) printw("[Jaffar] State saved in '%s'.\n", saveFileName.c_str());
      if (status == false) printw("[Jaffar] Error saving file '%s'.\n", saveFileName.c_str());

      // Do no show frame info again after this action
      showFrameInfo = false;
    }

    // RNG setting command
    if (command == 'g')
    {
      // Obtaining RNG state
      printw("Enter new RNG state: ");

      // Setting input as new rng
      char str[80];
      getstr(str);
      *showSDLPop.random_seed = std::stol(str);

      // Replacing current sequence
      frameSequence[currentStep] = showState.saveState();
    }

    // Set current HP
    if (command == 'w')
    {
      // Obtaining RNG state
      printw("Enter new current HP: ");

      // Setting input as new rng
      char str[80];
      getstr(str);
      *showSDLPop.hitp_curr = std::stol(str);

      // Replacing current sequence
      frameSequence[currentStep] = showState.saveState();
    }

    // Set max HP
    if (command == 'e')
    {
      // Obtaining RNG state
      printw("Enter new max HP: ");

      // Setting input as new rng
      char str[80];
      getstr(str);
      *showSDLPop.hitp_max = std::stol(str);

      // Replacing current sequence
      frameSequence[currentStep] = showState.saveState();
    }

    // loose tile sound setting command
    if (command == 'l')
    {
      // Obtaining RNG state
      printw("Enter new last loose tile sound id: ");

      // Setting input as new rng
      char str[80];
      getstr(str);
      *showSDLPop.last_loose_sound = std::stoi(str);

      // Replacing current sequence
      frameSequence[currentStep] = showState.saveState();
    }

    // loose tile sound setting command
    if (command == '1')
    {
      // Obtaining RNG state
      printw("Enter new need level 1 music value: ");

      // Setting input as new rng
      char str[80];
      getstr(str);
      *showSDLPop.need_level1_music = std::stoi(str);

      // Replacing current sequence
      frameSequence[currentStep] = showState.saveState();
    }

  } while (command != 'q');

  // Ending ncurses window
  endwin();
}
