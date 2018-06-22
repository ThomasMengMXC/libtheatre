#include "props.h"

Props *init_props(void) {
	Props *props = malloc(sizeof(Props));
	props->data = NULL;
	props->screen = NULL;
	props->backstage = NULL;
	return props;
}

void free_props(Props *props) {
	free(props);
}

int activate_button(Props *props, short y, short x) {
	Button button = get_button(props->screen, y, x);
	if (button == NULL) return 0;
	button(props, y, x);
	return 0;
}

int activate_hover(Props *props, char start, short y, short x) {
	Hover hover = get_hover(props->screen, y, x);
	if (hover == NULL) return 0;
	hover(props, start, y, x);
	return 0;
}
