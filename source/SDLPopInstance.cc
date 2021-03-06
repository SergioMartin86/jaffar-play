#include "SDLPopInstance.h"
#include "types.h"
#include "utils.h"
#include <dlfcn.h>
#include <iostream>
#include <omp.h>

char *__prince_argv[] = {(char *)"prince"};
const char* seqNames[] = {"running", "startrun", "runstt1", "runstt4", "runcyc1", "runcyc7", "stand", "goalertstand", "alertstand", "arise", "guardengarde", "engarde", "ready", "ready_loop", "stabbed", "strikeadv", "strikeret", "advance", "fastadvance", "retreat", "strike", "faststrike", "guy4", "guy7", "guy8", "blockedstrike", "blocktostrike", "readyblock", "blocking", "striketoblock", "landengarde", "bumpengfwd", "bumpengback", "flee", "turnengarde", "alertturn", "standjump", "sjland", "runjump", "rjlandrun", "rdiveroll", "rdiveroll_crouch", "sdiveroll", "crawl", "crawl_crouch", "turndraw", "turn", "turnrun", "runturn", "fightfall", "efightfall", "efightfallfwd", "stepfall", "fall1", "patchfall", "stepfall2", "stepfloat", "jumpfall", "rjumpfall", "jumphangMed", "jumphangLong", "jumpbackhang", "hang", "hang1", "hangstraight", "hangstraight_loop", "climbfail", "climbdown", "climbup", "hangdrop", "hangfall", "freefall", "freefall_loop", "runstop", "jumpup", "highjump", "superhijump", "fallhang", "bump", "bumpfall", "bumpfloat", "hardbump", "testfoot", "stepback", "step14", "step13", "step12", "step11", "step10", "step10a", "step9", "step8", "step7", "step6", "step5", "step4", "step3", "step2", "step1", "stoop", "stoop_crouch", "standup", "pickupsword", "resheathe", "fastsheathe", "drinkpotion", "softland", "softland_crouch", "landrun", "medland", "hardland", "hardland_dead", "stabkill", "dropdead", "dropdead_dead", "impale", "impale_dead", "halve", "halve_dead", "crush", "deadfall", "deadfall_loop", "climbstairs", "climbstairs_loop", "Vstand", "Vraise", "Vraise_loop", "Vwalk", "Vwalk1", "Vwalk2", "Vstop", "Vexit", "Pstand", "Palert", "Pstepback", "Pstepback_loop", "Plie", "Pwaiting", "Pembrace", "Pembrace_loop", "Pstroke", "Prise", "Prise_loop", "Pcrouch", "Pcrouch_loop", "Pslump", "Pslump_loop", "Mscurry", "Mscurry1", "Mstop", "Mraise", "Mleave", "Mclimb", "unrecognized" };
const word seqOffsets[] = { 0x1973, 0x1975, 0x1978, 0x1981, 0x1995, 0x19A0, 0x19A6, 0x19A8, 0x19AC, 0x19C1, 0x19C4, 0x19D2, 0x19D8, 0x19DC, 0x19F9, 0x1A07, 0x1A13, 0x1A22, 0x1A2E, 0x1A3C, 0x1A42, 0x1A45, 0x1A4A, 0x1A4D, 0x1A54, 0x1A5A, 0x1A5E, 0x1A5F, 0x1A63, 0x1A68, 0x1A6E, 0x1A75, 0x1A7C, 0x1A83, 0x1A8B, 0x1A93, 0x1AB0, 0x1ACD, 0x1AFB, 0x1B04, 0x1B16, 0x1B1A, 0x1B1B, 0x1B29, 0x1B2D, 0x1B39, 0x1B53, 0x1B5A, 0x1B85, 0x1BA1, 0x1BBF, 0x1BDB, 0x1BE4, 0x1BFA, 0x1C01, 0x1C06, 0x1C1C, 0x1C38, 0x1C54, 0x1C69, 0x1C84, 0x1CA1, 0x1CA4, 0x1CD1, 0x1CD8, 0x1CDC, 0x1CEC, 0x1D04, 0x1D25, 0x1D36, 0x1D49, 0x1D4B, 0x1D4F, 0x1D68, 0x1D7D, 0x1D9B, 0x1DF6, 0x1DFC, 0x1E06, 0x1E25, 0x1E3B, 0x1E59, 0x1E78, 0x1E7D, 0x1E9C, 0x1EBB, 0x1EDA, 0x1EF7, 0x1EFC, 0x1F13, 0x1F19, 0x1F33, 0x1F48, 0x1F5D, 0x1F72, 0x1F82, 0x1F92, 0x1F9E, 0x1FA7, 0x1FAF, 0x1FB3, 0x1FCA, 0x1FDA, 0x1FFB, 0x2009, 0x202B, 0x2036, 0x203A, 0x205A, 0x209C, 0x20A5, 0x20A9, 0x20AE, 0x20BA, 0x20BE, 0x20C5, 0x20C9, 0x20CD, 0x20D1, 0x20D4, 0x20D9, 0x20DD, 0x212E, 0x2132, 0x2136, 0x214B, 0x214F, 0x2151, 0x2154, 0x2166, 0x216D, 0x2195, 0x2199, 0x21A8, 0x21B8, 0x21BC, 0x21C0, 0x21C4, 0x21E2, 0x21E6, 0x21EA, 0x21F8, 0x21FC, 0x223C, 0x2240, 0x2241, 0x2245, 0x2247, 0x2253, 0x2257, 0x225B, 0x226E, 0x2270 };

void SDLPopInstance::initialize(const bool useGUI)
{
 _IGTMins = 0;
 _IGTSecs = 0;
 _IGTMillisecs = 0;
 _move = ".";

  // Looking for sdlpop in default folders when running examples
  if (dirExists("extern/SDLPoP/"))
  {
    sprintf(*exe_dir, "extern/SDLPoP/");
    *found_exe_dir = true;
  }
  if (dirExists("../extern/SDLPoP/"))
  {
    sprintf(*exe_dir, "../extern/SDLPoP/");
    *found_exe_dir = true;
  }
  if (dirExists("../../extern/SDLPoP/"))
  {
    *found_exe_dir = true;
    sprintf(*exe_dir, "../../extern/SDLPoP/");
  }
  if (dirExists("../../../extern/SDLPoP/"))
  {
    sprintf(*exe_dir, "../../../extern/SDLPoP/");
    *found_exe_dir = true;
  }

  const char* sdlPopRootEnv = std::getenv("SDLPOP_ROOT");
  if (sdlPopRootEnv == NULL) EXIT_WITH_ERROR("[Error] SDLPOP_ROOT environment variable not found.\n");
  _sdlPopRoot = sdlPopRootEnv;

  // If not found, looking for the SDLPOP_ROOT env variable
  if (*found_exe_dir == false)
  {
    const char *envRoot = std::getenv("SDLPOP_ROOT");

    if (envRoot != NULL)
      if (dirExists(envRoot))
      {
        sprintf(*exe_dir, envRoot);
        *found_exe_dir = true;
      }
  }

  if (*found_exe_dir == false)
  {
    fprintf(stderr, "[ERROR] Could not find the root folder for  Please set the SDLPOP_ROOT environment variable to the path where SDLPop is installed.\n");
    exit(-1);
  }

  // Setting levels.dat path
  sprintf(*levels_file, "LEVELS.DAT");
  const char *lvlsPath = std::getenv("SDLPOP_LEVELS_FILE");
  if (lvlsPath != NULL) sprintf(*levels_file, lvlsPath);

  // Setting argument config
  *is_validate_mode = byte(!useGUI);
  *g_argc = 1;
  *g_argv = __prince_argv;

  // Fix feather fall problem when quickload/quicksaving
  init_copyprot();
  (*fixes)->fix_quicksave_during_feather = 1;
  (*fixes)->fix_quicksave_during_lvl1_music = 1;

  // debug only: check that the sequence table deobfuscation did not mess things up
  load_global_options();
  check_mod_param();
  turn_sound_on_off(1);
  load_mod_options();

  // CusPop option
  *is_blind_mode = 0;
  *enable_quicksave_penalty = 0;

  // Fix bug: with start_in_blind_mode enabled, moving objects are not displayed
  // until blind mode is toggled off+on??
  *need_drects = 1;

  apply_seqtbl_patches();
  *dathandle = open_dat("PRINCE.DAT", 0);

  /*video_mode =*/
  parse_grmode();

  init_timer(BASE_FPS);
  parse_cmdline_sound();

  set_hc_pal();

  *current_target_surface = rect_sthg(*onscreen_surface_, screen_rect);

  show_loading();
  set_joy_mode();

  *cheats_enabled = 0;
  *draw_mode = 0;
  *demo_mode = 0;

  init_copyprot_dialog();

  *play_demo_level = 0;


  //////////////////////////////////////////////
  // init_game_main

  *doorlink1_ad = /*&*/ level->doorlinks1;
  *doorlink2_ad = /*&*/ level->doorlinks2;
  *guard_palettes = (byte *)load_from_opendats_alloc(10, "bin", NULL, NULL);

  // (blood, hurt flash) #E00030 = red
  set_pal(12, 0x38, 0x00, 0x0C, 1);

  // (palace wall pattern) #C09850 = light brown
  set_pal(6, 0x30, 0x26, 0x14, 0);

  // Level color variations (1.3)
  *level_var_palettes = reinterpret_cast<byte *>(load_from_opendats_alloc(20, "bin", NULL, NULL));

  // PRINCE.DAT: sword
  (*chtab_addrs)[id_chtab_0_sword] = load_sprites_from_file(700, 1 << 2, 1);

  // PRINCE.DAT: flame, sword on floor, potion
  (*chtab_addrs)[id_chtab_1_flameswordpotion] = load_sprites_from_file(150, 1 << 3, 1);

  close_dat(*dathandle);
  load_all_sounds();
  hof_read();

  ///////////////////////////////////////////////////
  // start_game

  release_title_images(); // added
  free_optsnd_chtab();    // added
  *start_level = 1;

  ///////////////////////////////////////////////////////////////
  // init_game

  *offscreen_surface = make_offscreen_buffer(rect_top);

  *text_time_remaining = 0;
  *text_time_total = 0;
  *is_show_time = 1;
  *checkpoint = 0;
  *upside_down = 0; // N.B. upside_down is also reset in set_start_pos()
  *resurrect_time = 0;
  *rem_min = (*custom)->start_minutes_left; // 60
  *rem_tick = (*custom)->start_ticks_left;  // 719
  *hitp_beg_lev = (*custom)->start_hitp;    // 3
  *current_level = 0;
  startLevel(1);
  *need_level1_music = (*custom)->intro_music_time_initial;


  if (useGUI == true)
  {
   std::string imagePath;

   imagePath = _sdlPopRoot + std::string("/../../images/") + std::string("down.png");
   _downSurface = IMG_Load(imagePath.c_str());
   if (_downSurface == NULL) EXIT_WITH_ERROR("[Error] Could not load image: %s, Reason: %s\n", imagePath.c_str(), SDL_GetError());

   imagePath = _sdlPopRoot + std::string("/../../images/") + std::string("up.png");
   _upSurface = IMG_Load(imagePath.c_str());
   if (_upSurface == NULL) EXIT_WITH_ERROR("[Error] Could not load image: %s, Reason: %s\n", imagePath.c_str(), SDL_GetError());

   imagePath = _sdlPopRoot + std::string("/../../images/") + std::string("left.png");
   _leftSurface = IMG_Load(imagePath.c_str());
   if (_leftSurface == NULL) EXIT_WITH_ERROR("[Error] Could not load image: %s, Reason: %s\n", imagePath.c_str(), SDL_GetError());

   imagePath = _sdlPopRoot + std::string("/../../images/") + std::string("right.png");
   _rightSurface = IMG_Load(imagePath.c_str());
   if (_rightSurface == NULL) EXIT_WITH_ERROR("[Error] Could not load image: %s, Reason: %s\n", imagePath.c_str(), SDL_GetError());

   imagePath = _sdlPopRoot + std::string("/../../images/") + std::string("shift.png");
   _shiftSurface = IMG_Load(imagePath.c_str());
   if (_shiftSurface == NULL) EXIT_WITH_ERROR("[Error] Could not load image: %s, Reason: %s\n", imagePath.c_str(), SDL_GetError());

   imagePath = _sdlPopRoot + std::string("/../../images/") + std::string("down2.png");
   _down2Surface = IMG_Load(imagePath.c_str());
   if (_down2Surface == NULL) EXIT_WITH_ERROR("[Error] Could not load image: %s, Reason: %s\n", imagePath.c_str(), SDL_GetError());

   imagePath = _sdlPopRoot + std::string("/../../images/") + std::string("up2.png");
   _up2Surface = IMG_Load(imagePath.c_str());
   if (_up2Surface == NULL) EXIT_WITH_ERROR("[Error] Could not load image: %s, Reason: %s\n", imagePath.c_str(), SDL_GetError());

   imagePath = _sdlPopRoot + std::string("/../../images/") + std::string("left2.png");
   _left2Surface = IMG_Load(imagePath.c_str());
   if (_left2Surface == NULL) EXIT_WITH_ERROR("[Error] Could not load image: %s, Reason: %s\n", imagePath.c_str(), SDL_GetError());

   imagePath = _sdlPopRoot + std::string("/../../images/") + std::string("right2.png");
   _right2Surface = IMG_Load(imagePath.c_str());
   if (_right2Surface == NULL) EXIT_WITH_ERROR("[Error] Could not load image: %s, Reason: %s\n", imagePath.c_str(), SDL_GetError());

   imagePath = _sdlPopRoot + std::string("/../../images/") + std::string("shift2.png");
   _shift2Surface = IMG_Load(imagePath.c_str());
   if (_shift2Surface == NULL) EXIT_WITH_ERROR("[Error] Could not load image: %s, Reason: %s\n", imagePath.c_str(), SDL_GetError());
  }

}

void SDLPopInstance::startLevel(const word level)
{
 ///////////////////////////////////////////////////////////////
  // play_level
  if (level != *current_level) load_lev_spr(level);

  load_kid_sprite();
  load_level();
  pos_guards();
  clear_coll_rooms();
  clear_saved_ctrl();

  *drawn_room = 0;
  *mobs_count = 0;
  *trobs_count = 0;
  *next_sound = -1;
  *holding_sword = 0;
  *grab_timer = 0;
  *can_guard_see_kid = 0;
  *united_with_shadow = 0;
  *flash_time = 0;
  *leveldoor_open = 0;
  *demo_index = 0;
  *demo_time = 0;
  *guardhp_curr = 0;
  *hitp_delta = 0;
  Guard->charid = charid_2_guard;
  Guard->direction = dir_56_none;
  do_startpos();

  // (level_number != 1)
  *have_sword = (level == 0 || level >= (*custom)->have_sword_from_level);

  find_start_level_door();

  // busy waiting?
  while (check_sound_playing() && !do_paused()) idle();

  stop_sounds();

  restore_room_after_quick_load();
  draw_level_first();

  show_copyprot(0);
  *enable_copyprot = 1;
  reset_timer(timer_1);

  _prevDrawnRoom = *drawn_room;

  set_timer_length(timer_1, 12);
  // Setting exit door status
  isExitDoorOpen = isLevelExitDoorOpen();

  if (*need_level1_music != 0 && *current_level == (*custom)->intro_music_level)
   if ((*fixes)->fix_quicksave_during_lvl1_music)
    *need_level1_music = (*custom)->intro_music_time_restart;
}

void SDLPopInstance::setSeed(const dword randomSeed)
{
  *random_seed = randomSeed;
}

size_t SDLPopInstance::getElapsedMins()
{
 return 60 - *rem_min;
}

size_t SDLPopInstance::getElapsedSecs()
{
 return (720 - *rem_tick) / 12;
}

size_t SDLPopInstance::getElapsedMilisecs()
{
 return ceil( ((double)((720 - *rem_tick) % 12) * (60.0 / 720.0)) * 1000.0 );
}

void SDLPopInstance::draw()
{
  restore_room_after_quick_load();
  draw_game_frame();

  char IGTText[512];
  sprintf(IGTText, "IGT %2lu:%02lu.%03lu", _IGTMins, _IGTSecs, _IGTMillisecs);
  display_text_bottom(IGTText);

  SDL_Surface* downSurface = _downSurface;
  SDL_Surface* upSurface = _upSurface;
  SDL_Surface* leftSurface = _leftSurface;
  SDL_Surface* rightSurface = _rightSurface;
  SDL_Surface* shiftSurface = _shiftSurface;

  if (_move.find("D") != std::string::npos) downSurface = _down2Surface;
  if (_move.find("U") != std::string::npos) upSurface = _up2Surface;
  if (_move.find("L") != std::string::npos) leftSurface = _left2Surface;
  if (_move.find("R") != std::string::npos) rightSurface = _right2Surface;
  if (_move.find("S") != std::string::npos) shiftSurface = _shift2Surface;

  draw_image_transp_vga(downSurface, 280, 170);
  draw_image_transp_vga(upSurface, 280, 150);
  draw_image_transp_vga(leftSurface, 260, 170);
  draw_image_transp_vga(rightSurface, 300, 170);
  draw_image_transp_vga(shiftSurface, 260, 150);

  update_screen();

  if (Kid->sword == sword_2_drawn) set_timer_length(timer_1, 6);
  else set_timer_length(timer_1, 5);
  do_simple_wait(timer_1);

  SDL_RenderClear(*renderer_);
  SDL_RenderCopy(*renderer_, *target_texture, NULL, NULL);
  SDL_RenderPresent(*renderer_);
}

std::string SDLPopInstance::serializeFileCache()
{
 size_t cacheSize = 0;

 // Cache File Counter
 cacheSize += sizeof(size_t);

 // Cache File paths
 cacheSize += *_cachedFileCounter * sizeof(char) * POP_MAX_PATH;

 // Cache File buffer sizes
 cacheSize += *_cachedFileCounter * sizeof(size_t);

 // Size Per file:
 for (size_t i = 0; i < *_cachedFileCounter; i++)
  cacheSize += (*_cachedFileBufferSizes)[i];

 // Allocating cache string
 std::string cache;
 cache.resize(cacheSize);

 // Copying file counter
 size_t curPosition = 0;
 memcpy(&cache[curPosition], _cachedFileCounter, sizeof(size_t));
 curPosition += sizeof(size_t);

 // Copying file paths
 memcpy(&cache[curPosition], _cachedFilePathTable, *_cachedFileCounter * sizeof(char) * POP_MAX_PATH);
 curPosition += *_cachedFileCounter * sizeof(char) * POP_MAX_PATH;

 // Copying buffer sizes
 memcpy(&cache[curPosition], _cachedFileBufferSizes, *_cachedFileCounter * sizeof(size_t));
 curPosition += *_cachedFileCounter * sizeof(size_t);

 // Per file:
 for (size_t i = 0; i < *_cachedFileCounter; i++)
 {
  // Copying buffer sizes
  memcpy(&cache[curPosition], (*_cachedFileBufferTable)[i], (*_cachedFileBufferSizes)[i] * sizeof(char));
  curPosition += (*_cachedFileBufferSizes)[i] * sizeof(char);
 }

 return cache;
}

void SDLPopInstance::deserializeFileCache(const std::string& cache)
{
 // Copying file counter
 size_t curPosition = 0;
 memcpy(_cachedFileCounter, &cache[curPosition], sizeof(size_t));
 curPosition += sizeof(size_t);

 // Copying file paths
 memcpy(_cachedFilePathTable, &cache[curPosition], *_cachedFileCounter * sizeof(char) * POP_MAX_PATH);
 curPosition += *_cachedFileCounter * sizeof(char) * POP_MAX_PATH;

 // Copying buffer sizes
 memcpy(_cachedFileBufferSizes, &cache[curPosition], *_cachedFileCounter * sizeof(size_t));
 curPosition += *_cachedFileCounter * sizeof(size_t);

 // Per file:
 for (size_t i = 0; i < *_cachedFileCounter; i++)
 {
  // Allocating file content buffer
  (*_cachedFileBufferTable)[i] = (char*) malloc((*_cachedFileBufferSizes)[i]);

  // Copying buffer sizes
  memcpy((*_cachedFileBufferTable)[i], &cache[curPosition], (*_cachedFileBufferSizes)[i] * sizeof(char));
  curPosition += (*_cachedFileBufferSizes)[i] * sizeof(char);
 }
}

void SDLPopInstance::performMove(const std::string &move)
{
  (*key_states)[SDL_SCANCODE_UP] = 0;
  (*key_states)[SDL_SCANCODE_DOWN] = 0;
  (*key_states)[SDL_SCANCODE_LEFT] = 0;
  (*key_states)[SDL_SCANCODE_RIGHT] = 0;
  (*key_states)[SDL_SCANCODE_RSHIFT] = 0;

  bool recognizedMove = false;

  if (move.find(".") != std::string::npos) { recognizedMove = true; }
  if (move.find("R") != std::string::npos)
  {
    (*key_states)[SDL_SCANCODE_RIGHT] = 1;
    recognizedMove = true;
  }
  if (move.find("L") != std::string::npos)
  {
    (*key_states)[SDL_SCANCODE_LEFT] = 1;
    recognizedMove = true;
  }
  if (move.find("U") != std::string::npos)
  {
    (*key_states)[SDL_SCANCODE_UP] = 1;
    recognizedMove = true;
  }
  if (move.find("D") != std::string::npos)
  {
    (*key_states)[SDL_SCANCODE_DOWN] = 1;
    recognizedMove = true;
  }
  if (move.find("S") != std::string::npos)
  {
    (*key_states)[SDL_SCANCODE_RSHIFT] = 1;
    recognizedMove = true;
  }
  if (move == "CA") // Ctrl+A
  {
    *is_restart_level = 1;
    recognizedMove = true;
  }

  if (recognizedMove == false)
    EXIT_WITH_ERROR("[Error] Unrecognized move: %s\n", move.c_str());
}

dword SDLPopInstance::advanceRNGState(const dword randomSeed)
{
 return randomSeed * 214013 + 2531011;
}

dword SDLPopInstance::reverseRNGState(const dword randomSeed)
{
 return (randomSeed + 4292436285) * 3115528533;
}


void SDLPopInstance::advanceFrame()
{
  *guardhp_delta = 0;
  *hitp_delta = 0;
  timers();

  play_frame();

  if (*is_restart_level == 1)
   startLevel(*current_level);

  // if we're on lvl 4, check mirror
  if (*current_level == 4)
  {
   if (*jumped_through_mirror == -1) Guard->x = 245;
   check_mirror();
  }

  // If level has changed, then load it
  if (*current_level != *next_level)
  {
   if (*enable_copyprot)
   {
    if (*current_level == (*custom)->copyprot_level-1 && *next_level == (*custom)->copyprot_level)
     *next_level = 15;

    if (*current_level == 15)
     *next_level = (*custom)->copyprot_level;
   }

   startLevel(*next_level);

   // Handle cutscenes
   //if (*next_level == 2) for (size_t i = 0; i < 3; i++) *random_seed = advanceRNGState(*random_seed);
  }

  *is_restart_level = 0;
  _prevDrawnRoom = *drawn_room;
  isExitDoorOpen = isLevelExitDoorOpen();
}

int SDLPopInstance::getKidSequenceId()
{
 int seqIdx = Kid->curr_seq;
 for (int i = 0; i < 113; i++)
  if (seqIdx >= seqOffsets[i] && seqIdx < seqOffsets[i+1]) return i;
 return 153; // Unrecognized
}

int SDLPopInstance::getGuardSequenceId()
{
 int seqIdx = Guard->curr_seq;
 for (int i = 0; i < 113; i++)
  if (seqIdx >= seqOffsets[i] && seqIdx < seqOffsets[i+1])
   return i;
 return 153; // Unrecognized
}


void SDLPopInstance::printFrameInfo()
{
  int kidSeqIdx = getKidSequenceId();
  int guardSeqIdx = getGuardSequenceId();

  printf("[Jaffar]  + Current/Next Level: %2d / %2d\n", *current_level, *next_level);
  printf("[Jaffar]  + Cumulative IGT: %2lu:%02lu.%03lu\n", getElapsedMins(), getElapsedSecs(), getElapsedMilisecs());
  printf("[Jaffar]  + [Kid]   Room: %d, Pos.x: %3d, Pos.y: %3d, Frame: %3d, HP: %d/%d, Sequence: %2d (%s)\n", int(Kid->room), int(Kid->x), int(Kid->y), int(Kid->frame), int(*hitp_curr), int(*hitp_max), kidSeqIdx, seqNames[kidSeqIdx]);
  printf("[Jaffar]  + [Guard] Room: %d, Pos.x: %3d, Pos.y: %3d, Frame: %3d, HP: %d/%d, Sequence: %2d (%s)\n", int(Guard->room), int(Guard->x), int(Guard->y), int(Guard->frame), int(*guardhp_curr), int(*guardhp_max), guardSeqIdx, seqNames[guardSeqIdx]);
  printf("[Jaffar]  + Exit Room Timer: %d\n", *exit_room_timer);
  printf("[Jaffar]  + Exit Door Open: %s\n", isLevelExitDoorOpen() ? "Yes" : "No");
  printf("[Jaffar]  + Reached Checkpoint: %s\n", *checkpoint ? "Yes" : "No");
  printf("[Jaffar]  + Feather Fall: %d\n", *is_feather_fall);
  printf("[Jaffar]  + RNG State: 0x%08X (Last Loose Tile Sound Id: %d)\n", *random_seed, *last_loose_sound);

  // Level-Specific Settings
  if (*current_level == 9) printf("[Jaffar]  + Rightmost Door: %d\n", level->bg[349]);
}

bool SDLPopInstance::isLevelExitDoorOpen()
{
  bool door_open = *leveldoor_open;

  if (!door_open)
  {
    for (int i = 0; i < *trobs_count; ++i)
    {
      const auto &trob = (*trobs)[i];
      const auto idx = (trob.room - 1) * 30 + trob.tilepos;
      const auto type = level->fg[idx] & 0x1f;
      if (type == tiles_16_level_door_left)
      {
        door_open = true;
        break;
      }
    }
  }

  return door_open;
}

SDLPopInstance::SDLPopInstance(const char* libraryFile, const bool multipleLibraries)
{
  if (multipleLibraries)
   _dllHandle = dlmopen (LM_ID_NEWLM, libraryFile, RTLD_NOW | RTLD_LOCAL | RTLD_NODELETE);
  else
   _dllHandle = dlopen (libraryFile, RTLD_NOW);

  if (!_dllHandle)
    EXIT_WITH_ERROR("Could not load %s. Check that this library's path is included in the LD_LIBRARY_PATH environment variable. Try also reducing the number of openMP threads.\n", libraryFile);

  // Functions
  restore_room_after_quick_load = (restore_room_after_quick_load_t)dlsym(_dllHandle, "restore_room_after_quick_load");
  load_global_options = (load_global_options_t)dlsym(_dllHandle, "load_global_options");
  check_mod_param = (check_mod_param_t)dlsym(_dllHandle, "check_mod_param");
  load_ingame_settings = (load_ingame_settings_t)dlsym(_dllHandle, "load_ingame_settings");
  turn_sound_on_off = (turn_sound_on_off_t)dlsym(_dllHandle, "turn_sound_on_off");
  load_mod_options = (load_mod_options_t)dlsym(_dllHandle, "load_mod_options");
  apply_seqtbl_patches = (apply_seqtbl_patches_t)dlsym(_dllHandle, "apply_seqtbl_patches");
  open_dat = (open_dat_t)dlsym(_dllHandle, "open_dat");
  parse_grmode = (parse_grmode_t)dlsym(_dllHandle, "parse_grmode");
  init_timer = (init_timer_t)dlsym(_dllHandle, "init_timer");
  parse_cmdline_sound = (parse_cmdline_sound_t)dlsym(_dllHandle, "parse_cmdline_sound");
  set_hc_pal = (set_hc_pal_t)dlsym(_dllHandle, "set_hc_pal");
  rect_sthg = (rect_sthg_t)dlsym(_dllHandle, "rect_sthg");
  show_loading = (show_loading_t)dlsym(_dllHandle, "show_loading");
  set_joy_mode = (set_joy_mode_t)dlsym(_dllHandle, "set_joy_mode");
  init_copyprot_dialog = (init_copyprot_dialog_t)dlsym(_dllHandle, "init_copyprot_dialog");
  init_record_replay = (init_record_replay_t)dlsym(_dllHandle, "init_record_replay");
  init_menu = (init_menu_t)dlsym(_dllHandle, "init_menu");
  prandom = (prandom_t)dlsym(_dllHandle, "prandom");
  load_from_opendats_alloc = (load_from_opendats_alloc_t)dlsym(_dllHandle, "load_from_opendats_alloc");
  set_pal_256 = (set_pal_256_t)dlsym(_dllHandle, "set_pal_256");
  set_pal = (set_pal_t)dlsym(_dllHandle, "set_pal");
  load_sprites_from_file = (load_sprites_from_file_t)dlsym(_dllHandle, "load_sprites_from_file");
  close_dat = (close_dat_t)dlsym(_dllHandle, "close_dat");
  init_lighting = (init_lighting_t)dlsym(_dllHandle, "init_lighting");
  load_all_sounds = (load_all_sounds_t)dlsym(_dllHandle, "load_all_sounds");
  release_title_images = (release_title_images_t)dlsym(_dllHandle, "release_title_images");
  free_optsnd_chtab = (free_optsnd_chtab_t)dlsym(_dllHandle, "free_optsnd_chtab");
  make_offscreen_buffer = (make_offscreen_buffer_t)dlsym(_dllHandle, "make_offscreen_buffer");
  load_lev_spr = (load_lev_spr_t)dlsym(_dllHandle, "load_lev_spr");
  load_level = (load_level_t)dlsym(_dllHandle, "load_level");
  pos_guards = (pos_guards_t)dlsym(_dllHandle, "pos_guards");
  clear_coll_rooms = (clear_coll_rooms_t)dlsym(_dllHandle, "clear_coll_rooms");
  clear_saved_ctrl = (clear_saved_ctrl_t)dlsym(_dllHandle, "clear_saved_ctrl");
  do_startpos = (do_startpos_t)dlsym(_dllHandle, "do_startpos");
  find_start_level_door = (find_start_level_door_t)dlsym(_dllHandle, "find_start_level_door");
  check_sound_playing = (check_sound_playing_t)dlsym(_dllHandle, "check_sound_playing");
  do_paused = (check_sound_playing_t)dlsym(_dllHandle, "do_paused");
  load_kid_sprite = (load_kid_sprite_t)dlsym(_dllHandle, "load_kid_sprite");
  idle = (idle_t)dlsym(_dllHandle, "idle");
  hof_read = (hof_read_t)dlsym(_dllHandle, "hof_read");
  stop_sounds = (stop_sounds_t)dlsym(_dllHandle, "stop_sounds");
  show_copyprot = (show_copyprot_t)dlsym(_dllHandle, "show_copyprot");
  reset_timer = (reset_timer_t)dlsym(_dllHandle, "reset_timer");
  free_peels = (free_peels_t)dlsym(_dllHandle, "free_peels");
  play_level_2 = (play_level_2_t)dlsym(_dllHandle, "play_level_2");
  timers = (timers_t)dlsym(_dllHandle, "timers");
  play_frame = (play_frame_t)dlsym(_dllHandle, "play_frame");
  draw_game_frame = (draw_game_frame_t)dlsym(_dllHandle, "draw_game_frame");
  update_screen = (update_screen_t)dlsym(_dllHandle, "update_screen");
  do_simple_wait = (do_simple_wait_t)dlsym(_dllHandle, "do_simple_wait");
  reset_level_unused_fields = (reset_level_unused_fields_t)dlsym(_dllHandle, "reset_level_unused_fields");
  load_room_links = (load_room_links_t)dlsym(_dllHandle, "load_room_links");
  set_timer_length = (set_timer_length_t)dlsym(_dllHandle, "set_timer_length");
  draw_level_first = (draw_level_first_t)dlsym(_dllHandle, "draw_level_first");
  play_level = (play_level_t)dlsym(_dllHandle, "play_level");
  save_recorded_replay = (save_recorded_replay_t)dlsym(_dllHandle, "save_recorded_replay");
  start_recording = (start_recording_t)dlsym(_dllHandle, "start_recording");
  add_replay_move = (add_replay_move_t)dlsym(_dllHandle, "add_replay_move");
  process_trobs = (process_trobs_t)dlsym(_dllHandle, "process_trobs");
  do_mobs = (do_mobs_t)dlsym(_dllHandle, "do_mobs");
  check_skel = (check_skel_t)dlsym(_dllHandle, "check_skel");
  check_can_guard_see_kid = (check_can_guard_see_kid_t)dlsym(_dllHandle, "check_can_guard_see_kid");
  open_dat = (open_dat_t)dlsym(_dllHandle, "open_dat");
  check_mirror = (check_mirror_t)dlsym(_dllHandle, "check_mirror");
  init_copyprot = (init_copyprot_t)dlsym(_dllHandle, "init_copyprot");
  alter_mods_allrm = (alter_mods_allrm_t) dlsym(_dllHandle, "alter_mods_allrm");
  start_replay = (start_replay_t) dlsym(_dllHandle, "start_replay");
  start_game = (start_game_t) dlsym(_dllHandle, "start_game");
  display_text_bottom = (display_text_bottom_t) dlsym(_dllHandle, "display_text_bottom");
  redraw_screen = (redraw_screen_t) dlsym(_dllHandle, "redraw_screen");
  draw_image_transp_vga = (draw_image_transp_vga_t) dlsym(_dllHandle, "draw_image_transp_vga");

  // State variables
  Kid = (char_type *)dlsym(_dllHandle, "Kid");
  Guard = (char_type *)dlsym(_dllHandle, "Guard");
  Char = (char_type *)dlsym(_dllHandle, "Char");
  Opp = (char_type *)dlsym(_dllHandle, "Opp");
  trobs_count = (short *)dlsym(_dllHandle, "trobs_count");
  trobs = (trobs_t *)dlsym(_dllHandle, "trobs");
  mobs_count = (short *)dlsym(_dllHandle, "mobs_count");
  mobs = (mobs_t *)dlsym(_dllHandle, "mobs");
  level = (level_type *)dlsym(_dllHandle, "level");
  drawn_room = (word *)dlsym(_dllHandle, "drawn_room");
  leveldoor_open = (word *)dlsym(_dllHandle, "leveldoor_open");
  hitp_curr = (word *)dlsym(_dllHandle, "hitp_curr");
  guardhp_curr = (word *)dlsym(_dllHandle, "guardhp_curr");
  current_level = (word *)dlsym(_dllHandle, "current_level");
  next_level = (word *)dlsym(_dllHandle, "next_level");
  checkpoint = (word *)dlsym(_dllHandle, "checkpoint");
  upside_down = (word *)dlsym(_dllHandle, "upside_down");
  exit_room_timer = (word *)dlsym(_dllHandle, "exit_room_timer");
  hitp_max = (word *)dlsym(_dllHandle, "hitp_max");
  hitp_beg_lev = (word *)dlsym(_dllHandle, "hitp_beg_lev");
  grab_timer = (word *)dlsym(_dllHandle, "grab_timer");
  holding_sword = (word *)dlsym(_dllHandle, "holding_sword");
  united_with_shadow = (short *)dlsym(_dllHandle, "united_with_shadow");
  pickup_obj_type = (short *)dlsym(_dllHandle, "pickup_obj_type");
  kid_sword_strike = (word *)dlsym(_dllHandle, "kid_sword_strike");
  offguard = (word *)dlsym(_dllHandle, "offguard");
  have_sword = (word *)dlsym(_dllHandle, "have_sword");
  guardhp_max = (word *)dlsym(_dllHandle, "guardhp_max");
  demo_index = (word *)dlsym(_dllHandle, "demo_index");
  demo_time = (short *)dlsym(_dllHandle, "demo_time");
  curr_guard_color = (word *)dlsym(_dllHandle, "curr_guard_color");
  guard_notice_timer = (short *)dlsym(_dllHandle, "guard_notice_timer");
  guard_skill = (word *)dlsym(_dllHandle, "guard_skill");
  shadow_initialized = (word *)dlsym(_dllHandle, "shadow_initialized");
  guard_refrac = (word *)dlsym(_dllHandle, "guard_refrac");
  justblocked = (word *)dlsym(_dllHandle, "justblocked");
  droppedout = (word *)dlsym(_dllHandle, "droppedout");
  curr_row_coll_room = (curr_row_coll_room_t *)dlsym(_dllHandle, "curr_row_coll_room");
  curr_row_coll_flags = (curr_row_coll_flags_t *)dlsym(_dllHandle, "curr_row_coll_flags");
  above_row_coll_room = (above_row_coll_room_t *)dlsym(_dllHandle, "above_row_coll_room");
  above_row_coll_flags = (above_row_coll_flags_t *)dlsym(_dllHandle, "above_row_coll_flags");
  below_row_coll_room = (below_row_coll_room_t *)dlsym(_dllHandle, "below_row_coll_room");
  below_row_coll_flags = (below_row_coll_flags_t *)dlsym(_dllHandle, "below_row_coll_flags");
  prev_collision_row = (sbyte *)dlsym(_dllHandle, "prev_collision_row");
  flash_color = (word *)dlsym(_dllHandle, "flash_color");
  flash_time = (word *)dlsym(_dllHandle, "flash_time");
  need_level1_music = (word *)dlsym(_dllHandle, "need_level1_music");
  is_screaming = (word *)dlsym(_dllHandle, "is_screaming");
  is_feather_fall = (word *)dlsym(_dllHandle, "is_feather_fall");
  last_loose_sound = (word *)dlsym(_dllHandle, "last_loose_sound");
  random_seed = (dword *)dlsym(_dllHandle, "random_seed");
  preserved_seed = (dword *)dlsym(_dllHandle, "preserved_seed");
  rem_min = (short *)dlsym(_dllHandle, "rem_min");
  rem_tick = (word *)dlsym(_dllHandle, "rem_tick");
  control_x = (sbyte *)dlsym(_dllHandle, "control_x");
  control_y = (sbyte *)dlsym(_dllHandle, "control_y");
  control_shift = (sbyte *)dlsym(_dllHandle, "control_shift");
  control_forward = (sbyte *)dlsym(_dllHandle, "control_forward");
  control_backward = (sbyte *)dlsym(_dllHandle, "control_backward");
  control_up = (sbyte *)dlsym(_dllHandle, "control_up");
  control_down = (sbyte *)dlsym(_dllHandle, "control_down");
  control_shift2 = (sbyte *)dlsym(_dllHandle, "control_shift2");
  ctrl1_forward = (sbyte *)dlsym(_dllHandle, "ctrl1_forward");
  ctrl1_backward = (sbyte *)dlsym(_dllHandle, "ctrl1_backward");
  ctrl1_up = (sbyte *)dlsym(_dllHandle, "ctrl1_up");
  ctrl1_down = (sbyte *)dlsym(_dllHandle, "ctrl1_down");
  ctrl1_shift2 = (sbyte *)dlsym(_dllHandle, "ctrl1_shift2");
  curr_tick = (dword *)dlsym(_dllHandle, "curr_tick");
  dathandle = (dat_type **)dlsym(_dllHandle, "dathandle");
  level_var_palettes = (byte **)dlsym(_dllHandle, "level_var_palettes");
  is_blind_mode = (word *)dlsym(_dllHandle, "is_blind_mode");
  seed_was_init = (word *)dlsym(_dllHandle, "seed_was_init");
  dathandle = (dat_type **)dlsym(_dllHandle, "dathandle");
  need_drects = (word *)dlsym(_dllHandle, "need_drects");
  g_argc = (int *)dlsym(_dllHandle, "g_argc");
  g_argv = (char ***)dlsym(_dllHandle, "g_argv");
  current_target_surface = (surface_type **)dlsym(_dllHandle, "current_target_surface");
  onscreen_surface_ = (SDL_Surface **)dlsym(_dllHandle, "onscreen_surface_");
  screen_rect = (rect_type *)dlsym(_dllHandle, "screen_rect");
  cheats_enabled = (word *)dlsym(_dllHandle, "cheats_enabled");
  draw_mode = (word *)dlsym(_dllHandle, "draw_mode");
  demo_mode = (word *)dlsym(_dllHandle, "demo_mode");
  play_demo_level = (int *)dlsym(_dllHandle, "play_demo_level");
  doorlink1_ad = (byte **)dlsym(_dllHandle, "doorlink1_ad");
  doorlink2_ad = (byte **)dlsym(_dllHandle, "doorlink2_ad");
  guard_palettes = (byte **)dlsym(_dllHandle, "guard_palettes");
  chtab_addrs = (chtab_addrs_t *)dlsym(_dllHandle, "chtab_addrs");
  start_level = (short *)dlsym(_dllHandle, "start_level");
  offscreen_surface = (surface_type **)dlsym(_dllHandle, "offscreen_surface");
  rect_top = (rect_type *)dlsym(_dllHandle, "rect_top");
  text_time_remaining = (word *)dlsym(_dllHandle, "text_time_remaining");
  text_time_total = (word *)dlsym(_dllHandle, "text_time_total");
  is_show_time = (word *)dlsym(_dllHandle, "is_show_time");
  resurrect_time = (word *)dlsym(_dllHandle, "resurrect_time");
  custom = (custom_options_type **)dlsym(_dllHandle, "custom");
  next_sound = (short *)dlsym(_dllHandle, "next_sound");
  can_guard_see_kid = (short *)dlsym(_dllHandle, "can_guard_see_kid");
  hitp_delta = (short *)dlsym(_dllHandle, "hitp_delta");
  guardhp_delta = (short *)dlsym(_dllHandle, "guardhp_delta");
  different_room = (word *)dlsym(_dllHandle, "different_room");
  next_room = (word *)dlsym(_dllHandle, "next_room");
  is_guard_notice = (word *)dlsym(_dllHandle, "is_guard_notice");
  need_full_redraw = (word *)dlsym(_dllHandle, "need_full_redraw");
  is_validate_mode = (byte *)dlsym(_dllHandle, "is_validate_mode");
  exe_dir = (exe_dir_t *)dlsym(_dllHandle, "exe_dir");
  found_exe_dir = (bool *)dlsym(_dllHandle, "found_exe_dir");
  key_states = (key_states_t *)dlsym(_dllHandle, "key_states");
  is_cutscene = (word *)dlsym(_dllHandle, "is_cutscene");
  enable_quicksave_penalty = (byte *)dlsym(_dllHandle, "enable_quicksave_penalty");
  is_restart_level = (word *)dlsym(_dllHandle, "is_restart_level");
  last_loose_sound = (word *)dlsym(_dllHandle, "last_loose_sound");
  window_ = (SDL_Window **)dlsym(_dllHandle, "window_");
  enable_copyprot = (byte *)dlsym(_dllHandle, "enable_copyprot");
  _cachedFilePointerTable = (cachedFilePointerTable_t*)dlsym(_dllHandle, "_cachedFilePointerTable");
  _cachedFileBufferTable = (cachedFileBufferTable_t*)dlsym(_dllHandle, "_cachedFileBufferTable");
  _cachedFileBufferSizes = (cachedFileBufferSizes_t*)dlsym(_dllHandle, "_cachedFileBufferSizes");
  _cachedFilePathTable = (cachedFilePathTable_t*)dlsym(_dllHandle, "_cachedFilePathTable");
  _cachedFileCounter = (cachedFileCounter_t*)dlsym(_dllHandle, "_cachedFileCounter");
  fixes = (fixes_options_type**)dlsym(_dllHandle, "fixes");
  copyprot_plac = (word *) dlsym(_dllHandle, "copyprot_plac");
  renderer_ = (SDL_Renderer**) dlsym(_dllHandle, "renderer_");
  target_texture = (SDL_Texture**) dlsym(_dllHandle, "target_texture");
  jumped_through_mirror = (short*) dlsym(_dllHandle, "jumped_through_mirror");
  can_guard_see_kid = (short*) dlsym(_dllHandle, "can_guard_see_kid");
  levels_file = (levels_file_t*) dlsym(_dllHandle, "levels_file");
}

SDLPopInstance::~SDLPopInstance()
{
  dlclose(_dllHandle);
}
