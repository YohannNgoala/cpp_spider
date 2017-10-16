#ifndef PROTOCOL_HPP_
#define PROTOCOL_HPP_

namespace Spider {
enum RequestCode  {
  /* Server requests */
  PING = 0,
  ASK_ID,
  KEYBOARD_TRACK_ACTIVATE,
  KEYBOARD_TRACK_DEACTIVATE,
  MOUSE_MVMT_TRACK_ACTIVATE,
  MOUSE_MVMT_TRACK_DEACTIVATE,
  MOUSE_CLICK_TRACK_ACTIVATE,
  MOUSE_CLICK_TRACK_DEACTIVATE,
  DISCONNECT,
  PAUSE_ACTIVATE,
  PAUSE_DEACTIVATE,
  SET_FREQUENCY,
  GET_STATUS,

  /* Success response */
  VALID_KEY_EVENT = 200, /* Followed by a t_keyboard_event structure */
  VALID_MOUSE_EVENT,     /* Followed by a t_mouse_event structure */
  VALID_PING_RESPONSE,
  VALID_ASK_ID_RESPONSE, /* Followed by a t_client_id structure */
  VALID_KEYBOARD_TRACK_ACTIVATE,
  VALID_KEYBOARD_TRACK_DEACTIVATE,
  VALID_MOUSE_MVMT_TRACK_ACTIVATE,
  VALID_MOUSE_MVMT_TRACK_DEACTIVATE,
  VALID_MOUSE_CLICK_TRACK_ACTIVATE,
  VALID_MOUSE_CLICK_TRACK_DEACTIVATE,
  VALID_DISCONNECT,
  VALID_PAUSE_ACTIVATE,
  VALID_PAUSE_DEACTIVATE,
  VALID_SET_FREQUENCY,
  VALID_GET_STATUS_RESPONSE, /* Followed by a t_status structure */

  /* 300-332 codes reserved for Vidal's group*/
  /* 333-375 codes reserved for Ben's group */
  /* 376-399 codes reserved for Riadh's group */

  /* 400 - ... Erreurs */
  ERROR_PING = 400,
  ERROR_ASK_ID,
  ERROR_KEYBOARD_TRACK_ACTIVATE,
  ERROR_KEYBOARD_TRACK_DEACTIVATE,
  ERROR_MOUSE_MVMT_TRACK_ACTIVATE,
  ERROR_MOUSE_MVMT_TRACK_DEACTIVATE,
  ERROR_MOUSE_CLICK_TRACK_ACTIVATE,
  ERROR_MOUSE_CLICK_TRACK_DEACTIVATE,
  ERROR_DISCONNECT,
  ERROR_PAUSE_ACTIVATE,
  ERROR_PAUSE_DEACTIVATE,
  ERROR_SET_FREQUENCY,
  ERROR_GET_STATUS
};

enum ClickType { NONE = 0, LEFT, RIGHT, OTHER };

struct t_client_id {
  char host[256];
  char user[256];
};

struct t_mouse_event {
  int timestamp;
  char procname[256];
  int pos_x;
  int pos_y;
  ClickType click_type;
};

struct t_keyboard_event {
  int timestamp;
  char procname[256];
  int key;
};

struct t_status {
  bool keyboard_tracking;
  bool mouse_click_tracking;
  bool mouse_move_tracking;
  bool paused;
};
} // namespace Spider

#endif /* !PROTOCOL_HPP_ */
