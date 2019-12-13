#ifndef GRAPHICS_H
#define GRAPHICS_H


/**
 * Draws the player. This depends on the player state, so it is not a DrawFunc.
 */
void draw_player(int u, int v, int key);

/**
 * Takes a string image and draws it to the screen. The string is 121 characters
 * long, and represents an 11x11 tile in row-major ordering (across, then down,
 * like a regular multi-dimensional array). The available colors are:
 *      R = Red
 *      Y = Yellow
 *      G = Green
 *      D = Brown ("dirt")
 *      5 = Light grey (50%)
 *      3 = Dark grey (30%)
 *      Any other character is black
 * More colors can be easily added by following the pattern already given.
 */
void draw_img(int u, int v, const char* img);

/**
 * DrawFunc functions. 
 * These can be used as the MapItem draw functions.
 */
void draw_nothing(int u, int v);
void draw_wall(int u, int v);
void draw_plant(int u, int v);
void draw_NPC(int u, int v);
void draw_NPC_2(int u, int v);
void draw_door(int u, int v);
void draw_tree(int u, int v);
void draw_sonar(int u, int v);
void cover_sonar(int u, int v);
void draw_treasure(int u, int v);
void cover_treasure(int u, int v);
void draw_end();
void draw_lose();
void draw_change_treasure();
void draw_lava(int u, int v);
void draw_ice(int u, int v);
void draw_ice_2(int u, int v);
void draw_swamp(int u, int v);
void draw_piece_1(int u, int v);
void draw_piece_2(int u, int v);
void draw_piece_3(int u, int v);
void draw_NPC_3(int u, int v);

/**
 * Draw the upper status bar.
 */
void draw_upper_status(int player_x, int player_y, int player_has_key, int player_health);

/**
 * Draw the lower status bar.
 */ 
void draw_lower_status(int ran_x, int ran_y, int player_x, int player_y, bool player_has_sonar, int player_health, bool sonar_improved);

/**
 * Draw the border for the map.
 */
void draw_border();

#endif // GRAPHICS_H