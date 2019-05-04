import http.requests.*;

int[] gTimes = new int[10];
int total_scores = 0;
String alpha = "ABCDEFGJHIJKLMNOPQRSTUVWXYZ1234567890";
String rank, time, team;
String[] game_names = {"Zombie", "Room", "Graphics"};
char[][] cmd_names = {{'*','*','*'},{'*','*','*'},{'*','*','*'}};
int[][] char_ix = {{0,0,0},{0,0,0},{0,0,0}};
int[] cmd_times = {0,0,0};
int[] char_pos = {0,0,0};

String server_addr = "http://127.0.0.1:8484/"; //0.39
String spaces;
JSONArray[] gameData = new JSONArray[3];

byte STATE = 0;
byte ZOMBIE_NAME = 1;
byte PLAYING = 2;
byte SHOW_SCORES = 3;

int INPUT_GAME = 0;

boolean server_connect = false;
boolean[] name_ok = {false,false,false};
boolean[] time_ok = {false,false,false};

long last_update_time = 0;
long start_draw_error_rect = 0;
long ic_col_change = 0;

PFont arcade_font;

PImage z_logo;
PImage r_logo;
PImage g_logo;
color red = color(200,10,10);
color white = color(200);
color yellow = color(250,210,160);
color green = color(10,200,10);
color blue = color(10,10,200);
color orange = color(250,150,0);
color ic_color;
float cmd_name_width;
float game_width, game_logo_width, game_logo_height;
float rank_width, time_width, team_width, space_width, header_width;
int header_size, text_size;


boolean score_loaded = false;
boolean[] enter_name = {false,false,false};
boolean[] enter_time = {false,false,false};

void setup()
{
  //size(1400, 900);
  fullScreen();
  background(0);
  fill(0);
  stroke(255);
  strokeWeight(2);
  
  STATE = SHOW_SCORES;
  game_width = width/3;
  game_logo_width = 9*game_width/10;
  game_logo_height = 150;
  
  z_logo = loadImage("Zombie.jpg");
  r_logo = loadImage("Jack.jpg");
  g_logo = loadImage("Dragons.jpg");
  
  for (int g = 0; g < 3; g++) gameData[g] = new JSONArray();
  
  rank = "RANK";
  time = "TIME";
  team = "TEAM";
  spaces = "     ";
  
  ic_color = orange;
  arcade_font = createFont("arcade.ttf", 20);
  textFont(arcade_font);
  
  //Calculate Text Size
  header_size = 0;
  header_width = rank_width + time_width + team_width + space_width;
  while (header_width < game_logo_width)
  {
    header_size++;
    textSize(header_size);
    rank_width = textWidth(rank);
    time_width = textWidth(time);
    team_width = textWidth(team);
    space_width = textWidth(spaces);
    header_width = rank_width + time_width + team_width + 2*space_width;
  }
  println("Text size = " + str(header_size));
  text_size = header_size;
  cmd_name_width = textWidth("***");
}

void draw()
{
  background(0);
  drawFrames();
  drawHeaders();
  drawScores();
  if (STATE == PLAYING || STATE == ZOMBIE_NAME) enterName(0);
  if (INPUT_GAME > 0 && STATE != ZOMBIE_NAME)
  {
    if (enter_name[INPUT_GAME]) enterName(INPUT_GAME);
    if (enter_time[INPUT_GAME])
    {
      enterName(INPUT_GAME);
      enterTime(INPUT_GAME);
    }
  }
  if (server_connect) updateState(); 
  else connectToServer();
}

String toStr(char[] a)
{
  int s = a.length;
  String ret = "";
  for (int i = 0; i < s; i++)
  {
    ret += a[i];
  }
  return ret;
}
