void drawFrames()
{
  fill(0);
  rect(0, 0, width-2, height-2);
  line(game_width, 0, game_width, height);
  line(2*game_width, 0, 2*game_width, height);
}

void drawHeaders()
{
  image(z_logo, game_width/2 - game_logo_width/2, game_width/2 - game_logo_width/2, game_logo_width, game_logo_height);
  image(r_logo, game_width + game_width/2 - game_logo_width/2, game_width/2 - game_logo_width/2, game_logo_width, game_logo_height);
  image(g_logo, 2*game_width + game_width/2 - game_logo_width/2, game_width/2 - game_logo_width/2, game_logo_width, game_logo_height);
  fill(255);
  textSize(text_size);
  for (int i = 0; i < 3; i++)
  {
    textAlign(LEFT);
    text(rank, i*game_width + game_width/2 - game_logo_width/2, game_width/2 - game_logo_width/2 + game_logo_height + header_size + 5);
    textAlign(CENTER);
    text(time, i*game_width + game_width/2, game_width/2 - game_logo_width/2 + game_logo_height + header_size + 5);
    textAlign(RIGHT);
    text(team, i*game_width + game_width/2 + game_logo_width/2, game_width/2 - game_logo_width/2 + game_logo_height + header_size + 5);
  }
  textAlign(CENTER);
  if(millis() - ic_col_change > 1000) 
  {
    if(ic_color == white) ic_color = yellow;
    else ic_color = white;
    ic_col_change = millis();
  }
  fill(ic_color);
  text("INCERT COIN", width/2, height - text_size/2);
  textAlign(RIGHT);
  fill(255);
  text("CREDIT: 00", width - 10, height - text_size/2);
}

void drawScores()
{
  for (int g = 0; g < 3; g++)
  {
    if (!score_loaded)
    {
      try
      {
        gameData[g] = loadJSONArray(server_addr+"getscores?game=" + game_names[g]);
        println("Game Data from " + game_names[g] + " recieved: " + str(gameData[g].size())+ " commands");
      }
      catch (Exception e)
      {
        println(e);
        println("Game Data from " + game_names[g] + " not recieved");
      }
    }
    for (int c = 0; c < gameData[g].size() && c < 5; c++)
    {
      JSONObject co = gameData[g].getJSONObject(c);
      String name = co.getString("Name");
      Integer time = co.getInt("Time");
      textAlign(CENTER);
      textSize(text_size);
      text(str(c+1), g*game_width + game_width/2 - game_logo_width/2 + rank_width/2, game_width/2 - game_logo_width/2 + game_logo_height + header_size + 5 + (c+1)*text_size);
      String timestr = getTime(hours(Integer.valueOf(time)), minutes(Integer.valueOf(time)), seconds(Integer.valueOf(time)));
      text(timestr, g*game_width + game_width/2, game_width/2 - game_logo_width/2 + game_logo_height + header_size + 5 + (c+1)*text_size);
      text(name, g*game_width + game_width/2 + game_logo_width/2 - team_width/2, game_width/2 - game_logo_width/2 + game_logo_height + header_size + 5 + (c+1)*text_size);
    }
  }
  score_loaded = true;
}

void enterName(int game)
{
  textAlign(CENTER);
  float cmd_name_offset = 0.0;
  boolean canSend = true;
  String cmd_name_str = "";
  for (int i = 0; i < 3; i++)
  {
    textAlign(LEFT);
    if (char_pos[game] == i && enter_name[game])
    {
      fill(200);
      strokeWeight(0);
      rect(game_width/2 + game*game_width + game_logo_width/2 - team_width/2 - 1.5*textWidth(cmd_names[game][i]) + cmd_name_offset, game_width/2 - game_logo_width/2 + game_logo_height + header_size + 10 + (min(5,gameData[game].size())+1)*text_size, textWidth(cmd_names[game][i]), 5);
      fill(0);
    }
    fill(255);
    if (cmd_names[game][i] == '*') 
    {
      if (millis() - start_draw_error_rect < 2000) fill(red);
      canSend = false;
    }
    text(cmd_names[game][i], game_width/2 + game*game_width + game_logo_width/2 - team_width/2 - 1.5*textWidth(cmd_names[game][i]) + cmd_name_offset, game_width/2 - game_logo_width/2 + game_logo_height + header_size + 5 + (min(5,gameData[game].size())+1)*text_size);
    cmd_name_offset += textWidth(cmd_names[game][i]);
    cmd_name_str += cmd_names[game][i];
  }
  cmd_name_width = cmd_name_offset;
  name_ok[game] = canSend;
}

void enterTime(int game)
{
  String timestr = getTime(hours(Integer.valueOf(cmd_times[game])), minutes(Integer.valueOf(cmd_times[game])), seconds(Integer.valueOf(cmd_times[game])));
  fill(255);
  textAlign(CENTER);
  text(timestr, INPUT_GAME*game_width + game_width/2, game_width/2 - game_logo_width/2 + game_logo_height + header_size + 5 + (min(5,gameData[INPUT_GAME].size())+1)*text_size);
}

String getTime(int h, int m, int s)
{
  String h_str = str(h);
  if (h < 10) h_str = "0" + str(h);
  String m_str = str(m);
  if (m < 10) m_str = "0" + str(m);
  String s_str = str(s);
  if (s < 10) s_str = "0" + str(s);
  return h_str+":"+m_str+":"+s_str;
}

int seconds(int secs) {
  return int(secs % 60);
}
int minutes(int secs) {
  return int((secs / (60)) % 60);
}
int hours(int secs) {
  return int((secs / (60*60)) % 24);
}
