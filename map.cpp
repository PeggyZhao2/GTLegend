#include "map.h"

#include "globals.h"
#include "graphics.h"

unsigned int buckets_num = 10;
unsigned int buckets_num2 = 10;
//unsigned int buckets_num3 = 5;

/**
 * The Map structure. This holds a HashTable for all the MapItems, along with
 * values for the width and height of the Map.
 */
struct Map {
    HashTable* items;
    int w, h;
};

/**
 * Storage area for the maps.
 * This is a global variable, but can only be access from this file because it
 * is static.
 */
//static Map map;
//static Map maze;
static Map map[3];
static int active_map;

/**
 * The first step in HashTable access for the map is turning the two-dimen
 sional
 * key information (x, y) into a one-dimensional unsigned integer.
 * This function should uniquely map (x,y) onto the space of unsigned integers.
 */
static unsigned XY_KEY(int X, int Y) {
    // TODO: Fix me!
    return (X + (Y)*map_width());
}

/**
 * This is the hash function actually passed into createHashTable. It takes an
 * unsigned key (the output of XY_KEY) and turns it into a hash value (some
 * small non-negative integer).
 */
unsigned map_hash(unsigned key)
{
    // TODO: Fix me!
    return key % buckets_num;
}

void maps_init()
{
    // TODO: Implement!    
    // Initialize hash table
    // Set width & height
    map[0].items = createHashTable(map_hash, buckets_num);
    map[0].w = 50;
    map[0].h = 50;
    //making a maze
    map[1].items = createHashTable(map_hash, buckets_num2);
    map[1].w = 15;
    map[1].h = 9;
    //end page
    //map[1].items = createHashTable(map_hash, buckets_num3);
    map[2].w = 11;
    map[2].h = 11;
}

int get_active_num () {
    return active_map;    
}

Map* get_active_map()
{
    // There's only one map
    //if (!active_map) {
//        return &map[];
//    } else {
//        return &maze;    
//    }
    return &map[active_map];
}

Map* set_active_map(int m)
{
    active_map = m;
    return &map[active_map];
}

void print_map()
{
    // As you add more types, you'll need to add more items to this array.
    char lookup[] = {'W', 'P'};
    for(int y = 0; y < map_height(); y++)
    {
        for (int x = 0; x < map_width(); x++)
        {
            MapItem* item = get_here(x,y);
            if (item) pc.printf("%c", lookup[item->type]);
            else pc.printf(" ");
        }
        pc.printf("\r\n");
    }
}

int map_width()
{
    //if (!active_map) {
//        Map* map = get_active_map();
//        return map -> w;
//    } else {
//        return maze.w;
//    }
    //Map* map = get_active_map();
    return map[active_map].w;
    //if (!active_map) {
//        return map[0].w;
//    } else {
//        return map[1] -> w;    
//    }
}

int map_height()
{
    //if (!active_map) {
//        Map* map = get_active_map();
//        return map -> h;
//    } else {
//        return maze.h;    
//    }
    return map[active_map].h;
}

int map_area()
{
    //if (!active_map) {
//        Map* map = get_active_map();
//        return (map -> h) * (map -> w);
//    } else {
//        return (maze.w * maze.h);    
//    }
    return map[active_map].h * map[active_map].w;
}

MapItem* get_north(int x, int y)
{
    //Map* map = get_active_map();
    unsigned int key = XY_KEY(x, y - 1);
//    if (!active_map) {
//        MapItem* mitem = (MapItem*) getItem(map -> items, key);
//        return mitem;
//    } else {
//        return (MapItem*) getItem(maze.items, key);    
//    }
    return (MapItem*) getItem(map[active_map].items, key);
}

MapItem* get_south(int x, int y)
{
    //Map* map = get_active_map();
    unsigned int key = XY_KEY(x, y + 1);
//    if (!active_map) {
//        MapItem* mitem = (MapItem*) getItem(map -> items, key);
//        return mitem;
//    } else {
//        return (MapItem*) getItem(maze.items, key);    
//    }
    return (MapItem*) getItem(map[active_map].items, key);
}

MapItem* get_east(int x, int y)
{
    //Map* map = get_active_map();
    unsigned int key = XY_KEY(x + 1, y);
//    if (!active_map) {
//        MapItem* mitem = (MapItem*) getItem(map -> items, key);
//        return mitem;
//    } else {
//        return (MapItem*) getItem(maze.items, key);     
//    }
    return (MapItem*) getItem(map[active_map].items, key);
}

MapItem* get_west(int x, int y)
{
    //Map* map = get_active_map();
    unsigned int key = XY_KEY(x - 1, y);
    return (MapItem*) getItem(map[active_map].items, key);
}

MapItem* get_here(int x, int y)
{
    //Map* map = get_active_map();
    unsigned int key = XY_KEY(x, y);
    return (MapItem*) getItem(map[active_map].items, key);
}


void map_erase(int x, int y)
{
    //Map* map = get_active_map();
    unsigned int key = XY_KEY(x, y);
    removeItem(map[active_map].items, key);
}
 
 
 
 
 

void add_wall(int x, int y, int dir, int len)
{
    for(int i = 0; i < len; i++)
    {
        MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
        w1->type = WALL;
        w1->draw = draw_wall;
        w1-> walkable = false;
        w1->data = NULL;
        unsigned key = (dir == HORIZONTAL) ? XY_KEY(x+i, y) : XY_KEY(x, y+i);
        void* val = insertItem(get_active_map()->items, key, w1);
        if (val) free(val); // If something is already there, free it
    }
}

void add_plant(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = PLANT;
    w1->draw = draw_plant;
    w1->walkable = true;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_NPC (int x, int y) {
    MapItem* npc = (MapItem*) malloc(sizeof(MapItem));
    npc -> type = NPC;
    npc -> draw = draw_NPC;
    npc -> walkable = false;
    npc -> data = 0;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), npc);
    if (val) free(val); // If something is already there, free it
}

void add_NPC_2 (int x, int y) {
    MapItem* npc2 = (MapItem*) malloc(sizeof(MapItem));
    npc2 -> type = NPC_2;
    npc2 -> draw = draw_NPC_2;
    npc2 -> walkable = false;
    npc2 -> data = 0;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), npc2);
    if (val) free(val); // If something is already there, free it
}

void add_door(int x, int y) {
    MapItem* door1 = (MapItem*) malloc(sizeof(MapItem));
    door1 -> type = door;
    door1 -> draw = draw_door;
    door1 -> walkable = false; //?? walkable T after get key
    door1 -> data = NULL; //???
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), door1);
    if (val) free(val); // If something is already there, free it
}

////////*******APPLE TREE**********/////////
//void add_tree(int x, int y) {
//    MapItem* tree1 = (MapItem*) malloc(sizeof(MapItem));
//    tree1 -> type = tree; 
//    tree1 -> draw = draw_tree;
//    tree1 -> walkable = true;
//    tree1 -> data = NULL;
//    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), tree1);
//    if (val) free(val); // If something is already there, free it
//}

void add_sonar(int x, int y) {
    MapItem* sonar1 = (MapItem*) malloc(sizeof(MapItem));
    sonar1 -> type = sonar;
    sonar1 -> draw = draw_sonar; //??
    sonar1 -> walkable = false;
    sonar1 -> data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), sonar1);
    if (val) free(val); // If something is already there, free it
}

void delete_sonar(int x, int y) {
    MapItem* sonar1 = (MapItem*) get_here(x, y);
    free(sonar1);
    cover_sonar(x, y);
}

void add_treasure(int x, int y) {
    MapItem* t = (MapItem*) malloc(sizeof(MapItem));
    t -> type = treasure;
    t -> draw = draw_treasure;
    t -> walkable = false;
    t -> data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), t);
    if (val) free(val); // If something is already there, free it
}

void delete_treasure(int x, int y) {
    MapItem* t = (MapItem*) get_here(x, y);
    free(t);
    cover_treasure(x, y);
}

//void add_lava(int x, int y) {
//    MapItem* lava1 = (MapItem*) malloc(sizeof(MapItem));
//    lava1 -> type = lava;
//    lava1 -> draw = draw_lava();
//    lava1 -> walkable = true;
//    lava1 -> data = NULL;
//    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), lava1);
//    if (val) free(val); // If something is already there, free it
//}

void add_lava(int x, int y, int dir, int len)
{
    for (int i = 0; i < len; i++)
    {
    MapItem* lava1 = (MapItem*) malloc(sizeof(MapItem));
    lava1->type = lava;
    lava1->draw = draw_lava;
    lava1->walkable = true;
    lava1->data = NULL;
    unsigned key = (dir == HORIZONTAL) ? XY_KEY(x+i, y) : XY_KEY(x, y+i);
    void* val = insertItem(get_active_map()->items, key, lava1);
    if (val) free(val); // If something is already there, free it
    }
}

void add_ice(int x, int y) {
    MapItem* ice1 = (MapItem*) malloc(sizeof(MapItem));
    ice1 -> type = ice;
    ice1 -> draw = draw_ice;
    ice1 -> walkable = false;
    ice1 -> data = 2;
    //void* val = insertItem(get_active_map()->items, XY_KEY(x, y), npc);
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), ice1);
    if (val) free(val); // If something is already there, free it
}

void break_ice(int x, int y, int health) {
    //set data - 1
    MapItem* ice_block = get_here(x, y);
    ice_block->data = ice_block->data - 1;    //after breaking once, data=1
    
    //redraw
    if (ice_block->data == 1) { //data =1, breaking once
        ice_block -> draw = draw_ice_2;
    } else if (ice_block->data == 0) {    //disappear
        map_erase(x, y);
    }
}

void add_swamp(int x, int y, int dir, int len) {
    for (int i = 0; i < len; i++) {
        MapItem* swamp1 = (MapItem*) malloc(sizeof(MapItem));
        swamp1->type = swamp;
        swamp1->draw = draw_swamp;
        swamp1->walkable = true;
        swamp1->data = NULL;
        unsigned key = (dir == HORIZONTAL) ? XY_KEY(x+i, y) : XY_KEY(x, y+i);
        void* val = insertItem(get_active_map()->items, key, swamp1);
        if (val) free(val); // If something is already there, free it
    }
}

void add_fake_treasure (int x, int y) {
    MapItem* ft = (MapItem*) malloc(sizeof(MapItem));
    ft->type = fake_treasure;
    ft->draw = draw_treasure;
    ft->walkable = true;
    ft->data = NULL;
    
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), ft);
    if (val) free(val); // If something is already there, free it
}

void add_treasure_pieces (int x, int y) {
    MapItem* piece1 = (MapItem*) malloc(sizeof(MapItem));
    piece1->type = treasure_pieces;
    piece1->draw = draw_piece_1;
    piece1->walkable = true;
    piece1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x+2, y-1), piece1);
    if (val) free(val); // If something is already there, free it
    
    //MapItem* piece2 = (MapItem*) malloc(sizeof(MapItem));
//    piece2->type = treasure_pieces;
//    piece2->draw = draw_piece_2;
//    piece2->walkable = true;
//    piece2->data = NULL;
//    void* val2 = insertItem(get_active_map()->items, XY_KEY(x-1, y+1), piece2);
//    if (val2) free(val); // If something is already there, free it
    
//    MapItem* piece3 = (MapItem*) malloc(sizeof(MapItem));
//    piece3->type = treasure_pieces;
//    piece3->draw = draw_piece_3;
//    piece3->walkable = true;
//    piece3->data = NULL;
//    void* val3 = insertItem(get_active_map()->items, XY_KEY(x, y), piece3);
//    if (val3) free(val); // If something is already there, free it
}

void add_NPC_3(int x, int y) {
    MapItem* npc3 = (MapItem*) malloc(sizeof(MapItem));
    npc3->type = NPC_3;
    npc3->draw = draw_NPC_3;
    npc3->walkable = false;
    npc3->data = NULL;
    
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), npc3);
    if (val) free(val); // If something is already there, free it
}

void add_tree(int x, int y) {
    MapItem* tree1 = (MapItem*) malloc(sizeof(MapItem));
    tree1->type = tree;
    tree1->draw = draw_tree;
    tree1->walkable = false;
    tree1->data = 1;
    
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), tree1);
    if (val) free(val); // If something is already there, free it
}











//map[2]--game over screen and words "YOU WIN!"
// 1. adding random_generator & analog
// 2. replace ran_x with random_generator()
// 3. lower status bar: show direction
// 4. upper status bar: has key or not
// 5. add sprite: treasure, player, sonar, npc, npc_2----color wrong
// 6. no add_tree()
// 7. add_lava();  draw_lava();  lava_sprite; add to init_main()
// 8. Player.health;  draw_upper-status(health, x, y);  main() set Player.health = 50;  main check health and lava
// 9. add ice(breakable)????


//Problems:
// 1. HP: upper status bar not enough space: put on lower_status_bar----------resolved----------------
// 2. lower_status bar: show direction before get the sonar---add a initial default------------resolved-------------
// 3. When die from lava, Don't say you win, say you die------resolved--------------------------------
// 4. erase for treasure or sonar doesn't work---------------resolved-------------------
// 5. destory_map(MapItem* map) to save memory space
// 6. need a door to replace the sonar-----------resolved-----------
// 7. is it random???? (22, 38, first) (23, 23, second)--(38, 19, first)--------resolved-------------
// 8. coordinate more than 100 ----------resolved----------------
// 9. ice----------resolved----------------
// 10. hopping
// 11. start page---------------resolved-------------------
// 12. adding sprites * 8: NPC1, player, lava, ice, sonar, NPC2, plants(flower), swamp, treasure-------resolved---------
// 13. animation: fake treasure-->blow up into 4 pieces---> collect and disappear one by one
// 14. fake treasure; disappear----------------resolved-----------------------
// 15. sound
// 16. omni_mode---lava don't die------------------------resolved---------------------
// 17. improve tool(sonar)-----------------resolved--------------
// 18. random fake treasure---------------resolved----------------