import http.requests.*;

int[] gTimes = new int[10];
int total_scores = 0;
String alpha = "ABCDEFGJHIJKLMNOPQRSTUVWXYZ1234567890";
String rank, time, team;
String[] gameNames = {"Zombie", "Room", "Graphics"};
String server_addr = "//http://192.168.0.44:8484/"; 
String spaces;
JSONArray[] gameData = new JSONArray[3];

byte STATE = 0;
byte ENTER_NAME = 1;
byte SHOW_RECORD = 2;
byte PLAYING = 3;

int INPUT_GAME = 0;

boolean server_connect = false;
boolean name_ok = false;
boolean time_ok = false;

long last_update_time = 0;
long start_draw_error_rect = 0;

PFont arcade_font;

PImage z_logo;
PImage r_logo;
PImage g_logo;
color red = color(200,10,10);
color white = color(200);
color rect_color;
float cmd_name_width;
float game_width, game_logo_width, game_logo_height;
float rank_width, time_width, team_width, space_width, header_width;
int header_size, text_size;
int cmd_time = 0;

boolean score_loaded = false;
boolean enter_name = false;
boolean enter_time = false;

void setup()
{
  size(1400, 600);
  background(0);
  fill(0);
  stroke(255);
  strokeWeight(2);
  cmd_name = new char[3];
  char_ix = new int[3];
  for (int c = 0; c < 3; c++) cmd_name[c] = '*';
  STATE = SHOW_RECORD;
  game_width = width/3;
  game_logo_width = 360;
  game_logo_height = 100;
  
  z_logo = loadImage("Zombie.jpg");
  r_logo = loadImage("Zombie.jpg");
  g_logo = loadImage("Zombie.jpg");
  
  for (int g = 0; g < 3; g++) gameData[g] = new JSONArray();
  
  rank = "RANK";
  time = "TIME";
  team = "TEAM";
  spaces = "     ";
  
  rect_color = white;
  arcade_font = createFont("arcade.ttf", 20);
  textFont(arcade_font);
  
  //Calculate Text Size
  header_size = 0;
  header_width = rank_width + time_width + team_width + space_width;
  while (header_width < 360)
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
  if (STATE == ENTER_NAME) enterName(0);
  else if (INPUT_GAME > 0) 
  {
    if (enter_name) enterName(INPUT_GAME);
    if (enter_time) 
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
