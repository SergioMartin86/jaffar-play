#include "state.h"
#include "common.h"
#include "utils.h"

size_t _currentStep;
char quick_control[] = "........";
float replay_curr_tick = 0.0;

template <class T>
void AddItem(std::vector<State::Item> *dest, T &val, State::ItemType type)
{
  dest->push_back({&val, sizeof(val), type});
}

std::vector<State::Item> GenerateItemsMap(SDLPopInstance *sdlPop)
{
  std::vector<State::Item> dest;
  AddItem(&dest, quick_control, State::PER_FRAME_STATE);
  AddItem(&dest, *sdlPop->level, State::HASHABLE_MANUAL);
  AddItem(&dest, *sdlPop->checkpoint, State::PER_FRAME_STATE);
  AddItem(&dest, *sdlPop->upside_down, State::PER_FRAME_STATE);
  AddItem(&dest, *sdlPop->drawn_room, State::HASHABLE);
  AddItem(&dest, *sdlPop->current_level, State::PER_FRAME_STATE);
  AddItem(&dest, *sdlPop->next_level, State::PER_FRAME_STATE);
  AddItem(&dest, *sdlPop->mobs_count, State::HASHABLE_MANUAL);
  AddItem(&dest, *sdlPop->mobs, State::HASHABLE_MANUAL);
  AddItem(&dest, *sdlPop->trobs_count, State::HASHABLE_MANUAL);
  AddItem(&dest, *sdlPop->trobs, State::HASHABLE_MANUAL);
  AddItem(&dest, *sdlPop->leveldoor_open, State::HASHABLE);
  AddItem(&dest, *sdlPop->Kid, State::HASHABLE);
  AddItem(&dest, *sdlPop->hitp_curr, State::PER_FRAME_STATE);
  AddItem(&dest, *sdlPop->hitp_max, State::PER_FRAME_STATE);
  AddItem(&dest, *sdlPop->hitp_beg_lev, State::PER_FRAME_STATE);
  AddItem(&dest, *sdlPop->grab_timer, State::HASHABLE);
  AddItem(&dest, *sdlPop->holding_sword, State::HASHABLE);
  AddItem(&dest, *sdlPop->united_with_shadow, State::HASHABLE);
  AddItem(&dest, *sdlPop->have_sword, State::HASHABLE);
  /*AddItem(&dest, *sdlPop->ctrl1_forward, State::HASHABLE);
  AddItem(&dest, *sdlPop->ctrl1_backward, State::HASHABLE);
  AddItem(&dest, *sdlPop->ctrl1_up, State::HASHABLE);
  AddItem(&dest, *sdlPop->ctrl1_down, State::HASHABLE);
  AddItem(&dest, *sdlPop->ctrl1_shift2, State::HASHABLE);*/
  AddItem(&dest, *sdlPop->kid_sword_strike, State::HASHABLE);
  AddItem(&dest, *sdlPop->pickup_obj_type, State::HASHABLE);
  AddItem(&dest, *sdlPop->offguard, State::HASHABLE);
  // guard
  AddItem(&dest, *sdlPop->Guard, State::PER_FRAME_STATE);
  AddItem(&dest, *sdlPop->Char, State::PER_FRAME_STATE);
  AddItem(&dest, *sdlPop->Opp, State::PER_FRAME_STATE);
  AddItem(&dest, *sdlPop->guardhp_curr, State::PER_FRAME_STATE);
  AddItem(&dest, *sdlPop->guardhp_max, State::PER_FRAME_STATE);
  AddItem(&dest, *sdlPop->demo_index, State::PER_FRAME_STATE);
  AddItem(&dest, *sdlPop->demo_time, State::PER_FRAME_STATE);
  AddItem(&dest, *sdlPop->curr_guard_color, State::PER_FRAME_STATE);
  AddItem(&dest, *sdlPop->guard_notice_timer, State::HASHABLE);
  AddItem(&dest, *sdlPop->guard_skill, State::PER_FRAME_STATE);
  AddItem(&dest, *sdlPop->shadow_initialized, State::PER_FRAME_STATE);
  AddItem(&dest, *sdlPop->guard_refrac, State::HASHABLE);
  AddItem(&dest, *sdlPop->justblocked, State::HASHABLE);
  AddItem(&dest, *sdlPop->droppedout, State::HASHABLE);
  // collision
  AddItem(&dest, *sdlPop->curr_row_coll_room, State::PER_FRAME_STATE);
  AddItem(&dest, *sdlPop->curr_row_coll_flags, State::PER_FRAME_STATE);
  AddItem(&dest, *sdlPop->below_row_coll_room, State::PER_FRAME_STATE);
  AddItem(&dest, *sdlPop->below_row_coll_flags, State::PER_FRAME_STATE);
  AddItem(&dest, *sdlPop->above_row_coll_room, State::PER_FRAME_STATE);
  AddItem(&dest, *sdlPop->above_row_coll_flags, State::PER_FRAME_STATE);
  AddItem(&dest, *sdlPop->prev_collision_row, State::PER_FRAME_STATE);
  // flash
  AddItem(&dest, *sdlPop->flash_color, State::PER_FRAME_STATE);
  AddItem(&dest, *sdlPop->flash_time, State::PER_FRAME_STATE);
  // sounds
  AddItem(&dest, *sdlPop->need_level1_music, State::HASHABLE);
  AddItem(&dest, *sdlPop->is_screaming, State::HASHABLE);
  AddItem(&dest, *sdlPop->is_feather_fall, State::HASHABLE);
  AddItem(&dest, *sdlPop->last_loose_sound, State::HASHABLE);
  // AddItem(&dest, *sdlPop->next_sound, State::HASHABLE);
  // AddItem(&dest, *sdlPop->current_sound, State::HASHABLE);
  // random
  AddItem(&dest, *sdlPop->random_seed, State::PER_FRAME_STATE);
  // remaining time
  AddItem(&dest, *sdlPop->rem_min, State::PER_FRAME_STATE);
  AddItem(&dest, *sdlPop->rem_tick, State::PER_FRAME_STATE);
  // saved controls
  AddItem(&dest, *sdlPop->control_x, State::PER_FRAME_STATE);
  AddItem(&dest, *sdlPop->control_y, State::PER_FRAME_STATE);
  AddItem(&dest, *sdlPop->control_shift, State::PER_FRAME_STATE);
  AddItem(&dest, *sdlPop->control_forward, State::PER_FRAME_STATE);
  AddItem(&dest, *sdlPop->control_backward, State::PER_FRAME_STATE);
  AddItem(&dest, *sdlPop->control_up, State::PER_FRAME_STATE);
  AddItem(&dest, *sdlPop->control_down, State::PER_FRAME_STATE);
  AddItem(&dest, *sdlPop->control_shift2, State::PER_FRAME_STATE);
  AddItem(&dest, *sdlPop->ctrl1_forward, State::PER_FRAME_STATE);
  AddItem(&dest, *sdlPop->ctrl1_backward, State::PER_FRAME_STATE);
  AddItem(&dest, *sdlPop->ctrl1_up, State::PER_FRAME_STATE);
  AddItem(&dest, *sdlPop->ctrl1_down, State::PER_FRAME_STATE);
  AddItem(&dest, *sdlPop->ctrl1_shift2, State::PER_FRAME_STATE);
  // Support for overflow glitch
  AddItem(&dest, *sdlPop->exit_room_timer, State::PER_FRAME_STATE);
  // replay recording state
  AddItem(&dest, replay_curr_tick, State::PER_FRAME_STATE);
  AddItem(&dest, *sdlPop->is_guard_notice, State::PER_FRAME_STATE);
  AddItem(&dest, *sdlPop->can_guard_see_kid, State::PER_FRAME_STATE);
  return dest;
}

State::State(SDLPopInstance *sdlPop, const std::string& saveString)
{
  _sdlPop = sdlPop;
  _items = GenerateItemsMap(sdlPop);

  // Update the SDLPop instance with the savefile contents
  loadState(saveString);
  _sdlPop->startLevel(*_sdlPop->next_level);

  // Backing up current RNG state
  auto rngState = *_sdlPop->random_seed;
  auto looseTileSound = *_sdlPop->last_loose_sound;

  // Processing screen objects that might affect RNG state
  _sdlPop->play_frame();

  // Update the SDLPop instance with the savefile contents again
  loadState(saveString);

  // Recover original RNG state
  _sdlPop->setSeed(rngState);
  *_sdlPop->last_loose_sound = looseTileSound;
}

void State::loadState(const std::string &data)
{
  if (data.size() != _FRAME_DATA_SIZE)
    EXIT_WITH_ERROR("[Error] Wrong state size. Expected %lu, got: %lu\n", _FRAME_DATA_SIZE, data.size());

  size_t curPos = 0;
  for (const auto &item : _items)
  {
    memcpy(item.ptr, &data.c_str()[curPos], item.size);
    curPos += item.size;
  }

  _sdlPop->isExitDoorOpen = _sdlPop->isLevelExitDoorOpen();
  *_sdlPop->different_room = 1;
  // Show the room where the prince is, even if the player moved the view away
  // from it (with the H,J,U,N keys).
  *_sdlPop->next_room = *_sdlPop->drawn_room = _sdlPop->Kid->room;
  _sdlPop->load_room_links();
}

std::string State::saveState() const
{
  std::string res;
  res.reserve(_FRAME_DATA_SIZE);
  for (const auto &item : _items)
  {
    res.append(reinterpret_cast<const char *>(item.ptr), item.size);
  }

  if (res.size() != _FRAME_DATA_SIZE)
    EXIT_WITH_ERROR("[Error] Wrong state size. Expected %lu, got: %lu\n", _FRAME_DATA_SIZE, res.size());

  return res;
}
