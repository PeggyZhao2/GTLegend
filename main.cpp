// Project includes
#include "globals.h"
#include "hardware.h"
#include "map.h"
#include "graphics.h"
#include "speech.h"
//#include "hardware.cpp"
#include<time.h>

static bool maze_enable = false; 
int p_ran_x = 0;
int p_ran_y = 0;
int ran_x = 0;
int ran_y = 0;
int treasure_found_times = 0;
bool has_sonar = false;
int sonar_x = 2;
int sonar_y = 6;
int ice_x = 22;
int ice_y = 22;
int fake_treasure_x = random_generator();
int fake_treasure_y = random_generator();
//int fake_treasure_x_2 = random_generator();
//int fake_treasure_y_2 = random_generator();
//int fake_treasure_x_3 = random_generator();
//int fake_treasure_y_3 = random_generator();
//int fake_treasure_x_4 = random_generator();
//int fake_treasure_y_4 = random_generator();
bool sonar_improved = false;
//bool fake_broken = false;
bool taggle = true;


// Functions in this file
int get_action (GameInputs inputs);
int update_game (int action);
void draw_game (int init);
void init_main_map ();
void init_maze_map ();
int main ();
int Interact(MapItem* item);
void init_end_page(int health);
int get_health();
void set_health(int health);
//void god_mode();
//void clear_god_mode();

/**
 * The main game state. Must include Player locations and previous locations for
 * drawing to work properly. Other items can be added as needed.
 */
struct {
    int x,y;    // Current locations
    int px, py; // Previous locations
    int has_key;
    bool omni_pressed;  //omni_pressed T, god_mode()
    bool menu;
    int health;
    int numQuest;
    // You can add other properties for the player here
} Player;

int get_health(){
    return Player.health;    
}

void set_health(int health) {
    Player.health = health;
}

/**
 * Given the game inputs, determine what kind of update needs to happen.
 * Possbile return values are defined below.
 */
#define NO_ACTION 0
#define ACTION_BUTTON 1
#define MENU_BUTTON 2
#define GO_LEFT 3
#define GO_RIGHT 4
#define GO_UP 5
#define GO_DOWN 6
#define OMNI_BUTTON 7
MapItem* nextBlock;
int get_action(GameInputs inputs)
{
    //return NO_ACTION;
    if (!inputs.b1) { //button1(omni_button)
        return OMNI_BUTTON;
    } 
    if (!inputs.b2) {  //button2 is pressed
        return ACTION_BUTTON;
    } 
    if (!inputs.b3) {
        return MENU_BUTTON;
    } 
    if (inputs.ax < -0.4) {  //goes up
        nextBlock = get_north(Player.x, Player.y);
        if (Player.omni_pressed || nextBlock -> walkable) {
            return GO_UP;
        }
    }
    if (inputs.ax >= 0.4) {     //goes down--south
        //pc.printf("!!!!!!!!!!!!!!!!!!!");
        nextBlock = get_south(Player.x, Player.y);
        if (Player.omni_pressed || nextBlock -> walkable) {
            return GO_DOWN;
        }
    }
    if (inputs.ay < -0.4) {     //goes left--west
        nextBlock = get_west(Player.x, Player.y);
        if (Player.omni_pressed || nextBlock -> walkable) {
            return GO_LEFT;
        }
    }
    if (inputs.ay >= 0.4) {  //goes right,---east
        nextBlock = get_east(Player.x, Player.y);
        if (Player.omni_pressed || nextBlock -> walkable) {
            return GO_RIGHT;
        }   
    }
    
    return NO_ACTION;
 }

/**
 * Update the game state based on the user action. For example, if the user
 * requests GO_UP, then this function should determine if that is possible by
 * consulting the map, and update the Player position accordingly.
 * 
 * Return values are defined below. FULL_DRAW indicates that for this frame,
 * draw_game should not optimize drawing and should draw every tile, even if
 * the player has not moved.
 */
#define NO_RESULT 0
#define GAME_OVER 1
#define WIN 2
#define FULL_DRAW 3
int update_game(int action)
{
    // Save player previous location before updating
    Player.px = Player.x;
    Player.py = Player.y;
    
    // Do different things based on the each action.
    // You can define functions like "go_up()" that get called for each case.
    switch(action)
    {
        case GO_UP:
        {     
            Player.y = Player.y - 1;
            break;}
        case GO_LEFT:{   
            Player.x = Player.x - 1;
            break;    }        
        case GO_DOWN:   
            Player.y = Player.y + 1;
            break;
        case GO_RIGHT: { 
            pc.printf("Right\r\n");
            Player.x = Player.x + 1;
            break;}
        case ACTION_BUTTON:{ 
            //ACTION_BUTTON: talk to npc, grab an item, open the door ...
            //NPC: NPC type = 3
            //Full_draw: 3
            taggle = true;
            int upper_block = Interact(get_north(Player.x, Player.y)); //RETURN Full_draw: 3---if upper_block is NCP
            if (upper_block == FULL_DRAW) {
                return upper_block;
            }
            int lower_block = Interact(get_south(Player.x, Player.y));
            if (lower_block == FULL_DRAW) {
                return lower_block;    
            }
            int left_block = Interact(get_west(Player.x, Player.y));
            if (left_block == FULL_DRAW) {
                return left_block;    
            }
            int right_block = Interact(get_east(Player.x, Player.y));
            if (right_block == FULL_DRAW) {
                return right_block;    
            }
            if (has_sonar) { // sonar found
                //taggle = false;
//                draw_lower_status(ran_x, ran_y, Player.x, Player.y, has_sonar, Player.health, sonar_improved);
                if (sonar_improved) {
                    uLCD.locate(0,3);
                    uLCD.printf("X:%02d Y:%02d", ran_x, ran_y, WHITE);
                    wait(.5); 
                    
                    //char* l1 = ((char*)ran_x);
//                    char* l2 = ((char*)ran_y);
//                    speech(l1, l2);
                }
                else {
                    char* direction1 = "";
                    char* direction2 = "";
    
                    if (Player.x < ran_x) { //current is on the left---go east
                        direction1 = "E";    
                    }
                    if (Player.x > ran_x) {
                        direction1 = "W";
                    }
                    if (Player.y < ran_y) { //current is on top --- go down/south
                        direction2 = "S";
                    }
                    if (Player.y > ran_y) {
                        direction2 = "N";
                    } 
                    speech(direction1, direction2);
                }
                return FULL_DRAW;
                
            }
            
            //when walkable == true
            //int current_block = Interact(get_here(Player.x, Player.y));
//            if (current_block == FULL_DRAW) {
//                return current_block;    
//            }
            
            //door
            }
        case MENU_BUTTON: 
            //if (!Player.menu) {
//                questMenu(Player.numQuest);
//                Player.menu = true;
//            }
            break;
        case OMNI_BUTTON:
            //toggle omni mode 
            Player.omni_pressed = !Player.omni_pressed;
        //case NO_ACTION: break;
        default:        break;
    }
    return NO_RESULT;
}


/////////////*****************PRIVATE METHOD*****************///////////
/////enable omni//////
//void god_mode () {
//    
//}


///////**********PRIVATE MOETHOD***************/////////////////////////
/////////////return: FULL_DRAW, NO_RESULT, GAME_OVER, WIN////////////
/////////////use a switch case(npc, door, fire, ice, ...) to decide game state
int Interact(MapItem* item) {
    switch(item -> type) {
        case NPC:
            //Interact_help(item);
            char* l1;
            char* l2;
            l1 = "Hi, please find";
            l2 = "the treasure.";
            speech(l1, l2);
            l1 = "Go to the other";
            l2 = "NPC, for key.";
            speech(l1, l2);
            l1 = "He is at lower";
            l2 = "right corner, ";
            speech(l1, l2);
            l1 = "wearing white";
            l2 = "^~^";
            speech(l1, l2);
            Player.numQuest = 1;
            //Player.has_key = 1;         //status bar!!!!!!!
            //item -> data++;
            
            add_door(20, 20);
            return FULL_DRAW;
            
        case NPC_2:
            l1 = "Go to the pink ";
            l2 = "door, climb the";
            speech(l1, l2);
            l1 = "laddar. Here is ";
            l2 = "the key.";
            speech(l1, l2);
            
            //Player.numQuest;
            Player.has_key = 1; 
            //item -> data++;
            return FULL_DRAW;
            
        case NPC_3:
            if (has_sonar) {
                sonar_improved = true;
                l1 = "Your sonar is ";
                l2 = "improved!";
                speech(l1, l2);
            } else {
                l1 = "You have no ";
                l2 = "sonar";
                speech(l1, l2);    
            }
            return FULL_DRAW;
            
        case door:
            ///////************************//////////////
            //if on main(0), go to maze, toggle the active_map
            //if on maze, go to main
            if (get_active_num() == 0) {
                if (Player.has_key == 1) { //has key    
                    set_active_map(1); 
                    Player.x = 2;
                    Player.y = 2;
                } else 
                {    //no key
                    l1 = "Go to NPC2";
                    l2 = "for the key.";
                    speech(l1, l2);
                }
            } // end for get_active_num() if
            else {  // on maze
                set_active_map(0);
                Player.x = 21;
                Player.y = 21;
            }
        
            return FULL_DRAW;
        case sonar:
            has_sonar = true;
            map_erase(sonar_x, sonar_y);
            
            //door replace the sonar
            add_door(sonar_x, sonar_y);
            
            set_active_map(0);
            //random
            ran_x = random_generator();
            ran_y = random_generator();
            //draw treasure
            add_treasure(ran_x, ran_y);
            add_fake_treasure(fake_treasure_x, fake_treasure_y);
            //add_fake_treasure(fake_treasure_x_2, fake_treasure_y_2);
//            add_fake_treasure(fake_treasure_x_3, fake_treasure_y_3);
//            add_fake_treasure(fake_treasure_x_4, fake_treasure_y_4);
//            
            return FULL_DRAW;
        case treasure: 
            if (treasure_found_times == 0){
                l1 = "The treasure ";
                l2 = "location is changed.";
                speech(l1, l2);
                
                //draw_change_treasure();
                
                p_ran_x = ran_x;
                p_ran_y = ran_y;
                //treasure_move();
                srand(time(NULL));
                ran_x = rand() % 48 + 1;
                srand(time(NULL));
                ran_y = rand() % 48 + 1;
                
                map_erase(p_ran_x, p_ran_y);
                //draw treasure
                add_treasure(ran_x, ran_y);
                
                treasure_found_times++;
            } else {
                //music
                speaker.period(1.0/110.0);//
                speaker = 0.03;
                wait(0.4);
                
                speaker.period(1.0/220.0);//
                speaker = 0.03;
                wait(0.7);
                
                speaker.period(1.0/160.0);//
                speaker = 0.03;
                wait(0.3);
                
                speaker.period(1.0/330.0);//
                speaker = 0.03;
                wait(1);
                
                speaker.period(1.0/110.0);//
                speaker = 0.03;
                wait(0.4);
                
                speaker.period(1.0/220.0);//
                speaker = 0.03;
                wait(0.7);
                speaker = 0;
                /////////////////////
                uLCD.cls();
                init_end_page(Player.health);
            }
            return FULL_DRAW;
            
            
        case fake_treasure:
            //erase fake treasure
            map_erase(fake_treasure_x, fake_treasure_y);
            
            char* l3 = "Fake Treasure!";
            char* l4 = "";
            speech(l3, l4);
            //add_treasure_pieces
            //add_treasure_pieces (fake_treasure_x, fake_treasure_y);
            //fake_broken = true;
            
            return FULL_DRAW;
                 
        //ase treasure_pieces://????
//            map_erase(Player.x, Player.y); 
//        
//        
//            return FULL_DRAW;
            
        case ice:
            break_ice(ice_x, ice_y, 6);
            //reduce health
            //Player.health = Player.health - 3;
            
            return FULL_DRAW;
    }//switch
    return 0;
    
}




///////////////*********PRIVATE METHOD*************////////////////
/////////////helper method for Interact(): Interact_help()/////////
///////////////return void----input: MapItem///////////////////////
//void Interact_help(MapItem* item) {
//    char* l1;
//    char* l2;
//    switch (item -> data) {
//        case 0:
//            l1 = "Hi, please find";
//            l2 = "the treasure.";
//            speech(l1, l2);
//            l1 = "Go to the maze";
//            l2 = "for sonar first.";
//            speech(l1, l2);
//            //Player.numQuest = 1;
//            Player.has_key = 0;
//            
//            item -> data++;
//            break;
//        case 1:
//        
//            break;
//    }
//}





/**
 * Entry point for frame drawing. This should be called once per iteration of
 * the game loop. This draws all tiles on the screen, followed by the status 
 * bars. Unless init is nonzero, this function will optimize drawing by only 
 * drawing tiles that have changed from the previous frame.
 */
 //no alteration
void draw_game(int init)    //return full_draw: 3
{
    // Draw game border first
    if(init) draw_border();
    
    // Iterate over all visible map tiles
    for (int i = -5; i <= 5; i++) // Iterate over columns of tiles
    {
        for (int j = -4; j <= 4; j++) // Iterate over one column of tiles
        {
            // Here, we have a given (i,j)
            
            // Compute the current map (x,y) of this tile
            int x = i + Player.x;
            int y = j + Player.y;
            
            // Compute the previous map (px, py) of this tile
            int px = i + Player.px;
            int py = j + Player.py;
                        
            // Compute u,v coordinates for drawing
            int u = (i+5)*11 + 3;
            int v = (j+4)*11 + 15;
            
            // Figure out what to draw
            DrawFunc draw = NULL;
            if (i == 0 && j == 0) // Only draw the player on init---------------init
            {
                draw_player(u, v, Player.has_key);
                continue;
            }
            else if (x >= 0 && y >= 0 && x < map_width() && y < map_height()) // Current (i,j) in the map
            {
                MapItem* curr_item = get_here(x, y);
                MapItem* prev_item = get_here(px, py);
                if (init || curr_item != prev_item) // Only draw if they're different
                {
                    if (curr_item) // There's something here! Draw it
                    {
                        draw = curr_item->draw;
                    }
                    else // There used to be something, but now there isn't
                    {
                        draw = draw_nothing;
                    }
                }
            }
            else if (init) // If doing a full draw, but we're out of bounds, draw the walls.
            {
                draw = draw_wall;
            }

            // Actually draw the tile
            if (draw) draw(u, v);
        }
    }

    // Draw status bars    
    draw_upper_status(Player.x, Player.y, Player.has_key, Player.health);
    //draw_lower_status(ran_x, ran_y, Player.x, Player.y, has_sonar, Player.health, sonar_improved);
}


/**
 * Initialize the main world map. Add walls around the edges, interior chambers,
 * and plants in the background so you can see motion. Note: using the similar
 * procedure you can init the secondary map(s).
 */
void init_main_map()
{
    // "Random" plants
    Map* map = set_active_map(0);
    for(int i = map_width() + 3; i < map_area(); i += 39)
    {
        add_plant(i % map_width(), i / map_width());
    }
    pc.printf("plants\r\n");
        
    pc.printf("Adding walls!\r\n");
    add_wall(0,              0,              HORIZONTAL, map_width());
    add_wall(0,              map_height()-1, HORIZONTAL, map_width());
    add_wall(0,              0,              VERTICAL,   map_height());
    add_wall(map_width()-1,  0,              VERTICAL,   map_height());
    pc.printf("Walls done!\r\n");
    //******************adding more**************//
    add_NPC(5, 5);
    add_NPC_2(30, 30);
    add_NPC_3(8, 8);
    pc.printf("NPC done!\n");
    ////////tree//////////////?????????????????/
    //add_tree(10, 10);
    
    
    add_lava(2, 2, HORIZONTAL, 10);
    add_lava(13, 13, HORIZONTAL, 22);
    pc.printf("Lava is done!");
    
    add_ice(ice_x, ice_y);
    pc.printf("Ice is done!");
    
    add_swamp(7, 10, HORIZONTAL, 8);
    add_swamp(15, 20, VERTICAL, 10);

    print_map();
}

void init_end_page(int health) {
    if (health >= 1) {
        draw_end();
    } else {
        draw_lose();
    }
    Map* end_page = set_active_map(2);            
    print_map();   
}
///////////for maze map///////////////
void init_maze_map() {
    Map* maze = set_active_map(1);
    //implementation of maze//
    //add_XXX();
    //for(int i = map_width() + 3; i < map_area(); i += 39)
//    {
//        add_plant(i % map_width(), i / map_width());
//    }
//    pc.printf("plants\r\n");
        
    //pc.printf("Adding walls!\r\n");
    add_wall(0,              0,              HORIZONTAL, map_width());
    add_wall(0,              map_height()-1, HORIZONTAL, map_width());
    add_wall(0,              0,              VERTICAL,   map_height());
    add_wall(map_width()-1,  0,              VERTICAL,   map_height());
    pc.printf("Walls done!\r\n");
//    /////******add more walls*****////////
    add_wall(1, 7, HORIZONTAL, 3);//more???
    add_wall(4, 7, HORIZONTAL, 5);
    add_wall(2, 5, HORIZONTAL, 7);
    add_wall(10, 6, HORIZONTAL, 3); //4
    add_wall(13, 5, VERTICAL, 4);
    add_wall(1, 3, VERTICAL, 4);

////////////////*******************?/////////////
    add_sonar(sonar_x, sonar_y);    
    
  //  MapItem* current = get_here(Player.x, Player.y);
//    //nextBlock = get_south(Player.x, Player.y)
//    pc.printf("^^^^^^^^^^^^   %B    ^^^^^6", current -> walkable);
    print_map();    
}

/**
 * Program entry point! This is where it all begins.
 * This function orchestrates all the parts of the game. Most of your
 * implementation should be elsewhere - this holds the game loop, and should
 * read like a road map for the rest of the code.
 */
//int action;
int main()
{
    // First things first: initialize hardware
    ASSERT_P(hardware_init() == ERROR_NONE, "Hardware init failed!");
    
    //start page
    uLCD.text_width(2);
    uLCD.text_height(2);
    uLCD.locate(3, 3);
    uLCD.printf("3");
    speaker.period(1.0/392.0);//
    speaker = 0.03;
    wait(1);
    uLCD.cls();
    
    speaker.period(1.0/440.0);//
    speaker = 0.03;
    wait(0.3);
    
    speaker.period(1.0/390.0);//
    speaker = 0.03;
    wait(0.4);
    
    uLCD.text_width(2);
    uLCD.text_height(2);
    uLCD.locate(3, 3);
    uLCD.printf("2");
    speaker.period(1.0/220.0);//
    speaker = 0.03;
    wait(.7);
    uLCD.cls();
    
    uLCD.text_width(2);
    uLCD.text_height(2);
    uLCD.locate(3, 3);
    uLCD.printf("1");
    speaker.period(1.0/110.0);//
    speaker = 0.03;
    wait(.5);
    uLCD.cls();
    
    speaker.period(1.0/200.0);//
    speaker = 0.03;
    wait(0.7);
    
    speaker = 0;
    

    // Initialize the maps
    maps_init();        //initial both map and maze
    init_main_map();
    init_maze_map();
    
    // Initialize game state

    //set_active_map(0);
    set_active_map(0);
    Player.x = Player.y = 5;
    Player.health = 50;

    GameInputs in;
    int action;
    int result;


    // Initial drawing
    draw_game(true);

    // Main game loop
    while(1)
    {
        // Timer to measure game update speed
        Timer t; t.start();
        
        // Actuall do the game update:
        // 1. Read inputs        
        in = read_inputs();
        // 2. Determine action (get_action) 
        action = get_action(in);       
        // 3. Update game (update_game)
        result = update_game(action);
        // check health & lava
        if(((get_here(Player.x, Player.y))-> type == lava) || get_here(Player.x, Player.y) -> type == swamp) {
            wait(0.5);
            if (!Player.omni_pressed) {  //is not pressed
                Player.health = Player.health - 10.0;
            }
            //timer for restart game
            if(Player.health < 1) {
                init_end_page(Player.health);
            }
        }
        //
//        //check for fake_treasure
//        if (get_here(Player.x, Player.y)->type == fake_treasure) {
//                 
//        }//end if fake_treasure       
                 
                
        // 3b. Check for game over
        if (result == 1) {
            //return 1;
            init_end_page(Player.health);
        }
        // 4. Draw frame (draw_game)
        draw_game(result);
        // 5. Frame delay
        t.stop();
        int dt = t.read_ms();
        if (dt < 100) wait_ms(100 - dt);
    }
    
}
