#include "props_struct.h"

Props *init_props(void);
void free_props(Props *props);

int activate_button(short y, short x, Props *props);
void request_sc_change(Props *props, short newSC);

